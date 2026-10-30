#!/usr/bin/env python3
"""Wave 2 family CGSI (interface-only rows): a row-local CGameScriptInterface stand-in whose block is only virtual slot
declarations (invented `vNNN` fillers plus the slots the row calls) becomes the shared retail model
(FABLE_RETAIL_NSCRIPT: engine/CGameScriptInterface.h, vtable 0x01260F0C, 918 slots). Every named slot the local block
declares must exist in the header under the same name at the same slot index (the slot index is counted from the local
block's declaration order); otherwise the row is skipped. Rows that also define CGameScriptInterfaceBase / CBaseClass /
CBaseClassNonCopyable locally are skipped. Idempotent. Wave 3: --rename also accepts invented names at a proven slot index (RENAME below: slot 7 is NewScriptFrame 0x006E7410)
and rewrites the row's calls through a CGameScriptInterface pointer to the real name; a named slot the row never calls is
treated as a filler (its name does not reach the bytes: virtual calls carry no relocation).
usage: mig_cgsi.py [--dry] [--rename] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
R = os.environ["FABLE_ROOT"]
H = open(R + "/rebuild/include/engine/CGameScriptInterface.h").read()
H = H[H.index("class CGameScriptInterface : public CGameScriptInterfaceBase {"):]
SLOT = {}
for m in re.finditer(r"virtual [^;]*?\b(~?\w+)\(([^;]*)\)[^;]*;\s*//\s*(\d+) 0x", H):
    SLOT.setdefault(m.group(1), set()).add(int(m.group(3)))
SLOT.setdefault("~CGameScriptInterface", set()).add(0)
DECL = re.compile(r"virtual\s+([^;{}]*?)(~\w+|\b\w+)\s*\(([^;{}]*)\)\s*(const)?\s*(?:=\s*0)?\s*;")
REPL = ("// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:\n"
        "//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).\n"
        "#define FABLE_RETAIL_NSCRIPT\n#include \"engine/CGameScriptInterface.h\"\n")
OTHER = ("CGameScriptInterfaceBase", "CBaseClass", "CBaseClassNonCopyable")
RENAME = {7: {"Update": "NewScriptFrame", "YieldFrame": "NewScriptFrame", "Pump": "NewScriptFrame"}}
DO_RENAME = "--rename" in sys.argv
def called(t, name): return re.search(r"(?:->|\.)" + re.escape(name) + r"\s*\(", t) is not None
def fix(t):
    if re.search(r"^#include \"engine/CGameScriptInterface.h\"", t, re.M): return None, "already"
    b = find_block(t, "CGameScriptInterface")
    if not b: return None, "noblock"
    blk = t[b[0]:b[1]]
    n = norm(blk)
    m = re.match(r"^(?:class|struct) CGameScriptInterface ?\{ ?public: ?(.*)\};$", n)
    if not m: return None, "head"
    body = m.group(1)
    decls = list(DECL.finditer(body))
    if DECL.sub("", body).replace("public:", "").strip(): return None, "non-virtual items"
    idx = 0; named = []; ren = []
    for d in decls:
        name = d.group(2)
        if name.startswith("~"):
            if idx != 0: return None, "dtor not first"
            idx += 1; continue
        if re.fullmatch(r"[vsg]\d+|VFunc\d+|vf\d+|Slot\d+|UnknownSlot\d+", name): idx += 1; continue
        if idx not in SLOT.get(name, ()):
            if DO_RENAME and name in RENAME.get(idx, {}):
                ren.append((name, RENAME[idx][name])); named.append(name + "->" + RENAME[idx][name]); idx += 1; continue
            if DO_RENAME and not called(t[:b[0]] + t[b[1]:], name): idx += 1; continue
            return None, f"slot {idx} {name} not in header"
        named.append(name); idx += 1
    for c in OTHER:
        if find_block(t, c): return None, "local " + c
    out = t[:b[0]] + REPL + t[b[1]:]
    for old, new in ren:
        recv = set(re.findall(r"CGameScriptInterface\s*\*\s*(?:const\s+)?(\w+)", t))
        for r in recv: out = re.sub(r"\b" + r + r"(\s*->\s*)" + old + r"(\s*\()", r + r"\g<1>" + new + r"\g<2>", out)
        if called(out, old): return None, f"call to {old} through an untyped receiver"
    return out, "ok " + ",".join(named)
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
