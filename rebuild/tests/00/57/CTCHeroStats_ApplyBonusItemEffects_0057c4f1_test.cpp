// Behaviour test for 0x0057C4F1 CTCHeroStats::ApplyBonusItemEffects (links the REAL row object with what it carries: the
// IS_THING_ALIVE and GetTCDriver<CTCDParticleEmitter> copies and the interface-map lookups). The classes come from the shared
// headers only so that the out-of-line callees defined here get their retail names; every object is raw native-layout storage and
// every offset used is one the row reads or writes:
//   CTCHeroStats   ParentThing +4, Morality +0x28, Fatness +0x38, Stamina +0x58, MaxStamina +0x5C, Agreeableness +0x80,
//                  InherentAttractiveness +0x84, InherentScariness +0x88, ExtraMaxHealth +0x8C, ExtraMaxStamina +0x90,
//                  FramesLeftForSpeedMultiplier +0x94, FrameUpdatedClothingStats +0xB0, FrameUpdatedTattooStats +0xB4
//   CThingPhysical vtable slot 59 (+0xEC) ModifyHealth, interface bitset +0x20, interface map +0x44, PTCDriver +0x5C, dead bit
//                  +0x91 & 1, MaxHealth +0xB0, Health +0xB4
//   CBonusItemDef  +0x28..+0x70 as engine/CBonusItemDef.h; CGameDefinitionManager hero-stats def +0xDC (over-eating factor +0x7C);
//                  CWorld game time manager +0x1C
// Every callee records an event with its `this` check and arguments; the test compares the event list (floats to 1e-5 relative)
// and the whole 0x1DC-byte component with the independent model in RunModel().
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_APPLYBONUSITEMEFFECTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL_MAXHEALTH
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTHMODIFIERS
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_SETACTIONSPEEDMULTIPLIER_OUTOFLINE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#include <math.h>
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCHeroStats.h"
#include "engine/CBonusItemDef.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCHaste.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"
#include "engine/GFFloatToLongCeil.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
long CMainGameComponent::ConstantFPS = 15;
class CFpsAccess : public CMainGameComponent { public: static long& Ref() { return ConstantFPS; } };

// ---- events: one record per callee call ----
struct Event { char code; long a, b; float f; };
static Event g_ev[64], g_want[64]; static int g_nev, g_nwant;
static void Ev(char code, long a = 0, long b = 0, float f = 0.0f) { if (g_nev < 64) { Event e = { code, a, b, f }; g_ev[g_nev++] = e; } }
static void Want(char code, long a = 0, long b = 0, double f = 0.0) { Event e = { code, a, b, (float)f }; g_want[g_nwant++] = e; }

// ---- the scene ----
static unsigned char g_self[0x1dc], g_model[0x1dc], g_thing[0x100], g_particle[0x100], g_bonus[0x74];
static unsigned char g_gdm[0xe0], g_hsdef[0x100], g_world[0x80], g_tm[8], g_exp[8], g_sound[8], g_haste[8], g_ptcd[8], g_decoy[8];
static struct { long type; void* tc; } g_map[8];
static float g_pos[3];
static float g_max_extra_health; static long g_max_extra_stamina; static bool g_night, g_day; static int g_particle_kind;   // 0 none 1 dead 2 alive

struct StrData { const char* PData; unsigned long StringLength; unsigned long DataLength; long NoRefs; };
static StrData g_strpool[16]; static int g_strn;
static const char* Text(const CCharString& s) { return (*(StrData**)&s)->PData; }
CCharString::CCharString(const char* text, long length)
{
    StrData* d = &g_strpool[g_strn++ & 15]; d->PData = text; d->StringLength = (unsigned long)strlen(text); *(StrData**)this = d;
    Ev('s', length);
}
CCharString::~CCharString() { Ev('d'); }

