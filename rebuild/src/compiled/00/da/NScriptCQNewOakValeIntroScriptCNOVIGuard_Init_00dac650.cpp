// NScript::CQ_NewOakValeIntroScript::CNOVI_Guard::Init() -- make the Oakvale guard undamageable, unkillable and not counted for combos, centre his wander on his home position (0..6 m), put him in scripting state group 4 and keep his weapons out
// Leaf: ?Init@CNOVI_Guard@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAC650, 272 bytes, plain `ret` (virtual, slot 2).
// Callees (CGameScriptInterface virtuals, retail slot / address; ghidra_out/cgsi_vtable_slots.tsv is 4 rows high here):
//   EntitySetAsDamageable 516 0x0088EAD0, EntitySetAsKillable 517 0x0088EB30, EntitySetAsToAddToComboMultiplierWhenHit
//   526 0x0088EBC0, SetWanderCentrePoint 763 0x008A23B0, SetWanderMinDistance 766 0x008A2690, SetWanderMaxDistance 769
//   0x008A2950, SetScriptingStateGroup 784 0x008A36D0 (CScriptThing BY VALUE: inline CBase vtable 0x01231710 then
//   CScriptThing vtable 0x01238C8C stored into the argument area, PImp copied with add-ref), EntitySheatheWeapons 506
//   0x00890BA0; CScriptThing slot 7 GetHomePos() (+0x1c, C3DVector built straight into the argument area).
// Layout: entity-script base as the landed CNOVI_Theresa::Init 0x00DAC4F0: vptr +0, CGameScriptInterface* +4, Me +8.
// Naming wave 35: identity reg 00dabbd3 "NOVI_Guard" in 00dabac0; Alloc 00dac580; vtable 012d83c0 slot 2; PDB ?Init@CNOVI_Guard@CQ_NewOakValeIntroScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CNOVI_Guard::Init (wave3/labels_alloc.tsv); PDB ?Init@CNOVI_Guard@CQ_NewOakValeIntroScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CNOVI_Guard@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
#include "engine/C3DVector.h"
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
};
struct CBase {                                   // vtable 0x01231710
    CBase() {}
    virtual ~CBase();
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual C3DVector GetHomePos() const;        // CScriptThing slot 7 (+0x1c) 0x004AA9A0
};
struct CScriptThingImp;
class CScriptThing : public CBase { public:                    // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImp> PImp;       // +0x04
    CScriptThing(const CScriptThing& other) : CBase(), PImp(other.PImp) {}
    virtual ~CScriptThing();
    virtual C3DVector GetHomePos() const;
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
    class CNOVI_Guard;
};
}
namespace NScript {
class CQ_NewOakValeIntroScript::CNOVI_Guard {
public:
    virtual void v00();
    CGameScriptInterface* GSI;                   // +0x04
    CScriptThing Me;                             // +0x08
    CQ_NewOakValeIntroScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData* PMasterData;         // +0x18
private:
    virtual void Init();                          // PDB ?Init@CNOVI_Guard@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
};
}
void NScript::CQ_NewOakValeIntroScript::CNOVI_Guard::Init()
{
    GSI->EntitySetAsDamageable(Me, false);
    GSI->EntitySetAsKillable(Me, false, false);
    GSI->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    GSI->SetWanderCentrePoint(Me, Me.GetHomePos());
    GSI->SetWanderMinDistance(Me, 0.0f);
    GSI->SetWanderMaxDistance(Me, 6.0f);
    GSI->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
    GSI->EntitySheatheWeapons(Me, false);
}