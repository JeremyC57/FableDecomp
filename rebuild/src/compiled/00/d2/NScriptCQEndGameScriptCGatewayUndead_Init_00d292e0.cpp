// NScript::CQ_EndGameScript::CGatewayUndead::Init() -- give the undead the hero as its best enemy target, then remember its health as the raise-undead bar amount (+0x1c)
// Leaf: ?Init@CGatewayUndead@CQ_EndGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D292E0, length 64 bytes.
// Callees: the script interface's slots 70 GetHero (CScriptThing&), 451 GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) and 264 GetHealth(const CScriptThing&) (float).
// Ego_r.pdb CGatewayUndead (32 bytes): PQuest +4, the CScriptThing +8, ParentClass +0x14, PMasterData +0x18, RaiseUndeadBarAmount +0x1c. Shape: the interface pointer re-read for each
// call but its vtable kept in edi across the one expression (`GiveThingBestEnemyTarget(Thing, GetHero())`), the thing's address in ebx, `fstp [esi+1Ch]`. Twin bodies at 0x00D292E0 / 0x00E95D80.
// Naming wave 35: identity reg 00d275de "GatewayUndead" in 00d27460; Alloc 00d29230; vtable 012cbc74 slot 2; PDB ?Init@CGatewayUndead@CQ_EndGameScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); replaces CHHUndead::Init: current name also held by 00e95d80 (wave3/labels_alloc.tsv); PDB ?Init@CGatewayUndead@CQ_EndGameScript@NScript@@EAEXXZ in FableWin.pdb.
//   Was `?Init@CHHUndead@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own); class CHHUndead renamed CGatewayUndead); same bytes.
class CScriptThing { public:                             // 8 bytes at +8 of the script object
    unsigned char _body[8];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CV_HauntedHouseScript;
struct CQ_SunnyvaleMasterData;
namespace NScript {
class CQ_EndGameScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CGatewayUndead;
};
}
using NScript::CQ_EndGameScript;
namespace NScript {
class CQ_EndGameScript::CGatewayUndead {
public:
        CGameScriptInterface*   PQuest;                // +0x04
    CScriptThing            Thing;                 // +0x08
    unsigned char           _pad10[4];             // +0x10
    CV_HauntedHouseScript*  ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    float                   RaiseUndeadBarAmount;  // +0x1c
private:
    virtual void Init();                          // PDB ?Init@CGatewayUndead@CQ_EndGameScript@NScript@@EAEXXZ
};
}

void NScript::CQ_EndGameScript::CGatewayUndead::Init()
{
    PQuest->GiveThingBestEnemyTarget(Thing, PQuest->GetHero());
    RaiseUndeadBarAmount = PQuest->GetHealth(Thing);
}