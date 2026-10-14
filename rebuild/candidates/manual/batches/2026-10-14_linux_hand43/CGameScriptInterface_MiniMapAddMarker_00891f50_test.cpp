#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[160] = ""; static long g_arg[12]; static float g_farg = 0; static bool g_valid = true;
static void rec(const char* s) { std::strcat(g_seq, s); }
static bool seqIs(const char* a, const char* b) { return !std::strcmp(g_seq, a) || !std::strcmp(g_seq, b); }   // the order of an object expression and its arguments is unspecified
struct CTCBase_Retail { int x; };
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { rec("L"); SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CPhysicsBody_Retail { unsigned char _pad[0x3c]; unsigned long m_collisionFlags; };
static long g_pos = 0;
struct CThingPhysics_Retail {
    virtual void SetCollisionFlags(unsigned long f) { rec("C"); g_arg[0] = (long)f; }
    virtual void* GetPosition() { rec("P"); return &g_pos; }
    unsigned char m_transform[0x2c]; CPhysicsBody_Retail* m_pBody; CPhysicsBody_Retail m_body;
    CThingPhysics_Retail() { std::memset(m_transform, 0, sizeof(m_transform)); m_pBody = &m_body; m_body.m_collisionFlags = 0; }
};
struct CScriptThing; struct CDefPointer_Retail; struct CThing_Retail;
struct CDefVirtual_Retail { virtual void v00() { } virtual void Destroy() { rec("X"); } };
struct CDefBase_Retail : CDefVirtual_Retail { long m_count; CDefBase_Retail() : m_count(1) { } };
struct CDefPointer_Retail {
    CDefBase_Retail* m_p;
    CDefPointer_Retail() : m_p(0) { }
    CDefPointer_Retail(const CDefPointer_Retail& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer_Retail() { if (m_p) { if (--m_p->m_count == 0) m_p->Destroy(); } }
};
static CThing_Retail* g_pKilled = 0; static CThing_Retail* g_pHome = 0; static CDefBase_Retail* g_pBrain = 0; static long g_brainCountAtCall = 0;
struct CThing_Retail {
    virtual CThing_Retail* GetStandingOnThing() { rec("S"); return m_pStandingOn; }
    unsigned char m_type; unsigned long m_presentBits[9]; CInterfaceMap_Retail m_interfaces; CThingPhysics_Retail* m_pPhysics; unsigned char m_flags;
    unsigned char m_dead : 1; unsigned char m_91_1 : 7; unsigned char m_92_0 : 8; unsigned char m_93_0 : 1; unsigned char m_killedByScript : 1; unsigned char m_93_2 : 6;
    CThing_Retail* m_pStandingOn; CInterfaceMap_Retail::SEntry m_entries[4]; long m_entryCount;
    CThingPhysics_Retail* GetPhysics() { return m_pPhysics; }
    void Kill(bool flag) { rec("K"); g_pKilled = this; g_arg[0] = flag; }
    inline CScriptThing GetScriptThing();
    void AssignHomeBuilding(CThing_Retail* b) { rec("A"); g_pHome = b; }
    void SetNewBrain(CDefPointer_Retail brain) { rec("B"); g_pBrain = brain.m_p; g_brainCountAtCall = brain.m_p ? brain.m_p->m_count : -1; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    CThing_Retail() { Reset(); }
    void Reset() { m_type = 0; std::memset(m_presentBits, 0, sizeof(m_presentBits)); m_pPhysics = 0; m_flags = 0; m_dead = 0; m_91_1 = 0; m_92_0 = 0; m_93_0 = 0; m_killedByScript = 0; m_93_2 = 0; m_pStandingOn = 0; m_entryCount = 0; m_interfaces.first = m_entries; m_interfaces.last = m_entries; m_entries[0].type = 0x7fffffff; m_entries[0].component = 0; }
    void AddComponent(long type, void* comp)   // keeps the entries sorted; the end entry carries a null component
    {
        long i = m_entryCount; while (i > 0 && m_entries[i - 1].type > type) { m_entries[i] = m_entries[i - 1]; --i; }
        m_entries[i].type = type; m_entries[i].component = (CTCBase_Retail*)comp; ++m_entryCount;
        m_interfaces.last = m_entries + m_entryCount; m_entries[m_entryCount].type = 0x7fffffff; m_entries[m_entryCount].component = 0;
        m_presentBits[type >> 5] |= (1UL << (type & 31));
    }
};
struct CScriptThing {
    virtual CThing_Retail* GetThing() const { rec("g"); return PImp_Data; }
    virtual bool IsValid() const { rec("v"); return g_valid; }
    CThing_Retail* PImp_Data; void* PImp_Info;
    CScriptThing() : PImp_Data(0), PImp_Info(0) { rec("0"); }
    CScriptThing(CThing_Retail* t) : PImp_Data(t), PImp_Info(0) { }
};
inline CScriptThing CThing_Retail::GetScriptThing() { rec("G"); return CScriptThing(this); }
static CThing_Retail g_thing, g_other; static CScriptThing g_script, g_script2; static CThingPhysics_Retail g_physics;
static void reset(unsigned char type, unsigned char flags) { g_thing.Reset(); g_other.Reset(); g_thing.m_type = type; g_thing.m_flags = flags; g_thing.m_pPhysics = &g_physics; g_script.PImp_Data = &g_thing; g_script2.PImp_Data = &g_other; g_seq[0] = 0; g_valid = true; std::memset(g_arg, 0, sizeof(g_arg)); }
// ---- strings
struct CCharStringData_Retail { const char* m_pData; unsigned long m_length; };
static CCharStringData_Retail g_blocks[8]; static int g_blockCount = 0; static int g_strDtors = 0; static int g_strCtors = 0; static int g_wideDtors = 0;
struct CWideString {
    const char* PStringData;
    CWideString() : PStringData("") { }
    CWideString(const CWideString& o) : PStringData(o.PStringData) { }
    ~CWideString() { ++g_wideDtors; }
    bool operator!=(const wchar_t*) const { rec("!"); return PStringData[0] != 0; }
};
struct CCharString {
    CCharStringData_Retail* m_pData;
    CCharString() : m_pData(0) { }
    CCharString(const char* text, long length = -1) { ++g_strCtors; m_pData = &g_blocks[g_blockCount++ & 7]; m_pData->m_pData = text; m_pData->m_length = length < 0 ? (unsigned long)std::strlen(text) : (unsigned long)length; }
    ~CCharString() { ++g_strDtors; }
    CCharString& operator=(const CCharString& o) { rec("="); m_pData = o.m_pData; return *this; }
    CWideString ToWideString() const { rec("w"); CWideString w; w.PStringData = m_pData ? m_pData->m_pData : ""; return w; }
    const char* GetString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
};
struct CWideStringVector_Retail { int x; };
static const char* g_bankText = "hello"; static long g_lookup = 1; static unsigned long g_hash = 0;
struct CDataBank_Retail {
    CWideString GetTextBySymbol(const CCharString& s) { rec("T"); g_arg[0] = s.GetString()[0]; CWideString w; w.PStringData = g_bankText; return w; }
    void* LookupText(unsigned long hash) { rec("l"); g_hash = hash; return g_lookup ? (void*)this : 0; }
};
struct CEngineGlobals_Retail { unsigned char _pad[0x14]; CDataBank_Retail* m_pTextBank; };
static CDataBank_Retail g_bank; static CEngineGlobals_Retail g_engine; static CEngineGlobals_Retail* g_pEngineGlobals_013b86a0 = &g_engine;
static bool ConstructString(CWideString* text, CWideStringVector_Retail* args) { rec("X"); text->PStringData = "formatted"; g_arg[1] = args->x; return true; }
static unsigned long Calc_004014a0(unsigned long seed, const void* data, unsigned long length) { rec("H"); unsigned long h = seed; for (unsigned long i = 0; i < length; ++i) h = h * 31 + ((const unsigned char*)data)[i]; return h; }
// ---- the world and its managers
struct CThingManager_Retail { bool m_allEntitiesPaused; void SetNonScriptedEntitesPaused(bool p) { rec("N"); g_arg[0] = p; } };
struct CBulletTimeManager_Retail {
    void SetBulletTimeSlowdownMultiplier(long priority, unsigned long key, float m) { rec("S"); g_arg[1] = priority; g_arg[2] = (long)key; g_farg = m; }
    void RemoveBulletTimeSlowdown(unsigned long key) { rec("R"); g_arg[2] = (long)key; }
};
struct CMessageEvent_Retail { int x; };
struct CMessageEventFilter_IsOfType { long m_type; CMessageEventFilter_IsOfType(long t) : m_type(t) { } };
struct CMessageEventFilter_CanBeSeenOrHeard { long m_frame; long m_maxFrame; CMessageEventFilter_CanBeSeenOrHeard(long f, long m) : m_frame(f), m_maxFrame(m) { } };
template<class A, class B> struct CFilter_Bind2 { const A* m_pFirst; const B* m_pSecond; CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { } };
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
static CMessageEvent_Retail g_msg; static bool g_haveMsg = true; static const CMessageEvent_Retail* g_pRemoved = 0;
struct CMessageEventManager_Retail {
    CMessageEvent_Retail* FindMostRecentMessage(const CSeenOrHeardFilter& f) { rec("F"); g_arg[0] = f.m_pFirst->m_type; g_arg[1] = f.m_pSecond->m_frame; g_arg[2] = f.m_pSecond->m_maxFrame; return g_haveMsg ? &g_msg : 0; }
    void RemoveMessage(const CMessageEvent_Retail* m) { rec("M"); g_pRemoved = m; }
};
struct CBaseIntelligentPointer_Retail { virtual void v00() { } void* m_pItem; void* GetPItem() { rec("p"); return m_pItem; } };
struct CTCOpinionSource_Retail { int x; };
static CCharString g_attitude; static float g_opinion = 2.5f; static CThing_Retail* g_pOpinionThing = 0; static const char* g_opinionName = 0; static long g_opinionId = 0; static void* g_pOut = (void*)1;
struct COpinionReactionManager_Retail {
    CBaseIntelligentPointer_Retail m_exclusiveSource;
    const CCharString& GetThingAttitudeToPlayerAsString(CThing_Retail* t) { rec("a"); g_pOpinionThing = t; return g_attitude; }
    float GetOpinionValue(CThing_Retail* t, long opinion) { rec("o"); g_pOpinionThing = t; g_arg[0] = opinion; return g_opinion; }
    bool AttachOpinionSource(CThing_Retail* t, const CCharString& name, CTCOpinionSource_Retail** pp) { rec("n"); g_pOpinionThing = t; g_opinionName = name.GetString(); g_pOut = pp; return true; }
    bool AttachOpinionSource(CThing_Retail* t, long id, CTCOpinionSource_Retail** pp) { rec("i"); g_pOpinionThing = t; g_opinionId = id; g_pOut = pp; return true; }
    void ClearOpinionSource(CThing_Retail* t, bool flag) { rec("c"); g_pOpinionThing = t; g_arg[0] = flag; }
    void SetExclusiveOpinionSource(CThing_Retail* t) { rec("x"); g_pOpinionThing = t; }
    void* GetExclusiveOpinionSource() { return m_exclusiveSource.GetPItem(); }
};
static CThingManager_Retail g_tm; static CBulletTimeManager_Retail g_bullet; static CMessageEventManager_Retail g_messages; static COpinionReactionManager_Retail g_opinions;
struct CWorld {
    virtual CThingManager_Retail* GetThingManager() { rec("t"); return &g_tm; }
    CMessageEventManager_Retail* PMessageEventManager_Data; CBulletTimeManager_Retail* PBulletTimeManager_ptr; COpinionReactionManager_Retail* POpinionReactionManager_Data; bool CreatureGenerationEnabled;
    CWorld() : PMessageEventManager_Data(&g_messages), PBulletTimeManager_ptr(&g_bullet), POpinionReactionManager_Data(&g_opinions), CreatureGenerationEnabled(true) { }
    COpinionReactionManager_Retail* GetOpinionReactionManager() { return POpinionReactionManager_Data; }
};
static CWorld g_world;
// ---- the game UI singleton, the main component, the player
struct CFontBank_Retail { int x; }; struct CRGBColour { int r; };
static CFontBank_Retail g_font; static const CFontBank_Retail* g_pFontUsed = 0; static const char* g_text[4]; static long g_colour = -1; static float g_time = -1; static bool g_b1, g_b2;
struct CSubtitleRenderer_Retail {
    void SetText(CFontBank_Retail* f, const CCharString& t, unsigned long colour, float time, bool a, bool b) { rec("1"); g_pFontUsed = f; g_text[0] = t.GetString(); g_colour = (long)colour; g_time = time; g_b1 = a; g_b2 = b; }
    void SetText(CFontBank_Retail* f, const CCharString& t, const CCharString& n, const CCharString& a, const CCharString& b, unsigned long colour, float time, bool c, bool d) { rec("2"); g_pFontUsed = f; g_text[0] = t.GetString(); g_text[1] = n.GetString(); g_text[2] = a.GetString(); g_text[3] = b.GetString(); g_colour = (long)colour; g_time = time; g_b1 = c; g_b2 = d; }
};
static CThing_Retail* g_pMarked = 0; static long g_markerId = 0;
struct CMiniMapDisplay_Retail {
    void AddMarker(CThing_Retail* t, long id) { rec("+"); g_pMarked = t; g_markerId = id; }
    void SetMarkerGraphic(CThing_Retail* t, long id) { rec("#"); g_pMarked = t; g_markerId = id; }
    void RemoveMarker(CThing_Retail* t) { rec("-"); g_pMarked = t; }
};
static CThing_Retail* g_pBarThing = 0; static const CRGBColour* g_pBarColour = 0;
struct CDrawQuestInfo_Retail { long AddBarHealth(CThing_Retail* t, const CRGBColour& c, const CCharString& n, float v) { rec("Q"); g_pBarThing = t; g_pBarColour = &c; g_text[0] = n.GetString(); g_farg = v; return 77; } };
static CMiniMapDisplay_Retail g_minimap; static CDrawQuestInfo_Retail g_quest;
struct CGameUI_Retail {
    CSubtitleRenderer_Retail m_subtitles; CDrawQuestInfo_Retail* m_pQuestInfo;
    CGameUI_Retail() : m_pQuestInfo(&g_quest) { }
    CDrawQuestInfo_Retail* GetQuestInfo() { return m_pQuestInfo; }
    CMiniMapDisplay_Retail* GetMiniMapDisplay() { rec("m"); return &g_minimap; }
    void ShowGuildMasterMessage(const CCharString& t, long id, bool flag) { rec("G"); g_text[0] = t.GetString(); g_arg[0] = id; g_arg[1] = flag; }
    void ShowOnScreenMessage(const CWideString& w, long a, long b, long c) { rec("O"); g_text[0] = w.PStringData; g_arg[0] = a; g_arg[1] = b; g_arg[2] = c; }
};
static CGameUI_Retail g_ui; static CGameUI_Retail* g_pGameUI_013b8790 = &g_ui;
struct CMainGameComponent_Retail { CFontBank_Retail* m_pFontBank; CMainGameComponent_Retail() : m_pFontBank(&g_font) { } };
static CMainGameComponent_Retail g_component;
static CThing_Retail* g_pHero = &g_thing;
struct CPlayer_Retail { CThing_Retail* GetHeroThing() { rec("h"); return g_pHero; } };
struct CPlayerManager_Retail { CPlayer_Retail* GetPlayer() { rec("y"); static CPlayer_Retail p; return &p; } };
static CPlayerManager_Retail g_pm;
struct CUserProfileManager_Retail { bool m_profileLoaded; bool m_defaultSetting; bool m_profileSetting; static CUserProfileManager_Retail& Get() { rec("U"); static CUserProfileManager_Retail m; return m; } };
static CDefBase_Retail g_def; static bool g_defFound = true;
struct CDefinitionManager_Retail { bool GetDef(const CCharString& n, CDefPointer_Retail& out) { rec("D"); g_text[0] = n.GetString(); if (g_defFound) { out.m_p = &g_def; ++g_def.m_count; } return g_defFound; } };
static CDefinitionManager_Retail g_defs;
struct COpenDoor_Retail { bool m_open; bool m_force; COpenDoor_Retail(bool o, bool f) : m_open(o), m_force(f) { } void operator()(CThing_Retail* t) { rec("d"); g_arg[0] = m_open; g_arg[1] = m_force; g_pKilled = t; } };
struct CTCTrap_Retail { bool m_active; void Activate() { rec("+"); m_active = true; } void Deactivate() { rec("-"); m_active = false; } };
struct CTCFishingSpot_Retail { bool m_enabled; };
struct CTCFish_Retail { long m_weight; };
static long g_lastFishWeight_013bae80 = 0;
struct CTCHeroFishing_Retail { long m_fishingLevel; };
struct CTCOracleMinigame { bool Success; };
struct CTCFireheartMinigame_Retail { bool m_forceQuit; void PlayGame() { rec("f"); } void Quit() { rec("q"); } };
struct CTCDoor { long Jammed; unsigned char TriggerType[4]; };
struct CTCBuyableHouse { bool IsScripted; };
static const float g_fNoOpinion_0122dedc = 0.0f;
struct CDisplayEngine_Retail;
struct CGameScriptInterface {
    virtual void v000() { }
    virtual void CameraUsePhysicsPoint(long a, const unsigned char* tr, void* pos, long c, long d, long e) const { rec("V"); g_arg[0] = a; g_arg[1] = (long)(tr == g_physics.m_transform); g_arg[2] = (long)(pos == &g_pos); g_arg[3] = c; g_arg[4] = d; g_arg[5] = e; }
    CWorld* World; CMainGameComponent_Retail* Component; CDisplayEngine_Retail* DisplayEngine; CDefinitionManager_Retail* DefinitionManager; CPlayerManager_Retail* PlayerManager;
    long GetTextIDFromSymbol(const CCharString& s) const { rec("I"); return (long)s.GetString()[0] - '0'; }
    long GetCurrentWorldFrameForMessages() const { rec("c"); return 100; }
    long GetMaxWorldFrameForMessages() const { rec("x"); return 200; }
    CGameScriptInterface() : World(&g_world), Component(&g_component), DisplayEngine(0), DefinitionManager(&g_defs), PlayerManager(&g_pm) { }
    void MiniMapAddMarker(const CScriptThing& thing, const CCharString& symbol) const { if (thing.IsValid()) { long id = GetTextIDFromSymbol(symbol); if (id > 0) { CThing_Retail* t = thing.GetThing(); g_pGameUI_013b8790->GetMiniMapDisplay()->AddMarker(t, id); } } }
};

int main()
{
    CGameScriptInterface i; reset(2, 0);
    CCharString s5("5"), s0("0"); i.MiniMapAddMarker(g_script, s5); if (!seqIs("vIgm+", "vImg+") || g_pMarked != &g_thing || g_markerId != 5) { std::printf("00891f50_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; i.MiniMapAddMarker(g_script, s0); if (std::strcmp(g_seq, "vI")) { std::printf("00891f50_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_valid = false; i.MiniMapAddMarker(g_script, s5); if (std::strcmp(g_seq, "v")) { std::printf("00891f50_TEST FAIL\n"); return 1; }
    std::printf("00891f50_TEST PASS\n");
    return 0;
}
