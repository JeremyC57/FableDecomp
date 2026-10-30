#!/usr/bin/env python3
"""Differential migration check: for each landed row, compile the HEAD source and the edited worktree source under the
variant (flags x pragma) where HEAD grades against retail, then require the edited source to give the same leaf bytes and
the same relocation (offset, symbol) list. usage (repo root, lane env): mcheck.py [-j N] [--all] <addr|src>...
Prints one line per row: addr STATUS detail. STATUS: SAME | RELOC_RENAMED | DIFFER | SRC_FAIL | NOCHANGE | OLD_NOMATCH | SKIP"""
import sys, os, re, json, subprocess
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
ROOT = Path(os.environ["FABLE_ROOT"]); os.chdir(ROOT)
sys.path.insert(0, str(ROOT / "tools/decomp_pipeline"))
import verify_and_land as V
def _run_tool(exe, a, e, _t=180):
    cmd = ([V.WINE] if V.WINE else []) + [str(exe)] + V.tool_args(a)
    try: return subprocess.run(cmd, capture_output=True, text=True, errors="replace", env=e, timeout=_t)
    except subprocess.TimeoutExpired: return subprocess.CompletedProcess(cmd, 124, "timeout", "timeout")
V.run_tool = _run_tool
S = Path(os.environ["S"])
WORK = S / "mig9/work/mc"
args = sys.argv[1:]; jobs = 3; force = False
if "-j" in args: i = args.index("-j"); jobs = int(args[i+1]); del args[i:i+2]
if "--all" in args: args.remove("--all"); force = True
ps = open("rebuild/build_candidates.ps1", encoding="utf-8-sig").read()
ent = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: ent[kv["Address"].lower()] = kv
bysrc = {v.get("Source"): a for a, v in ent.items()}
cat = {}
for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); cat[q[0].lower()] = q
PRAGMAS = ["", '#pragma optimize("s",on)', '#pragma optimize("t",on)', '#pragma optimize("g",on)', '#pragma optimize("",off)']
CACHE = S / "mig9/work/mc_variant.json"
cache = json.loads(CACHE.read_text()) if CACHE.exists() else {}
sys.path.insert(0, str(ROOT / "tools/decomp_pipeline/linux_lane/lane_tools"))
OPS = {"operator=": "??4", "operator==": "??8", "operator!=": "??9", "operator[]": "??A", "operator()": "??R"}
def leaf_of(src, name):
    m = re.search(r"^//\s*Leaf:\s*(\S+)", src, re.M)
    if m: return m.group(1)
    mod, leafname = name.rsplit("::", 1) if "::" in name else ("_global", name)
    cls = mod.rsplit("::", 1)[-1]
    def head(n): return ("?$" + n.split("<")[0]) if "<" in n else n
    if leafname.startswith("~"): return "??1" + head(leafname[1:]) + "@"
    if leafname == cls or (leafname.split("<")[0] == cls.split("<")[0] and mod != "_global"): return "??0" + head(cls) + "@"
    if leafname in OPS and mod != "_global": return OPS[leafname] + head(cls) + "@"
    return leafname
RLN = re.compile(r"^([0-9a-fA-F]{8})\s+(\S+)\s+(.+)$")
def named_relocs(obj, sec):
    hp = subprocess.run([V.OBJDUMP, "-h", str(obj)], capture_output=True, text=True, check=True)
    flags = []; hl = hp.stdout.splitlines()
    for i, l in enumerate(hl):
        if V.TEXTSEC_H.match(l): flags.append("RELOC" in (hl[i+1] if i+1 < len(hl) else ""))
    if sec >= len(flags) or not flags[sec]: return []
    bi = sum(flags[:sec+1]) - 1
    rp = subprocess.run([V.OBJDUMP, "-r", str(obj)], capture_output=True, text=True, check=True)
    blocks = []; intext = False
    for l in rp.stdout.splitlines():
        if l.startswith("RELOCATION RECORDS FOR ["):
            intext = bool(V.TEXTSEC_R.match(l))
            if intext: blocks.append([])
            continue
        if intext:
            m = RLN.match(l.strip())
            if m: blocks[-1].append((int(m.group(1), 16), m.group(2), m.group(3)))
    return blocks[bi] if bi < len(blocks) else []
