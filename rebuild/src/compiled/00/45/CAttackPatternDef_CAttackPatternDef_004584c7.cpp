// CAttackPatternDef::CAttackPatternDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CAttackPatternDef vtable, then the members' constructors
// Leaf: ??0CAttackPatternDef@@QAE@XZ
// Retail: Fable.exe @ 0x004584C7. Identity: the vtable it stores, 0x01234A1C, is RTTI .?AVCAttackPatternDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CAttackPatternDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CAttackPatternDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CAttackPatternDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;

class CAttackPatternDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CAttackPatternDef();                                   // 0x004584C7
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<long> AttackPattern;
};

CAttackPatternDef::CAttackPatternDef()
{
}
