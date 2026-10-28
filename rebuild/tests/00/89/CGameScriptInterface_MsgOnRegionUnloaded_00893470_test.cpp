#define ADDR "00893470"
#include <cstdio>
#include <cstring>
// Behaviour model of the Msg family: the message manager stub records the filter it was handed and returns a canned message.
class CCharString { public:
    const char* s;
    CCharString() : s("") { }
    CCharString(const char* t) : s(t) { }
    const CCharString& operator=(const CCharString& o) { s = o.s; return *this; }
    const CCharString& operator=(const char* t) { s = t; return *this; }
};
struct CMessageEvent_Retail { long m_extraData; };
class CMessageEventFilter_IsOfType { public: long m_type; CMessageEventFilter_IsOfType(long type) : m_type(type) { } };
class CMessageEventFilter_CanBeSeenOrHeard { public: long m_frame; long m_maxFrame; CMessageEventFilter_CanBeSeenOrHeard(long f, long m) : m_frame(f), m_maxFrame(m) { } };
class CMessageEventFilter_HasExtraData { public: long m_data; CMessageEventFilter_HasExtraData(long d) : m_data(d) { } };
class CMessageEventFilter_OccuredBetween { public: long m_frame; long m_maxFrame; CMessageEventFilter_OccuredBetween(long f, long m) : m_frame(f), m_maxFrame(m) { } };
template<class A, class B> class CFilter_Bind2 { public: const A* m_pFirst; const B* m_pSecond; CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { } };
template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst; const B* m_pSecond; const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_HasExtraData, CMessageEventFilter_OccuredBetween> CBoastFilter;
static long g_type = 0, g_frame = 0, g_max = 0, g_data = 0, g_calls = 0;
static CMessageEvent_Retail* g_result = 0;
struct CMessageEventManager_Retail {
    CMessageEvent_Retail* FindMostRecentMessage(const CSeenOrHeardFilter& f)
    { ++g_calls; g_type = f.m_pFirst->m_type; g_frame = f.m_pSecond->m_frame; g_max = f.m_pSecond->m_maxFrame; return g_result; }
    CMessageEvent_Retail* FindMostRecentMessage(const CBoastFilter& f)
    { ++g_calls; g_type = f.m_pFirst->m_type; g_data = f.m_pSecond->m_data; g_frame = f.m_pThird->m_frame; g_max = f.m_pThird->m_maxFrame; return g_result; }
};
struct CRegion_Retail { CCharString m_name; };
struct CWorldMap_Retail {
    CRegion_Retail m_regions[4];
    CRegion_Retail& GetRegion(long index) { return m_regions[index]; }
};
class CWorld { public:
    CWorldMap_Retail             m_map;
    CMessageEventManager_Retail  m_manager;
    CMessageEventManager_Retail* PMessageEventManager_Data;
    CWorldMap_Retail* PeekWorldMap() { return &m_map; }
};
struct CQuestManager_Retail {
    CCharString m_quests[3];
    const CCharString& GetRegisteredQuestFromIndex(long index) { return m_quests[index]; }
};
static CQuestManager_Retail g_questManager;
static CQuestManager_Retail* g_pQuestManager = &g_questManager;
struct CBoastRecord_Retail { long m_boast; CCharString m_name; };
struct CRewardRecord_Retail { CCharString m_name; };
struct CThing_Retail;
class CScriptThing { public:
    CThing_Retail* m_pThing; int m_assigned;
    CScriptThing() : m_pThing(0), m_assigned(0) { }
    CScriptThing& operator=(const CScriptThing& o) { m_pThing = o.m_pThing; ++m_assigned; return *this; }
};
struct CThing_Retail {
    int m_id;
    CScriptThing GetScriptThing() { CScriptThing t; t.m_pThing = this; return t; }
};
class CDefString { public:
    long m_index;
    CDefString(long i) : m_index(i) { }
    CDefString(const CDefString& o) : m_index(o.m_index) { }
};
struct CDefinitionManager_Retail {
    CDefString GetDefNameFromGlobalIndex(long index) { return CDefString(index * 10); }
};
struct CDefStringTable_Retail {
    CCharString GetString(long index) { return index == 30 ? CCharString("EXPRESSION_LAUGH") : CCharString("?"); }
};
static CDefStringTable_Retail g_defStringTable;
static bool g_tutorialsEnabled = true;
struct CUserProfileManager_Retail {
    bool m_hasActiveProfile; bool m_defaultTutorialsOn; bool m_profileTutorialsOn;
    static CUserProfileManager_Retail& Get();
    bool AreTutorialsOn() const { return m_hasActiveProfile ? m_profileTutorialsOn : m_defaultTutorialsOn; }
};
static CUserProfileManager_Retail g_profiles = { true, false, true };
CUserProfileManager_Retail& CUserProfileManager_Retail::Get() { return g_profiles; }
static long g_curFrame = 100, g_maxFrame = 200;
class CGameScriptInterface { public:
    CWorld*                    World;
    CDefinitionManager_Retail* DefinitionManager;
    long GetCurrentWorldFrameForMessages() const { return g_curFrame; }
    long GetMaxWorldFrameForMessages() const { return g_maxFrame; }
    bool MsgOnRegionUnloaded(CCharString& name) const;
};
static CWorld g_world;
static CDefinitionManager_Retail g_defs;
static CGameScriptInterface g_gsi;
static CMessageEvent_Retail g_message;
static void setup(bool found, long extra)
{
    g_world.PMessageEventManager_Data = &g_world.m_manager;
    g_gsi.World = &g_world; g_gsi.DefinitionManager = &g_defs;
    g_world.m_map.m_regions[2].m_name = "REGION_TWO";
    g_questManager.m_quests[1] = "QUEST_ONE";
    g_message.m_extraData = extra;
    g_result = found ? &g_message : 0;
    g_type = g_frame = g_max = g_data = g_calls = 0;
}
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", ADDR, __LINE__); return 1; } } while (0)

bool CGameScriptInterface::MsgOnRegionUnloaded(CCharString& name) const
{
    long frame = GetCurrentWorldFrameForMessages() - 1;
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager_Retail* messages = World->PMessageEventManager_Data;
    CMessageEvent_Retail* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x27) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
    if (message) {
        name = World->PeekWorldMap()->GetRegion(message->m_extraData).m_name;
        return true;
    }
    return false;
}

int main()
{
    CCharString name("unchanged");
    setup(false, 2); CHECK(!g_gsi.MsgOnRegionUnloaded(name)); CHECK(std::strcmp(name.s, "unchanged") == 0);
    CHECK(g_type == 0x27 && g_frame == 99 && g_max == 200);
    setup(true, 2); CHECK(g_gsi.MsgOnRegionUnloaded(name)); CHECK(std::strcmp(name.s, "REGION_TWO") == 0);
    std::printf("00893470_TEST PASS\n");
    return 0;
}