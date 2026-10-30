#!/usr/bin/env python3
"""Wave 7: two leftover local CScriptBase shapes.
(a) `namespace NScript { class CScriptBase { ~; v01..v03; OnPersist; unsigned char _base[0x48 - 4]; }; class X : public
    CScriptBase` -- the level-script base chain folded into one 0x48 stand-in (inside NScript). It becomes a global 0x40
    CScriptBase + non-template CLevelScriptBase (Game, MasterScriptData) before the namespace, X derives from it, then
    lsb_nt.fix() gives CLevelScriptBase<T> from the shared header.
(b) a methods-only stand-in (no data members; filler virtuals / ctor / method declarations): the 0x3c pad and the dtor are
    added so mig_sb.fix() accepts it (the header's members are opaque; the compile and mcheck decide).
usage: sb48.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
import mig_sb, lsb_nt
A = "class CScriptBase { public: virtual ~CScriptBase(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void OnPersist(CPersistContext& context); unsigned char _base[0x48 - 4]; };"
def fix(t):
    b = find_block(t, "CScriptBase")
    if not b: return None, "noblock"
    n = norm(t[b[0]:b[1]])
    if n == A:
        m = re.search(r"namespace NScript\s*\{\s*$", t[:b[0]])
        if not m: return None, "a: not after namespace NScript {"
        body = t[m.end():]
        out = t[:b[0]] + t[b[1]:]
        code = re.sub(r"//[^\n]*", "", out)
        for v in ("v01", "v02", "v03"):
            if re.search(r"(?:\.|->)\s*" + v + r"\s*\(", code): return None, "a: filler called"
        out, k = re.subn(r"(public\s+)CScriptBase\b", r"\1CLevelScriptBase", out)
        if k != 1: return None, "a: derived %d" % k
        pre = ("class CGameScriptInterfaceBase;\nclass CScriptDataBase;\n"
               "class CScriptBase { public: virtual ~CScriptBase(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void OnPersist(CPersistContext& context); unsigned char _body[0x3c]; };\n"
               "class CLevelScriptBase : public CScriptBase {\npublic:\n    CGameScriptInterfaceBase* Game;\n    CScriptDataBase* MasterScriptData;\n};\n")
        out = out[:m.start()] + pre + out[m.start():]
        r, why = lsb_nt.fix(out)
        return (r, "a " + why) if r else (None, "a: " + why)
    m = re.match(r"^(?:class|struct) CScriptBase ?\{(.*)\};$", n)
    if not m: return None, "head"
    its = mig_sb.items(m.group(1))
    if its is None: return None, "parse"
    if any(re.fullmatch(r"[^()]*[\w\]]\s*;", i) and not re.fullmatch(r"(public|protected|private)\s*:", i) for i in its):
        return None, "b: has data members"
    hasd = any("~CScriptBase" in i for i in its)
    blk = t[b[0]:b[1]]
    j = blk.rindex("}")
    add = ("" if hasd else " virtual ~CScriptBase();") + " unsigned char _body[0x3c];"
    blk2 = blk[:j] + "public:" + add + "\n" + blk[j:]
    out = t[:b[0]] + blk2 + t[b[1]:]
    r, why = mig_sb.fix(out)
    return (r, "b " + why) if r else (None, "b: " + why)
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
