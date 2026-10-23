// CTCCreatureOpinionOfHero::KnowsOfDeed(long deedHandle, bool includePending) -- look the deed up in the hero's deed log and ask the typed KnowsOfDeed whether this creature knows of it
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Leaf: ?KnowsOfDeed@CTCCreatureOpinionOfHero@@QAE_NJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00683650, length 125 bytes (`ret 8`).
// Callees: CTCBase::GetPlayerManager @ 0x00686D50, CPlayerManager::GetMainPlayer @ 0x00449960, CPlayer::GetHeroThing @ 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x51 = CTCHeroOpinionDeedLog,
// word 2 bit 17), CTCHeroOpinionDeedLog::GetDeedFromHandle(long) @ 0x006C58F0, the three-argument
// KnowsOfDeed(long, EOpinionDeedType, bool) @ 0x00681AE0 (landed).
// Shape: an inline accessor `GetDeedLog(hero)->GetDeedFromHandle(h)` (the component goes straight into ecx); GetInterface result unchecked (the absent arm reads the unset out pointer, which VC placed in the deed handle's
// argument slot); the deed's type is its first word; the typed overload's result is returned.
// Model: engine/CTCCreatureOpinionOfHero.h is the PDB layout; retail STLport lists are 4 bytes (PDB 12), so HeroOpinionEnemy
// sits at +0x342 (as in the landed SetHeroOpinionEnemy 0x006815D0 and KnowsOfDeed 0x00681AE0), hence a local model.
// Retail CThing: interface bitset +0x20, map +0x44, Dead bit +0x91.
struct CTCBaseInterface;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCVillage {
    bool GetIsHeroEnemyOfGuards();              // 0x00658604
};
struct CTCVillageMember {                       // interface 0x23
    CTCVillage* GetVillage();                   // 0x007EE540 (the manifest's "IS_THING_ALIVE<...>" is propagated)
};
struct COpinionDeed {
    long Type;                                  // +0x00 EOpinionDeedType
};
struct CTCHeroOpinionDeedLog {                  // interface 0x51
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
};
struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0 (the manifest's "_Cons_val<...>" is propagated)
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                   // 0x00449960
};
struct CTCCreatureOpinionOfHero {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    CPlayerManager* GetPlayerManager_00686d50();         // 0x00686D50 (CTCBase)
    bool KnowsOfDeed(long deedHandle, long deedType, bool includePending);   // 0x00681AE0
    bool KnowsOfDeed(long deedHandle, bool includePending);
};

static __forceinline CTCBaseInterface* GetTC(CThing* thing, long type)
{
    CInterfaceMap* map = &thing->m_interfaces;
    long key = type;
    CInterfaceMap::SEntry* it = map->LowerBound(&key);
    return ((it == map->last || type < it->type) ? map->last : it)->component;
}
static __forceinline CTCHeroOpinionDeedLog* GetDeedLog(CThing* thing)
{
    CTCBaseInterface* tc;
    if (thing->HasTC(0x51))
        tc = GetTC(thing, 0x51);
    return (CTCHeroOpinionDeedLog*)tc;
}

bool CTCCreatureOpinionOfHero::KnowsOfDeed(long deedHandle, bool includePending)
{
    CThing* hero = GetPlayerManager_00686d50()->GetMainPlayer()->GetHeroThing();
    if (hero) {
        COpinionDeed* deed = GetDeedLog(hero)->GetDeedFromHandle(deedHandle);
        if (deed)
            return KnowsOfDeed(deedHandle, deed->Type, includePending);
    }
    return false;
}