// ---- out-of-line callees, under their retail names ----
namespace std { template<> bool bitset<274>::test(size_t n) const { Ev('b', (long)n, (void*)this == (void*)(g_thing + 0x20)); return ((At<unsigned long>((void*)this, 4 * (int)(n >> 5)) >> (n & 31)) & 1) != 0; } }
const CGameDefinitionManager& CTCBase::GetDefinitionManager() const { Ev('G', (void*)this == (void*)g_self); return *(const CGameDefinitionManager*)(void*)g_gdm; }
CWorld& CTCBase::GetWorld() const { Ev('w', (void*)this == (void*)g_self); return *(CWorld*)(void*)g_world; }
void CTCHeroStats::SetFatness(float fatness) { Ev('F', (void*)this == (void*)g_self, 0, fatness); }
static void __fastcall Thing_ModifyHealth(void* self, int, float amount, bool flag) { Ev('H', self == (void*)g_thing, flag, amount); }
static void* g_thing_vtbl[64];
void CThingPhysical::ModifyHealthOverTime(float amount, long frames) { Ev('O', (void*)this == (void*)g_thing, frames, amount); }
void CThingPhysical::SetMaxHealth(float max_health, bool flag) { Ev('X', (void*)this == (void*)g_thing, flag, max_health); }
void CTCHeroStats::ModifyStaminaBy(long amount) { Ev('S', (void*)this == (void*)g_self, amount); }
void CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory category, long amount) { Ev('E', (long)category + 10 * ((void*)this == (void*)g_exp), amount); }
void CTCHeroStats::SetMorality(long morality) { Ev('M', (void*)this == (void*)g_self, morality); }
float CTCHeroStats::GetMaxExtraMaxHealth() const { Ev('h', (void*)this == (void*)g_self); return g_max_extra_health; }
long CTCHeroStats::GetMaxExtraMaxStamina() const { Ev('t', (void*)this == (void*)g_self); return g_max_extra_stamina; }
unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& name, bool a, bool b, bool c, long d)
{
    Ev('P', (void*)this == (void*)g_sound, (strcmp(Text(name), "MOONFISH") == 0 ? 1 : strcmp(Text(name), "GOLDEN_CARROT") == 0 ? 2 : 0) + 10 * a + 100 * b + 1000 * c + 10000 * (d == -1));
    return 7;
}
bool CGameTimeManager::IsNightTime() const { Ev('n', (void*)this == (void*)g_tm); return g_night; }
bool CGameTimeManager::IsDay() const { Ev('y', (void*)this == (void*)g_tm); return g_day; }
void CGameTimeManager::SetTimeOfDay(float time) { Ev('T', (void*)this == (void*)g_tm, 0, time); }
void CTCHaste::SetActionSpeedMultiplier(void* key, float multiplier) { Ev('A', (void*)this == (void*)g_haste, key == (void*)g_self, multiplier); }
const C3DVector& CThing::GetPos() const { Ev('p', (void*)this == (void*)g_thing); return *(const C3DVector*)(void*)g_pos; }
CThing* FABLE_FASTCALL CTCDParticleEmitter::Create(long effect, const C3DVector& pos, bool flag)
{
    Ev('C', effect, ((void*)&pos == (void*)g_pos) + 10 * flag); return g_particle_kind == 0 ? 0 : (CThing*)(void*)g_particle;
}
void CTCDParticleEmitter::AttachToThing(CThing& thing, long type, const CCharString& name, long a, float b)
{
    Ev('K', ((void*)this == (void*)g_ptcd) + 10 * ((void*)this == 0) + 100 * ((void*)&thing == (void*)g_thing) + 1000 * (Text(name)[0] == 0), type + 100 * a, b);
}
void CTCHeroStats::Drink(float strength) { Ev('D', (void*)this == (void*)g_self, 0, strength); }

