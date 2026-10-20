// behaviour test for 0065a686 CTCVillage::PlayerCanApologiseForCurrentCrimes: false within 450 frames of the last apology;
// otherwise false when any recorded crime is non-apologisable or older than the given seconds, else true (also with no hero)
#include <cstdio>
struct CTCBase;
static long g_frame = 1000; static int g_destroyed = 0;
long g_framesPerSecond = 30;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    bool          hasLog;
    CInterfaceMap m_interfaces;
    bool HasTC(long type) const { return type == 0x51 && hasLog; }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
static CThing* g_hero = 0;
struct CPlayer { CThing* GetHeroThing() { return g_hero; } };
struct CWorld {
    CPlayer player;
    CPlayer* GetCurrentPlayer() const { return (CPlayer*)&player; }
    static long GetFrame() { return g_frame; }
};
static CWorld g_world;
struct CMainGameComponent { CWorld* World; };
static CMainGameComponent g_main; CMainGameComponent* g_pMainGameComponent = &g_main;
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
struct COpinionDeedEffectsDef : CDefVirtual, CRefCounted {
    long CrimeSeverity;
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_count; }   // test-side: counts stay
                                                                                                       // exact with or without NRVO
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
struct COpinionDeed { long DeedType; long DeedHandle; float Position[3]; long FrameHappened; };
static COpinionDeedEffectsDef g_effects[2];
static COpinionDeed g_deeds[4];
struct CTCHeroOpinionDeedLog {
    COpinionDeed* GetDeedFromHandle(long handle) { return handle >= 0 && handle < 4 ? &g_deeds[handle] : 0; }
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType)
    { CDefPointer<COpinionDeedEffectsDef> p; p.Object = &g_effects[deedType]; ++p.Object->m_count; return p; }
};
struct SNode { long _M_color; SNode* _M_parent; SNode* _M_left; SNode* _M_right; long _M_value_field; };
static SNode* g_nodes[8]; static int g_nodeCount = 0;
SNode* Successor(SNode* node) { for (int i = 0; i < g_nodeCount - 1; ++i) if (g_nodes[i] == node) return g_nodes[i + 1]; return 0; }
struct CTCVillage {
    virtual void v00() {}
    SNode*        PlayersCrimes;
    long          LastFrameApologised;
    CWorld* GetWorld() { return &g_world; }
    bool PlayerCanApologiseForCurrentCrimes(long seconds);
};
bool CTCVillage::PlayerCanApologiseForCurrentCrimes(long seconds)
{
    if (LastFrameApologised != 0 && GetWorld()->GetFrame() - LastFrameApologised < 450)
        return false;
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetHeroThing();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        if (hero->HasTC(0x51))
            log = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
        long now = CWorld::GetFrame();
        SNode* end = PlayersCrimes;
        for (SNode* it = end->_M_left; it != end; it = Successor(it)) {
            COpinionDeed* deed = log->GetDeedFromHandle(it->_M_value_field);
            if (deed) {
                bool refused = !(log->GetDeedEffectsDef(deed->DeedType).Object->CrimeSeverity & 8)
                               || now - deed->FrameHappened > g_framesPerSecond * seconds;
                if (refused)
                    return false;
            }
        }
    }
    return true;
}
int main()
{
    g_main.World = &g_world;
    g_effects[0].CrimeSeverity = 8; g_effects[1].CrimeSeverity = 4;
    g_effects[0].m_count = 1; g_effects[1].m_count = 1;          // held elsewhere: never destroyed here
    g_deeds[0].DeedType = 0; g_deeds[0].FrameHappened = 900;
    g_deeds[1].DeedType = 0; g_deeds[1].FrameHappened = 950;
    g_deeds[2].DeedType = 1; g_deeds[2].FrameHappened = 990;
    CTCHeroOpinionDeedLog log;
    CInterfaceMap::SEntry entries[1] = { { 0x51, (CTCBase*)&log } };
    CThing hero; hero.hasLog = true; hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1; g_hero = &hero;
    SNode header, a, b, c; a._M_value_field = 0; b._M_value_field = 1; c._M_value_field = 9;   // c: no deed
    g_nodes[0] = &a; g_nodes[1] = &b; g_nodes[2] = &c; g_nodes[3] = &header; g_nodeCount = 4; header._M_left = &a;
    CTCVillage village; village.PlayersCrimes = &header; village.LastFrameApologised = 0;
    bool ok = village.PlayerCanApologiseForCurrentCrimes(10);                // ages 100, 50 <= 300
    ok = ok && !village.PlayerCanApologiseForCurrentCrimes(3);               // 100 > 90
    village.LastFrameApologised = 700; ok = ok && !village.PlayerCanApologiseForCurrentCrimes(10);   // 300 < 450
    village.LastFrameApologised = 0; b._M_value_field = 2;                   // severity 4: not apologisable
    ok = ok && !village.PlayerCanApologiseForCurrentCrimes(10);
    g_hero = 0; ok = ok && village.PlayerCanApologiseForCurrentCrimes(10);
    ok = ok && g_effects[0].m_count == 1 && g_effects[1].m_count == 1 && g_destroyed == 0;
    if (!ok) { std::printf("0065a686_TEST FAIL\n"); return 1; }
    std::printf("0065a686_TEST PASS\n");
    return 0;
}