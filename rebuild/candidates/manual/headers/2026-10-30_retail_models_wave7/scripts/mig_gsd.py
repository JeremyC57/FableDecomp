#!/usr/bin/env python3
"""Wave 2 family GSD (dd084063, 151 rows): CDefClassBase::GetSubDef<T> rows move from the row-local CDefBase / CRefCounted /
CDefPointer / 15-slot CDefClassBase stand-ins onto the shared retail models (FABLE_RETAIL_DEF_CLASS_BASE: engine/CDefPointer.h,
engine/CSubDefClassBase.h or engine/CParentDefClassBase.h by T's RTTI base) and the FableWin.pdb member set of the template:
T::StatGetTypeName(), the const slot-14 GetPSubDefInfo, the template converting ctor CDefPointer<const CDefClassBase>(
CDefPointer<CDefClassBase>), SetDynamicCast<const CDefClassBase>. Idempotent (rows already on CDefPointer.h are skipped).
usage: mig_gsd.py [--dry] <file>..."""
import sys, re, os
R = os.environ["FABLE_ROOT"]
RT = {}
for l in open(R + "/ghidra_out/rtti_bases.tsv", encoding="utf-8", errors="replace"):
    q = l.rstrip("\n").split("\t")
    if len(q) >= 4 and q[0].startswith(".?AV") and q[0].endswith("@@") and "@" not in q[0][4:-2]:
        RT.setdefault(q[0][4:-2], q[3].split("@")[0] if q[3] else "")
PDB = set()
for k in ("fablewin", "egor"):
    for l in open(f"{R}/ghidra_out/{k}_pdb_names.tsv", encoding="utf-8", errors="replace"):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2: PDB.add(q[1])
HDR = {"CSubDefClassBase": "engine/CSubDefClassBase.h", "CParentDefClassBase": "engine/CParentDefClassBase.h"}
SHAPE = """// Body/shape (wave 2 class-model migration, 2026-10-30): the shared retail models (FABLE_RETAIL_DEF_CLASS_BASE) and the
//   FableWin.pdb member set of the template: T::StatGetTypeName() (inline; the class-name literal) feeds the const slot-14
//   GetPSubDefInfo; the manager's CDefPointer<CDefClassBase> temporary converts (template ctor, addref) into a local
//   CDefPointer<const CDefClassBase> and is released (the `mov edi,[eax+4] ; dec edi` shape); SetDynamicCast<const
//   CDefClassBase> assigns it into *out (release old, store, addref); the local's destructor releases (`dec [esi+4] ; jne`).
"""
def fix(t):
    if '#include "engine/CDefPointer.h"' in t: return None, "already"
    m = re.search(r"template <>\nbool CDefClassBase::GetSubDef<(\w+)>\(CDefPointer<const \1>\* out\) const\n", t)
    if not m: return None, "no GetSubDef specialisation"
    T = m.group(1)
    i = t.find('#include "fable_string.h"\n')
    if i < 0: return None, "no fable_string include"
    head = t[:i]
    j = head.find("// Body/shape")
    if j < 0: return None, "no Body/shape comment"
    k = j
    lines = head[j:].split("\n")
    n = 0
    while n < len(lines) and lines[n].startswith("//"): n += 1
    rest = "\n".join(lines[n:])
    head = head[:j] + SHAPE + rest
    head = head.replace("The index lookup is the def's virtual\n//   slot 14 (+0x38, descriptive name).", "The index lookup is the def's virtual\n//   slot 14 (+0x38), the const GetPSubDefInfo.")
    chain = []; b = RT.get(T, "")
    while b and b not in HDR:
        chain.append(b); b = RT.get(b, "")
    if b not in HDR: return None, f"no RTTI chain to a shared def base for {T}"
    out = head + '#include "fable_string.h"\n#define FABLE_RETAIL_DEF_CLASS_BASE\n'
    out += f'#include "{HDR[b]}"\n#include "engine/CDefPointer.h"\n\n'
    prev = b
    for c in reversed(chain):
        out += f"class {c} : public {prev} {{ }};{' ' * max(1, 40 - len(c) - len(prev))}// RTTI .?AV{c}@@ : {prev} (intermediate base)\n"
        prev = c
    out += f"class {T} : public {prev} {{                    // RTTI .?AV{T}@@ : {prev}\npublic:\n"
    out += f'    static CCharString __fastcall StatGetTypeName() {{ return CCharString("{T}", -1); }}   // ?StatGetTypeName@{T}@@SI?AVCCharString@@XZ (inline)\n'
    for mang, decl in ((f"?Transfer@{T}@@UAEXAAVCPersistContext@@@Z", "virtual void Transfer(CPersistContext&);"),
                       (f"?Copy@{T}@@UAEXPBVCDefClassBase@@@Z", "virtual void Copy(const CDefClassBase*);"),
                       (f"?GetSizeofClass@{T}@@UAEKXZ", "virtual unsigned long GetSizeofClass();")):
        if mang in PDB: out += f"    {decl:<62}// {mang}\n"
    out += "};\n\n"
    out += """class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};

template <>
bool CDefClassBase::GetSubDef<%s>(CDefPointer<const %s>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(%s::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
""" % (T, T, T)
    return out, "ok"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if a != "--dry"]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
