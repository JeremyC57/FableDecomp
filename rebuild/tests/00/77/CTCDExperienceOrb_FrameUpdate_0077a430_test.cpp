// Behaviour test for 0x0077A430 CTCDExperienceOrb::FrameUpdate (links the REAL row object with what it carries: the
// GetAllTCDsInRoughArea<CTCDExperienceOrb, ..> instance 0x0077ADF0, the vector overflow path and the interface-map lookups).
// The classes come from the shared headers only so that the out-of-line callees defined here get their retail names; every
// object is raw native-layout storage and every offset used is one the row or the search instance reads:
//   CTCDExperienceOrb  ParentThing +4, CombinationImmunityTimer +0x14, LifeTimer +0x18, DecayOverTime +0x1C
//   CThing             interface bitset +0x20 (fade component 0xF9 = word +0x3C bit 25), interface map +0x44 / +0x48,
//                      PTCDriver +0x5C (vtable slot 28 IsDriverType), PhysicsTC +0x60, dead bit +0x91 & 1
//   NThingMap::CThingMap  level-info vector +4 (20-byte records, row width +4), cell-vector vector +0x14; cell = first node;
//                      node: PMapwho +0, PNext +4; CTCMapwho ParentThing +4, SearchID +0x4C
//   CThingManager      NonScriptedEntitesPauseMode +0x54, AllEntitesPauseMode +0x55; CPlayer PlayerInterface +0x0C;
//                      CTCFadeOutAndIn FadingOut +0x0C, FadingIn +0x0D
// A thing's position lives where this test's GetPos() keeps it. Each case scatters up to five neighbours and the hero over a
// 64 x 64 cell map, records every callee call with its arguments and compares the call sequence, the two timers and the
// acceleration vectors (to 1e-4 relative: the row keeps part of the sums in extended registers) with the model in RunModel().
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_TCDRIVER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCDSINROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE
#define FABLE_RETAIL_CTCD_STATGETDRIVERTYPE
#define FABLE_RETAIL_CTCDEXPERIENCEORB_STATGETDRIVERTYPE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_PEEKPLAYERINTERFACE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER_JOYSTICKDEVICE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CTCDEXPERIENCEORB_FRAMEUPDATE
#define FABLE_RETAIL_CTCFADEOUTANDIN
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CTCDriverBase.h"
#include "engine/CTCDExperienceOrb.h"
#include "engine/CTCFadeOutAndIn.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/ThingAlive.h"
#include "engine/GFFloatToLongCeil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// A fault ends the run at once instead of waiting for a debugger.
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall OnFault(void*) { ExitProcess(3); return 0; }

template<class T> static T& At(void* p, int off) { return *(T*)((char*)p + off); }
static unsigned g_seed;
static unsigned Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static int RndN(int n) { return (int)(Rnd() % (unsigned)n); }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() & 0xFFFF) / 65535.0f; }

// ---- globals the row object reads ----
extern const float DELTA_NOTIONAL_ZERO = 0.0001f;                          // 0x0129BA3C (retail value)
unsigned long GFastOneOverSqrtTable[128];                                  // 0x013CE558 (filled in main: 8-bit mantissas of 1/sqrt)
long NThingMap::MaxLayers = 1;                                             // 0x013BA848
extern const NThingMap::CLayerInfo NThingMap::LayerInfo[1] = { { 1, 0 } }; // 0x0137DAD8: one layer, no scale shift
unsigned long CThingSearchTools::NoLookups, CThingSearchTools::NoThingsScanned, CThingSearchTools::SearchID;

// ---- call log: a letter per call, the float arguments kept aside ----
static char g_log[512]; static int g_logn;
static void Log(char c) { if (g_logn < 500) g_log[g_logn++] = c; g_log[g_logn] = 0; }
static float g_acc[2][3]; static int g_nacc;

// ---- the scene ----
enum { NTHINGS = 7, GRID = 64, POS = 0xC0 };                               // POS: where this test keeps a thing's position
static unsigned char g_self[0x20];
static unsigned char g_thing[NTHINGS][0x100];                              // [0] the parent orb, [1..5] neighbours, [6] the hero
static unsigned char g_driver[NTHINGS][8], g_mapwho[NTHINGS][0x54];
static struct Node { void* PMapwho; Node* PNext; Node* PPrevious; void* PCell; } g_node[NTHINGS];
static Node* g_cells[GRID * GRID];
static struct { void* start; void* finish; void* eos; } g_cellvec;
static struct { long Size, ExtX, ExtY, ScaleShift, NoThings; } g_level;
static unsigned char g_map[0x40], g_tools[0x100], g_tm[0x100], g_pm[8], g_player[0x20], g_iface[8], g_phys[8], g_fade[0x20], g_newfade[8];
static struct { long type; void* tc; } g_parent_map[4];
static bool g_is_orb[NTHINGS], g_event, g_hero_null;
static long g_device;
static int g_nthings;                                                      // neighbours in use (1..n)

