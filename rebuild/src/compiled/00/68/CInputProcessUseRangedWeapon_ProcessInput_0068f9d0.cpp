// CInputProcessUseRangedWeapon::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- outside ranged mode, forward the input to ProcessRangedAttackInput when the hero's primary carried thing is a live projectile weapon; in ranged mode, when the hero carries a melee weapon and the primary weapon is not an unloaded analogue-loading projectile weapon, cancel aiming, reset the projectile mode and leave player mode 0x22
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068F9D0, length 444 bytes (`ret 8`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the player's character pointer +0x34); the interface map LowerBound
//   const @ 0x004365B0 / non-const @ 0x0040F020; CTCCarrying::GetThingInPrimarySlot @ 0x006A1440; CTCWeapon::IsProjectileWeapon
//   @ 0x005DAA40 / IsProjectileWeaponLoaded @ 0x005DA980 / IsAnalogueLoading @ 0x005DA950; IsThingCarryingMeleeWeapon @ 0x006A1620
//   (`__fastcall`); ProcessRangedAttackInput @ 0x0068EF50; CTCHeroRangedAttacks::CancelAimingWeapon @ 0x00789FF0 /
//   ResetProjectileWeaponMode @ 0x00789B30; CThing::GetPlayer @ 0x004C7A30; CPlayer::RemoveMode @ 0x00634190.
// Layout: CInputProcessGameBase (MyPlayer +0x18); retail CPlayer: character pointer +0x34, ranged-mode flag +0x218 (descriptive);
//   CThing interface bits +0x20 / map +0x44 / dead bit +0x91 as the landed CTCWound ctor 0x00757680. Interfaces 0x46 CTCCarrying,
//   0x48 CTCWeapon, 0x81 CTCHeroRangedAttacks.
// Shape: the carrying and ranged-attacks lookups are pointer-returning inline GetInterface forms (`xor ecx,ecx` default); the weapon
//   test is the out-parameter form inside one && chain. The carrying component is a NAMED local: that lets the two lookup-key
//   temporaries share one stack slot (first half: the 4-byte frame word; second half: the dead `event` slot) -- chaining
//   `GetConstInterface(0x46)->GetThingInPrimarySlot()` gives each key its own slot (8-byte frame).
class CInputEvent;
class CProcessedInput;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CPlayer;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                       // +0x91 bit 0: dead
    CPlayer* GetPlayer_004c7a30();                          // 0x004C7A30
    template<class T> __forceinline T* GetConstInterface(long interfaceType) const
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            result = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
        return result;
    }
    template<class T> __forceinline T* GetInterface(long interfaceType)
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            result = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
        return result;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();               // 0x006A1440
};
struct CTCWeapon {
    bool IsProjectileWeapon();                     // 0x005DAA40
    bool IsProjectileWeaponLoaded();               // 0x005DA980
    bool IsAnalogueLoading();                      // 0x005DA950
};
struct CTCHeroRangedAttacks {
    void CancelAimingWeapon();                     // 0x00789FF0
    void ResetProjectileWeaponMode();              // 0x00789B30
};
bool __fastcall IsThingCarryingMeleeWeapon(const CThing* thing);   // 0x006A1620
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct CPlayer {
    unsigned char           _pad00[0x34];
    CBaseIntelligentPointer PCharacter;            // +0x34
    unsigned char           _pad3c[0x218 - 0x3c];
    bool                    InRangedWeaponMode;    // +0x218 (descriptive)
    CThing* GetCharacter() const { return PCharacter.GetPItem(); }
    void RemoveMode(long mode);                    // 0x00634190
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);   // slot 4
    unsigned char           _pad04[0x10];
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
};
class CInputProcessUseRangedWeapon : public CInputProcessGameBase {
public:
    void ProcessRangedAttackInput(const CInputEvent& event, CProcessedInput& processed, CThing* weapon);   // 0x0068EF50
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessUseRangedWeapon::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetCharacter();
    if (!MyPlayer->InRangedWeaponMode) {
        CTCCarrying* carrying = hero->GetConstInterface<CTCCarrying>(0x46);
        CThing* weapon = carrying->GetThingInPrimarySlot();
        CTCWeapon* tcWeapon;
        if (weapon && !(weapon->m_flags91 & 1) && weapon->GetInterface(&tcWeapon, 0x48) && tcWeapon->IsProjectileWeapon())
            ProcessRangedAttackInput(event, processed, weapon);
        return;
    }
    if (!IsThingCarryingMeleeWeapon(hero))
        return;
    CTCCarrying* carrying = hero->GetConstInterface<CTCCarrying>(0x46);
    CThing* weapon = carrying->GetThingInPrimarySlot();
    CTCWeapon* tcWeapon;
    if (!weapon || (weapon->m_flags91 & 1) || !weapon->GetInterface(&tcWeapon, 0x48) || tcWeapon->IsProjectileWeaponLoaded()
        || !tcWeapon->IsAnalogueLoading()) {
        CTCHeroRangedAttacks* rangedAttacks = hero->GetInterface<CTCHeroRangedAttacks>(0x81);
        rangedAttacks->CancelAimingWeapon();
        rangedAttacks->ResetProjectileWeaponMode();
        hero->GetPlayer_004c7a30()->RemoveMode(0x22);
    }
}