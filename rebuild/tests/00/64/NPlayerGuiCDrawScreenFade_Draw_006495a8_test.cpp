// Behaviour test for 0x006495A8 NPlayerGui::CDrawScreenFade::Draw (links the REAL row object: the row body plus the TU's COMDATs it
// carries -- IS_THING_ALIVE<const CThingPlayerCreature*>, std::bitset<274>::test, the const CVectorMap::LowerBound, PeekTC, the inline
// accessors). Fixtures (native layout): the drawer (vptr, PlayerGui +4, PlayerManager +8, World +0xC, Component +0x10, Drawing +0x18,
// PrimitiveHandles' begin +0x1C, DrawnFullFadeOut +0x28); the player GUI (its SubtitleRenderer +0x18: DisplayFlags +0x94, InCutscene
// +0xF6, CutsceneSkippableWhilePaused +0xF9, DisplayingText +0xFD); the world (Paused +0xA4); the component (display engine word +0x28);
// the display engine (ScreenFadeOutInfo.Active +0xBC, ToColour B,G,R,A +0xD4); the hero (UsingComponents bitset +0x20, Components
// vector of (type, component) pairs +0x44/+0x48, Dead bit 0 of +0x91); the receive-items component (DisplayingItems +0x26); the prepare
// info (Engine +0x34, vtable slot 23 = SetPersistentPrimitive). Every out-of-line callee is a scripted stub that counts its calls; the
// sprite constructor records its arguments, SetPersistentPrimitive the handle, the sprite and the flags.
// Reference model: Model() below, written from the row's statements; each run compares the drawn sprite (or none), Drawing,
// DrawnFullFadeOut and the call counts of the short-circuited predicates.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }

#pragma comment(linker, "/alternatename:@CallRow@12=?Draw@CDrawScreenFade@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z")
#pragma comment(linker, "/alternatename:?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ=@StubGetMainPlayer@8")
#pragma comment(linker, "/alternatename:?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ=@StubPeekHero@8")
#pragma comment(linker, "/alternatename:?IsOverridingScreenFade@CTCQuestCompletionUI@@QBE_NXZ=@StubOverriding@8")
#pragma comment(linker, "/alternatename:?GetScreenFadeOutAlpha@CDisplayEngine@@QBEEXZ=@StubAlpha@8")
#pragma comment(linker, "/alternatename:?IsScreenFullyFadedOut@CDisplayEngine@@QBE_NXZ=@StubFullyFaded@8")
#pragma comment(linker, "/alternatename:?IsDisplayingMiniMapOnRegionLoad@CWorld@@QBE_NXZ=@StubMiniMap@8")
#pragma comment(linker, "/alternatename:?IsPerformingRegionLoad@CWorld@@QBE_NXZ=@StubRegionLoad@8")
#pragma comment(linker, "/alternatename:?IsCutsceneSkippable@CSubtitleRenderer@@QBE_NXZ=@StubSkippable@8")
#pragma comment(linker, "/alternatename:?GFAXToVX@@YIMM@Z=@StubAXToVX@4")
#pragma comment(linker, "/alternatename:??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEKABVC2DVector@@5@Z=@StubSprite@76")

bool g_bSaveLoadFailed;                         // ?g_bSaveLoadFailed@@3_NA
double VIRTUAL_SCREEN_WIDTH = 640.0;            // ?VIRTUAL_SCREEN_WIDTH@@3NA
double VIRTUAL_SCREEN_HEIGHT = 480.0;           // ?VIRTUAL_SCREEN_HEIGHT@@3NA

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

// --- native-layout fixtures ------------------------------------------------------------------------------------------------
static unsigned char g_drawer[0x2c], g_gui[0x200], g_world[0x200], g_component[0x40], g_engine[0x100], g_hero[0x100];
static unsigned char g_questUI[0x40], g_receive[0x40], g_playerManager[0x10], g_player[0x40], g_info[0x40], g_engine3d[0x10];
static unsigned char g_handles[16];
struct Pair { long type; void* component; };
static Pair g_components[4];
static void* g_vtbl3d[32];
template <class T> static void Put(unsigned char* base, int off, T v) { memcpy(base + off, &v, sizeof(T)); }
template <class T> static T Get(const unsigned char* base, int off) { T v; memcpy(&v, base + off, sizeof(T)); return v; }

