// NCreatureMode::CModeRockTrollVulnerable::OnActivate() -- set the recovery count to five seconds of frames, base activation, then suspend the troll's animation and set it performing the "vulnerable into" action
// Leaf: ?OnActivate@CModeRockTrollVulnerable
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DE6A0, 90 bytes (+ int3 padding), `ret`; vtable 0x01285774 slot 5.
// Callees: __ftol2 @ 0x00BFEA70, CModeBase::OnActivate @ 0x0073AAC0 (qualified), CThingAnimatable::SetSuspendAnimation(bool,
//   long) @ 0x00833A10, CCreatureAction_TrollVulnerableInto(CThingCreatureBase*) @ 0x008E4420 (landed; manifest
//   "CCreatureAction_BattleChargeTurnLeft" is propagated), CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//   ~CCombatActionBase @ 0x0062E900.
// Globals: the frames-per-second long @ 0x01375550 (`fild`), 5.0f @ 0x012462A8 (a compiler literal).
// Name: FableWin.pdb `?OnActivate@CModeRockTrollVulnerable@NCreatureMode@@UAEXXZ` (slot 5).
// Layout: Ego_r.pdb CModeRockTrollVulnerable (0x34, matches retail): RecoveryCount +0x1c, Filter +0x20, PCreature +0x2c,
//   bRecovering +0x30.
// Shape: the frame count is computed before the base call (VC7.1 hoists the fild above the frame set-up); PCreature is held in
//   edi for the suspend and the SetCurrentAction receiver and re-read for the action's ctor argument.
struct CCreatureActionBase;

struct CThingAnimatable {
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};
struct CThingCreatureBase : CThingAnimatable {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CCreatureActionBase {               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                  // 0x0062E900
};
struct CCreatureAction_TrollVulnerableInto : CCombatActionBase {
    CCreatureAction_TrollVulnerableInto(CThingCreatureBase* owner);   // 0x008E4420
};

extern long g_FramesPerSecond;             // 0x01375550

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    void*         ParentThing;             // +0x04
    unsigned char _body[0x14];
    void OnActivate();                     // 0x0073AAC0
};

namespace NCreatureMode {
struct CModeRockTrollVulnerable : CModeBase {   // vtable 0x01285774
    long                RecoveryCount;     // +0x1c
    unsigned char       Filter[0x0c];      // +0x20 CAnimFilterOfGroup
    CThingCreatureBase* PCreature;         // +0x2c
    bool                bRecovering;       // +0x30
    void OnActivate();
};
}

void NCreatureMode::CModeRockTrollVulnerable::OnActivate()
{
    RecoveryCount = (long)(g_FramesPerSecond * 5.0f);
    CModeBase::OnActivate();
    CThingCreatureBase* creature = PCreature;
    creature->SetSuspendAnimation(true, 0);
    creature->SetCurrentAction(CCreatureAction_TrollVulnerableInto(PCreature));
}