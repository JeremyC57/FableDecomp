// CMurderTwistSheriff::Init() -- the sheriff stands his ground: not pushable, no movement in actions, unkillable and undamageable, looking at the hero, no information marker; his dialogue flags reset and NextLine 1
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAEE80, 173 bytes (`ret`).
// Callees (CGameScriptInterface virtuals, slot / address): SetIsThingForcePushable 745 0x0088F9F0, SetIsPushableByHero 844
// 0x008A6DD0 (CScriptThing BY VALUE: inline CBaseClass vtable 0x01231710 then CScriptThing vtable 0x01238C8C stored into the
// argument area, PImp copied with add-ref), EntitySetAsUseMovementInActions 529 0x0088EC20, EntitySetAsKillable 517 0x0088EB30,
// EntitySetAsDamageable 516 0x0088EAD0, GetHero 70 0x00891CA0, EntityForceToLookAtThing 498 0x0089D830,
// SetThingHasInformation 360 0x0088E440.
// Layout (PDB CMurderTwistSheriff 0x38, matches retail): entity-script base (vptr, Game +0x04, Me +0x08), ParentClass +0x14,
// PMasterData +0x18, MyEntityHandle +0x1c, AttractTimer +0x2c, NextLine +0x30, HasGivenReminderToHero +0x34,
// HasBeingRumbledCutscenePlayed +0x35, HasExpressedSuspicionToHero +0x36, HasSaidWellDoneToHero +0x37.
// Shape: the one zero register (ebx) feeds every false argument and the four flag stores; the interface vtable is
// fetched once for the GetHero / EntityForceToLookAtThing pair (callee first, then its argument).
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CScriptThingImp;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptGameResourceObjectScriptedThingBase { public: unsigned char _body[0x10]; };
struct CQ_SunnyvaleMasterData;
struct CV_MurderTwistFinalScript;
class CTimer { public: unsigned long _handle; };

struct CMurderTwistSheriff {
    virtual void v00();
    CGameScriptInterface*   Game;                            // +0x04
    CScriptThing            Me;                              // +0x08
    CV_MurderTwistFinalScript* ParentClass;                  // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                     // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    CTimer                  AttractTimer;                    // +0x2c
    unsigned long           NextLine;                        // +0x30
    bool                    HasGivenReminderToHero;          // +0x34
    bool                    HasBeingRumbledCutscenePlayed;   // +0x35
    bool                    HasExpressedSuspicionToHero;     // +0x36
    bool                    HasSaidWellDoneToHero;           // +0x37
    void Init();
};

void CMurderTwistSheriff::Init()
{
    Game->SetIsThingForcePushable(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsUseMovementInActions(Me, false);
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntityForceToLookAtThing(Me, Game->GetHero());
    Game->SetThingHasInformation(Me, false, false, false);
    HasGivenReminderToHero = false;
    HasBeingRumbledCutscenePlayed = false;
    HasExpressedSuspicionToHero = false;
    HasSaidWellDoneToHero = false;
    NextLine = 1;
}