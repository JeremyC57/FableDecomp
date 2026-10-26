// NInputProcess::IsThingToKeepStrafing(const CThingPlayerCreature& creature) -- the hero keeps strafing while it holds a live projectile weapon outside ranged-weapon mode and is firing, loading, has the strafe action (0xA, held) queued or is busy with an action, or while its weapon is analogue-loading but not yet loaded
// Leaf: ?IsThingToKeepStrafing@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068EB20, 384 bytes (380 without padding), `__fastcall` (ecx = creature), plain `ret`,
//   bool in al. Ego_r.pdb `?IsThingToKeepStrafing@NInputProcess@@YI_NABVCThingPlayerCreature@@@Z`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (x3: 0x46 CTCCarrying, 0x48 CTCWeapon, 0x81
//   CTCHeroRangedAttacks), CTCCarrying::GetThingInPrimarySlot 0x006A1440, CTCWeapon::IsProjectileWeapon 0x005DAA40 /
//   IsAnalogueLoading 0x005DA950 / IsProjectileWeaponLoaded 0x005DA980, CMainGameComponent::GetGamePlayerInterface 0x00416359
//   (on g_pMainGameComponent 0x013B86A0), CThing::GetPlayer const 0x004C7A10 (twice), CPlayer::GetJoystickDeviceNumber 0x00487510,
//   CGamePlayerInterface::IsGameActionEventInQueue 0x00446B10, CThingCreatureBase::IsPerformingAnyAction 0x006622A0.
// Layout: retail CThing interface bits +0x20 (0x46 = word +0x28 bit 6, 0x48 = word +0x28 bit 8, 0x81 = word +0x30 bit 1), map
//   +0x44, dead bit +0x91; CPlayer InRangedWeaponMode +0x218 (descriptive, as the landed 0x0068F9D0); CTCHeroRangedAttacks
//   FireCycleInProgress +0x14, Loading +0x17 (landed ctor 0x00789BB0).
// Shape: the carrying lookup is the out-parameter GetInterface with no null default (the absent path reads the shared frame word);
//   the weapon pointer is pre-nulled (`xor ebp,ebp`) and filled by the out-parameter form inside the && chain; the ranged-attacks
//   lookup is the pointer-returning form (`xor eax,eax` default, dereferenced unconditionally). The three bools are set-if
//   locals (`= false; if (x) = true;`), homed in bytes 1..3 of the 4-byte frame word; the game-player interface is a local taken
//   before the GetPlayer()->GetJoystickDeviceNumber() argument chain. A same-TU noinline LowerBound body keeps the lookup keys uncaptured
//   so they share the second frame word with the carrying pointer.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CPlayer {
public:
    unsigned char _pad000[0x218];
    bool          InRangedWeaponMode;               // +0x218
public:
    long GetJoystickDeviceNumber() const;   // 0x00487510
};
class CGamePlayerInterface {
public:
    bool IsGameActionEventInQueue(long action, long buttonEventType, long playerNumber);   // 0x00446B10
};
class CMainGameComponent {
public:
    CGamePlayerInterface* GetGamePlayerInterface(); // 0x00416359
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
inline CMainGameComponent* GetMainGameComponent() { return g_pMainGameComponent; }

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                          // +0x91 bit 0: dead
    CPlayer* GetPlayer() const;                     // 0x004C7A10
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetConstInterface(long interfaceType) const
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            result = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
        return result;
    }
};
class CThingCreatureBase : public CThing {
public:
    bool IsPerformingAnyAction() const;             // 0x006622A0
};
class CThingPlayerCreature : public CThingCreatureBase { };

class CTCCarrying {
public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CTCWeapon {
public:
    bool IsAnalogueLoading();                       // 0x005DA950
    bool IsProjectileWeaponLoaded();                // 0x005DA980
public:
    bool IsProjectileWeapon() const;   // 0x005DAA40
};
class CTCHeroRangedAttacks {
public:
    unsigned char _pad00[0x14];
    bool FireCycleInProgress;                       // +0x14
    bool Aiming;                                    // +0x15
    bool FireQueued;                                // +0x16
    bool Loading;                                   // +0x17
};

namespace NInputProcess {

bool __fastcall IsThingToKeepStrafing(const CThingPlayerCreature& creature)
{
    CTCCarrying* carrying;
    creature.GetInterface(&carrying, 0x46);
    CThing* primary = carrying->GetThingInPrimarySlot();
    bool holdingProjectileWeapon = false;
    CTCWeapon* weapon = 0;
    if (primary && !(primary->Flags91 & 1) && primary->GetInterface(&weapon, 0x48) && weapon->IsProjectileWeapon())
        holdingProjectileWeapon = true;
    CTCHeroRangedAttacks* ranged = creature.GetConstInterface<CTCHeroRangedAttacks>(0x81);
    bool fireCycle = false;
    if (ranged->FireCycleInProgress)
        fireCycle = true;
    bool loading = false;
    if (ranged->Loading)
        loading = true;
    CGamePlayerInterface* gamePlayerInterface = GetMainGameComponent()->GetGamePlayerInterface();
    bool actionQueued = gamePlayerInterface->IsGameActionEventInQueue(0xa, 2, creature.GetPlayer()->GetJoystickDeviceNumber());
    CPlayer* player = creature.GetPlayer();
    if (holdingProjectileWeapon && !player->InRangedWeaponMode) {
        if (actionQueued || fireCycle || loading || creature.IsPerformingAnyAction())
            return true;
    }
    if (weapon && weapon->IsAnalogueLoading() && !weapon->IsProjectileWeaponLoaded())
        return true;
    return false;
}

}