def compile_(src, flags, pragma, leaf, work, tag, env=None):
    work.mkdir(parents=True, exist_ok=True)
    text = V.vc71((pragma + "\n" + src) if pragma else src)
    sp = work / f"{tag}.cpp"; sp.write_text(text, encoding="utf-8")
    obj = work / f"{tag}.obj"
    if obj.exists(): obj.unlink()
    cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + list(flags) + [f"/Fo{obj}", str(sp)], env or E)
    if cp.returncode or not obj.exists(): return None, (cp.stdout or "")[-6000:]
    b, sec, sym = V.obj_text(obj, leaf)
    return (b, sec, sym, named_relocs(obj, sec)), None
def grades(b, rel, retail):
    if b == retail: return True
    offs = [r[0] for r in rel]
    return V.mask(retail, offs) == V.mask(b, offs)
E = V.env()
# the HEAD source compiles against the HEAD include tree (work/head, from `git archive HEAD rebuild/include`), so a shared
# header edit is checked too; the edited source compiles against the worktree headers
EOLD = dict(E); EOLD["INCLUDE"] = E["INCLUDE"].replace(V.wpath(ROOT / "rebuild" / "include"), V.wpath(S / "mig9/work/head/rebuild/include"))
assert EOLD["INCLUDE"] != E["INCLUDE"]
def one(w):
    a = w.lower()
    if a not in ent:
        rel = os.path.relpath(w, "rebuild/src/compiled").replace(os.sep, "/"); a = bysrc.get(rel, "")
    if a not in ent or a not in cat: return (w, "SKIP", "not in catalog")
    kv = ent[a]; path = "rebuild/src/compiled/" + kv["Source"]
    new = open(path, encoding="utf-8").read()
    try: old = subprocess.run(["git", "show", "HEAD:" + path], capture_output=True, text=True, check=True).stdout
    except subprocess.CalledProcessError: return (a, "SKIP", "not in HEAD")
    if old == new and not force: return (a, "NOCHANGE", "")
    q = cat[a]; leaf = leaf_of(new, q[1]); retail = bytes.fromhex(q[3])
    while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
    fl = kv.get("CompilerFlags", "").split(); pre = [f for f in fl if f not in V.BASE_FLAGS] if fl else []
    if fl and "/Oy" not in fl and "/Oy-" not in pre: pre.append("/Oy-")
    if kv.get("Compiler") == "qfe4035": return (a, "SKIP", "qfe row")
    work = WORK / a
    variants = [cache[a]] if a in cache else [(pre + xf, P) for xf in V.EXTRA_FLAGSETS for P in PRAGMAS]
    ob = None
    for fl2, P in variants:
        r, err = compile_(old, fl2, P, leaf, work, "old", EOLD)
        if r and grades(r[0], r[3], retail): ob = r; var = (fl2, P); break
    if not ob: return (a, "OLD_NOMATCH", "")
    cache[a] = list(var)
    nb, err = compile_(new, var[0], var[1], leaf, work, "new")
    if not nb:
        el = [re.sub(r"^.*?\\(\w[\w.]*\(\d+\)) :", r"\1:", l.strip()) for l in (err or "").splitlines() if "error" in l]
        return (a, "SRC_FAIL", " | ".join(el[:3])[:700] or (err or "").replace("\n", " | ")[-600:])
    if nb[0] != ob[0]:
        g = grades(nb[0], nb[3], retail)
        return (a, "DIFFER", f"{len(nb[0])}v{len(ob[0])} grades={g} sym={nb[2]}")
    if nb[3] != ob[3]:
        d = [f"{x[0]:x}:{x[2]}->{y[2]}" for x, y in zip(ob[3], nb[3]) if x != y]
        if len(nb[3]) != len(ob[3]): d.append(f"count {len(ob[3])}->{len(nb[3])}")
        og = "MATCH" if ob[0] == retail else "RELOCATION_MATCH"
        return (a, "RELOC_RENAMED", og + " ; " + " ; ".join(d))
    if nb[2] != ob[2]: return (a, "RELOC_RENAMED", f"leafsym {ob[2]} -> {nb[2]}")
    og = "MATCH" if ob[0] == retail else "RELOCATION_MATCH"
    ng = "MATCH" if nb[0] == retail else "RELOCATION_MATCH"
    return (a, "SAME", f"{len(nb[0])}b {og}->{ng} {'/'.join(var[0])} {var[1]}".strip())
with ThreadPoolExecutor(jobs) as ex:
    for res in ex.map(one, args):
        print("\t".join(res), flush=True)
CACHE.write_text(json.dumps(cache))