// --- scripted state -------------------------------------------------------------------------------------------------------
static bool s_heroPresent, s_heroDead, s_hasQuestUI, s_overriding, s_hasReceive, s_receiveDisplaying;
static bool s_fadeActive, s_fullyFaded, s_miniMap, s_regionLoad, s_paused, s_inCutscene, s_skippable, s_whilePaused, s_displayingText;
static unsigned long s_displayFlags; static unsigned char s_alpha, s_R, s_G, s_B;
static int c_mainPlayer, c_peekHero, c_overriding, c_alpha, c_fullyFaded, c_miniMap, c_regionLoad, c_skippable, c_axtovx, c_sprite, c_set;
struct SpriteArgs { void* self; float box[4]; long layer; unsigned long bank, frame; long blend, interp; float a0, a1;
                    unsigned char alpha, r, g, b; unsigned long flags; float uv0[2], uv1[2]; };
static SpriteArgs s_sprite; static void* s_setHandle; static void* s_setSprite; static long s_setFlags;

extern "C" void* __fastcall StubGetMainPlayer(void* self, void*) { ++c_mainPlayer; CHECK(self == g_playerManager); return g_player; }
extern "C" void* __fastcall StubPeekHero(void* self, void*) { ++c_peekHero; CHECK(self == g_player); return s_heroPresent ? g_hero : 0; }
extern "C" bool __fastcall StubOverriding(void* self, void*) { ++c_overriding; CHECK(self == g_questUI); return s_overriding; }
extern "C" unsigned char __fastcall StubAlpha(void* self, void*) { ++c_alpha; CHECK(self == g_engine); return s_alpha; }
extern "C" bool __fastcall StubFullyFaded(void* self, void*) { ++c_fullyFaded; CHECK(self == g_engine); return s_fullyFaded; }
extern "C" bool __fastcall StubMiniMap(void* self, void*) { ++c_miniMap; CHECK(self == g_world); return s_miniMap; }
extern "C" bool __fastcall StubRegionLoad(void* self, void*) { ++c_regionLoad; CHECK(self == g_world); return s_regionLoad; }
extern "C" bool __fastcall StubSkippable(void* self, void*) { ++c_skippable; CHECK(self == g_gui + 0x18); return s_skippable; }
extern "C" float __fastcall StubAXToVX(float x) { ++c_axtovx; CHECK(x == 8.0f); return x * 1.5f; }
extern "C" void* __fastcall StubSprite(void* self, void*, unsigned __int64 id, const float* box, long layer, unsigned long bank,
    unsigned long frame, long blend, long interp, float a0, float a1, unsigned char alpha, unsigned char r, unsigned char g,
    unsigned char b, unsigned long flags, const float* uv0, const float* uv1)
{
    ++c_sprite; CHECK(id == 0);
    s_sprite.self = self; memcpy(s_sprite.box, box, 16); s_sprite.layer = layer; s_sprite.bank = bank; s_sprite.frame = frame;
    s_sprite.blend = blend; s_sprite.interp = interp; s_sprite.a0 = a0; s_sprite.a1 = a1; s_sprite.alpha = alpha; s_sprite.r = r;
    s_sprite.g = g; s_sprite.b = b; s_sprite.flags = flags; memcpy(s_sprite.uv0, uv0, 8); memcpy(s_sprite.uv1, uv1, 8);
    return self;
}
extern "C" void __fastcall StubSetPersistent(void* self, void*, void* handle, void* sprite, long flags)
{ ++c_set; CHECK(self == g_engine3d); s_setHandle = handle; s_setSprite = sprite; s_setFlags = flags; }
extern "C" void __fastcall Trap(void*, void*) { printf("FAIL unexpected virtual\n"); fflush(stdout); exit(3); }

extern "C" void __fastcall CallRow(void* self, void* edx, const void* info);