// fake vtable for the drivers: slot 28 (+0x70) is IsDriverType(long) const
static bool __fastcall Drv_IsDriverType(void* self, int, long type)
{
    int i = (int)(((unsigned char(*)[8])self) - g_driver);
    if (type != 26 || i < 0 || i >= NTHINGS) { Log('X'); return false; }
    return g_is_orb[i];
}
static void* g_drv_vtbl[32];

// ---- out-of-line callees, under their retail names ----
const C3DVector& CThing::GetPos() const { return *(const C3DVector*)((const char*)this + POS); }
const CThingSearchTools& CTCBase::PeekThingSearchTools() const { return *(const CThingSearchTools*)(void*)g_tools; }
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI&, std::vector<NThingMap::CThingMap*>& maps) const
{
    Log((void*)this == (void*)g_tools ? 'a' : 'X'); maps.push_back((NThingMap::CThingMap*)(void*)g_map);
}
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const
{
    Log((void*)&map == (void*)g_map ? 'c' : 'X'); return area;
}
bool CThingFilter_IsNot::operator()(const CThing& thing) const { return &thing != &Thing; }
bool CThingFilter_In2DRange::operator()(const CThing& thing) const
{
    const C3DVector& p = thing.GetPos();
    float dx = p.X - Pos.X, dy = p.Y - Pos.Y;
    return dx * dx + dy * dy < Range * Range;
}
void CTCDExperienceOrb::CombineWithExperienceOrb(CThing& other)
{
    int i = (int)(((unsigned char(*)[0x100])(void*)&other) - g_thing);
    Log((void*)this == (void*)g_self && i >= 1 && i <= 5 ? (char)('0' + i) : 'X');
}
void CTCPhysicsBase::ApplyAcceleration(const C3DVector& acceleration)
{
    Log((void*)this == (void*)g_phys && g_nacc < 2 ? 'P' : 'X');
    if (g_nacc < 2) { g_acc[g_nacc][0] = acceleration.X; g_acc[g_nacc][1] = acceleration.Y; g_acc[g_nacc][2] = acceleration.Z; ++g_nacc; }
}
CThingManager& CTCBase::GetThingManager() { Log((void*)this == (void*)g_self ? 'm' : 'X'); return *(CThingManager*)(void*)g_tm; }
CPlayerManager& CTCBase::GetPlayerManager() { Log((void*)this == (void*)g_self ? 'g' : 'X'); return *(CPlayerManager*)(void*)g_pm; }
const CPlayerManager& CTCBase::PeekPlayerManager() const { Log((void*)this == (void*)g_self ? 'k' : 'X'); return *(const CPlayerManager*)(void*)g_pm; }
CPlayer& CPlayerManager::GetMainPlayer() { Log((void*)this == (void*)g_pm ? 'n' : 'X'); return *(CPlayer*)(void*)g_player; }
const CPlayer& CPlayerManager::GetMainPlayer() const { Log((void*)this == (void*)g_pm ? 'o' : 'X'); return *(const CPlayer*)(void*)g_player; }
long CPlayerManager::GetMainPlayerJoystickDeviceNumber() const { Log((void*)this == (void*)g_pm ? 'j' : 'X'); return g_device; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const
{
    Log((void*)this == (void*)g_player ? 'h' : 'X'); return g_hero_null ? 0 : (CThingPlayerCreature*)(void*)g_thing[6];
}
bool CGamePlayerInterface::IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long device) const
{
    Log((void*)this == (void*)g_iface && (long)action == 0x1a && (long)type == 2 && device == g_device ? 'q' : 'X'); return g_event;
}
float C3DVector::GetMagnitudeAndNormalise()
{
    float m = (float)sqrt((double)X * X + (double)Y * Y + (double)Z * Z);
    X /= m; Y /= m; Z /= m; Log('N'); return m;
}
void CThing::Kill(bool now) { Log((void*)this == (void*)g_thing[0] && now ? 'K' : 'X'); }
CCharString::CCharString(const char* text, long length) { Log(strcmp(text, "CTCFadeOutAndIn") == 0 && length == -1 ? 's' : 'X'); *(const char**)this = text; }
CCharString::~CCharString() { Log('d'); }
CTCBase* CThing::AddTC(const CCharString& name, bool flag, const CTCParamsBase* params)
{
    Log((void*)this == (void*)g_thing[0] && strcmp(*(const char**)&name, "CTCFadeOutAndIn") == 0 && !flag && params == 0 ? 'A' : 'X');
    return (CTCBase*)(void*)g_newfade;
}
void CTCFadeOutAndIn::FadeOut(float time, bool flag) { Log((void*)this == (void*)g_newfade && time == 1.5f && flag ? 'F' : 'X'); }

