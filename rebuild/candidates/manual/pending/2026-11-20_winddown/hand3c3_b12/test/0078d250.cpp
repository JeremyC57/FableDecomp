// Behaviour test for 0x0078D250 CTCBerserk::FrameUpdate (links the REAL row object, which also carries the rough-area search
// GetAllThingsInRoughArea<CThingGameObject, list, CFilter_Bind2<In2DRange, CanBeHitByEpicSpell>> 0x0078DB20, the list destructor
// and the nine-argument CHitParameters constructor it calls).
// No class models: the row is entered and its out-of-line callees are bound through `/alternatename` (thiscall = fastcall with a
// spare edx); the only classes declared here exist to give four data symbols their retail names. Every object is raw
// native-layout storage:
//   CTCBerserk       ParentThing +4, Timer +0xC, MaxIncreaseInStrength +0x18, ReleasedAnimationTimer +0x1C, AnimTimer +0x20,
//                    BerserkScale +0x2C, SpellLevel +0x44, JustReleased +0x48, StartUpEffect +0x58 (pointer word +0x5C), PDef +0x68,
//                    RadialBlurFadeKey +0x6C, Paused +0x70, IsInCutscene +0x72
//   the berserk def  FramesToFadeOut +0x40, LowerBerserkPercent +0x44, BlurCenterFromFloor +0x4C, BerserkBlastRadius +0xBC,
//                    TintColorR / G / B (vector<long> starts) +0x15C / +0x168 / +0x174
//   CThing           component bitset +0x20, interface map {start, finish} +0x44, graphic appearance +0x64, type flags +0x6C,
//                    state byte +0x91 (bit 0 dead); vtable slot 65 (+0x104) OnHit; the test keeps each thing's position at +0x180
//   the thing map    level infos {start} +4 (20 bytes each, extents X +4), cells {start} +0x14 (a 12-byte vector per layer of
//                    4-byte cells = first node); node {mapwho, next}; mapwho ParentThing +4, SearchID +0x4C
//   CHitParameters   Localised, Directional +0, Position +2, Direction +0xE, fifteen flags +0x1A, PHitLocation +0x48,
//                    PThingHitting / PWeapon pointer words +0x54 / +0x5C
// An independent model (RunModel) predicts the whole event list (every callee with its arguments, in order) and the final state
// of the component, the opinion source and the mapwho search stamps; both are compared after every case. Floats the function
// computes are compared within 2e-4 relative for the normalised direction / hit point (one table step of the fast inverse root)
// and 3e-7 relative for everything else.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { ExitProcess(3); return 0; }

extern "C" void __fastcall CallFrameUpdate(void* self);
#pragma comment(linker, "/alternatename:@CallFrameUpdate@4=?FrameUpdate@CTCBerserk@@UAEXXZ")

// ---- data the row object needs under its retail names
extern unsigned long GFastOneOverSqrtTable[128];
unsigned long GFastOneOverSqrtTable[128];                            // 0x013CE558
namespace NThingMap {
class CLayerInfo { public: long Size, ScaleShift; unsigned long UsedColour, UnusedColour; };
extern long MaxLayers;
long MaxLayers = 2;                                                  // 0x013BA848
extern const CLayerInfo LayerInfo[] = { { 8, 0, 0, 0 }, { 16, 1, 0, 0 } };   // 0x0137DAD8
}
class CThingSearchTools { public: static unsigned long NoLookups, NoThingsScanned, SearchID; };
unsigned long CThingSearchTools::NoLookups, CThingSearchTools::NoThingsScanned, CThingSearchTools::SearchID;
void operator delete(void* p) { free(p); }

// ---- deterministic random numbers
static unsigned long g_seed = 0x0078d250u;
static unsigned long Rnd() { g_seed = g_seed * 1664525ul + 1013904223ul; return g_seed >> 8; }
static long RndRange(long lo, long hi) { return lo + (long)(Rnd() % (unsigned long)(hi - lo + 1)); }
static float RndFloat(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() & 0xffff) / 65535.0f; }
static float FromBits(unsigned long bits) { float f; memcpy(&f, &bits, 4); return f; }
static unsigned long Bits(float f) { unsigned long b; memcpy(&b, &f, 4); return b; }
// a value as the float the row would store (through memory: a cast alone is not rounded by this compiler)
static double Single(double value) { float f = (float)value; return (double)FromBits(Bits(f)); }
template <class T> static T& At(void* base, int offset) { return *(T*)((char*)base + offset); }

// ---- raw objects
enum { THING = 0x200, POS = 0x180, SEARCH_THINGS = 7, MAPS = 2, LAYERS = 2, EXT0 = 6, EXT1 = 3, MAX_NODES = 96, LEVELS = 4 };
static unsigned char g_berserk[0x74], g_def[0x180], g_creature[THING], g_effect[THING], g_things[SEARCH_THINGS][THING];
static unsigned char g_world[0x80], g_environment[0x1968], g_tools[0x100];
static unsigned char g_opinionManager[8], g_interpolator[8], g_modeManager[8], g_morph[8], g_appearance[8], g_opinionSource[0x8c];
static unsigned char g_mapwho[SEARCH_THINGS + 1][0x54];             // the last one is the creature's own
static long g_tintR[LEVELS], g_tintG[LEVELS], g_tintB[LEVELS];
static void* g_vtable[80];
struct Pair { long type; void* component; };
static Pair g_interfaces[8]; static int g_interfaceCount;
// the thing maps
struct Node { void* mapwho; Node* next; Node* previous; void* cell; };
static Node g_nodes[MAX_NODES]; static int g_nodeCount;
static Node* g_cells[MAPS][LAYERS][EXT0 * EXT0];
static unsigned long g_cellVectors[MAPS][LAYERS][3];
static long g_levelInfo[MAPS][LAYERS][5];
static unsigned char g_maps[MAPS][0x34];
static long g_localBox[MAPS][4];
static void* g_areaMaps[MAPS]; static int g_mapCount;

