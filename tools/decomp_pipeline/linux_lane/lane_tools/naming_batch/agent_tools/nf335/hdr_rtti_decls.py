#!/usr/bin/env python3
"""hdr_rtti_decls.py (NF324, 2026-11-24): the add-only header layer for flatten_pilot.py v4's `header` bucket.
usage (repo root): python3 -u hdr_rtti_decls.py <GEN.tsv> <agent folder> [--only cls,cls] [--max-classes N]
For every class whose GEN.tsv rows say "engine/<cls>.h does not declare <meth> (add-only opt-in)":
  - the polymorphic class definition (`class <cls> : <base> {`) in the CURRENT repo header is located (exactly one; `#if` stack of plain
    `#ifdef` / `#if defined()` switches only, each defined by a landed row and none `_PASS`); a class written on one line is skipped (NF319 Q3);
  - an opt-in block `#ifdef FABLE_RETAIL_<CLS>_RTTI_DECLS` (or `..._NF324_RTTI_DECLS` when that macro already exists) goes just before the
    class's closing `};`: each RTTI-placed member in the PDB spelling (access, virtual, const, types from llvm-undname), then the access in
    force at that point is restored; declarations only;
  - the copy is written to <agent>/work/tree/engine/<cls>.h (a full copy of rebuild/include, made once) and <agent>/hdr/engine/<cls>.h, the
    original to <agent>/orig/hdr_<cls>.h, and moved asserts are pinned (lane_merge/pin_asserts.py on the copy);
  - an override probe (clang, the class switches + a landed precedent's preamble): for each member, a subclass declaring it `override` must
    compile against the ORIGINAL header (the base chain already declares that virtual, so the new declaration takes no new slot); a
    made-up-name control must fail. Members that fail are left out (and the class is skipped when none is left).
Output: <agent>/work/HDR_PLAN.tsv (cls, verdict, macro, switches, members, reason) and the tree / hdr / orig files."""
import os, re, sys, json, subprocess, shutil, collections, glob
argv = sys.argv[1:]
genf, A = argv[0], os.path.abspath(argv[1])
only = set(argv[argv.index("--only") + 1].split(",")) if "--only" in argv else None
S = os.environ.get("S", "")
def rd(p): return open(p, encoding="utf-8", errors="replace").read()
G = [l.split("\t") for l in rd(genf).splitlines()[1:]]
rows = [g for g in G if g[4] == "header" and "(add-only opt-in)" in g[5]]
bycls = collections.defaultdict(list)
for g in rows:
    m = re.match(r'\?([^@]+)@([^@]+)@@', g[3]); bycls[m.group(2)].append(g)
LM = collections.Counter(); alltext = {}
for f in glob.glob("rebuild/src/compiled/*/*/*.cpp"):
    t = rd(f); alltext[f] = t
    for m in set(re.findall(r'^\s*#\s*define\s+(FABLE_RETAIL_\w+)', t, re.M)): LM[m] += 1
pdbset = collections.defaultdict(set)
for fn in ("egor", "fablewin"):
    for l in open(f"ghidra_out/{fn}_pdb_names.tsv", encoding="utf-8", errors="replace"):
        q = l.rstrip("\n").split("\t")
        if len(q) > 1: pdbset[q[1]].add({"egor": "Ego_r.pdb", "fablewin": "FableWin.pdb"}[fn])
syms = [g[3] for g in rows]
und = {}
out = subprocess.run(["llvm-undname"], input="\n".join(syms) + "\n", capture_output=True, text=True).stdout.splitlines()
lines = [l for l in out if l.strip()]
for i, l in enumerate(lines):
    if l in syms and i + 1 < len(lines): und[l] = lines[i + 1]
def split_top(s):
    out, d, cur = [], 0, ""
    for ch in s:
        if ch in "(<[": d += 1
        elif ch in ")>]": d -= 1
        if ch == "," and d == 0: out.append(cur); cur = ""
        else: cur += ch
    if cur.strip(): out.append(cur)
    return [x.strip() for x in out]
def cpp_type(t):
    t = t.strip()
    m = re.match(r'(class|struct|enum|union) (\S+)( const)?\s*(\*|&)?( const)?$', t)
    if m: return ("const " if m.group(3) else "") + m.group(1) + " " + m.group(2) + (m.group(4) or "") + (" const" if m.group(5) else "")
    return re.sub(r'\s+([*&])', r'\1', t)
def decl_of(u):
    m = re.match(r'(public|protected|private): (virtual )?(.*?) __thiscall (.+)::([^:(]+)\((.*)\)( const)?$', u)
    if not m: return None
    ps = [] if m.group(6).strip() in ("void", "") else split_top(m.group(6))
    ret = m.group(3).strip(); ret = ret[5:] if ret.startswith("enum ") else cpp_type(ret)
    return m.group(1), bool(m.group(2)), f"{'virtual ' if m.group(2) else ''}{ret} {m.group(5)}({', '.join(cpp_type(p) for p in ps)}){' const' if m.group(7) else ''}"
def block_end(t, i):
    d = 1
    while d and i < len(t):
        d += {"{": 1, "}": -1}.get(t[i], 0); i += 1
    return i
