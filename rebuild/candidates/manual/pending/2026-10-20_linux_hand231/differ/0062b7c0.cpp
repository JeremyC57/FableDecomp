// Grade: DIFFER(280v280) -- register swap only: the hero lands in edi and the interface-map pointer in esi where retail puts the hero in the dead this register (esi) and the map in the dead event register (edi)
// CWorld::EAControlledCreatureSheatheWeapon(const CGameEvent* event) -- unless the player is in mode 0x2F, have the event player's hero sheathe the weapon in its primary carry slot, stop ranged aiming and leave ranged mode (0x22)
// Leaf: EAControlledCreatureSheatheWeapon
//
// Retail 0x0062B7C0, 280 bytes (`ret 4`). Identity: the catalog label "CTCHeroRangedAttacks::OnWeaponSwitch" is wrong -- `this` is
// the CWorld (CWorld::GetPlayer(long) @ 0x0049D850 is called on it with the event's player byte, like the landed
// CWorld::EARemoveCurrentMode 0x0062C4B0), and the body is Ego_r.pdb `?EAControlledCreatureSheatheWeapon@CWorld@@QAEXPBVCGameEvent@@@Z`
// (it builds the sheathe action; the callers 0x0062BA77/0x0062BCAE are its sibling event actions).
// Callees: CWorld::GetPlayer @ 0x0049D850 (twice), CPlayer::HasMode @ 0x00634160, CPlayer::GetHeroThing @ 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined HasTC + GetTC: CTCCarrying 0x46, CTCHeroRangedAttacks
// 0x81 = bit 1 of +0x30; the carried item's CTCWeapon test 0x48), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440,
// CCreatureAction_SheatheItemToInventory(CThingCreatureBase*, CCreatureActionBase*, long) @ 0x0069FFD0 and its out-of-line
// dtor @ 0x0048C950, CThingCreatureBase::SetCurrentAction @ 0x006644F0, CTCHeroRangedAttacks::CancelAimingWeapon @ 0x00789FF0
// and ::ResetProjectileWeaponMode @ 0x00789B30, CThing::GetPlayer @ 0x004C7A30, CPlayer::RemoveMode @ 0x00634190.
// Layout: CGameEvent Player +4 (signed char); retail CThing bitset +0x20, interface map +0x44, dead bit +0x91.
// Shape: the action is a temporary passed straight to SetCurrentAction (as the landed CPlayer::SetUpControlsBasedOnAggressiveMode
// 0x00487DE0); the ranged block sits inside the carrying test; the hero's interface map address is computed once.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct CPlayer;
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                       // +0x20 bitset<274>
    CInterfaceMap Components;                               // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    CPlayer* GetPlayer() const;                             // 0x004C7A30
    __forceinline bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CCreatureActionBase {                                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
};
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0
};
struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {   // retail sizeof 0xb0
    unsigned char PNextAction[8];                           // +0xa8 CCountedPointer<CCreatureActionBase>
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* next, long kind);   // 0x0069FFD0
    ~CCreatureAction_SheatheItemToInventory();              // 0x0048C950 (out-of-line copy)
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                        // 0x006A1440
};
struct CTCHeroRangedAttacks {
    void CancelAimingWeapon();                              // 0x00789FF0
    void ResetProjectileWeaponMode();                       // 0x00789B30
};
struct CPlayer {
    bool HasMode(long mode);                                // 0x00634160 (EPlayerMode)
    CThing* GetHeroThing();                                 // 0x00487DC0
    void RemoveMode(long mode);                             // 0x00634190
};
struct CGameEvent {
    long          Type;                                     // +0x00
    signed char   Player;                                   // +0x04
    unsigned char Data[32];                                 // +0x05
    unsigned char EndPos;                                   // +0x25
    bool          Valid;                                    // +0x26
    bool          Replacement;                              // +0x27
};

struct CWorld {
    CPlayer* GetPlayer(long number);                        // 0x0049D850
    void EAControlledCreatureSheatheWeapon(const CGameEvent* event);
};

void CWorld::EAControlledCreatureSheatheWeapon(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (player->HasMode(0x2f))
        return;
    CThingCreatureBase* hero = (CThingCreatureBase*)GetPlayer(event->Player)->GetHeroThing();
    if (hero && !hero->Dead && hero->HasTC(0x46)) {
        CThing* item = hero->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        if (item && !item->Dead && item->HasTC(0x48))
            hero->SetCurrentAction(&CCreatureAction_SheatheItemToInventory(hero, 0, 100));
        if (hero->HasTC(0x81)) {
            CTCHeroRangedAttacks* ranged = hero->GetTC<CTCHeroRangedAttacks>(0x81);
            ranged->CancelAimingWeapon();
            ranged->ResetProjectileWeaponMode();
            hero->GetPlayer()->RemoveMode(0x22);
        }
    }
}