// ---- per-case decisions of the callees
static int g_actionAccepted, g_opinionAttached, g_modeActive;
static int g_inRange[SEARCH_THINGS + 1], g_epicOk[SEARCH_THINGS + 1], g_enemy[SEARCH_THINGS + 1];
static unsigned long g_blend;                                       // what GFGetInterpolatedColour returns

// ---- the event list
struct Ev { char kind; long a[6]; float f[7]; };
enum { MAX_EV = 512 };
static Ev g_got[MAX_EV], g_want[MAX_EV]; static int g_gotCount, g_wantCount;
static int g_modelling;
static Ev& Log(char kind, long a0 = 0, long a1 = 0, long a2 = 0, long a3 = 0, long a4 = 0, long a5 = 0)
{
    static Ev overflow;
    Ev* list = g_modelling ? g_want : g_got; int& count = g_modelling ? g_wantCount : g_gotCount;
    Ev& e = count < MAX_EV ? list[count++] : overflow;
    memset(&e, 0, sizeof(e)); e.kind = kind; e.a[0] = a0; e.a[1] = a1; e.a[2] = a2; e.a[3] = a3; e.a[4] = a4; e.a[5] = a5;
    return e;
}
static int ThingIndex(void* thing)
{
    if (thing == g_creature) return 100;
    if (thing == g_effect) return 200;
    for (int i = 0; i < SEARCH_THINGS; i++) if (thing == g_things[i]) return i;
    return -1;
}
static int TextIndex(const char* text)
{
    if (!text) return -1;
    if (!strcmp(text, "SIMPLE_NOISE")) return 1;
    if (!strcmp(text, "OPINION_SOURCE_BERSERK")) return 2;
    return 0;
}
static void* g_actionObject;

