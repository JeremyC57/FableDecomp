// CObjectFamilyDef::CObjectFamilyDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CObjectFamilyDef vtable, then the members' constructors
// Leaf: ??0CObjectFamilyDef@@QAE@XZ
// Retail: Fable.exe @ 0x0045851D. Identity: the vtable it stores, 0x01234A84, is RTTI .?AVCObjectFamilyDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CObjectFamilyDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CObjectFamilyDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CObjectFamilyDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;
class CObjectFamilyEntry { public:                         // Ego_r.pdb CObjectFamilyEntry (8 bytes); no engine header
    long Object;                              // +0x00
    long Probability;                         // +0x04
};

class CObjectFamilyDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CObjectFamilyDef();                                   // 0x0045851D
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<CObjectFamilyEntry> Objects;
};

CObjectFamilyDef::CObjectFamilyDef()
{
}