def stack_at(t, pos):
    st = []
    for ln in t[:pos].splitlines():
        s = ln.strip()
        if re.match(r'#\s*if', s): st.append(s)
        elif re.match(r'#\s*(else|elif)', s) and st: st[-1] = "ELSE " + st[-1]
        elif re.match(r'#\s*endif', s) and st: st.pop()
    return st
def parse_stack(st):
    """an #if stack -> (switches to define, macros that must stay undefined) or None for a condition this tool does not read.
    `#ifdef X` / `#if defined(X)` / the #else of `#ifndef X` need X; `#if defined(A) || defined(B) ...` needs the first of them a landed row
    defines (no _PASS); `#ifndef X` / the #else of `#ifdef X` or of an `||` chain need them undefined."""
    need, forbid = [], set()
    for x in st:
        el = x.startswith("ELSE ")
        y = x[5:] if el else x
        m1 = re.match(r'#\s*ifdef\s+(\w+)\s*$', y) or re.match(r'#\s*if\s+defined\s*\(?\s*(\w+)\s*\)?\s*$', y)
        m2 = re.match(r'#\s*ifndef\s+(\w+)\s*$', y)
        ors = re.match(r'#\s*if\s+((?:!?\s*defined\s*\(\s*\w+\s*\)\s*(?:\|\|\s*)?)+)$', y)
        if m1: (forbid.add(m1.group(1)) if el else need.append(m1.group(1)))
        elif m2: (need.append(m2.group(1)) if el else forbid.add(m2.group(1)))
        elif ors and "!" not in ors.group(1):
            names = re.findall(r'defined\s*\(\s*(\w+)\s*\)', ors.group(1))
            if el: forbid.update(names)
            else:
                ok = [n for n in names if LM.get(n, 0) > 0 and "_PASS" not in n]
                if not ok: return None
                need.append(ok[0])
        else: return None
    return need, forbid
def precedent(cls, sws):
    best = None; inc = '#include "engine/' + cls + '.h"'
    for f, tx in alltext.items():
        if inc not in tx or not all(re.search(r'^\s*#\s*define\s+' + s + r'\b', tx, re.M) for s in sws): continue
        pre = [l.strip() for l in tx[:tx.index(inc)].splitlines() if re.match(r'\s*#\s*(define|include|undef)\b', l)
               and not re.match(r'\s*#\s*define\s+__(fastcall|stdcall|cdecl|thiscall)\b', l)]
        if best is None or len(pre) < len(best[0]): best = (pre, os.path.basename(f))
    return best
VCINC = f"{S}/wine/vc71rtm/include"
def clang_ok(text, tree):
    p = f"{A}/work/probe/p.cpp"; os.makedirs(os.path.dirname(p), exist_ok=True); open(p, "w").write(text)
    r = subprocess.run(["timeout", "60", "clang++", "-target", "i686-pc-windows-msvc", "-fms-extensions", "-fms-compatibility",
                        "-fms-compatibility-version=13.10", "-Xclang", "-fno-wchar", "-fsyntax-only", "-w", f"-I{tree}",
                        "-isystem", VCINC, p], capture_output=True, text=True, errors="replace")
    return r.returncode == 0, [l for l in r.stderr.splitlines() if "error" in l][:3]
tree = f"{A}/work/tree"
if not os.path.exists(tree): shutil.copytree("rebuild/include", tree, symlinks=True)
os.makedirs(f"{A}/hdr/engine", exist_ok=True); os.makedirs(f"{A}/orig", exist_ok=True)
plan = []
others_hdr = set()
for o in glob.glob(f"{S}/w22/agentNF3*/hdr/engine/*.h"):
    if "/agentNF324/" not in o: others_hdr.add(os.path.basename(o)[:-2])
