#!/usr/bin/env python3
"""s_unit_check.py [repo_root]: check every option S unit in the catalog (proposed home: tools/decomp_pipeline/linux_lane/lane_tools/coordinator/).

Option S (owner ruling, advisor decision 2026-11-24; pilot NF376): one retail translation unit serves several catalog addresses. The
OWNER row's Source holds every body; each other member is a THIN row whose Source is only comments, its own `// Leaf: <full symbol>` and
`#include "../src/compiled/<owner Source>"` (the EH-funclet / 0x00B2AB10 shape), with its own catalog entry, TestSource and oracle.
A thin row is an S member when it carries a `// Leaf:` and its text says `Option S`. For each one this prints a line and FAILs when:
  - the include target is missing or is not the Source of a catalog row (the owner);
  - the owner is itself thin (one level only);
  - CompilerFlags / Compiler differ from the owner's (one object, one flag set);
  - the member's Leaf is not a full mangled symbol, or its function is not defined in the owner text (`Class::Name(` heuristic);
  - the owner text does not define the unit's functions in retail address order (owner + members, by their `Class::Name(`
    definition positions).
Exit 0 always; the summary line says `s units: N checked, M failed`."""
import os, re, sys
root = sys.argv[1] if len(sys.argv) > 1 else "."
comp = os.path.join(root, "rebuild", "src", "compiled")
ps = open(os.path.join(root, "rebuild", "build_candidates.ps1"), encoding="utf-8-sig", errors="replace").read()
ent = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if kv.get("Address") and kv.get("Source"): ent[kv["Address"].lower()] = kv
by_src = {}
for a, kv in ent.items(): by_src.setdefault(kv["Source"], []).append(a)
INC = re.compile(r'^\s*#include\s+"\.\./src/compiled/([^"]+)"', re.M)
def text(src):
    try: return open(os.path.join(comp, src), encoding="utf-8", errors="replace").read()
    except OSError: return None
def thin(t):
    code = re.sub(r"//[^\n]*", "", t)
    m = INC.findall(code)
    return m[0] if len(m) == 1 and not re.sub(r'#include\s+"[^"]+"', "", code).strip() else None
def leaf(t):
    m = re.search(r"^//\s*Leaf:\s*(\S+)", t, re.M); return m.group(1) if m else ""
def defname(sym):                                   # ?Name@Class@@... -> Class::Name ; ??0Class@@ / ??1Class@@ -> Class::Class / Class::~Class
    m = re.match(r"\?\?([01])([A-Za-z_]\w*)@@", sym)
    if m: return m.group(2) + "::" + ("~" if m.group(1) == "1" else "") + m.group(2)
    m = re.match(r"\?([A-Za-z_]\w*)@([A-Za-z_]\w*)@@", sym)
    return (m.group(2) + "::" + m.group(1)) if m else None
units = {}; bad = 0; n = 0
for a, kv in sorted(ent.items()):
    t = text(kv["Source"])
    if not t or "Option S" not in t: continue
    own = thin(t)
    if not own: continue
    units.setdefault(own, []).append(a)
for own, mem in sorted(units.items()):
    n += 1; why = []
    owners = by_src.get(own, [])
    ot = text(own)
    if ot is None: why.append("owner file missing")
    elif not owners: why.append("owner is not a catalog Source")
    elif thin(ot): why.append("owner is itself thin")
    o = owners[0] if owners else None
    rows = ([o] if o else []) + mem
    for a in mem:
        for k in ("CompilerFlags", "Compiler"):
            if o and ent[a].get(k, "") != ent[o].get(k, ""): why.append(f"{a} {k} {ent[a].get(k,'')!r} != owner {ent[o].get(k,'')!r}")
    pos = []
    for a in rows:
        lf = leaf(text(ent[a]["Source"]) or "")
        dn = defname(lf) if lf.startswith("?") else None
        if not dn: why.append(f"{a} leaf {lf!r} is not a full symbol"); continue
        m = re.search(r"(?<![\w:])" + re.escape(dn) + r"\s*\(", re.sub(r"//[^\n]*", "", ot or ""))
        if not m: why.append(f"{a} {dn} not defined in the owner"); continue
        pos.append((int(a, 16), m.start(), a))
    if [p[2] for p in sorted(pos)] != [p[2] for p in sorted(pos, key=lambda p: p[1])]:
        why.append("owner text order " + " ".join(p[2] for p in sorted(pos, key=lambda p: p[1])) + " is not retail address order")
    bad += bool(why)
    print(("FAIL " if why else "OK   ") + f"owner {o} {own} members {' '.join(mem)}" + ("" if not why else " :: " + "; ".join(why)))
print(f"s units: {n} checked, {bad} failed")
