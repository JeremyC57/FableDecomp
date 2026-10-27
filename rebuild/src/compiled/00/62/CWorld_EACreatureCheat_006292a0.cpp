// CWorld::EACreatureCheat(const CGameEvent* event) -- game-event action (debug cheat): kill the main player's controlled creature, if there is one
// Leaf: ?EACreatureCheat@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x006292A0, 37 bytes, thiscall `ret 4` (tail-jumps Kill with `true` in the event slot).
// Identity (proven by source order): registered by the static initialiser 0x0121B9B0 as the handler of game event 7 in the
//   dispatch table 0x013B9288 (no direct caller). The eight CWorld EA handlers before EASetPauseMode 0x006292D0 follow the
//   Ego_r.pdb donor order (UnfreezeControls, ApplyAllScriptedMapBrushes, StartThrowObject, DeactivateZTarget,
//   MoveHeroToRegion, UpdateFrame, FirstPersonViewStart, CreatureCheat); this is the eighth, directly before
//   EASetPauseMode as in the donor build. It ignores the event (the main player, not the event's player).
// Callees: CPlayerManager::GetMainPlayer() 0x00449970 (Ego_r.pdb ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ),
//   CPlayer::GetPControlledCreature() const 0x00487DC0 (Ego_r.pdb ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ:
//   `add ecx,2Ch` = ControlledCreature +0x2C, then GetPItem), CThing::Kill(bool) 0x004C9B80.
// Shape: World->PlayerManager (+0xC) straight into ecx, the null test, then `true` stored over the dead event slot and
//   Kill tail-jumped.
// Layout: CWorld through engine/CWorld.h (Donor include; PlayerManager +0x0C). CPlayerManager / CPlayer are
//   declaration-only partial views; CThingPlayerCreature's RTTI chain down to CThing @0 is modelled empty.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld

enum EPlayerMode { PLAYER_MODE_NULL = 0 };

class CThing {
public:
    void Kill(bool b);                                     // 0x004C9B80 (Ego_r.pdb ?Kill@CThing@@QAEX_N@Z)
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject { };
class CThingPlayerCreature : public CThingCreatureBase { };

class CGameEvent {                                         // 0x28 bytes (Ego_r.pdb = retail)
public:
    long          Type;                                    // +0x00
    signed char   Player;                                  // +0x04
    unsigned char Data[32];                                // +0x05
    unsigned char EndPos;                                  // +0x25
    bool          Valid;                                   // +0x26
    bool          Replacement;                             // +0x27
};

class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;  // 0x00487DC0
    void RemoveMode(EPlayerMode mode);                     // 0x00634190
};

class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                              // 0x00449970
};

class CWorld : public CWorldDonor {
public:
    void EACreatureCheat(const CGameEvent* event);
};

void CWorld::EACreatureCheat(const CGameEvent* event)
{
    CThingPlayerCreature* creature = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (creature)
        creature->Kill(true);
}