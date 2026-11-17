#!/usr/bin/env python3
"""mkagent_egod.py <agent-name> <addr8> [<addr8> ...]   (egod lane coordinator; repo root, lane env sourced, $S set)

Like ../../2026-11-13_acctC/lane/mkagent.py, for the rows reserved in ../RESERVED.tsv. Builds $S/w23/<agent-name>/ with, per row,
ref/<addr>/: retail_dis.txt, draft.cpp (+ the draft folder's notes) from the list's closest_version, egod_locals.txt (Ego_d named
params / locals / scopes with resolved types; NAMED rows), egod_tu.txt (every Ego_d function of the row's object file(s), in object
order, with the retail address of each one the catalog names). Writes rows.tsv, the scratch oracle and BRIEF.md (BRIEF_TEMPLATE.md).
Env: EGOD_PDB (default /tmp/claude-0/egod_pdb/Ego_d.pdb), EGOD_TYPES (llvm-pdbutil dump -types text, default next to the PDB)."""
import csv, os, re, shutil, subprocess, sys, collections
ROOT = "/home/user/FableDecomp"; S = os.environ["S"]
PDB = os.environ.get("EGOD_PDB", "/tmp/claude-0/egod_pdb/Ego_d.pdb"); TYP = os.environ.get("EGOD_TYPES", os.path.dirname(PDB) + "/types.txt")
sys.path.insert(0, ROOT + "/tools/decomp_pipeline/linux_lane/lane_tools/bounds")
import extent
HERE = os.path.dirname(os.path.abspath(__file__)); RES = os.path.dirname(HERE); PEND = os.path.dirname(RES)
a = sys.argv[1:]; lens = {}
while "--len" in a:
    i = a.index("--len"); k, v = a[i + 1].split("="); lens[k.lower()] = int(v); del a[i:i + 2]
name, addrs = a[0], [x.lower().replace("0x", "") for x in a[1:]]
res = {r["address"]: r for r in csv.DictReader(open(RES + "/RESERVED.tsv"), delimiter="\t")}
named = {r["address"]: r for r in csv.DictReader(open(RES + "/EGOD_NAMED.tsv"), delimiter="\t")}
ident = {r["address"]: r for r in csv.DictReader(open(RES + "/EGOD_IDENTIFY.tsv"), delimiter="\t")}
# catalog: undecorated "Module::Name" -> retail addresses
cat = collections.defaultdict(list)
for l in open(ROOT + "/rebuild/oracles/auto-re-candidates.tsv"):
    p = l.split("\t")
    if len(p) > 2 and re.match(r"^[0-9a-f]{8}$", p[0]): cat[p[1]].append(p[0])
funcs = collections.defaultdict(list)
for l in open(ROOT + "/ghidra_out/egod_carved_functions.tsv"):
    if l.startswith("#"): continue
    o, off, ln, k, n = l.rstrip("\n").split("\t", 4); funcs[o.split("\\")[-1].lower()].append((int(off), int(ln), n))
def tu(objs, target):
    out = []
    for ob in objs:
        fl = sorted(funcs.get(ob.split("\\")[-1].lower(), []))
        out.append(f"== {ob}: {len(fl)} functions in Ego_d object order (debug length; retail address(es) when the catalog has the name)")
        for off, ln, n in fl:
            short = n.split("<")[0]; hits = cat.get(n) or cat.get(short) or []
            out.append(f"{'>>' if n == target else '  '} {ln:6d}  {n}" + (f"   retail {' '.join(hits[:4])}" if hits else ""))
    return "\n".join(out) + "\n"