// ---- the callees
extern "C" void* __fastcall StubActionCtor(void* self, int, void* creature) { g_actionObject = self; Log('A', ThingIndex(creature)); return self; }
#pragma comment(linker, "/alternatename:??0CCreatureAction_ReleaseStrengthBerserk@@QAE@AAVCThingCreatureBase@@@Z=@StubActionCtor@12")
extern "C" bool __fastcall StubSetCurrentAction(void* self, int, void* action) { Log('S', ThingIndex(self), action == g_actionObject); return g_actionAccepted != 0; }
#pragma comment(linker, "/alternatename:?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z=@StubSetCurrentAction@12")
extern "C" void __fastcall StubActionDtor(void* self) { Log('D', self == g_actionObject); }
#pragma comment(linker, "/alternatename:??1CCombatActionBase@@UAE@XZ=@StubActionDtor@4")
extern "C" void* __fastcall StubStringCtor(void* self, int, const char* text, long length) { At<const char*>(self, 0) = text; Log('"', TextIndex(text), length); return self; }
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StubStringCtor@16")
extern "C" void __fastcall StubStringDtor(void* self) { Log('~', TextIndex(At<const char*>(self, 0))); }
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StubStringDtor@4")
extern "C" void __fastcall StubMakeNoise(void* self, int, void* name) { Log('N', ThingIndex(self), TextIndex(At<const char*>(name, 0)), At<unsigned char>(g_berserk, 0x48)); }
#pragma comment(linker, "/alternatename:?MakeNoise@CThingCreatureBase@@QAEXABVCCharString@@@Z=@StubMakeNoise@12")
extern "C" void* __fastcall StubGetPItem(void* self) { return At<void*>(self, 4); }
#pragma comment(linker, "/alternatename:?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ=@StubGetPItem@4")
extern "C" void __fastcall StubSetPItem(void* self, int, void* item) { At<void*>(self, 4) = item; }
#pragma comment(linker, "/alternatename:?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z=@StubSetPItem@12")
extern "C" void* __fastcall StubPointerCtor(void* self) { At<void*>(self, 4) = 0; return self; }
#pragma comment(linker, "/alternatename:??0CBaseIntelligentPointer@@QAE@XZ=@StubPointerCtor@4")
extern "C" void __fastcall StubPointerDtor(void*) { }
#pragma comment(linker, "/alternatename:??1CBaseIntelligentPointer@@UAE@XZ=@StubPointerDtor@4")
extern "C" void* __fastcall StubPointerDelete(void* self, int, unsigned int) { return self; }
#pragma comment(linker, "/alternatename:??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z=@StubPointerDelete@12")
extern "C" void __fastcall StubKill(void* self, int, int now) { Log('K', ThingIndex(self), now & 0xff); }
#pragma comment(linker, "/alternatename:?Kill@CThing@@QAEX_N@Z=@StubKill@12")
extern "C" void* __fastcall StubGetWorld(void* self) { if (self != g_berserk) Log('!', 1); return g_world; }
#pragma comment(linker, "/alternatename:?GetWorld@CTCBase@@IBEAAVCWorld@@XZ=@StubGetWorld@4")
extern "C" void* __fastcall StubSearchTools(void* self) { if (self != g_berserk) Log('!', 2); return g_tools; }
#pragma comment(linker, "/alternatename:?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ=@StubSearchTools@4")
extern "C" bool __fastcall StubAttach(void* self, int, void* thing, void* name, void** source)
{
    Log('O', self == g_opinionManager, ThingIndex(thing), TextIndex(At<const char*>(name, 0)));
    if (g_opinionAttached) *source = g_opinionSource;
    return g_opinionAttached != 0;
}
#pragma comment(linker, "/alternatename:?AttachOpinionSource@COpinionReactionManager@@QBE_NAAVCThing@@ABVCCharString@@PAPAVCTCOpinionSource@@@Z=@StubAttach@20")
extern "C" void* __fastcall StubGetPos(void* self) { return (char*)self + POS; }
#pragma comment(linker, "/alternatename:?GetPos@CThing@@QBEABVC3DVector@@XZ=@StubGetPos@4")
extern "C" void __fastcall StubAreaMaps(void* self, int, const long* area, void** maps)
{
    Log('T', self == g_tools, area[0], area[1], area[2], area[3]);
    maps[0] = g_areaMaps; maps[1] = g_areaMaps + g_mapCount; maps[2] = g_areaMaps + MAPS;
}
#pragma comment(linker, "/alternatename:?GetAllThingMapsInArea@CThingSearchTools@@IBEXABVC2DBoxI@@AAV?$vector@PAVCThingMap@NThingMap@@V?$allocator@PAVCThingMap@NThingMap@@@std@@@std@@@Z=@StubAreaMaps@16")
extern "C" long* __fastcall StubClip(void* self, int, long* result, void* map, const long* area)
{
    int index = (int)(((unsigned char*)map - g_maps[0]) / 0x34);
    Log('C', index, area[0], area[1], area[2], area[3]);
    memcpy(result, g_localBox[index], 16);
    return result;
}
#pragma comment(linker, "/alternatename:?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@ABV2@@Z=@StubClip@20")
extern "C" bool __fastcall StubInRange(void* self, int, void* thing)
{
    Ev& e = Log('F', ThingIndex(thing)); memcpy(e.f, self, 16);
    int index = ThingIndex(thing); return g_inRange[index == 100 ? SEARCH_THINGS : index] != 0;
}
#pragma comment(linker, "/alternatename:??RCThingFilter_In2DRange@@QBE_NABVCThing@@@Z=@StubInRange@12")
extern "C" bool __fastcall StubEpic(void*, int, void* thing)
{
    Log('E', ThingIndex(thing));
    int index = ThingIndex(thing); return g_epicOk[index == 100 ? SEARCH_THINGS : index] != 0;
}
#pragma comment(linker, "/alternatename:??RCThingFilter_CanBeHitByEpicSpell@@QBE_NABVCThing@@@Z=@StubEpic@12")
extern "C" bool __fastcall StubEnemies(void* thing1, void* thing2)
{
    Log('M', ThingIndex(thing1), ThingIndex(thing2));
    int index = ThingIndex(thing2); return index >= 0 && index < SEARCH_THINGS && g_enemy[index] != 0;
}
#pragma comment(linker, "/alternatename:?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z=@StubEnemies@8")
extern "C" void __fastcall StubAddDamage(void*, int, long attribute, float damage) { Log('W', attribute, (long)Bits(damage)); }
#pragma comment(linker, "/alternatename:?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z=@StubAddDamage@16")
extern "C" void __fastcall StubHitDtor(void*) { Log('x'); }
#pragma comment(linker, "/alternatename:??1CHitParameters@@QAE@XZ=@StubHitDtor@4")
extern "C" void __fastcall StubOnHit(void* self, int, void* hit)
{
    unsigned long flags = 0; int i;
    for (i = 0; i < 15; i++) flags |= (unsigned long)(At<unsigned char>(hit, 0x1a + i) != 0) << i;
    flags |= (unsigned long)(At<unsigned char>(hit, 0) != 0) << 15; flags |= (unsigned long)(At<unsigned char>(hit, 1) != 0) << 16;
    Ev& e = Log('X', ThingIndex(self), (long)flags, At<void*>(hit, 0x48) == 0, ThingIndex(At<void*>(hit, 0x54)), At<void*>(hit, 0x5c) == 0);
    memcpy(e.f, (char*)hit + 2, 24);
}
extern "C" bool __fastcall StubIsModeActive(void* self, int, unsigned long mode) { Log('I', self == g_modeManager, (long)mode); return g_modeActive != 0; }
#pragma comment(linker, "/alternatename:?IsModeActive@CTCCreatureModeManager@@QBE_NK@Z=@StubIsModeActive@12")
extern "C" bool __fastcall StubAddMode(void* self, int, unsigned long mode) { Log('a', self == g_modeManager, (long)mode); return true; }
#pragma comment(linker, "/alternatename:?AddMode@CTCCreatureModeManager@@QAE_NK@Z=@StubAddMode@12")
extern "C" void __fastcall StubBerserkSkin(void* self, int, float value) { Ev& e = Log('B', self == g_morph); e.f[0] = value; }
#pragma comment(linker, "/alternatename:?SetBerserkSkin@CTCHeroMorph@@QAEXM@Z=@StubBerserkSkin@12")
extern "C" unsigned long* __cdecl StubBlend(unsigned long* result, unsigned long a, unsigned long b, unsigned long t)
{
    Log('G', (long)a, (long)b, (long)t);
    *result = g_blend;
    return result;
}
#pragma comment(linker, "/alternatename:?GFGetInterpolatedColour@@YA?AVCRGBColour@@V1@0K@Z=_StubBlend")
extern "C" void __fastcall StubSetTint(void* self, int, const unsigned long* colour, void* owner) { Log('c', self == g_appearance, (long)*colour, owner == g_berserk); }
#pragma comment(linker, "/alternatename:?SetTintColour@CTCGraphicAppearance@NGraphicAppearance@@QAEXABVCRGBColour@@PAX@Z=@StubSetTint@16")
extern "C" void __fastcall StubSetScale(void* self, int, float scale) { Ev& e = Log('s', self == g_appearance); e.f[0] = scale; }
#pragma comment(linker, "/alternatename:?SetScale@CTCGraphicAppearance@NGraphicAppearance@@QAEXM@Z=@StubSetScale@12")
extern "C" void __fastcall StubRadialBlur(void* self, int, void* key, const float* position)
{
    Ev& e = Log('R', self == g_interpolator, (long)key); memcpy(e.f, position, 12);
}
#pragma comment(linker, "/alternatename:?RadialBlurSetCenterWorldPos@CEnvironmentSpellEffectInterpolator@@QAEXPAXABVC3DVector@@@Z=@StubRadialBlur@16")
extern "C" void __fastcall StubCleanup(void* self, int, int in_cutscene) { Log('U', self == g_berserk, in_cutscene & 0xff); }
#pragma comment(linker, "/alternatename:?CleanupAbility@CTCBerserk@@QAEX_N@Z=@StubCleanup@12")
extern "C" void __fastcall StubRemoveTC(void* self, int, long type) { Log('r', ThingIndex(self), type); }
#pragma comment(linker, "/alternatename:?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z=@StubRemoveTC@12")

