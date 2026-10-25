// Behaviour test for 0x0068EB20: NInputProcess::IsThingToKeepStrafing(const CThingPlayerCreature& creature) -- the hero keeps strafing while it holds a live projectile weapon outside ranged-weapon mode and is firing, loading, has the strafe action (0xA, held) queued or is busy with an action, or while its weapon is analogue-loading but not yet loaded
// A copy of the source model and logic (compiled separately from src/0068eb20.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
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
    long GetPlayerNumber();                         // 0x00487510
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
    CThing* GetThingInPrimarySlot();                // 0x006A1440
};
class CTCWeapon {
public:
    bool IsProjectileWeapon();                      // 0x005DAA40
    bool IsAnalogueLoading();                       // 0x005DA950
    bool IsProjectileWeaponLoaded();                // 0x005DA980
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
    bool actionQueued = gamePlayerInterface->IsGameActionEventInQueue(0xa, 2, creature.GetPlayer()->GetPlayerNumber());
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

// ---- test stand-ins ----
static CThing* g_primary = 0;
static CPlayer* g_player = 0;
static bool g_isProjectile, g_analogue, g_loaded, g_queued, g_busy;
static long g_queueArgs[3];
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_primary; }
bool CTCWeapon::IsProjectileWeapon() { return g_isProjectile; }
bool CTCWeapon::IsAnalogueLoading() { return g_analogue; }
bool CTCWeapon::IsProjectileWeaponLoaded() { return g_loaded; }
CPlayer* CThing::GetPlayer() const { return g_player; }
long CPlayer::GetPlayerNumber() { return 3; }
bool CThingCreatureBase::IsPerformingAnyAction() const { return g_busy; }
bool CGamePlayerInterface::IsGameActionEventInQueue(long action, long buttonEventType, long playerNumber)
{
    g_queueArgs[0] = action; g_queueArgs[1] = buttonEventType; g_queueArgs[2] = playerNumber;
    return g_queued;
}
static CGamePlayerInterface g_gpi;
CGamePlayerInterface* CMainGameComponent::GetGamePlayerInterface() { return &g_gpi; }
static CMainGameComponent g_main;
CMainGameComponent* g_pMainGameComponent = &g_main;

static void SetBit(CThing& t, long type, bool on)
{
    if (on) t.m_presentBits[type >> 5] |= 1UL << (type & 31);
    else t.m_presentBits[type >> 5] &= ~(1UL << (type & 31));
}
static int Fail(unsigned int combo) { std::printf("0068eb20_TEST FAIL (combo %u)\n", combo); return 1; }

int main()
{
    static CThingPlayerCreature hero;
    static CThing sword;
    static CPlayer player;
    std::memset(&hero, 0, sizeof hero); std::memset(&sword, 0, sizeof sword); std::memset(&player, 0, sizeof player);
    CTCCarrying carrying;
    CTCHeroRangedAttacks ranged; std::memset(&ranged, 0, sizeof ranged);
    CTCWeapon weapon;
    CInterfaceMap::SEntry heroEntries[3] = { { 0x10, 0 }, { 0x46, (CTCBase*)&carrying }, { 0x81, (CTCBase*)&ranged } };
    hero.m_interfaces.first = heroEntries; hero.m_interfaces.last = heroEntries + 3;
    SetBit(hero, 0x46, true); SetBit(hero, 0x81, true);
    CInterfaceMap::SEntry swordEntries[2] = { { 0x47, 0 }, { 0x48, (CTCBase*)&weapon } };
    sword.m_interfaces.first = swordEntries; sword.m_interfaces.last = swordEntries + 2;
    g_player = &player;
    for (unsigned int c = 0; c < 2048; ++c) {
        bool hasPrimary = (c & 1) != 0, dead = (c & 2) != 0, weaponBit = (c & 4) != 0;
        g_isProjectile = (c & 8) != 0; player.InRangedWeaponMode = (c & 16) != 0; g_queued = (c & 32) != 0;
        ranged.FireCycleInProgress = (c & 64) != 0; ranged.Loading = (c & 128) != 0; g_busy = (c & 256) != 0;
        g_analogue = (c & 512) != 0; g_loaded = (c & 1024) != 0;
        ranged.Aiming = ranged.FireQueued = (c & 96) == 96;    // the other ranged flags never matter
        g_primary = hasPrimary ? &sword : 0;
        sword.Flags91 = (unsigned char)(dead ? 0x81 : 0x80);
        SetBit(sword, 0x48, weaponBit);
        bool live = hasPrimary && !dead && weaponBit;
        bool holding = live && g_isProjectile;
        bool want = (holding && !player.InRangedWeaponMode && (g_queued || ranged.FireCycleInProgress || ranged.Loading || g_busy)) ||
                    (live && g_analogue && !g_loaded);
        g_queueArgs[0] = g_queueArgs[1] = g_queueArgs[2] = 0;
        if (NInputProcess::IsThingToKeepStrafing(hero) != want) return Fail(c);
        if (g_queueArgs[0] != 0xa || g_queueArgs[1] != 2 || g_queueArgs[2] != 3) return Fail(c);   // always asked
    }
    std::printf("0068eb20_TEST PASS\n");
    return 0;
}