// Grade: DIFFER(378v380) -- orb out-slot homed at [esp+0x20] instead of retail's reuse of the dead 'type' param slot [esp+0x14]; eax/ecx order around the hero-experience call. Non-helper form (CollectExperienceOrb inline at call site) is 384v380.
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
//   word, which still holds the orb, when the component is missing).
struct CTCBase;
#pragma pack(push, 1)
struct C3DVector { float X, Y, Z; };
#pragma pack(pop)
enum ECollideType { COLLIDE_TYPE_0 };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CPlayerManager;
struct CThing;
struct CPlayer { CThing* GetHeroThing(); };             // 0x00487DC0
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
    long GetMainPlayerNumber();                         // 0x00449700
};
struct CTCDExperienceOrb {
    unsigned char _pad00[0x10];
    long          ExperiencePoints;                     // +0x10
    void OnCollected(CThing* collector);                // 0x0077A180
};
struct CTCHeroExperience { void AddExperience(long points); };   // 0x006D77C0
struct CTCCoopSpirit { void AddExperience(long points); };       // 0x0066FB00

struct CThing {
    unsigned char  _pad00[0x10];
    unsigned char  ThingType;                           // +0x10
    unsigned char  _pad11[0x20 - 0x11];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x90 - 0x4c];
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
    __forceinline CTCCoopSpirit* GetCoopSpirit()
    {
        CTCCoopSpirit* spirit;
        if (HasTC(0xc7))
            spirit = GetTC<CTCCoopSpirit>(0xc7);
        return spirit;
    }
};
struct CThingCreatureBase : CThing {
    void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing);   // 0x00667310
};
struct CThingPlayerCreature : CThingCreatureBase {
    unsigned char _pad94[0x1cc - 0x94];                  // CThing rounds to 0x94
    bool          CollidedWithThing;                    // +0x1cc
    C3DVector     LastThingCollisionNormal;             // +0x1cd
    C3DVector     LastThingCollisionPosition;           // +0x1d9
    void OnCollisionWithObject(CThing* thing);          // 0x006ACA30
    __forceinline void CollectExperienceOrb(CTCDExperienceOrb* orb, CThing* hero)
    {
        if (hero && hero->HasTC(0x68))
            hero->GetTC<CTCHeroExperience>(0x68)->AddExperience(orb->ExperiencePoints);
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
