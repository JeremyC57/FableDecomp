// CNOVI_AffairWife::Init() -- reset the affair wife's flags and make her an untouchable, unpushable NPC without deed reactions
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB2A70, 160 bytes. __fastcall, `ret`.
// Class: catalog "CTalkingTrader1::Init" is wrong. 0x00DB2A70 is slot 2 of the vtable 0x012D8C98 (no RTTI) that the ctor
//        0x00DB29A0 stores; that ctor is registered under the entity-script name "NOVI_AffairWife" (0x012D82E8, factory
//        record at 0x00DABE9D), and the byte stores match CNOVI_AffairWife (PDB, 0x20): GoingForHusband +0x1C,
//        ForceFirstTimeSpeak +0x1D, SaidRunningLine +0x1E.
// Callees (CGameScriptInterface virtuals, retail slot / address): EntitySetAsDamageable 516 0x0088EAD0, EntitySetAsKillable 517
//        0x0088EB30, EntitySetAsToAddToComboMultiplierWhenHit 526 0x0088EBC0, SetThingHasInformation 360 0x0088E440,
//        EntitySetAsUseMovementInActions 529 0x0088EC20, SetIsPushableByHero 844 0x008A6DD0 (CScriptThing BY VALUE: inline
//        CBase vtable 0x01231710 then CScriptThing vtable 0x01238C8C), EntitySetDeedReactionsEnabled 595 0x0088F850.
// Layout: entity-script base: vptr +0x00, CGameScriptInterface* +0x04, CScriptThing Me +0x08 (12 bytes); ParentClass +0x14,
//         PMasterData +0x18.
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
};
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
struct CQ_NewOakValeIntroScript;
struct CQ_SunnyvaleMasterData;
struct CNOVI_AffairWife {
    virtual void v00();
    CGameScriptInterface* GSI;                   // +0x04
    CScriptThing Me;                             // +0x08
    CQ_NewOakValeIntroScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData* PMasterData;         // +0x18
    bool GoingForHusband;                        // +0x1c
    bool ForceFirstTimeSpeak;                    // +0x1d
    bool SaidRunningLine;                        // +0x1e
    void Init();
};
void CNOVI_AffairWife::Init()
{
    GoingForHusband = false;
    ForceFirstTimeSpeak = true;
    GSI->EntitySetAsDamageable(Me, false);
    GSI->EntitySetAsKillable(Me, false, false);
    GSI->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    GSI->SetThingHasInformation(Me, false, true, false);
    GSI->EntitySetAsUseMovementInActions(Me, false);
    GSI->SetIsPushableByHero(Me, false);
    SaidRunningLine = false;
    GSI->EntitySetDeedReactionsEnabled(Me, false);
}