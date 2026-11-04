// CThingPlayerCreature::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing) -- after the creature handling, a live player creature touching (type 1) a live, non-limbo thing remembers the collision (flag, normal, position); objects go to OnCollisionWithObject, experience orbs give their points to the main hero's experience component (0x68) and, for a co-op player, to this creature's co-op spirit (0xc7), then are collected
// Leaf: ?OnCollision@CThingPlayerCreature
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AD580, length 380 bytes (+4 int3), `ret 10h`; vtable slot 41 (the
//   catalog's "CThingPlayerCreature::vfunc_41").
//   Name and signature from Ego_r.pdb (`?OnCollision@CThingPlayerCreature@@UAEXW4ECollideType@@ABVC3DVector@@1PAVCThing@@@Z`).
// Callees: CThingCreatureBase::OnCollision @ 0x00667310, CThingPlayerCreature::OnCollisionWithObject @ 0x006ACA30,
//   CThing::GetTCDriver<CTCDExperienceOrb>(CTCDExperienceOrb**) @ 0x006AE5C0, CThing::GetPlayerManager @ 0x004C7A00,
//   CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, LowerBound @ 0x0040F020,
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
#include "engine/C3DVector.h"
#pragma pack(pop)
enum ECollideType { COLLIDE_TYPE_0 };

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CPlayerManager;
class CThing;
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };             // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
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
    unsigned char  _pad54[0x8];
    void*          PTCDriver;                           // +0x5c
    unsigned char  _pad60[0x90 - 0x60];
    signed char    PlayerNumber;                        // +0x90
    unsigned char  Dead : 1;                            // +0x91 bit 0
    unsigned char  _bits91 : 4;
    unsigned char  InLimbo : 1;                         // +0x91 bit 5
    unsigned char  _bits91b : 2;
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
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
        CInterfaceMap::value_type* end = map->end();
        return (T*)(it == end || type < it->first ? end : it)->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
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
protected:
    CPlayerManager& GetPlayerManager();   // 0x004C7A00
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
        if (PlayerNumber != GetPlayerManager().GetMainPlayerNumber())
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
    CollectExperienceOrb(orb, GetPlayerManager().GetMainPlayer().GetPControlledCreature());
}