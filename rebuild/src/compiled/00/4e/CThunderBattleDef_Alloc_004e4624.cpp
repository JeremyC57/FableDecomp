// CThunderBattleDef::Alloc() -- the def factory: new CThunderBattleDef (operator new 0x00BFEA1A), the inline default constructor (the out-of-line CParentDefClassBase() 0x00430370, then the CThunderBattleDef vtable) when the block is not null
// Leaf: ?Alloc@CThunderBattleDef@@SIPAVCDefClassBase@@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x004E4624. Identity: the vtable it stores, 0x01242D04, is RTTI .?AVCThunderBattleDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb ?Alloc@CThunderBattleDef@@SIPAVCDefClassBase@@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CThunderBattleDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CThunderBattleDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CPersistContext;

class CThunderBattleDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();                  // 0x004E4624
    CThunderBattleDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long NumGetHitsBeforeStormAttack;
    long NumMissesBeforeStormAttack;
    float LightningDamage;
    long BattleChargeLevel;
};

inline CThunderBattleDef::CThunderBattleDef()
{
}

CDefClassBase* FABLE_FASTCALL CThunderBattleDef::Alloc()
{
    return new CThunderBattleDef;
}
