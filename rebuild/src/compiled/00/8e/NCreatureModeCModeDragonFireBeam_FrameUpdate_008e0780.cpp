// NCreatureMode::CModeDragonFireBeam::FrameUpdate() -- hover update; start the beam once a player-type dragon's current action has performed, count the beam timer down, update the beam, and at zero set the "fire beam out of" action
// Leaf: ?FrameUpdate@CModeDragonFireBeam
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E0780, 112 bytes, `ret`; vtable 0x01285900 slot 3.
// Callees: CModeDragonHover::FrameUpdate @ 0x008DB460 (qualified base call), CCombatActionBase::HasPerformedAction @ 0x006925A0
//   (manifest "CTCInventoryBase::GetCanItemBeQuickAssigned" is propagated), CModeDragonFireBeam::StartBeamEffect @ 0x008DB620,
//   CModeDragonFireBeam::UpdateBeam @ 0x008DB810, CCreatureAction_DragonFireBeamOutOf(CThingCreatureBase*) @ 0x008E4A60 (the
//   manifest label "CCombatAction_Guildmaster_HealFollowersInto" is propagated: the ctor stores vtable 0x01286EE4 =
//   CCreatureAction_DragonFireBeamOutOf), CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Name: FableWin.pdb `?FrameUpdate@CModeDragonFireBeam@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: Ego_r.pdb CModeDragonFireBeam (0x94): CModeDragonHover 0x80, Timer +0x80, PBeamWeaponEffect +0x84. Creature: state
//   byte +0x10, PCurrentAction +0x100 (CCountedPointer<CCreatureActionBase>, PDB +0x11c less the retail 0x1c shift).
// Shape: the current action is read through an inline accessor (`mov eax,[eax+100h] ; test ; mov ecx,eax`); the timer is
//   decremented in memory then re-read after UpdateBeam; the creature named local is read only in the final block.
class CCreatureActionBase;

class CCreatureActionBase { public:               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    unsigned char _combat[0x6c];
    virtual ~CCombatActionBase();                  // 0x0062E900
    bool HasPerformedAction() const;       // 0x006925A0
};
class CThingCreatureBase { public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                 // +0x10 (1 = player creature)
    unsigned char _pad11[0x100 - 0x11];
    CCombatActionBase* PCurrentAction;     // +0x100 CCountedPointer<CCreatureActionBase>::Data
    void*              PCurrentActionInfo; // +0x104
    CCombatActionBase* GetCurrentAction() const { return PCurrentAction; }
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CCreatureAction_DragonFireBeamOutOf : public CCombatActionBase { public:
    CCreatureAction_DragonFireBeamOutOf(CThingCreatureBase* owner);   // 0x008E4A60
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
};
struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
class CModeDragonHover : public CModeStandBase { public: // retail sizeof 0x80
    unsigned char _hover[0x18];            // Filter +0x68, PTarget +0x74, UseHeadTurning +0x7c
    void FrameUpdate();                    // 0x008DB460
};
struct CModeDragonFireBeam : CModeDragonHover {   // vtable 0x01285900
    long          Timer;                   // +0x80
    unsigned char PBeamWeaponEffect[8];    // +0x84 CIntelligentPointer<CThing>
    void StartBeamEffect();                // 0x008DB620
    void UpdateBeam();                     // 0x008DB810
    void FrameUpdate();
};
}

void NCreatureMode::CModeDragonFireBeam::FrameUpdate()
{
    CModeDragonHover::FrameUpdate();
    if (ParentThing->m_state == 1 && ParentThing->GetCurrentAction() != 0 && ParentThing->GetCurrentAction()->HasPerformedAction())
        StartBeamEffect();
    --Timer;
    UpdateBeam();
    if (Timer <= 0) {
        CThingCreatureBase* creature = ParentThing;
        creature->SetCurrentAction(CCreatureAction_DragonFireBeamOutOf(creature));
    }
}