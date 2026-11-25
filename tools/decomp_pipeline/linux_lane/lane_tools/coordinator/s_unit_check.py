#!/usr/bin/env python3
"""s_unit_check.py [repo_root] [--objdir DIR] [--quiet]: check every option S unit in the catalog (post-landing baseline).

Option S (owner ruling, advisor decision 2026-11-24; pilot NF376, tool NF379): one retail translation unit serves several catalog addresses.
The OWNER row's Source holds every body; each other member is a THIN row whose Source is only comments, its own `// Leaf: <full symbol>`
and `#include "../src/compiled/<owner Source>"` (the EH-funclet / 0x00B2AB10 shape), with its own catalog entry, TestSource and oracle.
A thin row is an S member when its text says `Option S`. Per unit (owner + members) one line:
  OK    every check passed;
  FAIL  with the reasons:
    - the include target is missing, is not the Source of a catalog row, or is itself thin (one level only);
    - CompilerFlags / Compiler differ from the owner's (one object, one flag set);
    - a member's or the owner's `// Leaf:` is missing or not a full mangled symbol (`?...`);
    - an EXPLICIT member (its function is written in the owner, `Scope::Name(` / `Name(`) is not in retail address order in the owner text;
    - with --objdir: a member's Leaf is not a defined (DEF) symbol of the unit object (DIR/<addr>.obj, or DIR/<a:2>/<a2:4>/<a>/<a>.obj as
      land_scripted / vland leave them; any member's object will do, they are the same TU). A unit with no object there is reported
      `DEF not checked`, not failed.
  Members the owner text does not spell (compiler-emitted: ??_G / ??_E, an implicit or inline-from-header ??0 / ??1 / ??4, template
  instances) are listed as `implicit` on the line. They have no text position, so the order check skips them, and without --objdir they
  are NOT verified (the summary counts them); with --objdir they must be DEF symbols of the unit object.
Exit status 1 when any unit FAILs (a gate), else 0. The summary line: `s units: N checked, M failed, I implicit members (V verified)`."""
import os, re, sys, subprocess
args = [a for a in sys.argv[1:] if not a.startswith("--")]
root = args[0] if args else "."
objdir = sys.argv[sys.argv.index("--objdir") + 1] if "--objdir" in sys.argv else None
if objdir and objdir in args: args.remove(objdir); root = args[0] if args else "."
quiet = "--quiet" in sys.argv
comp = os.path.join(root, "rebuild", "src", "compiled")
ps = open(os.path.join(root, "rebuild", "build_candidates.ps1"), encoding="utf-8-sig", errors="replace").read()
ent = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if kv.get("Address") and kv.get("Source"): ent[kv["Address"].lower().replace("0x", "")] = kv
by_src = {}
for a, kv in ent.items(): by_src.setdefault(kv["Source"], []).append(a)
INC = re.compile(r'^[ \t]*#[ \t]*include[ \t]+"\.\./src/compiled/([^"]+)"', re.M)
def text(src):
    try: return open(os.path.join(comp, src), encoding="utf-8", errors="replace").read()
    except OSError: return None
def code_of(t): return re.sub(r"//[^\n]*", "", t or "")
def thin(t):
    c = code_of(t); m = INC.findall(c)
    return m[0] if len(m) == 1 and not re.sub(r'#[ \t]*include[ \t]+"[^"]+"', "", c).strip() else None
def leaf(t):
    m = re.search(r"^//\s*Leaf:\s*(\S+)", t or "", re.M); return m.group(1) if m else ""
OPS = {"4": "operator=", "8": "operator==", "9": "operator!=", "A": "operator[]", "R": "operator()", "M": "operator<", "H": "operator+",
       "G": "operator-", "D": "operator*", "K": "operator/", "Y": "operator+=", "Z": "operator-=", "C": "operator->", "2": "operator new",
       "3": "operator delete"}
