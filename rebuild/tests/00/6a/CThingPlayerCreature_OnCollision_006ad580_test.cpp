#include <stdio.h>
#include <string.h>
// CThingPlayerCreature::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing) -- after the creature handling, a live player creature touching (type 1) a live, non-limbo thing remembers the collision (flag, normal, position); objects go to OnCollisionWithObject, experience orbs give their points to the main hero's experience component (0x68) and, for a co-op player, to this creature's co-op spirit (0xc7), then are collected
// Leaf: ?OnCollision@CThingPlayerCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AD580, length 380 bytes (+4 int3), `ret 10h`; vtable slot 41 (the
//   catalog's "CThingPlayerCreature::vfunc_41").
//   Name and signature from Ego_r.pdb (`?OnCollision@CThingPlayerCreature@@UAEXW4ECollideType@@ABVC3DVector@@1PAVCThing@@@Z`).
// Callees: CThingCreatureBase::OnCollision @ 0x00667310, CThingPlayerCreature::OnCollisionWithObject @ 0x006ACA30,
//   CThing::GetTCDriver<CTCDExperienceOrb>(CTCDExperienceOrb**) @ 0x006AE5C0, CThing::GetPlayerManager @ 0x004C7A00,
//   CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, LowerBound @ 0x0040F020,
//   CTCHeroExperience::AddExperience(long) @ 0x006D77C0, CPlayerManager @ 0x00449700 (the main player's number;
//   manifest "CWorld::DrawGetEnvironment", propagated), CTCCoopSpirit::AddExperience(long) @ 0x0066FB00,
//   CTCDExperienceOrb::OnCollected(CThing*) @ 0x0077A180.
// Layout (retail): CThing ThingType +0x10, bitset +0x20 (0x68/0x69 = bits 8/9 of +0x2c, 0xc7 = bit 7 of +0x38), map
//   +0x44, PlayerNumber +0x90, Dead bit 0 / InLimbo bit 5 of +0x91; CThingPlayerCreature CollidedWithThing +0x1cc,
//   LastThingCollisionNormal +0x1cd, LastThingCollisionPosition +0x1d9 (Ego_r.pdb +0x1f8/+0x1f9/+0x205, unaligned);
//   CTCDExperienceOrb::ExperiencePoints +0x10 (PDB).
// Shape: the co-op spirit comes from the inline "uninitialised when absent" accessor (retail reads the shared frame
//   word, which still holds the orb, when the component is missing). GetTCDriver<CTCDExperienceOrb> has its real
//   same-TU body, so the orb out word does not escape and is homed in the dead `type` argument slot (as retail);
//   the hero-experience component comes from the out-parameter GetInterface (ternary entry pick), and LowerBound
//   has a same-TU body.
class CTCBase;
#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };
#pragma pack(pop)
enum ECollideType { COLLIDE_TYPE_0 };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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

class CPlayerManager;
class CThing;
class CPlayer { public: CThing* GetHeroThing(); };             // 0x00487DC0
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                           // 0x00449970
    long GetMainPlayerNumber();                         // 0x00449700
};
class CTCDExperienceOrb { public:
    unsigned char _pad00[0x10];
    long          ExperiencePoints;                     // +0x10
    void OnCollected(CThing* collector);                // 0x0077A180
};
class CTCHeroExperience { public: void AddExperience(long points); };   // 0x006D77C0
class CTCCoopSpirit { public: void AddExperience(long points); };       // 0x0066FB00

