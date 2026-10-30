// CMurderTwistAssassinGuard::Init() -- once the final murder-twist quest is under way, an assassin guard is fixed in place (not pushable, no movement in actions, unkillable, undamageable), looks at the hero, is friends with everything and persists; otherwise he is removed
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB5500, 207 bytes (`ret`).
// Callees (CGameScriptInterface virtuals, slot / address): RemoveThing 108 0x008910D0, SetIsThingForcePushable 745 0x0088F9F0,
// SetIsPushableByHero 844 0x008A6DD0 (CScriptThing by value, built in the argument area with add-ref),
// EntitySetAsUseMovementInActions 529 0x0088EC20, EntitySetAsKillable 517 0x0088EB30, EntitySetAsDamageable 516 0x0088EAD0,
// GetHero 70 0x00891CA0, EntityForceToLookAtThing 498 0x0089D830, SetThingPersistent 477 0x0088E5A0;
// CScriptThing::SetFriendsWithEverythingFlag (CScriptThing vtable 0x01238C8C slot 67, 0x004AB060) on Me itself.
// Layout (PDB CMurderTwistAssassinGuard 0x34, matches retail): Game +0x04, Me +0x08, ParentClass +0x14, PMasterData +0x18,
// MyEntityHandle +0x1c, NextLine +0x2c, HasBeenSpokenTo +0x30, PreemptiveAttackInProgress +0x31.
// The parent script's +0x48 word (below CV_MurderTwistFinalScript's own PDB members, in its quest-script base) is the
// tested "started" flag; its name is not recovered.
// Shape: early-out `return` arm laid out first; the zero arguments are immediates here (ebx holds the cached vtable).
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
struct CV_MurderTwistFinalScript {
    unsigned char _base[0x48];
    long          m_startedFlag48;               // +0x48 (quest-script base field, name not recovered)
};

struct CMurderTwistAssassinGuard {
    virtual void v00();
    CGameScriptInterface*   Game;                            // +0x04
    CScriptThing            Me;                              // +0x08
    CV_MurderTwistFinalScript* ParentClass;                  // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                     // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    unsigned long           NextLine;                        // +0x2c
    bool                    HasBeenSpokenTo;                 // +0x30
    bool                    PreemptiveAttackInProgress;      // +0x31
    void Init();
};

void CMurderTwistAssassinGuard::Init()
{
    if (ParentClass->m_startedFlag48 == 0) {
        Game->RemoveThing(Me, false, true);
        return;
    }
    Game->SetIsThingForcePushable(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsUseMovementInActions(Me, false);
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntityForceToLookAtThing(Me, Game->GetHero());
    Me.SetFriendsWithEverythingFlag(true);
    Game->SetThingPersistent(Me, true);
    NextLine = 1;
    HasBeenSpokenTo = false;
}