// NCreatureMode::CModeChargeForcePush::FrameUpdate() -- base update; while the creature still carries type flag 8, set it performing the force-push "out of" action (hitting only enemies when the mode says so)
// Leaf: ?FrameUpdate@CModeChargeForcePush
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DFF50, 121 bytes (+ int3 padding), `ret`; vtable 0x01268328 slot 3.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50 (qualified), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long,
//   const CSoundPair&) @ 0x00858030 (action code 0x96), CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//   ~CCombatActionBase @ 0x0062E900. Action vtable 0x01286874 = CCreatureAction_ForcePushOutOf (rtti-vtables.tsv).
// Name: FableWin.pdb `?FrameUpdate@CModeChargeForcePush@NCreatureMode@@UAEXXZ`; the action's PDB members are
//   `??0CCreatureAction_ForcePushOutOf@@QAE@AAVCThingCreatureBase@@@Z` and `SetOnlyHitEnemies()`.
// Layout: Ego_r.pdb CModeChargeForcePush: CModeStandBase 0x68, Filter +0x68, OnlyHitEnemies +0x74. Retail action sizeof
//   0x118 (CCombatActionBase 0x114 + OnlyHitEnemies +0x114; landed Clone 0x008E5030). Creature type flags +0x6c.
// Shape: a named action local built by the inline ctor (zeroed CSoundPair temporary in the first frame words, then the
//   base call, the vtable and the false OnlyHitEnemies store), `if (OnlyHitEnemies) action.SetOnlyHitEnemies();`
//   (the 0 then 1 byte stores), SetCurrentAction, then the generated dtor (just ~CCombatActionBase).
struct CThingCreatureBase;
struct CCreatureActionBase;

struct CThing {
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;          // +0x6c
};
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCreatureActionBase {               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _combat[0x6c];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                  // 0x0062E900
};
struct CCreatureAction_ForcePushOutOf : CCombatActionBase {   // vtable 0x01286874, retail sizeof 0x118
    bool OnlyHitEnemies;                   // +0x114
    virtual void v00();
    CCreatureAction_ForcePushOutOf(CThingCreatureBase& owner)
        : CCombatActionBase(&owner, 0x96, CSoundPair()), OnlyHitEnemies(false) { }
    void SetOnlyHitEnemies() { OnlyHitEnemies = true; }
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
    void FrameUpdate();                    // 0x0073AC50
};
struct CModeStandBase : CModeBase {        // retail 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
struct CModeChargeForcePush : CModeStandBase {
    unsigned char Filter[0x0c];            // +0x68 CAnimFilterOfGroup
    bool          OnlyHitEnemies;          // +0x74
    void FrameUpdate();
};
}

void NCreatureMode::CModeChargeForcePush::FrameUpdate()
{
    CModeBase::FrameUpdate();
    CThingCreatureBase* creature = ParentThing;
    if (creature->ThingTypeFlags & 8) {
        CCreatureAction_ForcePushOutOf action(*creature);
        if (OnlyHitEnemies)
            action.SetOnlyHitEnemies();
        creature->SetCurrentAction(action);
    }
}