def spelled(sym):
    """(regex of the definition in source, label) for a mangled symbol the owner can spell, else None (compiler-emitted / template)."""
    if sym.startswith("??_") or "?$" in sym: return None                     # ??_G / ??_E / ??_7 ..., template instances
    m = re.match(r"\?\?([0-9A-Z])([A-Za-z_]\w*)@((?:[A-Za-z_]\w*@)*)@", sym)
    if m:                                                                     # ??0 / ??1 / operators: innermost scope is the class
        code, first, rest = m.groups()
        if code in "01":
            cls = first; nm = ("~" if code == "1" else "") + cls
        elif code in OPS:
            scopes = [first] + [s for s in rest.split("@") if s]
            cls = scopes[0]; nm = OPS[code]
        else: return None
        return r"(?<![\w:~])" + re.escape(cls) + r"\s*::\s*" + re.escape(nm) + r"\s*\(", f"{cls}::{nm}"
    m = re.match(r"\?([A-Za-z_]\w*)@((?:[A-Za-z_]\w*@)*)@", sym)
    if not m: return None
    name, scopes = m.group(1), [s for s in m.group(2).split("@") if s]
    if scopes:                                                                # member or namespace function: Class::Name(
        return r"(?<![\w:])" + re.escape(scopes[0]) + r"\s*::\s*" + re.escape(name) + r"\s*\(", f"{scopes[0]}::{name}"
    return r"(?<![\w:.>])" + re.escape(name) + r"\s*\(", name                  # free function: its first spelling
def defs(obj):
    try: out = subprocess.run([os.environ.get("FABLE_OBJDUMP", "objdump"), "-t", obj], capture_output=True, text=True).stdout
    except OSError: return None
    s = set()
    for l in out.splitlines():
        m = re.search(r"\(sec\s+(-?\d+)\).*\(scl\s+(\d+)\)\s+\(nx\s+\d+\)\s+0x[0-9a-f]+\s+(\S+)$", l)
        if m and int(m.group(1)) > 0 and m.group(2) in ("2", "3"): s.add(m.group(3))
    return s
def unit_obj(rows):
    if not objdir: return None
    for a in rows:
        for p in (os.path.join(objdir, a + ".obj"), os.path.join(objdir, a[:2], a[2:4], a, a + ".obj")):
            if os.path.exists(p): return p
    return None
units = {}
for a, kv in sorted(ent.items()):
    t = text(kv["Source"])
    if not t or "Option S" not in t: continue
    own = thin(t)
    if own: units.setdefault(own, []).append(a)
n = bad = n_impl = n_ver = 0
for own, mem in sorted(units.items()):
    n += 1; why = []; impl = []
    owners = by_src.get(own, []); ot = text(own)
    if ot is None: why.append("owner file missing")
    elif not owners: why.append("owner is not a catalog Source")
    elif thin(ot): why.append("owner is itself thin")
    o = owners[0] if owners else None
    rows = ([o] if o else []) + mem
    for a in mem:
        for k in ("CompilerFlags", "Compiler"):
            if o and ent[a].get(k, "") != ent[o].get(k, ""): why.append(f"{a} {k} {ent[a].get(k, '')!r} != owner {ent[o].get(k, '')!r}")
    oc = code_of(ot); pos = []
    obj = unit_obj(rows); ds = defs(obj) if obj else None
    defnote = "" if not objdir else (" (objects: DEF checked)" if ds is not None else " (no object under --objdir: DEF not checked)")
    for a in rows:
        lf = leaf(text(ent[a]["Source"]))
        if not (lf.startswith("?") or re.fullmatch(r"_\$E\d+", lf)):   # `_$E<n>`: the compiler's dynamic-initializer symbol is complete (one per object)
            why.append(f"{a} leaf {lf!r} is not a full symbol"); continue
        if ds is not None and lf not in ds: why.append(f"{a} leaf not defined in the unit object")
        sp = spelled(lf)
        m = re.search(sp[0], oc) if sp else None
        if not m:
            impl.append(a); n_impl += 1; n_ver += ds is not None and lf in ds
            continue
        pos.append((int(a, 16), m.start(), a))
    if [p[2] for p in sorted(pos)] != [p[2] for p in sorted(pos, key=lambda p: p[1])]:
        why.append("owner text order " + " ".join(p[2] for p in sorted(pos, key=lambda p: p[1])) + " is not retail address order")
    bad += bool(why)
    if not quiet or why:
        print(("FAIL " if why else "OK   ") + f"owner {o} {own} members {' '.join(mem)}"
              + (f" implicit {' '.join(impl)}{' (DEF ok)' if ds is not None else ' (not verified)'}" if impl else "") + defnote
              + ("" if not why else " :: " + "; ".join(why)))
print(f"s units: {n} checked, {bad} failed, {n_impl} implicit members ({n_ver} verified)")
sys.exit(1 if bad else 0)