for cls in sorted(bycls):
    if only and cls not in only: continue
    if cls in others_hdr: plan.append((cls, "SKIP", "", "", "", "another agent's hdr/ overlay has this header")); continue
    h = f"rebuild/include/engine/{cls}.h"; t = rd(h)
    if rd(f"{tree}/engine/{cls}.h") != t and not os.path.exists(f"{A}/hdr/engine/{cls}.h"):
        plan.append((cls, "SKIP", "", "", "", "tree copy differs from the repo header")); continue
    defs = [m for m in re.finditer(r'^[ \t]*(class|struct)\s+' + re.escape(cls) + r'\s*:(?!:)\s*([^{;]*)\{', t, re.M)]
    if len(defs) != 1: plan.append((cls, "SKIP", "", "", "", f"{len(defs)} polymorphic definitions")); continue
    d = defs[0]; b1 = block_end(t, d.end()) - 1
    if "\n" not in t[d.start():b1]: plan.append((cls, "SKIP", "", "", "", "class written on one line (NF319 Q3)")); continue
    st = [x for x in stack_at(t, d.start()) if not re.match(r'#\s*ifndef\s+\w+_H\w*\s*$', x)]
    pst = parse_stack(st)
    if pst is None: plan.append((cls, "SKIP", "", "", "", "class model under a complex #if: " + "; ".join(st)[:120])); continue
    sws, forbid = pst
    if any(LM.get(s, 0) == 0 or "_PASS" in s for s in sws): plan.append((cls, "SKIP", "", ",".join(sws), "", "class switch not defined by a landed row or _PASS (NF318 Q2)")); continue
    macro = f"FABLE_RETAIL_{cls.upper()}_RTTI_DECLS"
    if macro in t: macro = f"FABLE_RETAIL_{cls.upper()}_NF324_RTTI_DECLS"
    acc = "public" if d.group(1) == "struct" else "private"
    body = t[d.end():b1]
    for lab in re.finditer(r'^\s*(public|protected|private)\s*:', re.sub(r'//[^\n]*', '', body), re.M): acc = lab.group(1)
    if re.search(r'^\s*#\s*(if|else|elif|endif)', body[body.rfind("\n", 0, len(body.rstrip())) + 1:], re.M):
        pass
    pr = precedent(cls, sws)
    if pr: pr = ([l for l in pr[0] if not any(re.match(r'#\s*define\s+' + f + r'\b', l) for f in forbid)], pr[1])
    pre = "\n".join((pr[0] if pr else []) + [f"#define {s}" for s in sws if not (pr and f"#define {s}" in pr[0])]) + f'\n#include "engine/{cls}.h"\n'
    ok0, e0 = clang_ok(pre + "int nf324_probe_base;\n", "rebuild/include")
    if not ok0: plan.append((cls, "SKIP", macro, ",".join(sws), "", "probe preamble fails clang: " + (e0[0] if e0 else "?")[:150])); continue
    ctl, _ = clang_ok(pre + f"struct NF324Ctl : {cls} {{ virtual void NF324MadeUpName() override; }};\n", "rebuild/include")
    if ctl: plan.append((cls, "SKIP", macro, ",".join(sws), "", "control: a made-up override compiles (probe not discriminating)")); continue
    mem = []; bad = []
    for g in sorted(bycls[cls], key=lambda g: g[0]):
        dd = decl_of(und.get(g[3], ""))
        if not dd or not dd[1]: bad.append(g[0] + " not a virtual member"); continue
        if re.search(r'<|\(\*', dd[2]): bad.append(g[0] + " template / function-pointer type"); continue
        ok, e = clang_ok(pre + f"struct NF324Probe : {cls} {{ {dd[2].replace('virtual ', '', 1)} override; }};\n", "rebuild/include")
        if not ok: bad.append(g[0] + " override probe fails: " + (e[0] if e else "?")[-100:]); continue
        mem.append((g, dd))
    if not mem: plan.append((cls, "SKIP", macro, ",".join(sws), "", "; ".join(bad)[:300])); continue
    blk = [f"#ifdef {macro}",
           "    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement",
           "    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual",
           f"    // its base chain declares (clang override probe, made-up-name control fails): no new slot."]
    cur = None
    for g, (ac, v, dt) in mem:
        blk.append(f"    // 0x{g[0].upper()}: {g[3]} ({' + '.join(sorted(pdbset.get(g[3], {'?'})))})")
        if ac != cur: blk.append(f"{ac}:"); cur = ac
        blk.append(f"    {dt};")
    if cur != acc: blk.append(f"{acc}:")
    blk.append("#endif")
    ins = t[:b1].rstrip(" \t")
    nt = ins + ("" if ins.endswith("\n") else "\n") + "\n".join(blk) + "\n" + t[b1:]
    open(f"{A}/orig/hdr_{cls}.h", "w").write(t)
    open(f"{tree}/engine/{cls}.h", "w").write(nt)
    pin = subprocess.run(["python3", "tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py", "rebuild/include", tree, f"engine/{cls}.h"],
                         capture_output=True, text=True).stdout.strip().splitlines()
    shutil.copy(f"{tree}/engine/{cls}.h", f"{A}/hdr/engine/{cls}.h")
    ok, e = clang_ok(pre.replace("#include", f"#define {macro}\n#include", 1) + "int nf324_probe_after;\n", tree)
    plan.append((cls, "EDIT" if ok else "SKIP", macro, ",".join(sws), ",".join(g[0] for g, _ in mem),
                 ("pinned: " + (pin[-1] if pin else "?") + ("; left out: " + "; ".join(bad) if bad else "")) if ok else "edited header fails clang: " + (e[0] if e else "?")[:150]))
    if not ok:   # restore the tree copy; the hdr / orig copies are moved aside by name (no unlink of a variable path)
        open(f"{tree}/engine/{cls}.h", "w").write(t)
        os.replace(f"{A}/hdr/engine/{cls}.h", f"{A}/work/hdr_failed_{cls}.h"); os.replace(f"{A}/orig/hdr_{cls}.h", f"{A}/work/orig_failed_{cls}.h")
with open(f"{A}/work/HDR_PLAN.tsv", "a") as o:
    for p in plan: o.write("\t".join(p) + "\n")
print(collections.Counter(p[1] for p in plan))
for p in plan:
    if p[1] != "EDIT": print(p[0], p[5][:160])
