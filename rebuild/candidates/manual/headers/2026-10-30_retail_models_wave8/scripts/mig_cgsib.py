#!/usr/bin/env python3
"""Wave 5 family CGSIB: a row-local CGameScriptInterfaceBase (the abstract script interface, retail vtable 0x0126009C,
RTTI : CBaseClassNonCopyable : CBaseClass) whose block is only virtual slot declarations becomes the shared retail model
engine/CGameScriptInterfaceBase.h (FABLE_RETAIL_NSCRIPT). Slot rules are mig_cgsi.py's (--rename / --macros): a named
slot must sit at the header's index for that name, an invented name the row never calls is a filler, the slot 7
NewScriptFrame respelling applies. An empty local `class CGameScriptInterface : public CGameScriptInterfaceBase { };`
is replaced too (the include is then engine/CGameScriptInterface.h, whose class derives from the same base). A non-empty
local CGameScriptInterface, a local CBaseClass / CBaseClassNonCopyable or any other non-virtual item skips the row.
Accepted fillers: slot macros (also invoked with a trailing `;`), empty-bodied virtuals, a default-ctor declaration
(the header's implicit ctor; mcheck decides).
usage: mig_cgsib.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mig_cgsi as M
from common import find_block, norm
BASE = "CGameScriptInterfaceBase"
RB = ("// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT\n"
      "//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).\n"
      "#define FABLE_RETAIL_NSCRIPT\n#include \"engine/CGameScriptInterfaceBase.h\"\n")
RD = ("// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers\n"
      "//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).\n"
      "#define FABLE_RETAIL_NSCRIPT\n#include \"engine/CGameScriptInterface.h\"\n")
def fix(t):
    if re.search(r'^\s*#include "engine/CGameScriptInterface(Base)?.h"', t, re.M): return None, "already"
    b = find_block(t, BASE)
    if not b: return None, "noblock"
    n = norm(t[b[0]:b[1]])
    m = re.match(r"^(?:class|struct) " + BASE + r" ?(?:: ?public CBaseClassNonCopyable ?)?\{ ?(?:public: ?)?(.*)\};$", n)
    if not m: return None, "head"
    body = m.group(1)
    defs, _ = M.macro_counts(t)
    if defs: body = re.sub(r"\b(" + "|".join(map(re.escape, defs)) + r")\s*\(([^()]*)\)\s*;", r"\1(\2) ", body)
    body = M.expand_macros(t, body)
    body = re.sub(r"(\bvirtual\b[^;{}]*\))\s*(const)?\s*\{\s*\}\s*;?", lambda q: q.group(1) + (" const" if q.group(2) else "") + ";", body)
    body = re.sub(r"(?<![~\w])" + BASE + r"\s*\(\s*\)\s*(?:\{\s*\}\s*;?|;)", "", body)
    decls = list(M.DECL.finditer(body))
    if re.sub(r"\b(public|protected|private):", "", M.DECL.sub("", body)).strip(): return None, "non-virtual items"
    outside = t[:b[0]] + t[b[1]:]
    idx = 0; ren = []
    for d in decls:
        name = d.group(2)
        if name.startswith("~"):
            if idx != 0: return None, "dtor not first"
            idx += 1; continue
        if re.fullmatch(r"[vsgV]\d+|VFunc\d+|vf\d+|Slot\d+|UnknownSlot\d+|slot\d+|pure\d+", name): idx += 1; continue
        if idx not in M.SLOT.get(name, ()) or idx > 916:
            if name in M.RENAME.get(idx, {}): ren.append((name, M.RENAME[idx][name])); idx += 1; continue
            if not M.called(outside, name): idx += 1; continue
            return None, f"slot {idx} {name} not in header"
        idx += 1
    if idx > 917: return None, f"{idx} slots"
    for c in ("CBaseClass", "CBaseClassNonCopyable"):
        if find_block(t, c): return None, "local " + c
    out = t[:b[0]] + "@@BASE@@\n" + t[b[1]:]
    repl = RB
    d = find_block(out, "CGameScriptInterface")
    if d:
        dn = norm(out[d[0]:d[1]])
        if not re.fullmatch(r"(?:class|struct) CGameScriptInterface ?: ?public " + BASE + r" ?\{ ?(?:public: ?)?\};", dn):
            return None, "local CGameScriptInterface with members"
        out = out[:d[0]] + out[d[1]:]; repl = RD
    out = out.replace("@@BASE@@\n", repl)
    for old, new in ren:
        recv = set(re.findall(r"CGameScriptInterface(?:Base)?\s*\*\s*(?:const\s+)?(\w+)", t))
        for r in recv: out = re.sub(r"\b" + r + r"(\s*->\s*)" + old + r"(\s*\()", r + r"\g<1>" + new + r"\g<2>", out)
        if M.called(out, old): return None, f"call to {old} through an untyped receiver"
    return out, "ok" + (" +derived" if repl is RD else "")
if __name__ == "__main__":
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if "--dry" not in sys.argv: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
