#!/usr/bin/env python3
"""Wave 5: CGSI rows whose local block carries data members next to the slots (mig_cgsi.py skips them as
"non-virtual items"). The block becomes engine/CGameScriptInterface.h when:
  * every virtual passes mig_cgsi's slot rules (--rename / --macros semantics);
  * every other item is a header data member (same name, same offset as the header computes it from the local
    block: vptr 4 when the block has a virtual or a `_vptr` word, natural alignment), an unnamed pad array, or the
    protected `long GetMaxWorldFrameForMessages() const` (0x006E7530, both PDBs), or a header slot the row defines but
    never calls, declared non-virtual in the local block (the definition compiles the same under the header's virtual);
  * a member the row uses keeps a usable type: `X_Retail*` stand-ins of the header's member class are respelled to
    the class itself (X_Retail -> X across the row, struct -> class), a local CDefinitionManager(_Retail) gets the
    landed `class CGameDefinitionManager : public CDefinitionManager { };` so DefinitionManager keeps its methods;
    `void*` / `long` stand-ins are left to the compiler and mcheck.
Wave 6 additions: GetMinWorldFrameForMessages (protected, wave 10 spelling) and GetScriptSpriteBySymbol 0x006E77F0
(rows spelled it GetTextIDFromSymbol: the declaration goes, code calls are respelled) are header members; a row-local
inline accessor `T* Name() const { return Member; }` / `T& Name() const { return *Member; }` over a header member goes and
its unqualified calls become `Member` / `Member->`; the `#define CGameScriptInterface CGameScriptInterface<Alias>` /
include / `#undef` trio is dropped first when the alias name has no other use; `m_pad*` / `OwnerFields` are pads.
usage: mig_cgsi_m.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.argv_saved = list(sys.argv)
import mig_cgsi as M
M.DO_RENAME = True; M.DO_MACROS = True
from common import find_block, norm
R = os.environ["FABLE_ROOT"]
HT = open(R + "/rebuild/include/engine/CGameScriptInterface.h").read()
HT = HT[HT.index("#ifdef FABLE_RETAIL_NSCRIPT"):HT.index("\n#else")]
HMEM = {}
for m in re.finditer(r"^\s+([\w:<>, ]+?\*?)\s+(\w+)(\[[^\]]*\])?;\s*// \+0x([0-9a-f]+)", HT, re.M):
    HMEM[m.group(2)] = (int(m.group(4), 16), m.group(1).replace(" ", ""))
PAD = re.compile(r"_?(m_pad|pad|members|body|unk|gap|ownerfields|fields)\w*", re.I)
SZ = {"long": 4, "int": 4, "unsigned long": 4, "unsigned int": 4, "float": 4, "bool": 1, "char": 1, "unsigned char": 1,
      "signed char": 1, "short": 2, "unsigned short": 2}
CLS = {"World": "CWorld", "Component": "CMainGameComponent", "DisplayEngine": "CDisplayEngine",
       "PlayerManager": "CPlayerManager", "ThingSearchTools": "CThingSearchTools"}
def ev(e):
    if not re.fullmatch(r"[0-9a-fA-FxX+\-*() ]+", e): raise ValueError(e)
    return int(eval(re.sub(r"0[xX]([0-9a-fA-F]+)", lambda m: str(int(m.group(1), 16)), e)))
def defined(t, name): return re.search(r"(?:class|struct)\s+" + name + r"\b[^;{]*\{", t) is not None
ALIAS = re.compile(r'[ \t]*#define CGameScriptInterface (CGameScriptInterface\w+)[^\n]*\n[ \t]*#include "engine/CGameScriptInterface.h"[^\n]*\n[ \t]*#undef CGameScriptInterface[^\n]*\n')
ACC = re.compile(r"(?:const\s+)?[\w:]+\s*([*&])\s*(\w+)\(\s*\)\s*const\s*\{\s*return\s*(\*?)\s*(\w+)\s*;?\s*\}")
def drop_alias(t):
    m = ALIAS.search(t)
    if not m: return t, ""
    rest = t[:m.start()] + t[m.end():]
    if re.search(r"\b" + m.group(1) + r"\b", re.sub(r"//[^\n]*", "", rest)): return None, m.group(1)
    return rest, m.group(1)
def fix(t):
    t, alias_name = drop_alias(t)
    if t is None: return None, "alias used: " + alias_name
    if re.search(r'^\s*#include "engine/CGameScriptInterface.h"', t, re.M): return None, "already"
    b = find_block(t, "CGameScriptInterface")
    if not b: return None, "noblock"
    n = norm(t[b[0]:b[1]])
    m = re.match(r"^(?:class|struct) CGameScriptInterface ?\{ ?(?:public: ?)?(.*)\};$", n)
    if not m: return None, "head"
    body = M.expand_macros(t, m.group(1))
    body = re.sub(r"\{\s*return\s*(\*?)\s*(\w+)\s*;\s*\}", r"{ return \1\2 }", body)
    decls = list(M.DECL.finditer(body))
    rest = M.DECL.sub(";", body)
    rest = re.sub(r"\b(public|protected|private):", ";", rest)
    items = [x.strip() for x in rest.split(";") if x.strip()]
    poly = bool(decls) or any(re.fullmatch(r"void\s*\*\s*_?_?v(ptr|ftable|table)\w*", x) for x in items)
    off = 4 if decls else 0; used = []; members = {}
    accs = []; sprite = False
    for it in items:
        if re.fullmatch(r"long Get(?:Max|Min)WorldFrameForMessages\(\s*\) const", it): continue
        if re.fullmatch(r"long (?:GetTextIDFromSymbol|GetScriptSpriteBySymbol)\(\s*const CCharString\s*&\s*\w*\s*\) const", it):
            sprite = True; continue
        am = ACC.fullmatch(it + ";") or ACC.fullmatch(it)
        if am and am.group(4) in HMEM and (am.group(1) == "&") == (am.group(3) == "*"):
            accs.append((am.group(2), am.group(4), am.group(1))); continue
        fm = re.fullmatch(r"[^(){}]*?\b(\w+)\s*\([^(){}]*\)\s*(const)?", it)
        if fm and fm.group(1) in M.SLOT and not M.called(t[:b[0]] + t[b[1]:], fm.group(1)):
            continue   # a slot the row defines (declared non-virtual locally): the header's virtual compiles the same body
        mm = re.fullmatch(r"((?:const\s+)?(?:unsigned\s+|signed\s+)?[\w:]+)\s*(\*?)\s*(\w+)\s*(?:\[([^\]]+)\])?", it)
        if not mm: return None, "item: " + it[:60]
        ty, ptr, name, arr = mm.group(1).strip(), mm.group(2), mm.group(3), mm.group(4)
        if ptr: sz, al = 4, 4
        elif ty in SZ: sz = al = SZ[ty]
        else: return None, "type: " + it[:60]
        try: cnt = ev(arr) if arr else 1
        except Exception: return None, "array: " + it[:60]
        off = (off + al - 1) // al * al
        if re.fullmatch(r"_?_?v(ptr|ftable|table)\w*", name):
            if off != 0: return None, "vptr not first"
        elif name in HMEM:
            if HMEM[name][0] != off: return None, f"{name} at +{off:#x}, header +{HMEM[name][0]:#x}"
            members[name] = (ty + ptr).replace(" ", "")
        elif not PAD.fullmatch(name): return None, "member " + name
        off += sz * cnt
    # slots (mig_cgsi rules)
    idx = 0; ren = []
    outside = t[:b[0]] + t[b[1]:]
    for d in decls:
        name = d.group(2)
        if name.startswith("~"):
            if idx != 0: return None, "dtor not first"
            idx += 1; continue
        if re.fullmatch(r"[vsg]\d+|VFunc\d+|vf\d+|Slot\d+|UnknownSlot\d+", name): idx += 1; continue
        if idx not in M.SLOT.get(name, ()):
            if name in M.RENAME.get(idx, {}): ren.append((name, M.RENAME[idx][name])); idx += 1; continue
            if not M.called(outside, name): idx += 1; continue
            return None, f"slot {idx} {name} not in header"
        idx += 1
    for c in M.OTHER:
        if find_block(t, c): return None, "local " + c
    out = t[:b[0]] + M.REPL + t[b[1]:]
    for old, new in ren:
        recv = set(re.findall(r"CGameScriptInterface\s*\*\s*(?:const\s+)?(\w+)", t))
        for r in recv: out = re.sub(r"\b" + r + r"(\s*->\s*)" + old + r"(\s*\()", r + r"\g<1>" + new + r"\g<2>", out)
        if M.called(out, old): return None, f"call to {old} through an untyped receiver"
    notes = []
    if alias_name: notes.append("alias")
    if sprite:
        code_n = 0
        def resp(s):
            return re.sub(r"\bGetTextIDFromSymbol\s*\(", "GetScriptSpriteBySymbol(", s)
        lines = out.split("\n")
        for i, l in enumerate(lines):
            c = l.find("//"); code, com = (l, "") if c < 0 else (l[:c], l[c:])
            if "GetTextIDFromSymbol" in code: lines[i] = resp(code) + com; code_n += 1
        out = "\n".join(lines); notes.append("sprite%d" % code_n)
    for name, mem, kind in accs:
        if re.search(r"(?:\.|->|::)\s*" + name + r"\s*\(", out): return None, "qualified call to accessor " + name
        if kind == "&": out, k = re.subn(r"\b" + name + r"\(\s*\)\s*\.", mem + "->", out)
        else: out, k = re.subn(r"\b" + name + r"\(\s*\)", mem, out)
        if re.search(r"\b" + name + r"\s*\(", re.sub(r"//[^\n]*", "", out)): return None, "accessor use left: " + name
        notes.append("acc:%s%d" % (name, k))
        members[mem] = HMEM[mem][1]
    # member types the row uses
    for name, lt in members.items():
        if not re.search(r"\b" + name + r"\b", outside): continue
        ht = HMEM[name][1]
        if lt == ht: continue
        base = lt.rstrip("*").replace("const", "")
        if name == "DefinitionManager" and base in ("CDefinitionManager_Retail", "CDefinitionManager"):
            if base == "CDefinitionManager_Retail":
                if re.search(r"\bCDefinitionManager\b", out): return None, "CDefinitionManager already named"
                out = re.sub(r"\bstruct(\s+)CDefinitionManager_Retail\b(\s*\{)", r"class\1CDefinitionManager\2 public:", out)
                out = re.sub(r"\bstruct(\s+)CDefinitionManager_Retail\s*;", r"class\1CDefinitionManager;", out)
                out = re.sub(r"\bCDefinitionManager_Retail\b", "CDefinitionManager", out)
            if defined(out, "CGameDefinitionManager"): return None, "local CGameDefinitionManager"
            bb = find_block(out, "CDefinitionManager")
            if not bb: return None, "no CDefinitionManager block"
            out = out[:bb[1]] + "class CGameDefinitionManager : public CDefinitionManager { };\n" + out[bb[1]:]
            notes.append("DefMgr")
        elif name in CLS and base == CLS[name] + "_Retail":
            X = CLS[name]
            if re.search(r"\b" + X + r"\b", re.sub(r"\bclass\s+" + X + r"\s*;", "", out.replace(M.REPL, ""))): return None, X + " already named"
            out = re.sub(r"\bstruct(\s+)" + X + r"_Retail\b(\s*(?::[^{;]*)?\{)", r"class\1" + X + r"\2 public:", out)
            out = re.sub(r"\bstruct(\s+)" + X + r"_Retail\s*;", r"class\1" + X + ";", out)
            out = re.sub(r"\b" + X + r"_Retail\b", X, out)
            notes.append(X)
        elif lt in ("void*", "long", "constvoid*"): notes.append(name + ":" + lt)
        else: return None, f"member {name} type {lt} vs {ht}"
    return out, "ok " + ",".join(notes)
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
