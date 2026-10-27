// CWorld::EAControlledCreatureJump(const CGameEvent* event) -- game-event action: make the event player's controlled creature jump, unless it is missing or dead
// Leaf: ?EAControlledCreatureJump@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x0062D380, 80 bytes, thiscall `ret 4`; 0x114-byte frame for the action temporary, no EH frame.
// Identity (proven by source order): registered by the static initialiser 0x0121B9B0 as the handler of game event 50 in the
//   dispatch table 0x013B9288 (no direct caller). Retail keeps the Ego_r.pdb donor order at the end of the CWorld EA run:
//   EALoadGameFromInGameMenu 0x0062CE60 (landed), EACreatureBlock 0x0062CF90, EAUseProjectileWeaponThirdPerson 0x0062D190,
//   EAControlledCreatureJump 0x0062D380, EAControlledCreatureRoll 0x0062D3D0; the body builds a CCreatureActionHeroJump.
// Callees: CWorld::GetPlayer(unsigned long) 0x0049D850, CPlayer::GetPControlledCreature() const 0x00487DC0,
//   CCreatureActionHeroJump::CCreatureActionHeroJump(CThingCreatureBase&) 0x0085BB60 (stores vtable 0x012766A4, RTTI
//   CCreatureActionHeroJump -> CCombatActionBase; Ego_r.pdb ??0CCreatureActionHeroJump@@QAE@AAVCThingCreatureBase@@@Z),
//   CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0, CCombatActionBase::~CCombatActionBase()
//   0x0062E900 (landed).
// Shape: the action is a TEMPORARY bound to the const reference (the constructor's eax pushed; a named local pushes its
//   own `lea`); its implicit destructor inlines to
//   the base destructor call with no vtable store (VC7.1 drops it in a compiler-generated destructor).
// Layout: CThing Dead = +0x91 bit 0 (Ego_r.pdb +0xA1 bit 0); retail CCombatActionBase is 0x114 bytes (landed 0x0062E900
//   model: CCreatureActionBase 0xA8 + combat members), CCreatureActionHeroJump adds none. CWorld through engine/CWorld.h
//   (Donor include); the action classes are partial views.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld

enum EPlayerMode { PLAYER_MODE_NULL = 0 };

class CThing {                                             // partial view (retail +0x91 = Ego_r.pdb +0xA1)
public:
    unsigned char _pad_0x00[0x91];
    unsigned char Dead : 1;                                // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CCreatureActionBase;
class CThingCreatureBase : public CThingGameObject {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
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

class CCreatureActionBase {                                // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                        // 0x00693EF0
    unsigned char _body[0xa8 - 4];
};
class __declspec(novtable) CCombatActionBase : public CCreatureActionBase {   // retail sizeof 0x114
public:
    unsigned char _combat[0x114 - 0xa8];                   // PTarget .. PreferredMovementVector (landed 0x0062E900 model)
    virtual ~CCombatActionBase();                          // 0x0062E900
};
class CCreatureActionHeroJump : public CCombatActionBase {
public:
    CCreatureActionHeroJump(CThingCreatureBase& creature);   // 0x0085BB60
};

class CWorld : public CWorldDonor {
protected:
    CPlayer* GetPlayer(unsigned long number);              // 0x0049D850
public:
    void EAControlledCreatureJump(const CGameEvent* event);
};

void CWorld::EAControlledCreatureJump(const CGameEvent* event)
{
    CThingPlayerCreature* creature = GetPlayer(event->Player)->GetPControlledCreature();
    if (creature && !creature->Dead)
        creature->SetCurrentAction(CCreatureActionHeroJump(*creature));
}