// ---- independent model (double arithmetic, as the x87 code at 53-bit precision) ----
static bool Has(const unsigned char* thing, int type) { return (At<unsigned long>((void*)thing, 0x20 + 4 * (type >> 5)) >> (type & 31)) & 1; }
static long CeilModel(float v)
{
    double r = floor((double)v + 0.5 + 0.5);
    if (r - ((double)v + 0.5) == 0.5 && fmod(r, 2.0) != 0.0) r -= 1.0;     // round to nearest, ties to even
    if ((float)(r - 1.0) == v) r -= 1.0;
    return (long)r;
}
static void RunModel()
{
    unsigned char* m = g_model; memcpy(m, g_self, sizeof g_model); g_nwant = 0;
    const unsigned char* b = g_bonus; double D = DELTA_NOTIONAL_ZERO;
    float hm = At<float>((void*)b, 0x28), secs = At<float>((void*)b, 0x34);
    double maxh = At<float>(g_thing, 0xb0), h = At<float>(g_thing, 0xb4);
    if (b[0x2c]) {
        double extra = (double)hm - (maxh - h);
        if (extra > D) { Want('G', 1); Want('F', 1, 0, (double)(float)extra * At<float>(g_hsdef, 0x7c) + At<float>(m, 0x38)); }
    }
    if (secs < DELTA_NOTIONAL_ZERO) Want('H', 1, 1, hm);
    else if (fabs((double)hm) > D) Want('O', 1, (long)((double)CFpsAccess::Ref() * secs), hm);
    Want('S', 1, At<long>((void*)b, 0x30));
    long str = At<long>((void*)b, 0x40), skill = At<long>((void*)b, 0x3c), will = At<long>((void*)b, 0x38);
    if (str > 0 || skill > 0 || will > 0) {
        Want('b', 0x68, 1);
        if (Has(g_thing, 0x68)) { Want('E', 10, str); Want('E', 11, skill); Want('E', 12, will); }
    }
    Want('M', 1, At<long>(m, 0x28) + At<long>((void*)b, 0x44));
    At<float>(m, 0x80) = (float)((double)At<float>(m, 0x80) + At<float>((void*)b, 0x48));
    At<float>(m, 0x84) = (float)((double)At<float>(m, 0x84) + At<float>((void*)b, 0x4c));
    At<float>(m, 0x88) = (float)((double)At<float>(m, 0x88) + At<float>((void*)b, 0x50));
    float mhi = At<float>((void*)b, 0x54);
    if (fabs((double)mhi) > D) {
        float capped = mhi; Want('h', 1); float maxextra = g_max_extra_health;
        if ((double)capped + At<float>(m, 0x8c) > (double)maxextra) capped = (float)((double)maxextra - At<float>(m, 0x8c));
        if (capped > 0.0f) {
            At<float>(m, 0x8c) = (float)((double)capped + At<float>(m, 0x8c));
            Want('X', 1, 0, (double)capped + maxh); Want('H', 1, 0, capped);
        }
    }
    long msi = At<long>((void*)b, 0x58);
    if (labs(msi) > 0) {
        long capped = msi; Want('t', 1); long maxextra = g_max_extra_stamina;
        if (capped + At<long>(m, 0x90) > maxextra) capped = maxextra - At<long>(m, 0x90);
        if (capped > 0) { At<long>(m, 0x90) += capped; At<long>(m, 0x5c) += capped; At<long>(m, 0x58) += capped; }
    }
    int which = b[0x5d] ? 1 : b[0x5c] ? 2 : 0;
    if (which) {
        Want('b', 0x10, 1);
        if (Has(g_thing, 0x10)) { Want('s', -1); Want('P', 1, which + 10 + 10000); Want('d'); }
        Want('w', 1);
        if (which == 1) { Want('n', 1); if (!g_night) Want('T', 1, 0, 0.875f); }
        else { Want('y', 1); if (!g_day) Want('T', 1, 0, 10.0f / 24.0f); }
    }
    float speed = At<float>((void*)b, 0x60), secs2 = At<float>((void*)b, 0x64);
    if (fabs(1.0 - (double)speed) > D && speed > DELTA_NOTIONAL_ZERO && secs2 > DELTA_NOTIONAL_ZERO) {
        Want('b', 0x8c, 1);
        if (Has(g_thing, 0x8c)) { Want('A', 1, 1, speed); At<long>(m, 0x94) = CeilModel((float)((double)CFpsAccess::Ref() * secs2)); }
    }
    At<long>(m, 0xb0) = 0; At<long>(m, 0xb4) = 0;
    long effect = At<long>((void*)b, 0x68);
    if (effect > 0) {
        Want('p', 1); Want('C', effect, 1);
        if (g_particle_kind == 2) {
            bool driver = (At<unsigned long>(g_particle, 0x28) & 0x800) != 0;
            Want('b', 75, 0);                                              // GetTCDriver's interface test, on the particle thing
            Want('s', -1); Want('K', (driver ? 1 : 10) + 100 + 1000, 8, 0.0f); Want('d');
        }
    }
    if (b[0x6c]) Want('D', 1, 0, At<float>((void*)b, 0x70));
}

static int failures, g_case, g_cov[10];
static void Fail(const char* what) { if (++failures <= 12) printf("FAIL case %d: %s\n", g_case, what); }
static bool Near(float a, float b) { return fabs((double)a - (double)b) <= 1e-5 * (fabs((double)b) + 1e-3); }

