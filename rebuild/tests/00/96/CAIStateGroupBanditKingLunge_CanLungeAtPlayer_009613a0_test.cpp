// Behaviour test for 009613a0: body copied from src/009613a0.cpp (CAIStateGroup_BanditKingLunge::CanLungeAtPlayer), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
static CWorld* g_world = 0; static int g_getPWorld = 0;
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return false; }
    virtual void Update() {}
    virtual void v13() {}
    virtual long GetTimeToInitiate() { return 0; }
    virtual void v15() {} virtual void v16() {} virtual void v17() {}
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld() { ++g_getPWorld; return g_world; }
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
static void MakeThing(CThing& t, CInterfaceMap::SEntry* entries, long type, CTCBase* component)
{
    memset(&t, 0, sizeof(t));
    entries[0].type = type; entries[0].component = component; entries[1].type = 0x100;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 2;
    t.m_presentBits[type >> 5] |= 1UL << (type & 31);
}
struct CWorld { int dummy; };
template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end;
    unsigned long size() const { return last - first; }
    const T& operator[](unsigned long i) const { return first[i]; }
};
struct CTCBossDef {
    unsigned char _pad[0x40];
    CRetailVector<long> PhaseHits;
    unsigned char _pad4c[4];
    float LungeIntervalSeconds;
};
static long g_phase = 0, g_hitsThisPhase = 0;
struct CTCBoss {
    unsigned char _pad[0x0c];
    CTCBossDef* Def;
    long GetBossPhase() const { return g_phase; }
    long GetHitsThisPhase() const { return g_hitsThisPhase; }
};
namespace std {
template<class T> inline const T& min(const T& left, const T& right) { return right < left ? right : left; }
}
long g_framesPerSecond = 30;
static long g_worldFrame = 0;
struct CWorldFrame { static long GetWorldFrame() { return g_worldFrame; } };
struct CAIStateGroup_BanditKingLunge : CAIStateGroupBase {
    unsigned char Player[8];
    long TimeOfLastLunge;
    bool CanLungeAtPlayer(CThing* player);
};
bool CAIStateGroup_BanditKingLunge::CanLungeAtPlayer(CThing* player)
{
    CTCBoss* boss;
    if (player && !(player->m_flags91 & 1) && PBrain->PCreature->GetInterface(&boss, 0xad)) {
        long hits = boss->Def->PhaseHits[std::min(boss->GetBossPhase(), (long)boss->Def->PhaseHits.size() - 1)];
        if (hits == 0) {
            if (TimeOfLastLunge) {
                long interval = (long)(g_framesPerSecond * boss->Def->LungeIntervalSeconds);
                if (((CWorldFrame*)GetPWorld())->GetWorldFrame() - TimeOfLastLunge > interval)
                    return true;
            }
            else
                TimeOfLastLunge = ((CWorldFrame*)GetPWorld())->GetWorldFrame();
        }
        else
            return boss->GetHitsThisPhase() >= hits;
    }
    return false;
}
int main()
{
    static CThing creature, player; static CInterfaceMap::SEntry entries[2]; static CTCBoss boss; static CTCBossDef def; static CAIBrain brain; static CWorld world;
    static long phaseHits[3] = { 0, 5, 7 };
    MakeThing(creature, entries, 0xad, (CTCBase*)&boss); brain.PCreature = &creature; memset(&player, 0, sizeof(player)); g_world = &world;
    boss.Def = &def; def.PhaseHits.first = phaseHits; def.PhaseHits.last = phaseHits + 3; def.PhaseHits.end = phaseHits + 3; def.LungeIntervalSeconds = 2.0f;
    CAIStateGroup_BanditKingLunge sg; sg.PBrain = &brain; sg.TimeOfLastLunge = 0;
    // 1: phase 0 (no hits needed), first call stamps the lunge time
    g_phase = 0; g_worldFrame = 1000; g_getPWorld = 0;
    CHECK(!sg.CanLungeAtPlayer(&player)); CHECK(sg.TimeOfLastLunge == 1000); CHECK(g_getPWorld == 1);
    // 2: not enough frames since the last lunge (interval 60)
    g_worldFrame = 1060; CHECK(!sg.CanLungeAtPlayer(&player)); CHECK(sg.TimeOfLastLunge == 1000);
    g_worldFrame = 1061; CHECK(sg.CanLungeAtPlayer(&player));
    // 3: phase 1 needs 5 hits
    g_phase = 1; g_hitsThisPhase = 4; CHECK(!sg.CanLungeAtPlayer(&player)); g_hitsThisPhase = 5; CHECK(sg.CanLungeAtPlayer(&player));
    // 4: phase beyond the table clamps to the last entry (7)
    g_phase = 9; g_hitsThisPhase = 6; CHECK(!sg.CanLungeAtPlayer(&player)); g_hitsThisPhase = 7; CHECK(sg.CanLungeAtPlayer(&player));
    // 5: dead or missing player, missing boss component
    player.m_flags91 = 1; CHECK(!sg.CanLungeAtPlayer(&player)); player.m_flags91 = 0; CHECK(!sg.CanLungeAtPlayer(0));
    creature.m_presentBits[0xad >> 5] = 0; CHECK(!sg.CanLungeAtPlayer(&player));
    if (fails == 0) printf("009613a0_TEST PASS\n"); else printf("009613a0_TEST FAIL\n");
    return fails;
}