class CThing { public:
    unsigned char  _pad00[0x10];
    unsigned char  ThingType;                           // +0x10
    unsigned char  _pad11[0x20 - 0x11];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x5c - 0x4c];
    void*          PTCDriver;                           // +0x5c
    unsigned char  _pad60[0x90 - 0x60];
    signed char    PlayerNumber;                        // +0x90
    unsigned char  Dead : 1;                            // +0x91 bit 0
    unsigned char  _bits91 : 4;
    unsigned char  InLimbo : 1;                         // +0x91 bit 5
    unsigned char  _bits91b : 2;
    CPlayerManager* GetPlayerManager();                 // 0x004C7A00
    bool GetTCDriver(CTCDExperienceOrb** out);          // 0x006AE5C0 (GetTCDriver<CTCDExperienceOrb>)
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (T*)(it == end || type < it->type ? end : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    __forceinline CTCCoopSpirit* GetCoopSpirit()
    {
        CTCCoopSpirit* spirit;
        if (HasTC(0xc7))
            spirit = GetTC<CTCCoopSpirit>(0xc7);
        return spirit;
    }
};
#pragma auto_inline(off)
// Same-TU body of the GetTCDriver<CTCDExperienceOrb> instantiation (retail 0x006AE5C0): the driver pointer is written
// only when the experience-orb interface (0x69) is present.
__declspec(noinline) bool CThing::GetTCDriver(CTCDExperienceOrb** out)
{
    if (m_presentBits[0x69 >> 5] & (1UL << (0x69 & 31))) {
        *out = (CTCDExperienceOrb*)PTCDriver;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThingCreatureBase : public CThing { public:
    void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing);   // 0x00667310
};
class CThingPlayerCreature : public CThingCreatureBase { public:
    unsigned char _pad94[0x1cc - 0x94];                  // CThing rounds to 0x94
    bool          CollidedWithThing;                    // +0x1cc
    C3DVector     LastThingCollisionNormal;             // +0x1cd
    C3DVector     LastThingCollisionPosition;           // +0x1d9
    void OnCollisionWithObject(CThing* thing);          // 0x006ACA30
    __forceinline void CollectExperienceOrb(CTCDExperienceOrb* orb, CThing* hero)
    {
        CTCHeroExperience* experience;
        if (hero && hero->GetInterface(&experience, 0x68))
            experience->AddExperience(orb->ExperiencePoints);
        if (PlayerNumber != GetPlayerManager()->GetMainPlayerNumber())
            GetCoopSpirit()->AddExperience(orb->ExperiencePoints);
        orb->OnCollected(this);
    }
    void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing);
};