// ---- independent model ----
struct Model { char log[512]; int n; float acc[2][3]; int nacc; long timer, life;
    void L(char c) { log[n++] = c; log[n] = 0; }
    void Acc(double x, double y, double z) { L('P'); acc[nacc][0] = (float)x; acc[nacc][1] = (float)y; acc[nacc][2] = (float)z; ++nacc; } };
static const float* Pos3(int i) { return (const float*)(g_thing[i] + POS); }
static double FastOneOverSqrt(float v)
{
    unsigned long bits; memcpy(&bits, &v, 4);
    unsigned long est = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float y; memcpy(&y, &est, 4);
    return (3.0 - (double)y * y * v) * y * 0.5;
}
static void RunModel(Model& m)
{
    m.n = 0; m.log[0] = 0; m.nacc = 0;
    m.timer = At<long>(g_self, 0x14); m.life = At<long>(g_self, 0x18);
    const float* p = Pos3(0);
    // the search: cells of the box (floor(x) +- 5, floor(y) +- 5), right and bottom edge excluded, row by row; each cell's
    // list in order; orb drivers only, not the parent, within 5 units in 2D
    int found[NTHINGS], nf = 0; long cx = (long)p[0], cy = (long)p[1];
    m.L('a'); m.L('c');
    for (long y = cy - 5; y < cy + 5; ++y)
        for (long x = cx - 5; x < cx + 5; ++x)
            for (Node* nd = g_cells[y * GRID + x]; nd; nd = nd->PNext) {
                int i = (int)(nd - g_node);
                if (!g_is_orb[i] || i == 0) continue;
                const float* q = Pos3(i); float dx = q[0] - p[0], dy = q[1] - p[1];
                if (dx * dx + dy * dy < 5.0f * 5.0f) found[nf++] = i;
            }
    if (nf) {
        float avg[3] = { 0.0f, 0.0f, 0.0f }; bool combined = false;
        for (int c = 0; c < nf; ++c) {
            const float* q = Pos3(found[c]);
            if (m.timer <= 0) {
                double dx = (double)p[0] - q[0], dy = (double)p[1] - q[1], dz = (double)p[2] - q[2];
                if (dx * dx + dy * dy + dz * dz <= (double)0.0016f) { m.L((char)('0' + found[c])); combined = true; break; }
            }
            avg[0] += q[0]; avg[1] += q[1]; avg[2] += q[2];
        }
        if (!combined) {
            double r = 1.0 / (double)nf;
            avg[0] = (float)(avg[0] * r); avg[1] = (float)(avg[1] * r); avg[2] = (float)(avg[2] * r);
            double fx = (double)avg[0] - p[0], fy = (double)avg[1] - p[1], fz = (double)avg[2] - p[2];
            double sq = fx * fx + fy * fy + fz * fz;
            if (sq > (double)DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                double k = FastOneOverSqrt((float)sq) * (double)0.006f;
                m.Acc(fx * k, fy * k, fz * k);
            }
        }
    }
    m.timer--;
    m.L('m'); if (g_tm[0x54]) return;
    m.L('m'); if (g_tm[0x55]) return;
    m.L('g'); m.L('n'); m.L('h');
    if (!g_hero_null && !(g_thing[6][0x91] & 1)) {
        m.L('k'); m.L('o'); m.L('g'); m.L('j'); m.L('q');
        if (g_event) {
            const float* h = Pos3(6);
            float fx = h[0] - p[0], fy = h[1] - p[1], fz = h[2] - p[2];
            float mag = (float)sqrt((double)fx * fx + (double)fy * fy + (double)fz * fz);
            m.L('N');
            double a = (double)0.7f / mag;
            m.Acc((double)(float)(fx / mag) * a, (double)(float)(fy / mag) * a, (double)(float)(fz / mag) * a);
        }
    }
    if (g_self[0x1c]) {
        if (--m.life < 0) {
            if (At<unsigned long>(g_thing[0], 0x3c) & 0x2000000) { if (!g_fade[0xd] && !g_fade[0xc]) m.L('K'); }
            else { m.L('s'); m.L('A'); m.L('d'); m.L('F'); }
        }
    }
}