// ---- the model
static long g_statRelease, g_statRefused, g_statKilled, g_statAttached, g_statNoLevel, g_statFound, g_statSelf, g_statHit, g_statFriend,
            g_statModeAdded, g_statRampUp, g_statRampDown, g_statMorph, g_statTint, g_statCleanup, g_statHeld, g_statClamped, g_statEmptyBox;

static double ModelFastOneOverSqrt(double squared_value)
{
    float value = (float)Single(squared_value);
    unsigned long bits = Bits(value);
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    double r = FromBits(estimate);
    return Single(((3.0 - r * r * (double)value) * r) * 0.5);
}
static void ModelRamp(double fraction_value)
{
    double fraction = Single(fraction_value);
    if (At<unsigned long>(g_creature, 0x20) & 8) {
        g_statMorph++;
        double skin = (double)At<float>(g_def, 0x44) + (double)At<float>(g_berserk, 0x18);
        if (skin > 1.0) { skin = 1.0; g_statClamped++; }
        Ev& e = Log('B', 1); e.f[0] = (float)Single(skin * fraction);
    }
    if (At<void*>(g_creature, 0x64)) {
        g_statTint++;
        long level = At<long>(g_berserk, 0x44);
        unsigned long c = ((unsigned long)g_tintB[level] & 0xff) | (((unsigned long)g_tintG[level] & 0xff) << 8)
                        | (((unsigned long)g_tintR[level] & 0xff) << 16) | 0xff000000ul;
        double scaled = Single(fraction * 256.0);
        double fl = floor(scaled); double diff = scaled - fl; long t = (long)fl;
        if (diff > 0.5 || (diff == 0.5 && (t & 1))) t++;                                      // round to nearest, ties to even
        Log('G', (long)0xfffffffful, (long)c, t);
        Log('c', 1, (long)g_blend, 1);
        Ev& e = Log('s', 1); e.f[0] = (float)Single(((double)At<float>(g_berserk, 0x2c) - 1.0) * fraction + 1.0);
    }
}
static unsigned char g_wantBerserk[0x74], g_wantSource[0x8c];
static unsigned long g_wantSearchId[SEARCH_THINGS + 1], g_wantStatics[3];

