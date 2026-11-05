// CWorld::Unload() -- reset the main player's modes, kill its controlled creature if it is still alive, then let the thing manager process the deaths
// Leaf: ?Unload@CWorld@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049E270, 66 bytes (int3 padding 0x0049E2B2..BF), thiscall, tail `jmp CThingManager::UpdateDeaths`.
// Identity (proven, two sources): (1) Ego_r.pdb / FableWin.pdb ?Unload@CWorld@@QAEXXZ; Ego_r donor order LoadWorldFromFile
//   (0x004A73C0) -> Unload (0x004A74A0) = retail LoadWorldFromFile 0x0049E220 (vtable 0x012390F0 slot 2) -> this row; (2) the body
//   reads CWorld::PlayerManager (+0x0C) and calls CWorld's own vtable slot 3 (GetThingManager 0x0049E1B0, `mov eax,[ecx+50h]`).
//   No caller in retail (unreferenced, as in the PDB name set only). The catalog label was right; the forge `_global::` stand-in aside.
// Callees (PDB spellings, both name sets): CPlayerManager::GetMainPlayer() 0x00449970 (?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ),
//   CPlayer::ResetModes() 0x00634870, CPlayer::GetPControlledCreature() const 0x00487DC0 (QBE, returns CThingPlayerCreature*),
//   CThing::Kill(bool) 0x004C9B80, CWorld::GetThingManager() (virtual slot 3, returns CThingManager&), CThingManager::UpdateDeaths()
//   0x0051F070 (tail jump on the returned reference).
// Shape: IS_THING_ALIVE is the PDB template (FableWin ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z,
//   inline in release) over the inline CThing::IsDead (`test byte ptr [eax+91h],1`); the CPlayer& lives in esi across the two calls.
// Layout: engine/CWorld.h retail opt-in (FABLE_RETAIL_CWORLD), engine/CPlayer.h (FABLE_RETAIL_PLAYER, + ResetModes /
//   GetPControlledCreature), engine/CPlayerManager.h (PDB methods), engine/CClass.h retail opt-in (CClass : CBaseObject, sizeof 0x14).
//   CThing has no engine header (engine/_quarantine only): every Ego_r member restated at its retail offset (Ego_r - 0x10 past the
//   containers: UsingComponents +0x20, PTCGraphicAppearance +0x64, ThingTypeFlags +0x6C, PBaseDef +0x70, Dead = bit 0 of +0x91, as
//   the landed CThing rows), sizeof 0x98. The creature chain's own members are opaque storage: CThingPhysical / CThingGameObject /
//   CThingCreatureBase splits from the generated headers' retail sizes (0xC0 / 0xD0 / 0x160), CThingPlayerCreature 0x208 proven by
//   its allocation (`mov ecx,208h ; call CThing::operator new` before the ctor 0x0052AB20 at 0x0052B882 / 0x006AC91C).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#include "engine/CClass.h"
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"

#include "engine/CThing.h"
FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);

class CThingPhysical : public CThing {                  // RTTI CThingPhysical : CThing
public:
    unsigned char CThingPhysicalMembers_[0xC0 - 0x98];         // own members (generated retail size 0xC0, not modelled)
};
class CThingGameObject : public CThingPhysical {        // RTTI CThingGameObject : CThingPhysical
public:
    unsigned char CThingGameObjectMembers_[0xD0 - 0xC0];       // own members (generated retail size 0xD0, not modelled)
};
class CThingCreatureBase : public CThingGameObject {    // RTTI CThingCreatureBase : CThingGameObject
public:
    unsigned char CThingCreatureBaseMembers_[0x160 - 0xD0];    // own members (generated retail size 0x160, not modelled)
};
class CThingPlayerCreature : public CThingCreatureBase {   // RTTI CThingPlayerCreature : CThingCreatureBase
public:
    unsigned char CThingPlayerCreatureMembers_[0x208 - 0x160]; // own members (not modelled)
};
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreature) == 0x208);

class CThingManager {
public:
    void UpdateDeaths();                                // 0x0051F070 ?UpdateDeaths@CThingManager@@QAEXXZ
};

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return &*thing && !thing->IsDead();
}

void CWorld::Unload()
{
    CPlayer& player = PlayerManager->GetMainPlayer();
    player.ResetModes();
    CThingPlayerCreature* creature = player.GetPControlledCreature();
    if (IS_THING_ALIVE(creature))
        creature->Kill(false);
    GetThingManager().UpdateDeaths();
}