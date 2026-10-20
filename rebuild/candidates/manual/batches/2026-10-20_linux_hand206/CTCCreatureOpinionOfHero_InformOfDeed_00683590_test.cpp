// Behaviour test for CTCCreatureOpinionOfHero::InformOfDeed(long, bool) (0x00683590): no hero -> nothing; a known deed handle
// forwards (type, handle, the type's effects def, flag) to the full InformOfDeed and releases the def reference afterwards.
#include <cstdio>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) { *ppOut = (T*)GetTC(interfaceType); return true; }
        return false;
    }
};
static CThing* g_hero = 0;
struct CPlayer { CThing* GetHeroCreature() { return g_hero; } };
static CPlayer g_player;
struct CPlayerManager { CPlayer* GetMainPlayer() { return &g_player; } };
static CPlayerManager g_pm;
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
static int g_destroyed = 0;
struct CDefVirtual {
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; }
};
struct COpinionDeedEffectsDef : CDefVirtual, CRefCounted {
    long ForType;
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    explicit CDefPointer(T* object) : Object(object) { if (object) ++object->m_count; }   // test-only: built in the return slot
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
};
struct COpinionDeed { long Type; };
static COpinionDeedEffectsDef g_effects;
struct CTCHeroOpinionDeedLog {
    COpinionDeed Deed; long Handle;
    COpinionDeed* GetDeedFromHandle(long handle) { return handle == Handle ? &Deed : 0; }
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType) { g_effects.ForType = deedType; return CDefPointer<COpinionDeedEffectsDef>(&g_effects); }
};
struct CTCCreatureOpinionOfHero {
    virtual void v00() { }
    CThing* ParentThing;
    int Calls; long LastType, LastHandle; const COpinionDeedEffectsDef* LastEffects; bool LastFlag; long CountDuringCall;
    CPlayerManager* GetPlayerManager() { return &g_pm; }
    void InformOfDeed(long deedType, long deedHandle, const COpinionDeedEffectsDef* effects, bool flag)
    { ++Calls; LastType = deedType; LastHandle = deedHandle; LastEffects = effects; LastFlag = flag; CountDuringCall = effects->m_count; }
    void InformOfDeed(long deedHandle, bool flag);
};

void CTCCreatureOpinionOfHero::InformOfDeed(long deedHandle, bool flag)
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        COpinionDeed* deed = log->GetDeedFromHandle(deedHandle);
        if (deed)
            InformOfDeed(deed->Type, deedHandle, log->GetDeedEffectsDef(deed->Type).Object, flag);
    }
}

int main()
{
    int bad = 0;
    CTCHeroOpinionDeedLog log; log.Deed.Type = 6; log.Handle = 77;
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0;
    CInterfaceMap::SEntry e[1] = { { 0x51, (CTCBase*)&log } };
    hero.m_interfaces.first = e; hero.m_interfaces.last = e + 1; hero.m_presentBits[2] = 1UL << 17;
    g_effects.m_count = 1;
    CTCCreatureOpinionOfHero o; o.Calls = 0;
    o.InformOfDeed(77, true);                        // no hero yet
    if (o.Calls) bad |= 1;
    g_hero = &hero;
    o.InformOfDeed(77, true);
    if (o.Calls != 1 || o.LastType != 6 || o.LastHandle != 77 || o.LastEffects != &g_effects || !o.LastFlag
        || o.CountDuringCall != 2 || g_effects.m_count != 1 || g_effects.ForType != 6 || g_destroyed) bad |= 2;
    o.InformOfDeed(78, false);                       // unknown handle
    if (o.Calls != 1) bad |= 4;
    g_effects.m_count = 0;
    o.InformOfDeed(77, false);                       // the temporary holds the only reference
    if (o.Calls != 2 || o.LastFlag || g_destroyed != 1) bad |= 8;
    if (bad) { std::printf("00683590_TEST FAIL\n"); return 1; }
    std::printf("00683590_TEST PASS\n");
    return 0;
}