static void Build(bool drawnFull)
{
    memset(g_drawer, 0, sizeof g_drawer); memset(g_gui, 0, sizeof g_gui); memset(g_world, 0, sizeof g_world);
    memset(g_component, 0, sizeof g_component); memset(g_engine, 0, sizeof g_engine); memset(g_hero, 0, sizeof g_hero);
    memset(g_receive, 0, sizeof g_receive); memset(g_info, 0, sizeof g_info);
    Put<void*>(g_drawer, 0x04, g_gui); Put<void*>(g_drawer, 0x08, g_playerManager); Put<void*>(g_drawer, 0x0c, g_world);
    Put<void*>(g_drawer, 0x10, g_component); Put<void*>(g_drawer, 0x1c, g_handles); Put<void*>(g_drawer, 0x20, g_handles + 8);
    Put<void*>(g_drawer, 0x24, g_handles + 8); Put<bool>(g_drawer, 0x28, drawnFull);
    Put<unsigned long>(g_gui, 0x94, s_displayFlags); Put<bool>(g_gui, 0xf6, s_inCutscene); Put<bool>(g_gui, 0xf9, s_whilePaused);
    Put<bool>(g_gui, 0xfd, s_displayingText); Put<bool>(g_world, 0xa4, s_paused); Put<void*>(g_component, 0x28, g_engine);
    Put<bool>(g_engine, 0xbc, s_fadeActive);
    g_engine[0xd4] = s_B; g_engine[0xd5] = s_G; g_engine[0xd6] = s_R; g_engine[0xd7] = 0x11;
    int n = 0;
    if (s_hasReceive) { g_hero[0x20 + 0xcb / 8] |= (unsigned char)(1 << (0xcb % 8)); g_components[n].type = 0xcb; g_components[n++].component = g_receive; }
    if (s_hasQuestUI) { g_hero[0x20 + 0xfc / 8] |= (unsigned char)(1 << (0xfc % 8)); g_components[n].type = 0xfc; g_components[n++].component = g_questUI; }
    Put<void*>(g_hero, 0x44, g_components); Put<void*>(g_hero, 0x48, g_components + n); Put<void*>(g_hero, 0x4c, g_components + n);
    g_hero[0x91] = s_heroDead ? 1 : 0;
    g_receive[0x26] = s_receiveDisplaying;
    for (int i = 0; i < 32; ++i) g_vtbl3d[i] = (void*)Trap;
    g_vtbl3d[23] = (void*)StubSetPersistent;
    Put<void*>(g_engine3d, 0, g_vtbl3d); Put<void*>(g_info, 0x34, g_engine3d);
    c_mainPlayer = c_peekHero = c_overriding = c_alpha = c_fullyFaded = c_miniMap = c_regionLoad = c_skippable = c_axtovx = c_sprite = c_set = 0;
    memset(&s_sprite, 0, sizeof s_sprite); s_setHandle = s_setSprite = 0; s_setFlags = 0;
}

// --- reference model ------------------------------------------------------------------------------------------------------
struct Expect { bool drawing, drawnFull; long layer; float box[4]; unsigned char alpha, r, g, b; int overriding, fullyFaded, miniMap,
                regionLoad, skippable, mainPlayer; };
static Expect Model(bool drawnFull)
{
    Expect e; memset(&e, 0, sizeof e); e.drawnFull = drawnFull; e.layer = -1;
    if (g_bSaveLoadFailed) {
        e.drawing = true; e.layer = 76; e.box[0] = 0; e.box[1] = 0; e.box[2] = 640.0f; e.box[3] = 480.0f; e.alpha = 255; return e;
    }
    e.mainPlayer = 1;
    bool alive = s_heroPresent && !s_heroDead;
    if (alive && s_hasQuestUI) { e.overriding = 1; if (s_overriding) return e; }
    if (!s_fadeActive) { e.drawnFull = false; return e; }
    e.drawing = true; e.box[0] = -8.0f; e.box[1] = -8.0f; e.box[2] = 12.0f + 640.0f; e.box[3] = 12.0f + 480.0f;
    e.alpha = s_alpha; e.r = s_R; e.g = s_G; e.b = s_B;
    if (drawnFull) { e.fullyFaded = 1; if (s_fullyFaded) { e.miniMap = 1; if (s_miniMap) e.regionLoad = 1; } }
    if (drawnFull && s_fullyFaded && s_miniMap && s_regionLoad) { e.layer = 70; return e; }
    if (alive && s_hasReceive && s_receiveDisplaying) { e.layer = 76; return e; }
    if (s_displayingText && (s_displayFlags & 8)) { e.layer = 76; return e; }
    if (s_paused && s_inCutscene) e.skippable = 1;
    if (s_paused && s_inCutscene && s_skippable && s_whilePaused) { e.layer = 70; return e; }
    e.layer = 66; if (s_alpha == 255) e.drawnFull = true;
    return e;
}

