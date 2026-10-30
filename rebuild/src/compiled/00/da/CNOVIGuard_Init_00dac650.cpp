// CNOVI_Guard::Init() -- make the Oakvale guard undamageable, unkillable and not counted for combos, centre his wander on his home position (0..6 m), put him in scripting state group 4 and keep his weapons out
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAC650, 272 bytes, plain `ret` (virtual, slot 2).
// Callees (CGameScriptInterface virtuals, retail slot / address; ghidra_out/cgsi_vtable_slots.tsv is 4 rows high here):
//   EntitySetAsDamageable 516 0x0088EAD0, EntitySetAsKillable 517 0x0088EB30, EntitySetAsToAddToComboMultiplierWhenHit
//   526 0x0088EBC0, SetWanderCentrePoint 763 0x008A23B0, SetWanderMinDistance 766 0x008A2690, SetWanderMaxDistance 769
//   0x008A2950, SetScriptingStateGroup 784 0x008A36D0 (CScriptThing BY VALUE: inline CBase vtable 0x01231710 then
//   CScriptThing vtable 0x01238C8C stored into the argument area, PImp copied with add-ref), EntitySheatheWeapons 506
//   0x00890BA0; CScriptThing slot 7 GetHomePos() (+0x1c, C3DVector built straight into the argument area).
// Layout: entity-script base as the landed CNOVI_Theresa::Init 0x00DAC4F0: vptr +0, CGameScriptInterface* +4, Me +8.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
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
struct CQ_NewOakValeIntroScript;
struct CQ_SunnyvaleMasterData;
struct CNOVI_Guard {
    virtual void v00();
    CGameScriptInterface* GSI;                   // +0x04
    CScriptThing Me;                             // +0x08
    CQ_NewOakValeIntroScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData* PMasterData;         // +0x18
    void Init();
};
void CNOVI_Guard::Init()
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