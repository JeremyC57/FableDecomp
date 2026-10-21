// Behaviour test for 00f058d0: src/00f058d0.cpp copied verbatim; per region-loaded message, the spies' allegiances are
// refreshed for Gibbet Woods, the Deserted Farm and Bowerstone Jail only; a frame passes between polls.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NScript::CQ_AmbushTradersScript::WatchForRegionChange() -- script thread: wait for each region load; on Gibbet Woods, the Deserted Farm or Bowerstone Jail refresh the spies' allegiances
// Leaf: ?WatchForRegionChange@CQ_AmbushTradersScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F058D0, 308 bytes. Identity: the catalog label CV_ChapelOfEvilScript::WatchForLeaving
//   is wrong -- the thread registration at 0x00F02CEE stores this address under the name "WatchForRegionChange" (0x012F7C48)
//   and the body calls ?RefreshSpyAllegiences@CQ_AmbushTradersScript@NScript@@ (0x00F05A10).
// Callees: CCharString() 0x0099E4B0, ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, Compare
//   0x00411570 (same-TU stand-in body), RefreshSpyAllegiences 0x00F05A10, game script interface slots 7 / 22.
// Layout: local models (no engine/ header for the quest script classes); Game +0x40.
// Shape: the natural `region == "GibbetWoods" || ... ` with the inline CCharString ==: its null-data arm is
//   `if (strcmp(text, "") == 0) return true; return false;` (VC7.1 branches off the repe cmpsb flags, and the non-null
//   arms' block compares are homed in a frame byte as retail).
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)       // 0x00411570 (the landed body, stand-in)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare(PData, text) == 0; }   // 0x004115A0 (inlined here)
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CGameScriptInterface {                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) CGameScriptInterface::NewScriptFrame @ 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual bool MsgOnRegionLoaded(CCharString& region) const;   // slot 22 (+0x58) CGameScriptInterface::MsgOnRegionLoaded @ 0x008933F0
};
struct CScriptBase {                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct CQ_AmbushTradersScript : CQuestScriptBase {
    void RefreshSpyAllegiences();                           // 0x00F05A10
    void WatchForRegionChange();
};
void CQ_AmbushTradersScript::WatchForRegionChange()
{
    CCharString region;
    while (!IsActiveThreadTerminating()) {
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (region == "GibbetWoods" || region == "DesertedFarm" || region == "BowerstoneJail") {
            if (IsActiveThreadTerminating())
                return;
            RefreshSpyAllegiences();
        }
        Game->NewScriptFrame();
    }
}
// ---- end source ----
void CGameScriptInterface::v000() {} void CGameScriptInterface::v001() {} void CGameScriptInterface::v002() {} void CGameScriptInterface::v003() {} void CGameScriptInterface::v004() {} void CGameScriptInterface::v005() {} void CGameScriptInterface::v006() {} void CGameScriptInterface::v008() {} void CGameScriptInterface::v009() {} void CGameScriptInterface::v010() {} void CGameScriptInterface::v011() {} void CGameScriptInterface::v012() {} void CGameScriptInterface::v013() {} void CGameScriptInterface::v014() {} void CGameScriptInterface::v015() {} void CGameScriptInterface::v016() {} void CGameScriptInterface::v017() {} void CGameScriptInterface::v018() {} void CGameScriptInterface::v019() {} void CGameScriptInterface::v020() {} void CGameScriptInterface::v021() {}
static int g_checks, g_limit, g_frames, g_msgs, g_refresh, g_ctors, g_dtors;
static CCharStringData* g_regions[8]; static bool g_loaded[8]; static int g_count;
CCharString::CCharString() : PStringData(0) { ++g_ctors; }
CCharString::~CCharString() { ++g_dtors; }
bool CScriptBase::IsActiveThreadTerminating() { return ++g_checks > g_limit; }
void CQ_AmbushTradersScript::RefreshSpyAllegiences() { ++g_refresh; }
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; }
bool CGameScriptInterface::MsgOnRegionLoaded(CCharString& region) const
{
    int i = g_msgs++;
    if (i >= g_count || !g_loaded[i]) return false;
    region.PStringData = g_regions[i];
    return true;
}
static CCharStringData MakeData(char* text) { CCharStringData d; memset(&d, 0, sizeof(d)); d.Data.PData = text; d.Data.StringLength = (unsigned long)strlen(text); return d; }
static void Run(CQ_AmbushTradersScript& s, int limit) { g_checks = 0; g_limit = limit; g_frames = 0; g_msgs = 0; g_refresh = 0; s.WatchForRegionChange(); }
int main()
{
    static char t1[] = "GibbetWoods", t2[] = "DesertedFarm", t3[] = "BowerstoneJail", t4[] = "Oakvale", t5[] = "GibbetWood";
    CCharStringData d1 = MakeData(t1), d2 = MakeData(t2), d3 = MakeData(t3), d4 = MakeData(t4), d5 = MakeData(t5);
    CGameScriptInterface game;
    static long raw[0x48 / 4]; memset(raw, 0, sizeof(raw));
    CQ_AmbushTradersScript& s = *(CQ_AmbushTradersScript*)raw; s.Game = &game;
    // nothing loaded: inner wait loop frames until termination
    g_count = 0; Run(s, 3);
    CHECK(g_refresh == 0 && g_frames == 3 && g_msgs == 3 && g_ctors == 1 && g_dtors == 1);
    // each spy region refreshes; other names (and a null block) do not
    CCharStringData* list[7] = { &d1, &d4, &d2, 0, &d3, &d5, &d1 };
    for (int i = 0; i < 7; ++i) { g_regions[i] = list[i]; g_loaded[i] = true; }
    g_count = 7; Run(s, 1000);
    CHECK(g_refresh == 4);
    CHECK(g_msgs > 7 && g_frames == g_msgs);   // one frame per poll: 7 after the regions, then the empty waits
    // a wait frame before the region arrives
    g_regions[0] = &d2; g_loaded[0] = false; g_regions[1] = &d2; g_loaded[1] = true; g_count = 2; Run(s, 1000);
    CHECK(g_refresh == 1 && g_msgs > 2);
    // termination right after the region message: no refresh
    g_loaded[0] = true; g_regions[0] = &d1; g_count = 1; Run(s, 1);
    CHECK(g_refresh == 0 && g_checks == 2);
    // termination after the spy test, before the refresh
    Run(s, 2);
    CHECK(g_refresh == 0 && g_checks == 3);
    if (fails == 0) printf("00f058d0_TEST PASS\n"); else printf("00f058d0_TEST FAIL\n");
    return fails;
}