static void RunModel()
{
    int i;
    g_modelling = 1; g_wantCount = 0;
    memcpy(g_wantBerserk, g_berserk, sizeof(g_berserk)); memcpy(g_wantSource, g_opinionSource, sizeof(g_opinionSource));
    for (i = 0; i <= SEARCH_THINGS; i++) g_wantSearchId[i] = At<unsigned long>(g_mapwho[i], 0x4c);
    g_wantStatics[0] = CThingSearchTools::NoLookups; g_wantStatics[1] = CThingSearchTools::NoThingsScanned; g_wantStatics[2] = CThingSearchTools::SearchID;
    const float* centre = &At<float>(g_creature, POS);
    if (At<unsigned char>(g_berserk, 0x48)) {
        g_statRelease++;
        Log('A', 100); Log('S', 100, 1); Log('D', 1);
        if (!g_actionAccepted) { g_statRefused++; g_modelling = 0; return; }
        g_wantBerserk[0x48] = 0;
        Log('"', 1, -1); Log('N', 100, 1, 0); Log('~', 1);               // the release flag is already cleared when the noise is made
        void* effect = At<void*>(g_berserk, 0x5c);
        if (effect && !(At<unsigned char>(effect, 0x91) & 1)) { Log('K', 200, 1); g_statKilled++; }
        Log('"', 2, -1); Log('O', 1, 100, 2); Log('~', 2);
        if (g_opinionAttached) { g_wantSource[0x77] = 1; g_statAttached++; }
        if (At<long>(g_berserk, 0x44) >= 0) {
            float radius = At<float>(g_def, 0xbc);
            long cx = (long)centre[0], cy = (long)centre[1], r = (long)radius;                 // truncation, as __ftol2
            long area[4] = { cx - r, cy - r, cx + r, cy + r };
            Log('T', 1, area[0], area[1], area[2], area[3]);
            g_wantStatics[0]++; g_wantStatics[1] = 0; g_wantStatics[2]++;
            unsigned long search_id = g_wantStatics[2];
            int found[SEARCH_THINGS + 1], found_count = 0;
            for (int m = 0; m < g_mapCount; m++) {
                Log('C', m, area[0], area[1], area[2], area[3]);
                for (int layer = 0; layer < LAYERS; layer++) {
                    long shift = NThingMap::LayerInfo[layer].ScaleShift;
                    long tlx = g_localBox[m][0] >> shift, tly = g_localBox[m][1] >> shift;
                    long brx = ((g_localBox[m][2] - 1) >> shift) + 1, bry = ((g_localBox[m][3] - 1) >> shift) + 1;
                    if (brx - tlx == 0 || bry - tly == 0) { g_statEmptyBox++; continue; }
                    long extent = layer ? EXT1 : EXT0;
                    for (long y = tly; y < bry; y++) for (long x = tlx; x < brx; x++)
                        for (Node* node = g_cells[m][layer][extent * y + x]; node; node = node->next) {
                            int who = (int)(((unsigned char*)node->mapwho - g_mapwho[0]) / 0x54);
                            if (g_wantSearchId[who] >= search_id) continue;
                            g_wantSearchId[who] = search_id;
                            void* thing = who == SEARCH_THINGS ? (void*)g_creature : (void*)g_things[who];
                            if ((At<unsigned long>(thing, 0x6c) & 7) != 7) continue;
                            Ev& e = Log('F', who == SEARCH_THINGS ? 100 : who); memcpy(e.f, centre, 12); e.f[3] = radius;
                            if (!g_inRange[who]) continue;
                            Log('E', who == SEARCH_THINGS ? 100 : who);
                            if (!g_epicOk[who]) continue;
                            found[found_count++] = who;
                        }
                }
            }
            for (i = 0; i < found_count; i++) {
                int who = found[i];
                g_statFound++;
                if (who == SEARCH_THINGS) { g_statSelf++; continue; }
                unsigned char* thing = g_things[who];
                if (At<unsigned long>(thing, 0x6c) & 8) {
                    Log('M', 100, who);
                    if (!g_enemy[who]) { g_statFriend++; continue; }
                }
                g_statHit++;
                const float* p = &At<float>(thing, POS);
                double dx = (double)p[0] - centre[0], dy = (double)p[1] - centre[1], dz = Single((double)p[2] - centre[2]);
                double factor = ModelFastOneOverSqrt(dx * dx + dz * dz + dy * dy);
                double nx = Single(dx * factor), ny = Single(dy * factor), nz = Single(dz * factor);
                Log('W', 0, (long)Bits(0.001f));
                // KnockDown, PlayHitResponse, MoveBack, CreateParticleEffectOnHit, CreateDustParticleEffectOnHit; Localised, Directional
                Ev& e = Log('X', who, 0x741 | (1 << 15) | (1 << 16), 1, 100, 1);
                e.f[0] = (float)((double)p[0] - nx); e.f[1] = (float)((double)p[1] - ny); e.f[2] = (float)((double)p[2] - nz);
                e.f[3] = (float)nx; e.f[4] = (float)ny; e.f[5] = (float)nz;
                Log('x');
            }
        } else g_statNoLevel++;
        Log('I', 1, 0x24);
        if (!g_modeActive) { Log('a', 1, 0x24); g_statModeAdded++; }
        g_modelling = 0;
        return;
    }
    long anim = At<long>(g_berserk, 0x20) + 1;
    memcpy(g_wantBerserk + 0x20, &anim, 4);
    float released = At<float>(g_berserk, 0x1c);
    if ((double)anim < (double)released) { g_statRampUp++; ModelRamp((double)anim / (double)released); }
    long timer = At<long>(g_berserk, 0xc), frames = At<long>(g_def, 0x40);
    bool paused = At<unsigned char>(g_berserk, 0x70) != 0, cutscene = At<unsigned char>(g_berserk, 0x72) != 0;
    if (!paused || timer < frames) {
        if (!cutscene) timer--;
        else if (timer >= frames) timer--;
        else g_statHeld++;
    } else g_statHeld++;
    memcpy(g_wantBerserk + 0xc, &timer, 4);
    Ev& blur = Log('R', 1, At<long>(g_berserk, 0x6c));
    blur.f[0] = centre[0]; blur.f[1] = centre[1]; blur.f[2] = (float)Single((double)centre[2] + (double)At<float>(g_def, 0x4c));
    if (timer < 0 && !cutscene) { Log('U', 1, 0); Log('r', 100, 0x84); g_statCleanup++; }
    else if (timer < frames) { g_statRampDown++; ModelRamp((double)timer / (double)frames); }
    g_modelling = 0;
}

