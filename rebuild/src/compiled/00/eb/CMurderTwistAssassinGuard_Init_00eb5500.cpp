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
class CScriptThing : public CBaseClass { public:               // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImp> PImp;       // +0x04
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
    virtual ~CScriptThing();
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066();
    virtual void SetFriendsWithEverythingFlag(bool friends);   // slot 67 (+0x10c) 0x004AB060
};
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