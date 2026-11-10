// CIdleSchedulerDef::CIdleSchedulerDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CIdleSchedulerDef vtable, then the members' constructors
// Leaf: ??0CIdleSchedulerDef@@QAE@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x004E3F21. Identity: the vtable it stores, 0x01243124, is RTTI .?AVCIdleSchedulerDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CIdleSchedulerDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CIdleSchedulerDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CIdleSchedulerDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
class CPersistContext;
enum EIdleStateGroup { };                                   // enumerators not needed here (no engine header)

class CIdleSchedulerDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CIdleSchedulerDef();                                   // 0x004E3F21
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::map<EIdleStateGroup, long> Period;
};

CIdleSchedulerDef::CIdleSchedulerDef()
{
}