// ---- one case
static int g_fail; static long g_case;
static void Fail(const char* what, long a, long b)
{
    if (g_fail < 12) printf("case %ld: %s (%ld, %ld)\n", g_case, what, a, b);
    g_fail++;
}
static bool Near(float got, float want, double relative)
{
    if (got != got) return false;
    return fabs((double)got - (double)want) <= relative * (1.0 + fabs((double)want));
}
static void AddNode(int map, int layer, int cell, int who)
{
    if (g_nodeCount >= MAX_NODES) return;
    Node& node = g_nodes[g_nodeCount++];
    node.mapwho = g_mapwho[who]; node.next = 0; node.previous = 0; node.cell = &g_cells[map][layer][cell];
    Node** link = &g_cells[map][layer][cell];
    if (Rnd() & 1) { node.next = *link; *link = &node; }            // at the head
    else { while (*link) link = &(*link)->next; *link = &node; }    // at the tail
}
static void SetupThing(unsigned char* thing, unsigned long flags, int dead)
{
    for (int i = 0; i < THING; i++) thing[i] = (unsigned char)Rnd();
    At<void*>(thing, 0) = g_vtable;
    At<unsigned long>(thing, 0x6c) = flags;
    At<unsigned char>(thing, 0x91) = (unsigned char)((Rnd() & 0xfe) | (dead ? 1 : 0));
    for (int k = 0; k < 3; k++) At<float>(thing, POS + 4 * k) = RndFloat(-40.0f, 40.0f);
}