int main()
{
    int i;
    SetUnhandledExceptionFilter((void*)OnFault);
    for (i = 0; i < 64; ++i) g_thing_vtbl[i] = 0;
    g_thing_vtbl[59] = (void*)Thing_ModifyHealth;                           // slot +0xEC
    g_seed = 0x57C4F1u;
    for (g_case = 0; g_case < 40000; ++g_case) {
        for (i = 0; i < (int)sizeof g_self; ++i) g_self[i] = (unsigned char)Rnd();
        memset(g_thing, 0, sizeof g_thing); memset(g_particle, 0, sizeof g_particle); memset(g_bonus, 0, sizeof g_bonus);
        At<void*>(g_self, 4) = g_thing; At<void*>(g_thing, 0) = g_thing_vtbl;
        At<float>(g_self, 0x38) = RndF(0.0f, 1.0f); At<long>(g_self, 0x28) = RndN(2000) - 1000;
        At<float>(g_self, 0x80) = RndF(-1.0f, 1.0f); At<float>(g_self, 0x84) = RndF(-1.0f, 1.0f); At<float>(g_self, 0x88) = RndF(-1.0f, 1.0f);
        At<float>(g_self, 0x8c) = RndF(0.0f, 50.0f); At<long>(g_self, 0x90) = RndN(40); At<long>(g_self, 0x5c) = 100 + RndN(100); At<long>(g_self, 0x58) = RndN(100);
        At<float>(g_thing, 0xb0) = RndF(100.0f, 300.0f); At<float>(g_thing, 0xb4) = At<float>(g_thing, 0xb0) - (RndN(3) ? RndF(0.0f, 40.0f) : 0.0f);
        // the hero's components: experience 0x68, sound player 0x10, haste 0x8C, each maybe absent, with decoys
        int n = 0;
        if (RndN(2)) { g_map[n].type = 0x05; g_map[n++].tc = g_decoy; At<unsigned long>(g_thing, 0x20) |= 1UL << 5; }
        if (RndN(4)) { g_map[n].type = 0x10; g_map[n++].tc = g_sound; At<unsigned long>(g_thing, 0x20) |= 1UL << 16; }
        if (RndN(2)) { g_map[n].type = 0x11; g_map[n++].tc = g_decoy; At<unsigned long>(g_thing, 0x20) |= 1UL << 17; }
        if (RndN(4)) { g_map[n].type = 0x68; g_map[n++].tc = g_exp; At<unsigned long>(g_thing, 0x2c) |= 1UL << 8; }
        if (RndN(2)) { g_map[n].type = 0x69; g_map[n++].tc = g_decoy; At<unsigned long>(g_thing, 0x2c) |= 1UL << 9; }
        if (RndN(4)) { g_map[n].type = 0x8c; g_map[n++].tc = g_haste; At<unsigned long>(g_thing, 0x30) |= 1UL << 12; }
        if (RndN(2)) { g_map[n].type = 0x8d; g_map[n++].tc = g_decoy; At<unsigned long>(g_thing, 0x30) |= 1UL << 13; }
        At<void*>(g_thing, 0x44) = g_map; At<void*>(g_thing, 0x48) = g_map + n; At<void*>(g_thing, 0x4c) = g_map + 8;
        At<void*>(g_gdm, 0xdc) = g_hsdef; At<float>(g_hsdef, 0x7c) = RndF(0.001f, 0.1f); At<void*>(g_world, 0x1c) = g_tm;
        // the bonus item
        At<float>(g_bonus, 0x28) = RndN(5) == 0 ? 0.0f : RndF(-20.0f, 80.0f); g_bonus[0x2c] = RndN(2) != 0;
        At<long>(g_bonus, 0x30) = RndN(60) - 10; At<float>(g_bonus, 0x34) = RndN(2) ? 0.0f : RndF(0.1f, 9.0f);
        At<long>(g_bonus, 0x38) = RndN(3) ? 0 : RndN(50) - 5; At<long>(g_bonus, 0x3c) = RndN(3) ? 0 : RndN(50) - 5; At<long>(g_bonus, 0x40) = RndN(3) ? 0 : RndN(50) - 5;
        At<long>(g_bonus, 0x44) = RndN(40) - 20;
        At<float>(g_bonus, 0x48) = RndF(-0.2f, 0.2f); At<float>(g_bonus, 0x4c) = RndF(-0.2f, 0.2f); At<float>(g_bonus, 0x50) = RndF(-0.2f, 0.2f);
        At<float>(g_bonus, 0x54) = RndN(2) ? 0.0f : RndF(-5.0f, 30.0f); At<long>(g_bonus, 0x58) = RndN(2) ? 0 : RndN(40) - 8;
        g_bonus[0x5c] = RndN(3) == 0; g_bonus[0x5d] = RndN(3) == 0;
        At<float>(g_bonus, 0x60) = RndN(3) == 0 ? 1.0f : RndN(8) == 0 ? 0.0f : RndF(0.2f, 2.5f);
        At<float>(g_bonus, 0x64) = RndN(4) == 0 ? 0.0f : RndN(4) == 0 ? (float)(1 + RndN(20)) : RndF(0.1f, 12.0f);
        At<long>(g_bonus, 0x68) = RndN(3) ? RndN(300) - 20 : 0; g_bonus[0x6c] = RndN(2) != 0; At<float>(g_bonus, 0x70) = RndF(0.1f, 3.0f);
        g_max_extra_health = RndF(0.0f, 80.0f); g_max_extra_stamina = RndN(60); g_night = RndN(2) != 0; g_day = RndN(2) != 0;
        g_particle_kind = RndN(5) == 0 ? RndN(2) : 2; g_particle[0x91] = g_particle_kind == 1;
        if (RndN(5)) { At<unsigned long>(g_particle, 0x28) |= 0x800; }       // the particle driver interface (75)
        At<void*>(g_particle, 0x5c) = g_ptcd;
        CFpsAccess::Ref() = RndN(2) ? 15 : 30;

        RunModel();
        g_nev = 0;
        ((CTCHeroStats*)(void*)g_self)->ApplyBonusItemEffects(*(const CBonusItemDef*)(void*)g_bonus);

        bool same = g_nev == g_nwant;
        for (i = 0; same && i < g_nev; ++i)
            same = g_ev[i].code == g_want[i].code && g_ev[i].a == g_want[i].a && g_ev[i].b == g_want[i].b && Near(g_ev[i].f, g_want[i].f);
        if (!same) {
            Fail("call sequence / arguments");
            if (failures <= 6) {
                printf("  got :"); for (i = 0; i < g_nev; ++i) printf(" %c(%ld,%ld,%g)", g_ev[i].code, g_ev[i].a, g_ev[i].b, g_ev[i].f); printf("\n");
                printf("  want:"); for (i = 0; i < g_nwant; ++i) printf(" %c(%ld,%ld,%g)", g_want[i].code, g_want[i].a, g_want[i].b, g_want[i].f); printf("\n");
            }
        }
        static const int floats[4] = { 0x80, 0x84, 0x88, 0x8c };
        for (i = 0; i < 4; ++i) { if (!Near(At<float>(g_self, floats[i]), At<float>(g_model, floats[i]))) Fail("appearance / extra health"); At<float>(g_self, floats[i]) = At<float>(g_model, floats[i]); }
        if (memcmp(g_self, g_model, sizeof g_self) != 0) {
            Fail("object state");
            if (failures <= 6) for (i = 0; i < (int)sizeof g_self; ++i) if (g_self[i] != g_model[i]) printf("  +%03X got %02X want %02X\n", i, g_self[i], g_model[i]);
        }
        for (i = 0; i < g_nwant; ++i) {
            char c = g_want[i].code;
            ++g_cov[c == 'F' ? 0 : c == 'O' ? 1 : c == 'E' ? 2 : c == 'X' ? 3 : c == 'T' ? 4 : c == 'A' ? 5 : c == 'K' ? 6 : c == 'D' ? 7 : c == 'P' ? 8 : 9];
        }
    }
    printf("fatness %d, over-time %d, experience %d, max health %d, set time %d, haste %d, attach %d, drink %d, sounds %d\n",
           g_cov[0], g_cov[1], g_cov[2], g_cov[3], g_cov[4], g_cov[5], g_cov[6], g_cov[7], g_cov[8]);
    for (i = 0; i < 9; ++i) if (g_cov[i] < 1000) Fail("coverage");
    if (failures == 0) printf("0057c4f1_TEST PASS\n"); else printf("0057c4f1_TEST FAIL (%d)\n", failures);
    return failures != 0;
}