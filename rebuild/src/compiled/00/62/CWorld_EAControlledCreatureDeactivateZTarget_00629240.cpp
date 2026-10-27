// CWorld::EAControlledCreatureDeactivateZTarget(const CGameEvent* event) -- game-event action: take the event player out of the Z-target player mode (2)
// Leaf: ?EAControlledCreatureDeactivateZTarget@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x00629240, 29 bytes, thiscall `ret 4` (tail-jumps RemoveMode with the mode in the event slot).
// Identity (proven by source order): the static initialiser 0x0121B9B0 registers it as the handler of game event 62 in the
//   dispatch table 0x013B9288 (no direct caller). The CWorld EA handlers keep their Ego_r.pdb (donor) source order in
//   retail: the eight handlers before EASetPauseMode 0x006292D0 are, by address, 0x006291F0 / 0x00629210 / 0x00629220 /
//   0x00629240 / 0x00629260 / 0x00629270 / 0x00629280 / 0x006292A0, and the donor build orders EAControlledCreatureUnfreezeControls,
//   EAApplyAllScriptedMapBrushes, EAControlledCreatureStartThrowObject, EAControlledCreatureDeactivateZTarget, EAMoveHeroToRegion,
//   EAUpdateFrame, EAFirstPersonViewStart, EACreatureCheat, EASetPauseMode -- this is the fourth. Its body (leave player
//   mode 2) fits: Ego_r.pdb CPlayer has InitModeZTarget / UninitModeZTarget.
// Callees: CWorld::GetPlayer(unsigned long) 0x0049D850 (Ego_r.pdb ?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z, protected),
//   CPlayer::RemoveMode(EPlayerMode) 0x00634190.
// Shape: the player fetched into a named local first (the one-expression form pushes the mode before the GetPlayer call
//   and loses the tail jump); the player byte sign-extended (`movsx`), GetPlayer, then the mode constant stored over the dead event slot and
//   RemoveMode tail-jumped.
// Layout: CGameEvent (0x28 bytes, Ego_r.pdb = retail): Type +0, signed char Player +4. CWorld through engine/CWorld.h
//   (Donor include); CPlayer is a declaration-only partial view.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld

enum EPlayerMode { PLAYER_MODE_NULL = 0, PLAYER_MODE_Z_TARGET = 2 };   // enumerator names are ours

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

class CWorld : public CWorldDonor {
protected:
    CPlayer* GetPlayer(unsigned long number);              // 0x0049D850
public:
    void EAControlledCreatureDeactivateZTarget(const CGameEvent* event);
};

void CWorld::EAControlledCreatureDeactivateZTarget(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    player->RemoveMode(PLAYER_MODE_Z_TARGET);
}