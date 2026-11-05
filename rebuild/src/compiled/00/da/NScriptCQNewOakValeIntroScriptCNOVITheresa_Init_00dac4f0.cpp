// NScript::CQ_NewOakValeIntroScript::CNOVI_Theresa::Init() -- reset Theresa's flags and make her an untouchable, uninformative, unpushable NPC
// Leaf: ?Init@CNOVI_Theresa@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members Count->RefCount are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAC4F0, 142 bytes (+2 int3). __fastcall, `ret`.
// Class: catalog "CTalkingTrader1::Init" is wrong. 0x00DAC4F0 is slot 2 of the vtable 0x012D83A4 (no RTTI) that the ctor
//        0x00DAC420 stores; that ctor is registered under the entity-script name "NOVI_Theresa" (0x012D8360, factory record
//        at 0x00DABB36), and the two byte stores match CNOVI_Theresa (PDB, 0x20): spoken_already +0x1C, DoneIntro +0x1D.
// Callees (CGameScriptInterface virtuals, retail slot / address): EntitySetAsDamageable 516 0x0088EAD0, EntitySetAsKillable 517
//        0x0088EB30, EntitySetAsToAddToComboMultiplierWhenHit 526 0x0088EBC0, SetThingHasInformation 360 0x0088E440,
//        SetIsPushableByHero 844 0x008A6DD0 (CScriptThing BY VALUE: inline CBase vtable 0x01231710 then CScriptThing
//        vtable 0x01238C8C stored into the argument area, PImp copied with add-ref), EntitySetAsUseMovementInActions 529
//        0x0088EC20.
// Layout: entity-script base: vptr +0x00, CGameScriptInterface* +0x04, CScriptThing Me +0x08 (12 bytes); ParentClass +0x14,
//         PMasterData +0x18.
// Naming wave 35: identity reg 00dabb6f "NOVI_Theresa" in 00dabac0; Alloc 00dac420; vtable 012d83a4 slot 2; PDB ?Init@CNOVI_Theresa@CQ_NewOakValeIntroScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CNOVI_Theresa::Init (wave3/labels_alloc.tsv); PDB ?Init@CNOVI_Theresa@CQ_NewOakValeIntroScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CNOVI_Theresa@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
#include "engine/CCountedPointer.h"
struct CBase {                                   // vtable 0x01231710
    CBase() {}
    virtual ~CBase();
};
struct CScriptThingImp;
class CScriptThing : public CBase { public:                    // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImp> PImp;       // +0x04
    CScriptThing(const CScriptThing& other) : CBase(), PImp(other.PImp) {}
    virtual ~CScriptThing();
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript { class CQ_NewOakValeIntroScript; } using NScript::CQ_NewOakValeIntroScript;
struct CQ_SunnyvaleMasterData;
namespace NScript {
class CQ_NewOakValeIntroScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CNOVI_Theresa;
};
}
namespace NScript {
class CQ_NewOakValeIntroScript::CNOVI_Theresa {
public:
    virtual void v00();
    CGameScriptInterface* GSI;                   // +0x04
    CScriptThing Me;                             // +0x08
    CQ_NewOakValeIntroScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData* PMasterData;         // +0x18
    bool spoken_already;                         // +0x1c
    bool DoneIntro;                              // +0x1d
private:
    virtual void Init();                          // PDB ?Init@CNOVI_Theresa@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
};
}
void NScript::CQ_NewOakValeIntroScript::CNOVI_Theresa::Init()
{
    DoneIntro = false;
    spoken_already = false;
    GSI->EntitySetAsDamageable(Me, false);
    GSI->EntitySetAsKillable(Me, false, false);
    GSI->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    GSI->SetThingHasInformation(Me, false, true, false);
    GSI->SetIsPushableByHero(Me, false);
    GSI->EntitySetAsUseMovementInActions(Me, false);
}