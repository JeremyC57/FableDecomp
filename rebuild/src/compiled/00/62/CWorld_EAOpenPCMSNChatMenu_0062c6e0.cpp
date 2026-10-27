// CWorld::EAOpenPCMSNChatMenu(const CGameEvent* event) -- game-event action: fetch the event player's controlled creature for the PC MSN chat menu, which this build does not open (nothing else is done)
// Leaf: ?EAOpenPCMSNChatMenu@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x0062C6E0, 24 bytes, thiscall `ret 4`.
// Identity (proven by source order): registered by the static initialiser 0x0121B9B0 as the handler of game event 71 in the
//   dispatch table 0x013B9288 (no direct caller). Its neighbours are the PC-menu handlers in Ego_r.pdb donor order:
//   EAOpenPCScoreboard 0x0062C640 (calls CTCInventory 0x00587B93), EAOpenPCSkillsMenu 0x0062C690 (0x00587BFF),
//   EAOpenPCMSNChatMenu, EAOpenPCInventoryMenu 0x0062C700 (0x0058807D), EAOpenPCOptionsMenu 0x0062C750 (landed). The donor
//   EAOpenPCMSNChatMenu is the only tiny one (0x20 bytes against 0x70 for its neighbours), and CTCInventory has no MSN
//   chat opener in either PDB: the hero fetch is all that is left of the body.
// Callees: CWorld::GetPlayer(unsigned long) 0x0049D850, CPlayer::GetPControlledCreature() const 0x00487DC0 (result unused).
// Shape: `call ; ret 4` -- no tail jump, the callee pops nothing.
// Layout: CWorld through engine/CWorld.h (Donor include); CPlayer is a declaration-only partial view.
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

class CWorld : public CWorldDonor {
protected:
    CPlayer* GetPlayer(unsigned long number);              // 0x0049D850
public:
    void EAOpenPCMSNChatMenu(const CGameEvent* event);
};

void CWorld::EAOpenPCMSNChatMenu(const CGameEvent* event)
{
    CThingPlayerCreature* hero = GetPlayer(event->Player)->GetPControlledCreature();
}