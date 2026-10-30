#!/usr/bin/env python3
"""Wave 7 item 3: SB rows whose local CScriptBase models the retail members in full (STLport list / CVectorMap members,
the Ego_r names in PDB order) over a local CBaseClassNonCopyable (declaration-only, or the `{}` inline ctor/dtor form).
The preprocessing reduces the local CScriptBase to the shape mig_sb.py accepts (the six PDB members -> one 0x3c pad,
inline `{}` slot bodies -> declarations: the header declares them out of line, retail 0x00CBD4B0..0x00CBD4E0), turns a
local CLevelScriptBase<T> inline ctor into the declaration (the header defines the same inline ctor), drops the local
CBaseClassNonCopyable block (engine/CBaseClassNonCopyable.h is included by CScriptBase.h), then mig_sb.fix().
The compile and mcheck decide whether the row used the members (the header keeps them opaque).
usage: sb_full.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
import mig_sb
MEM = ["Threads", "EntityScripts", "EntityScriptBindings", "LastWorldUpdateFrame", "PActiveThread", "RuntimeScriptedEntityPersist"]
BCNC = [r"class CBaseClassNonCopyable ?: ?public CBaseClass ?\{ ?protected: ?CBaseClassNonCopyable\(\); ?virtual ~CBaseClassNonCopyable\(\); ?\};",
        r"class CBaseClassNonCopyable ?: ?public CBaseClass ?\{ ?protected: ?CBaseClassNonCopyable\(\) ?\{\} ?~CBaseClassNonCopyable\(\) ?\{\} ?private: ?CBaseClassNonCopyable\(const CBaseClassNonCopyable ?&\); ?CBaseClassNonCopyable ?& ?operator=\(const CBaseClassNonCopyable ?&\); ?\};"]
def fix(t):
    nb = find_block(t, "CBaseClassNonCopyable")
    kind = ""
    if nb:
        n = norm(t[nb[0]:nb[1]])
        k = [i for i, p in enumerate(BCNC) if re.fullmatch(p, n)]
        if not k: return None, "bcnc shape"
        kind = "bcnc%d" % k[0]
        if '#include "engine/CBaseClass.h"' not in t and not find_block(t, "CBaseClass"): return None, "no CBaseClass"
    b = find_block(t, "CScriptBase")
    if not b: return None, "noblock"
    blk = t[b[0]:b[1]]
    # member declarations: each `<type> Name;` for the six PDB members, in order
    body = blk[blk.index("{") + 1: blk.rindex("}")]
    pos = []
    for m in MEM:
        mm = re.search(r"(?:(?<=[;{}:])|^)\s*[^;{}]*?[\s*>]" + m + r"\s*;", body, re.M)
        if not mm: break
        pos.append(mm)
    if len(pos) == len(MEM):
        if [p.start() for p in pos] != sorted(p.start() for p in pos): return None, "member order"
        # remove the member declarations (the first replaced by the pad)
        newbody = body; first = True
        for p in sorted(pos, key=lambda p: -p.start()):
            pass
        spans = [(p.start(), p.end()) for p in pos]
        out_body = ""; last = 0
        for i, (s, e) in enumerate(spans):
            out_body += body[last:s] + ("\n    unsigned char _body[0x3c];" if i == 0 else ""); last = e
        out_body += body[last:]
        body = out_body; kind += " full"
    elif pos: return None, "partial members %d" % len(pos)
    body2 = re.sub(r"(virtual\s+void\s+(?:RegisterMain|Main|Init)\s*\(\s*\))\s*\{\s*\}", r"\1;", body)
    body2 = re.sub(r"(virtual\s+void\s+OnPersist\s*\(\s*CPersistContext\s*&\s*\w*\s*\))\s*\{\s*\}", r"\1;", body2)
    if body2 != body: kind += " slots"
    blk2 = blk[:blk.index("{") + 1] + body2 + blk[blk.rindex("}"):]
    out = t[:b[0]] + blk2 + t[b[1]:]
    lb = find_block(out, "CLevelScriptBase")
    if lb:
        l = out[lb[0]:lb[1]]
        l2 = re.sub(r"CLevelScriptBase\s*\(\s*CScriptDataBase\s*\*\s*(\w+)\s*,\s*CGameScriptInterfaceBase\s*&\s*(\w+)\s*\)\s*:\s*(?:CScriptBase\s*\(\s*\)\s*,\s*)?Game\s*\(\s*&\s*\2\s*\)\s*,\s*MasterScriptData\s*\(\s*\(\s*\w+\s*\*\s*\)\s*\1\s*\)\s*\{\s*\}",
                    r"CLevelScriptBase(CScriptDataBase* \1, CGameScriptInterfaceBase& \2);", l)
        if l2 != l: kind += " lsbctor"
        out = out[:lb[0]] + l2 + out[lb[1]:]
    if nb:
        nb = find_block(out, "CBaseClassNonCopyable")
        out = out[:nb[0]] + out[nb[1]:]
    r, why = mig_sb.fix(out)
    if r is None: return None, "mig_sb: " + why
    return r, why + " " + kind.strip()
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