static int failures, g_case, g_cov[8];
static void Fail(const char* what) { if (++failures <= 12) printf("FAIL case %d: %s\n", g_case, what); }
static void Place(int i, float x, float y, float z)
{
    float* q = (float*)(g_thing[i] + POS); q[0] = x; q[1] = y; q[2] = z;
    Node** cell = &g_cells[(long)y * GRID + (long)x];
    g_node[i].PMapwho = g_mapwho[i]; g_node[i].PPrevious = 0; g_node[i].PCell = cell;
    if (RndN(2) || !*cell) { g_node[i].PNext = *cell; *cell = &g_node[i]; }           // at the head, or
    else { Node* t = *cell; while (t->PNext) t = t->PNext; t->PNext = &g_node[i]; g_node[i].PNext = 0; }   // at the tail
}
static void BuildCase()
{
    int i;
    memset(g_cells, 0, sizeof g_cells); memset(g_thing, 0, sizeof g_thing); memset(g_tm, 0, sizeof g_tm); memset(g_fade, 0, sizeof g_fade);
    for (i = 0; i < 0x20; ++i) g_self[i] = (unsigned char)Rnd();
    At<void*>(g_self, 4) = g_thing[0];
    At<long>(g_self, 0x14) = RndN(3) == 0 ? 1 + RndN(3) : -RndN(3);        // CombinationImmunityTimer
    At<long>(g_self, 0x18) = RndN(4) - 1;                                  // LifeTimer
    g_self[0x1c] = RndN(3) != 0;                                           // DecayOverTime
    for (i = 0; i < NTHINGS; ++i) {
        At<void*>(g_thing[i], 0x5c) = RndN(9) == 0 && i != 0 ? (void*)0 : (void*)g_driver[i];   // PTCDriver (sometimes none)
        At<void*>(g_driver[i], 0) = g_drv_vtbl;
        At<void*>(g_mapwho[i], 4) = g_thing[i]; At<unsigned long>(g_mapwho[i], 0x4c) = 0;      // ParentThing, SearchID
        g_is_orb[i] = i == 0 || RndN(5) != 0;
        if (!At<void*>(g_thing[i], 0x5c)) g_is_orb[i] = false;
    }
    g_is_orb[6] = false;
    CThingSearchTools::SearchID = 1 + RndN(100);
    At<void*>(g_thing[0], 0x60) = g_phys;
    float px = RndF(10.0f, 54.0f), py = RndF(10.0f, 54.0f), pz = RndF(-2.0f, 2.0f);
    Place(0, px, py, pz);
    g_nthings = RndN(6);
    bool vertical = RndN(10) == 0;                                         // every neighbour straight above the parent
    for (i = 1; i <= g_nthings; ++i) {
        int kind = RndN(8);
        if (vertical) { Place(i, px, py, pz + RndF(0.2f, 2.0f)); continue; }
        if (kind == 0) Place(i, px + RndF(-0.035f, 0.035f), py + RndF(-0.035f, 0.035f), pz + RndF(-0.035f, 0.035f));   // touching or just outside: the offsets straddle the 0.04 touch radius (audit: the 0.016f bound mutant survived the tighter scene)
        else if (kind == 1) Place(i, px, py, pz);                                                            // coincident
        else if (kind == 2) Place(i, px + RndF(-7.5f, 7.5f), py + RndF(-7.5f, 7.5f), pz + RndF(-3.0f, 3.0f));    // round the edge
        else Place(i, px + RndF(-3.0f, 3.0f), py + RndF(-3.0f, 3.0f), pz + RndF(-1.0f, 1.0f));
    }
    Place(6, px + RndF(-6.0f, 6.0f), py + RndF(-6.0f, 6.0f), pz + RndF(0.5f, 3.0f));                             // the hero
    g_thing[6][0x91] = RndN(6) == 0;                                       // hero dead
    g_hero_null = RndN(8) == 0; g_event = RndN(2) != 0; g_device = RndN(4);
    g_tm[0x54] = RndN(8) == 0; g_tm[0x55] = RndN(8) == 0;
    At<void*>(g_player, 0xc) = g_iface;
    // the parent's interface map: the fade component (0xF9) between two decoys, or absent
    int n = 0; bool fade = RndN(2) != 0;
    At<unsigned long>(g_thing[0], 0x20) |= 1UL << 5; g_parent_map[n].type = 5; g_parent_map[n++].tc = g_newfade;
    if (fade) { At<unsigned long>(g_thing[0], 0x3c) |= 0x2000000UL; g_parent_map[n].type = 0xf9; g_parent_map[n++].tc = g_fade; }
    At<unsigned long>(g_thing[0], 0x3c) |= 0x4000000UL; g_parent_map[n].type = 0xfa; g_parent_map[n++].tc = g_newfade;
    At<void*>(g_thing[0], 0x44) = g_parent_map; At<void*>(g_thing[0], 0x48) = g_parent_map + n; At<void*>(g_thing[0], 0x4c) = g_parent_map + 4;
    g_fade[0xc] = RndN(4) == 0; g_fade[0xd] = RndN(4) == 0;
}
static bool Near(float a, float b) { return fabs((double)a - (double)b) <= 1e-4 * (fabs((double)b) + 1e-4); }