A = f"{S}/w23/{name}"
for d in ("src", "test", "differ", "tmp", "probe", "ref", "hdr"): os.makedirs(f"{A}/{d}", exist_ok=True)
E_ = extent.Exe(); rows = []; lines = []
for n_, ad in enumerate(addrs, 1):
    r = res[ad]; R = f"{A}/ref/{ad}"; os.makedirs(R, exist_ok=True)
    ln = lens.get(ad) or E_.scan(int(ad, 16))["extent"]
    mod, nm = r["module"], r["name"]
    if nm.startswith(mod + "::"): nm = nm[len(mod) + 2:]
    rows.append((ad, mod, nm, ln)); have = []
    L = named.get(ad) or ident.get(ad) or {}
    cv = L.get("closest_version", "")
    if cv and os.path.isfile(f"{ROOT}/{cv}"):
        shutil.copy(f"{ROOT}/{cv}", f"{R}/draft.cpp"); have.append("draft.cpp (best earlier source; grade / residue in its first lines)")
        fold = os.path.dirname(f"{ROOT}/{cv}")
        if fold.endswith(("/differ", "/src", "/open")): fold = os.path.dirname(fold)
        for t in (cv[:-4] + ".test.cpp", cv.replace("/differ/", "/test/").replace("/src/", "/test/")):
            if t != cv and os.path.isfile(f"{ROOT}/{t}"): shutil.copy(f"{ROOT}/{t}", f"{R}/draft.test.cpp"); have.append("draft.test.cpp"); break
        os.makedirs(f"{R}/draft_folder", exist_ok=True)
        for f in os.listdir(fold):
            if f.endswith((".md", ".tsv")) and os.path.getsize(f"{fold}/{f}") < 400000: shutil.copy(f"{fold}/{f}", f"{R}/draft_folder/{f}")
        have.append("draft_folder/ (notes of the folder that holds the draft: grep them for this address)")
        for h in ("hdr", "hdrdiff"):
            if os.path.isdir(f"{fold}/{h}") and not os.path.isdir(f"{R}/{h}_STALE"): shutil.copytree(f"{fold}/{h}", f"{R}/{h}_STALE"); have.append(f"{h}_STALE/")
    elif cv: have.append(f"(closest_version {cv} not found)")
    for sub, dst in (("notes", "notes"), ("hdr", "hdr_STALE")):
        p = f"{PEND}/2026-11-13_acctB_parked/{sub}/{ad}"
        if os.path.isdir(p) and not os.path.isdir(f"{R}/{dst}"): shutil.copytree(p, f"{R}/{dst}"); have.append(dst + "/")
    objs = []
    if r["kind"] == "named":
        en = L.get("egod_name", "")
        loc = subprocess.run(["python3", "-I", HERE + "/egod_locals.py", PDB, en], capture_output=True, text=True).stdout
        open(f"{R}/egod_locals.raw.txt", "w").write(loc)
        rl = subprocess.run(["python3", "-I", HERE + "/egod_type.py", TYP, "--locals", f"{R}/egod_locals.raw.txt"], capture_output=True, text=True).stdout
        open(f"{R}/egod_locals.txt", "w").write(rl); os.remove(f"{R}/egod_locals.raw.txt"); have.append("egod_locals.txt")
        objs = [o for o in L.get("egod_objects", "").split(" | ") if o]
        extra = f"Ego_d name `{en}` ({L.get('egod_records_same_name')} record(s); debug length {L.get('egod_debug_length')})"
    else:
        en = ""; objs = [L.get("likely_object", "")] if L.get("likely_object") else []
        extra = f"IDENTIFY: likely object {L.get('likely_object')}; named neighbours: {L.get('named_neighbours','')[:300]}"
    if objs: open(f"{R}/egod_tu.txt", "w").write(tu(objs, en)); have.append("egod_tu.txt")
    out = subprocess.run(["python3", f"{S}/tools/xdis.py", ad, hex(ln)], capture_output=True, text=True).stdout
    open(f"{R}/retail_dis.txt", "w").write(out); last = out.strip().split("\n")[-1][:60]
    lines.append(f"{n_}. 0x{ad.upper()} {mod}::{nm} ({ln:,} bytes by extent scan; last instruction `{' '.join(last.split()[2:])}`; "
                 f"residue {r['residue_class'] or '-'}; best on record {r['best_grade_on_record'][:120] or '-'})\n   {extra}\n   ref/{ad}/: retail_dis.txt" + "".join(", " + h for h in have))
with open(f"{A}/rows.tsv", "w") as f:
    f.write("address\tmodule\tname\tlength\n"); [f.write("\t".join(map(str, x)) + "\n") for x in rows]
subprocess.run(["python3", ROOT + "/tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/oracle_from_exe.py", f"{A}/rows.tsv", name], stdout=subprocess.DEVNULL)
block = "## Your rows, in this order\n" + "\n".join(lines) + "\n"
t = open(HERE + "/BRIEF_TEMPLATE.md").read().replace("@@ROWS@@\n", block).replace("@@A@@", A).replace("@@NAME@@", name).replace("@@S@@", S)
open(f"{A}/BRIEF.md", "w").write(t)
print("\n".join(lines)); print("wrote", f"{A}/BRIEF.md")