static void OneCase()
{
    int i, m, layer;
    // the creature and its components
    SetupThing(g_creature, (Rnd() % 6 == 0 ? 3 : 7) | 8 | ((Rnd() & 0xff) << 8), 0);
    int has_morph = (int)(Rnd() % 3 != 0);
    At<unsigned long>(g_creature, 0x20) = (Rnd() * 2654435761ul & ~8ul) | (has_morph ? 8 : 0);
    At<unsigned long>(g_creature, 0x24) = (Rnd() * 2246822519ul) | 0x20000;                        // the mode manager is always there
    g_interfaceCount = 0;
    if (Rnd() & 1) { g_interfaces[g_interfaceCount].type = 1; g_interfaces[g_interfaceCount++].component = g_opinionManager; }
    if (has_morph || (Rnd() & 1)) { g_interfaces[g_interfaceCount].type = 3; g_interfaces[g_interfaceCount++].component = g_morph; }
    if (Rnd() & 1) { g_interfaces[g_interfaceCount].type = 0x20; g_interfaces[g_interfaceCount++].component = g_interpolator; }
    g_interfaces[g_interfaceCount].type = 0x31; g_interfaces[g_interfaceCount++].component = g_modeManager;
    if (Rnd() & 1) { g_interfaces[g_interfaceCount].type = 0x84; g_interfaces[g_interfaceCount++].component = g_berserk; }
    At<void*>(g_creature, 0x44) = g_interfaces; At<void*>(g_creature, 0x48) = g_interfaces + g_interfaceCount; At<void*>(g_creature, 0x4c) = g_interfaces + 8;
    At<void*>(g_creature, 0x64) = (Rnd() % 4 == 0) ? (void*)0 : (void*)g_appearance;
    SetupThing(g_effect, 7, (int)(Rnd() % 3 == 0));
    // the def
    for (i = 0; i < (int)sizeof(g_def); i++) g_def[i] = (unsigned char)Rnd();
    At<long>(g_def, 0x40) = RndRange(1, 40);
    At<float>(g_def, 0x44) = RndFloat(0.0f, 0.9f);
    At<float>(g_def, 0x4c) = RndFloat(-2.0f, 3.0f);
    At<float>(g_def, 0xbc) = RndFloat(0.0f, 30.0f);
    At<void*>(g_def, 0x15c) = g_tintR; At<void*>(g_def, 0x168) = g_tintG; At<void*>(g_def, 0x174) = g_tintB;
    for (i = 0; i < LEVELS; i++) { g_tintR[i] = (long)(Rnd() & 0xffff); g_tintG[i] = (long)(Rnd() & 0xffff); g_tintB[i] = (long)(Rnd() & 0xffff); }
    // the component
    for (i = 0; i < (int)sizeof(g_berserk); i++) g_berserk[i] = (unsigned char)Rnd();
    At<void*>(g_berserk, 4) = g_creature;
    int release = (int)(Rnd() % 3 == 0);
    At<long>(g_berserk, 0xc) = (Rnd() % 4 == 0) ? RndRange(-2, 1) : RndRange(-3, 60);
    At<float>(g_berserk, 0x18) = RndFloat(0.0f, 0.6f);
    At<float>(g_berserk, 0x1c) = (Rnd() % 5 == 0) ? (float)RndRange(1, 30) : RndFloat(1.0f, 30.0f);
    At<long>(g_berserk, 0x20) = RndRange(0, 30);
    At<float>(g_berserk, 0x2c) = RndFloat(0.5f, 2.0f);
    At<long>(g_berserk, 0x44) = release ? (Rnd() % 5 == 0 ? -1 : RndRange(0, LEVELS - 1)) : RndRange(0, LEVELS - 1);
    At<unsigned char>(g_berserk, 0x48) = (unsigned char)(release ? RndRange(1, 255) : 0);
    At<void*>(g_berserk, 0x5c) = (Rnd() % 4 == 0) ? (void*)0 : (void*)g_effect;
    At<void*>(g_berserk, 0x68) = g_def;
    At<unsigned char>(g_berserk, 0x70) = (unsigned char)(Rnd() % 3 == 0 ? RndRange(1, 255) : 0);
    At<unsigned char>(g_berserk, 0x72) = (unsigned char)(Rnd() % 3 == 0 ? RndRange(1, 255) : 0);
    // the world
    memset(g_world, 0x5a, sizeof(g_world)); At<void*>(g_world, 0x74) = g_opinionManager; At<void*>(g_world, 0x18) = g_environment;
    At<void*>(g_environment, 0x1964) = g_interpolator;
    for (i = 0; i < (int)sizeof(g_opinionSource); i++) g_opinionSource[i] = (unsigned char)Rnd();
    g_opinionSource[0x77] = (unsigned char)(Rnd() % 2 ? 0 : RndRange(2, 255));
    // the things round the creature and the maps they are filed in
    CThingSearchTools::SearchID = 1000 + (unsigned long)RndRange(0, 5000);
    CThingSearchTools::NoLookups = (unsigned long)Rnd(); CThingSearchTools::NoThingsScanned = (unsigned long)Rnd();
    for (i = 0; i < SEARCH_THINGS; i++) {
        SetupThing(g_things[i], (Rnd() % 7 == 0 ? RndRange(0, 6) : 7) | (Rnd() % 2 ? 8 : 0) | ((Rnd() & 0xfff) << 4), 0);   // bit 3: a creature
        g_inRange[i] = (int)(Rnd() % 5 != 0); g_epicOk[i] = (int)(Rnd() % 5 != 0); g_enemy[i] = (int)(Rnd() % 2);
    }
    g_inRange[SEARCH_THINGS] = (int)(Rnd() % 4 != 0); g_epicOk[SEARCH_THINGS] = (int)(Rnd() % 4 != 0); g_enemy[SEARCH_THINGS] = 1;
    for (i = 0; i <= SEARCH_THINGS; i++) {
        for (int k = 0; k < 0x54; k++) g_mapwho[i][k] = (unsigned char)Rnd();
        At<void*>(g_mapwho[i], 4) = i == SEARCH_THINGS ? (void*)g_creature : (void*)g_things[i];
        // already stamped by this search number or a later one: never visited
        At<unsigned long>(g_mapwho[i], 0x4c) = (Rnd() % 9 == 0) ? CThingSearchTools::SearchID + 1 + (unsigned long)RndRange(0, 3) : (unsigned long)RndRange(0, 999);
    }
    g_mapCount = (int)(Rnd() % 8 == 0 ? 0 : RndRange(1, MAPS));
    g_nodeCount = 0; memset(g_cells, 0, sizeof(g_cells));
    for (m = 0; m < MAPS; m++) {
        for (i = 0; i < 0x34; i++) g_maps[m][i] = (unsigned char)Rnd();
        for (layer = 0; layer < LAYERS; layer++) {
            long extent = layer ? EXT1 : EXT0;
            g_levelInfo[m][layer][0] = (long)Rnd(); g_levelInfo[m][layer][1] = extent; g_levelInfo[m][layer][2] = extent;
            g_levelInfo[m][layer][3] = NThingMap::LayerInfo[layer].ScaleShift; g_levelInfo[m][layer][4] = (long)Rnd();
            g_cellVectors[m][layer][0] = (unsigned long)g_cells[m][layer];
            g_cellVectors[m][layer][1] = g_cellVectors[m][layer][2] = (unsigned long)(g_cells[m][layer] + extent * extent);
        }
        At<void*>(g_maps[m], 4) = g_levelInfo[m]; At<void*>(g_maps[m], 8) = g_levelInfo[m] + LAYERS; At<void*>(g_maps[m], 0xc) = g_levelInfo[m] + LAYERS;
        At<void*>(g_maps[m], 0x14) = g_cellVectors[m]; At<void*>(g_maps[m], 0x18) = g_cellVectors[m] + LAYERS; At<void*>(g_maps[m], 0x1c) = g_cellVectors[m] + LAYERS;
        g_areaMaps[m] = g_maps[m];
        long tlx = RndRange(0, EXT0 - 1), tly = RndRange(0, EXT0 - 1);
        g_localBox[m][0] = tlx; g_localBox[m][1] = tly;
        g_localBox[m][2] = (Rnd() % 9 == 0) ? tlx : RndRange(tlx + 1, EXT0);                      // sometimes an empty box
        g_localBox[m][3] = (Rnd() % 9 == 0) ? tly : RndRange(tly + 1, EXT0);
    }
    for (i = 0; i <= SEARCH_THINGS; i++) {
        int copies = (i == SEARCH_THINGS) ? RndRange(0, 2) : RndRange(0, 3);                       // a thing can be filed in several cells
        for (int c = 0; c < copies; c++) {
            m = RndRange(0, MAPS - 1); layer = RndRange(0, LAYERS - 1);
            long extent = layer ? EXT1 : EXT0;
            AddNode(m, layer, RndRange(0, extent * extent - 1), i);
        }
    }
    memset(g_tools, 0, sizeof(g_tools));
    g_actionAccepted = (int)(Rnd() % 4 != 0); g_opinionAttached = (int)(Rnd() % 3 != 0); g_modeActive = (int)(Rnd() % 2);
    g_blend = Rnd() * 2654435761ul;

    RunModel();
    static unsigned char creature_before[THING], def_before[sizeof(g_def)], things_before[SEARCH_THINGS][THING];
    memcpy(creature_before, g_creature, THING); memcpy(def_before, g_def, sizeof(g_def)); memcpy(things_before, g_things, sizeof(g_things));
    g_gotCount = 0; g_actionObject = 0;

    CallFrameUpdate(g_berserk);

    if (g_gotCount != g_wantCount) Fail("event count", g_gotCount, g_wantCount);
    int count = g_gotCount < g_wantCount ? g_gotCount : g_wantCount;
    for (i = 0; i < count; i++) {
        const Ev& got = g_got[i]; const Ev& want = g_want[i];
        if (got.kind != want.kind) { Fail("event kind", got.kind, want.kind); break; }
        if (memcmp(got.a, want.a, sizeof(got.a))) { Fail("event arguments", got.kind, i); break; }
        bool same = true;
        if (got.kind == 'X') { for (int k = 0; k < 6; k++) same = same && Near(got.f[k], want.f[k], 2e-4); }
        else if (got.kind == 'F' || got.kind == 'R') same = !memcmp(got.f, want.f, got.kind == 'F' ? 16 : 8) && (got.kind == 'F' || Near(got.f[2], want.f[2], 3e-7));
        else if (got.kind == 'B' || got.kind == 's') same = Near(got.f[0], want.f[0], 3e-7);
        if (!same) { Fail("event floats", got.kind, i); break; }
    }
    if (memcmp(g_berserk, g_wantBerserk, sizeof(g_berserk))) Fail("component state", At<long>(g_berserk, 0xc), At<long>(g_berserk, 0x20));
    if (memcmp(g_opinionSource, g_wantSource, sizeof(g_opinionSource))) Fail("opinion source", g_opinionSource[0x77], g_wantSource[0x77]);
    for (i = 0; i <= SEARCH_THINGS; i++) if (At<unsigned long>(g_mapwho[i], 0x4c) != g_wantSearchId[i]) Fail("search stamp", i, (long)g_wantSearchId[i]);
    if (CThingSearchTools::NoLookups != g_wantStatics[0] || CThingSearchTools::NoThingsScanned != g_wantStatics[1] || CThingSearchTools::SearchID != g_wantStatics[2])
        Fail("search statics", (long)CThingSearchTools::NoLookups, (long)CThingSearchTools::SearchID);
    if (memcmp(creature_before, g_creature, THING) || memcmp(def_before, g_def, sizeof(g_def)) || memcmp(things_before, g_things, sizeof(g_things)))
        Fail("a thing or the def changed", 0, 0);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    int i;
    for (i = 0; i < 128; i++) {                                      // a plausible inverse-root mantissa table
        double mantissa = 1.0 + ((double)(i & 63) + 0.5) / 64.0;
        double value = (i >> 6) ? mantissa : 2.0 * mantissa;         // odd exponent field: [1, 2); even: [2, 4)
        double f = 2.0 / sqrt(value) - 1.0;
        long entry = (long)(f * 256.0); if (entry > 255) entry = 255; if (entry < 0) entry = 0;
        GFastOneOverSqrtTable[i] = (unsigned long)entry;
    }
    for (i = 0; i < 80; i++) g_vtable[i] = 0;
    g_vtable[65] = (void*)StubOnHit;
    const long cases = 60000;
    for (g_case = 0; g_case < cases; g_case++) OneCase();
    printf("cases %ld: release %ld (refused %ld, effect killed %ld, source attached %ld, no level %ld, mode added %ld),\n"
           "  found %ld (self %ld, friend %ld, hit %ld), empty layer boxes %ld; ramp up %ld, ramp down %ld (morph %ld, clamped %ld, tint %ld),\n"
           "  timer held %ld, cleanup %ld\n",
           cases, g_statRelease, g_statRefused, g_statKilled, g_statAttached, g_statNoLevel, g_statModeAdded, g_statFound, g_statSelf,
           g_statFriend, g_statHit, g_statEmptyBox, g_statRampUp, g_statRampDown, g_statMorph, g_statClamped, g_statTint, g_statHeld, g_statCleanup);
    if (!g_statRelease || !g_statRefused || !g_statKilled || !g_statAttached || !g_statNoLevel || !g_statModeAdded || !g_statFound
        || !g_statSelf || !g_statFriend || !g_statHit || !g_statEmptyBox || !g_statRampUp || !g_statRampDown || !g_statMorph
        || !g_statClamped || !g_statTint || !g_statHeld || !g_statCleanup) { printf("a path was never taken\n"); g_fail++; }
    if (g_fail) { printf("0078d250_TEST FAIL (%d)\n", g_fail); return 1; }
    printf("0078d250_TEST PASS\n");
    return 0;
}