static void Run(bool drawnFull)
{
    ++g_run; Build(drawnFull);
    Expect e = Model(drawnFull);
    CallRow(g_drawer, 0, g_info);
    CHECK(Get<bool>(g_drawer, 0x18) == e.drawing);
    CHECK(Get<bool>(g_drawer, 0x28) == e.drawnFull);
    CHECK(c_mainPlayer == e.mainPlayer && c_peekHero == e.mainPlayer);
    CHECK(c_overriding == e.overriding);
    CHECK(c_fullyFaded == e.fullyFaded && c_miniMap == e.miniMap && c_regionLoad == e.regionLoad && c_skippable == e.skippable);
    if (e.layer < 0) { CHECK(c_sprite == 0 && c_set == 0 && c_alpha == 0); return; }
    CHECK(c_sprite == 1 && c_set == 1);
    CHECK(c_alpha == (g_bSaveLoadFailed ? 0 : 1) && c_axtovx == (g_bSaveLoadFailed ? 0 : 2));
    CHECK(s_sprite.layer == e.layer);
    for (int i = 0; i < 4; ++i) CHECK(s_sprite.box[i] == e.box[i]);
    CHECK(s_sprite.bank == 0 && s_sprite.frame == 0 && s_sprite.blend == 2 && s_sprite.interp == 0 && s_sprite.a0 == 0.0f && s_sprite.a1 == 0.0f);
    CHECK(s_sprite.alpha == e.alpha && s_sprite.r == e.r && s_sprite.g == e.g && s_sprite.b == e.b && s_sprite.flags == 0);
    CHECK(s_sprite.uv0[0] == 0.0f && s_sprite.uv0[1] == 0.0f && s_sprite.uv1[0] == 0.0f && s_sprite.uv1[1] == 0.0f);
    CHECK(s_setHandle == g_handles && s_setSprite == s_sprite.self && s_setFlags == 0xc0);
}

static void Reset()
{
    g_bSaveLoadFailed = false; s_heroPresent = true; s_heroDead = false; s_hasQuestUI = false; s_overriding = false; s_hasReceive = false;
    s_receiveDisplaying = false; s_fadeActive = true; s_fullyFaded = false; s_miniMap = false; s_regionLoad = false; s_paused = false;
    s_inCutscene = false; s_skippable = false; s_whilePaused = false; s_displayingText = false; s_displayFlags = 0;
    s_alpha = 200; s_R = 10; s_G = 20; s_B = 30;
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    Reset(); g_bSaveLoadFailed = true; Run(false);                                     // 1 forced black
    Reset(); g_bSaveLoadFailed = true; Run(true);                                      // 2 forced black, flag untouched
    Reset(); s_hasQuestUI = true; s_overriding = true; Run(true);                      // 3 quest UI overrides: nothing
    Reset(); s_hasQuestUI = true; s_heroDead = true; s_overriding = true; Run(false);   // 4 dead hero: quest UI not asked
    Reset(); s_hasQuestUI = true; s_fadeActive = false; Run(true);                     // 5 no fade: flag cleared
    Reset(); s_fadeActive = false; Run(false);                                         // 6 no fade, flag clear
    Reset(); s_fullyFaded = true; s_miniMap = true; s_regionLoad = true; Run(true);    // 7 minimap region load: layer 3
    Reset(); s_fullyFaded = true; s_miniMap = true; s_regionLoad = true; Run(false);   // 8 same without the flag: falls through
    Reset(); s_fullyFaded = true; s_miniMap = true; s_hasReceive = true; s_receiveDisplaying = true; Run(true);   // 9 receive items: layer 8
    Reset(); s_hasReceive = true; s_receiveDisplaying = false; s_hasQuestUI = true; s_alpha = 255; Run(false);  // 10 not displaying -> M1, flag set
    Reset(); s_heroPresent = false; s_displayingText = true; s_displayFlags = 8; Run(false);                     // 11 subtitle flag 8: layer 8
    Reset(); s_displayingText = true; s_displayFlags = 7; s_paused = true; s_inCutscene = true; s_skippable = true; s_whilePaused = true; Run(true); // 12 cutscene: layer 3
    Reset(); s_displayingText = false; s_displayFlags = 8; s_paused = true; s_inCutscene = true; s_skippable = false; s_whilePaused = true; Run(true); // 13 not skippable -> M1
    Reset(); s_paused = true; s_inCutscene = true; s_skippable = true; s_whilePaused = false; s_alpha = 255; Run(false); // 14 not while paused -> M1, flag set
    Reset(); s_paused = false; s_inCutscene = true; s_skippable = true; s_whilePaused = true; Run(true);          // 15 not paused -> M1, flag kept
    Reset(); s_heroDead = true; s_hasReceive = true; s_receiveDisplaying = true; Run(false);                       // 16 dead hero: receive items ignored
    Reset(); s_fullyFaded = true; s_miniMap = false; s_regionLoad = true; Run(true);                                // 17 no minimap -> M1
    if (g_fail) { printf("006495a8_TEST FAIL\n"); return 1; }
    printf("006495a8_TEST PASS\n");
    return 0;
}