int main()
{
    static Model m; int i;
    SetUnhandledExceptionFilter((void*)OnFault);
    for (i = 0; i < 128; ++i) {
        double mant = (1.0 + ((i & 63) + 0.5) / 64.0) * ((i >> 6) ? 1.0 : 2.0);
        long t = (long)((2.0 / sqrt(mant) - 1.0) * 256.0 + 0.5);
        GFastOneOverSqrtTable[i] = (unsigned long)(t > 255 ? 255 : t);
    }
    for (i = 0; i < 32; ++i) g_drv_vtbl[i] = 0;
    g_drv_vtbl[28] = (void*)Drv_IsDriverType;
    // one thing map: level info vector +4, cell-vector vector +0x14 (one layer of GRID x GRID cells)
    g_level.Size = 1; g_level.ExtX = GRID; g_level.ExtY = GRID; g_level.ScaleShift = 0;
    g_cellvec.start = g_cells; g_cellvec.finish = g_cells + GRID * GRID; g_cellvec.eos = g_cells + GRID * GRID;
    At<void*>(g_map, 4) = &g_level; At<void*>(g_map, 8) = &g_level + 1; At<void*>(g_map, 0xc) = &g_level + 1;
    At<void*>(g_map, 0x14) = &g_cellvec; At<void*>(g_map, 0x18) = &g_cellvec + 1; At<void*>(g_map, 0x1c) = &g_cellvec + 1;
    g_seed = 0x77A430u;
    for (g_case = 0; g_case < 30000; ++g_case) {
        BuildCase();
        RunModel(m);
        g_logn = 0; g_log[0] = 0; g_nacc = 0;
        unsigned long lookups = CThingSearchTools::NoLookups;
        ((CTCDExperienceOrb*)(void*)g_self)->CTCDExperienceOrb::FrameUpdate();
        if (strcmp(g_log, m.log) != 0) { Fail("call sequence"); if (failures <= 12) printf("  got %s want %s\n", g_log, m.log); }
        if (g_nacc != m.nacc) Fail("acceleration count");
        else for (i = 0; i < g_nacc; ++i)
            if (!Near(g_acc[i][0], m.acc[i][0]) || !Near(g_acc[i][1], m.acc[i][1]) || !Near(g_acc[i][2], m.acc[i][2])) {
                Fail("acceleration"); if (failures <= 12) printf("  got %g %g %g want %g %g %g\n", g_acc[i][0], g_acc[i][1], g_acc[i][2], m.acc[i][0], m.acc[i][1], m.acc[i][2]);
            }
        if (At<long>(g_self, 0x14) != m.timer) Fail("CombinationImmunityTimer");
        if (At<long>(g_self, 0x18) != m.life) Fail("LifeTimer");
        if (CThingSearchTools::NoLookups != lookups + 1) Fail("one search");
        if (strpbrk(g_log, "12345")) ++g_cov[0];
        if (g_nacc && g_log[2] == 'P') ++g_cov[1];
        if (strchr(g_log, 'N')) ++g_cov[2];
        if (strchr(g_log, 'K')) ++g_cov[3];
        if (strchr(g_log, 'F')) ++g_cov[4];
    }
    printf("merge %d, flock %d, hero %d, kill %d, fade %d\n", g_cov[0], g_cov[1], g_cov[2], g_cov[3], g_cov[4]);
    if (g_cov[0] < 500 || g_cov[1] < 3000 || g_cov[2] < 3000 || g_cov[3] < 300 || g_cov[4] < 300) Fail("coverage");
    if (failures == 0) printf("0077a430_TEST PASS\n"); else printf("0077a430_TEST FAIL (%d)\n", failures);
    return failures != 0;
}