void CThingPlayerCreature::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing)
{
    CThingCreatureBase::OnCollision(type, position, normal, thing);
    if (Dead || !thing || thing->Dead || thing->InLimbo || type != 1)
        return;
    CollidedWithThing = true;
    LastThingCollisionNormal = normal;
    LastThingCollisionPosition = position;
    if (thing->ThingType == 5) {
        OnCollisionWithObject(thing);
        return;
    }
    CTCDExperienceOrb* orb;
    if (!thing->HasTC(0x69) || !thing->GetTCDriver(&orb))
        return;
    CollectExperienceOrb(orb, GetPlayerManager()->GetMainPlayer()->GetHeroThing());
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static int g_baseCalls, g_objectCalls, g_heroXp, g_coopXp, g_collected; static long g_heroPoints, g_coopPoints;
static void* g_heroXpThis; static void* g_coopXpThis; static CThing* g_collector; static void* g_collectedOrb; static CThing* g_objectThing;
void CThingCreatureBase::OnCollision(ECollideType, const C3DVector&, const C3DVector&, CThing*) { g_baseCalls++; }
void CThingPlayerCreature::OnCollisionWithObject(CThing* thing) { g_objectCalls++; g_objectThing = thing; }
static CPlayerManager g_manager; static CPlayer g_player; static CThing* g_hero; static long g_mainNumber;
CPlayerManager* CThing::GetPlayerManager() { return &g_manager; }
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
long CPlayerManager::GetMainPlayerNumber() { return g_mainNumber; }
CThing* CPlayer::GetHeroThing() { return g_hero; }
void CTCHeroExperience::AddExperience(long points) { g_heroXp++; g_heroPoints = points; g_heroXpThis = this; }
void CTCCoopSpirit::AddExperience(long points) { g_coopXp++; g_coopPoints = points; g_coopXpThis = this; }
void CTCDExperienceOrb::OnCollected(CThing* collector) { g_collected++; g_collector = collector; g_collectedOrb = this; }

static unsigned char g_selfBuf[0x200], g_thingBuf[0x100], g_heroBuf[0x100];
static CTCDExperienceOrb g_orb; static CTCHeroExperience g_heroExperience; static CTCCoopSpirit g_spirit;
static void Reset() { g_baseCalls = g_objectCalls = g_heroXp = g_coopXp = g_collected = 0; g_objectThing = 0; g_heroXpThis = g_coopXpThis = 0; g_collector = 0; }
int main()
{
    CThingPlayerCreature* self = (CThingPlayerCreature*)g_selfBuf;
    CThing* thing = (CThing*)g_thingBuf; CThing* hero = (CThing*)g_heroBuf;
    C3DVector position = { 1.0f, 2.0f, 3.0f }, normal = { 0.0f, 0.5f, 1.5f };
    ECollideType touch = (ECollideType)1;
    CInterfaceMap::SEntry heroMap[] = { { 0x10, 0 }, { 0x68, (CTCBase*)&g_heroExperience }, { 0x70, 0 } };
    hero->m_interfaces.first = heroMap; hero->m_interfaces.last = heroMap + 3;
    CInterfaceMap::SEntry selfMap[] = { { 0x10, 0 }, { 0xc7, (CTCBase*)&g_spirit } };
    self->m_interfaces.first = selfMap; self->m_interfaces.last = selfMap + 2;
    g_orb.ExperiencePoints = 250; thing->PTCDriver = &g_orb;
    // dead self: base handler only
    self->Dead = 1; Reset();
    self->OnCollision(touch, position, normal, thing);
    CHECK(g_baseCalls == 1 && self->CollidedWithThing == false && g_collected == 0);
    self->Dead = 0;
    // no thing / dead thing / thing in limbo / not a touch: base handler only
    Reset(); self->OnCollision(touch, position, normal, 0);
    CHECK(g_baseCalls == 1 && self->CollidedWithThing == false);
    thing->Dead = 1; Reset(); self->OnCollision(touch, position, normal, thing);
    CHECK(self->CollidedWithThing == false); thing->Dead = 0;
    thing->InLimbo = 1; Reset(); self->OnCollision(touch, position, normal, thing);
    CHECK(self->CollidedWithThing == false); thing->InLimbo = 0;
    Reset(); self->OnCollision((ECollideType)2, position, normal, thing);
    CHECK(self->CollidedWithThing == false && g_baseCalls == 1);
    // an object (type 5): the collision is remembered and handed to OnCollisionWithObject
    thing->ThingType = 5; Reset();
    self->OnCollision(touch, position, normal, thing);
    CHECK(self->CollidedWithThing == true && g_objectCalls == 1 && g_objectThing == thing && g_collected == 0);
    CHECK(self->LastThingCollisionNormal.Y == 0.5f && self->LastThingCollisionNormal.Z == 1.5f && self->LastThingCollisionPosition.X == 1.0f && self->LastThingCollisionPosition.Z == 3.0f);
    // not an orb (no interface 0x69): remembered, nothing else
    thing->ThingType = 2; self->CollidedWithThing = false; Reset();
    self->OnCollision(touch, position, normal, thing);
    CHECK(self->CollidedWithThing == true && g_objectCalls == 0 && g_collected == 0 && g_heroXp == 0);
    // an orb, no hero, main player: collected only
    thing->m_presentBits[0x69 >> 5] = 1UL << (0x69 & 31); g_hero = 0; self->PlayerNumber = 0; g_mainNumber = 0; Reset();
    self->OnCollision(touch, position, normal, thing);
    CHECK(g_collected == 1 && g_collector == self && g_collectedOrb == &g_orb && g_heroXp == 0 && g_coopXp == 0);
    // hero without the experience component, main player: collected only
    g_hero = hero; Reset();
    self->OnCollision(touch, position, normal, thing);
    CHECK(g_collected == 1 && g_heroXp == 0 && g_coopXp == 0);
    // hero with experience, co-op player with a co-op spirit: both get the points
    hero->m_presentBits[0x68 >> 5] = 1UL << (0x68 & 31); self->PlayerNumber = 1;
    self->m_presentBits[0xc7 >> 5] = 1UL << (0xc7 & 31); Reset();
    self->OnCollision(touch, position, normal, thing);
    CHECK(g_heroXp == 1 && g_heroPoints == 250 && g_heroXpThis == &g_heroExperience);
    CHECK(g_coopXp == 1 && g_coopPoints == 250 && g_coopXpThis == &g_spirit && g_collected == 1);
    // co-op player without the spirit: the call still goes out (on an unset pointer, as retail)
    self->m_presentBits[0xc7 >> 5] = 0; Reset();
    self->OnCollision(touch, position, normal, thing);
    CHECK(g_heroXp == 1 && g_coopXp == 1 && g_collected == 1);
    if (fails == 0) printf("006ad580_TEST PASS\n"); else printf("006ad580_TEST FAIL\n");
    return fails;
}