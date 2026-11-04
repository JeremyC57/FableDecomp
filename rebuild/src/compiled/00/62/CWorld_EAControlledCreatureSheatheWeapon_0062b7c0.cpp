// CWorld::EAControlledCreatureSheatheWeapon(const CGameEvent* event) -- unless the player is in mode 0x2F, have the event player's controlled creature sheathe the weapon in its primary carry slot, stop ranged aiming and leave ranged mode (0x22)
// Leaf: ?EAControlledCreatureSheatheWeapon@CWorld@@QAEXPBVCGameEvent@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062B7C0, 280 bytes, `ret 4`.
// Identity: Ego_r.pdb `?EAControlledCreatureSheatheWeapon@CWorld@@QAEXPBVCGameEvent@@@Z`; `this` is the CWorld (protected
//   CWorld::GetPlayer(unsigned long) 0x0049D850 is called on it with the event's player byte, as in the landed siblings
//   CWorld::EAControlledCreatureUseAbility 0x00629C40 and EARemoveCurrentMode 0x0062C4B0); the catalog label
//   "CTCHeroRangedAttacks::OnWeaponSwitch" is wrong.
// Callees (PDB spellings): CWorld::GetPlayer(unsigned long) 0x0049D850 (twice), CPlayer::HasMode(EPlayerMode) const
//   0x00634160, CPlayer::GetPControlledCreature() const 0x00487DC0, the interface map LowerBound 0x0040F020 (inlined HasTC +
//   GetTC: CTCCarrying 0x46, CTCHeroRangedAttacks 0x81 = bit 1 of +0x30; the carried item's weapon test 0x48),
//   CTCCarrying::GetThingInPrimarySlot() const 0x006A1440, CCreatureAction_SheatheItemToInventory(CThingCreatureBase&,
//   CCreatureActionBase*, long) 0x0069FFD0 and its dtor 0x0048C950, CThingCreatureBase::SetCurrentAction(const
//   CCreatureActionBase&) 0x006644F0, CTCHeroRangedAttacks::CancelAimingWeapon 0x00789FF0 / ResetProjectileWeaponMode
//   0x00789B30, CThing::GetPlayer() (CPlayer&) 0x004C7A30, CPlayer::RemoveMode(EPlayerMode) 0x00634190.
// Layout: CGameEvent Player +4 (signed char); retail CThing bitset<274> +0x20, interface map +0x44, dead bit +0x91.
// Shape (wave 25): the second GetPlayer result is re-assigned to the `player` local before GetPControlledCreature, as the
//   landed sibling EAControlledCreatureUseAbility does; that, with the PDB's const/reference callee spellings, gives retail's
//   colouring (creature in the dead `this` register esi, the interface map in the dead event register edi). The action is a
//   temporary passed straight to SetCurrentAction; the ranged block sits inside the carrying test.
// Layout: `this` is engine/CWorld.h (layout-renamed base, no member read); engine/CGameEvent.h as is; CPlayer and
//   CTCHeroRangedAttacks are engine/CPlayer.h / engine/CTCHeroRangedAttacks.h layout-renamed bases carrying the PDB methods;
//   CCreatureActionBase is local because engine/CCreatureActionBase.h models the vptr as data and the temporary's dtor chain
//   needs the real virtual dtor (retail sizeof 0xa8, the action 0xb0).
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer
#define CTCHeroRangedAttacks CTCHeroRangedAttacksLayout
#include "engine/CTCHeroRangedAttacks.h"
#undef CTCHeroRangedAttacks
#include "engine/CGameEvent.h"
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CPlayer;
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> UsingComponents;                       // +0x20 bitset<274>
    CInterfaceMap Components;                               // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    CPlayer& GetPlayer();                                   // 0x004C7A30
    __forceinline bool HasTC(long type) const
    {
        if (UsingComponents.test(type))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return (T*)it->second;
    }
};
class CCreatureActionBase {
public:                                                     // retail sizeof 0xa8
    virtual ~CCreatureActionBase();
    unsigned char _pad04[0xa8 - 0x04];
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase {
public:   // retail sizeof 0xb0
    unsigned char PNextAction[8];                           // +0xa8 CCountedPointer<CCreatureActionBase>
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase& owner, CCreatureActionBase* next, long kind);   // 0x0069FFD0
    virtual ~CCreatureAction_SheatheItemToInventory();      // 0x0048C950
};
class CTCCarrying {
public:
    CThing* GetThingInPrimarySlot() const;                  // 0x006A1440
};
class CTCHeroRangedAttacks : public CTCHeroRangedAttacksLayout {
public:
    void CancelAimingWeapon();                              // 0x00789FF0
    void ResetProjectileWeaponMode();                       // 0x00789B30
};
enum EPlayerMode { PLAYER_MODE_RANGED = 0x22, PLAYER_MODE_0x2F = 0x2f };
class CPlayer : public CPlayerLayout {
public:
    bool HasMode(EPlayerMode mode) const;                   // 0x00634160
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
    void RemoveMode(EPlayerMode mode);                      // 0x00634190
};

class CWorld : public CWorldLayout {
protected:
    CPlayer* GetPlayer(unsigned long number);               // 0x0049D850
public:
    void EAControlledCreatureSheatheWeapon(const CGameEvent* event);
};

void CWorld::EAControlledCreatureSheatheWeapon(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (player->HasMode(PLAYER_MODE_0x2F))
        return;
    player = GetPlayer(event->Player);
    CThingPlayerCreature* hero = player->GetPControlledCreature();
    if (hero && !hero->Dead && hero->HasTC(0x46)) {
        CThing* item = hero->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        if (item && !item->Dead && item->HasTC(0x48))
            hero->SetCurrentAction(CCreatureAction_SheatheItemToInventory(*hero, 0, 100));
        if (hero->HasTC(0x81)) {
            CTCHeroRangedAttacks* ranged = hero->GetTC<CTCHeroRangedAttacks>(0x81);
            ranged->CancelAimingWeapon();
            ranged->ResetProjectileWeaponMode();
            hero->GetPlayer().RemoveMode(PLAYER_MODE_RANGED);
        }
    }
}