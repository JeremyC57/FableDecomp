// Behaviour test for 0x0078A690 CTCHeroRangedAttacks::FrameUpdate (links the REAL row object with what it carries: the pull-back
// helpers, NMaths::GetRootsNumerically, GetDamageMultiplierFromThingAtBuildUpTime, GetShotBuildupTime and the CVectorMap LowerBound
// copies). The classes come from the shared headers only so that the out-of-line callees defined here get their retail names;
// every object is raw native-layout storage and every offset used is one the row reads:
//   CTCHeroRangedAttacks  ParentThing +4, PHeroCombatDef +0x10, FireCycleInProgress +0x14, Aiming +0x15, FireQueued +0x16,
//                         Loading +0x17, AimingTimer +0x18, PullBackTimer +0x1C, PullBackAmount +0x20, Accuracy +0x24,
//                         DamageMultiplier +0x28, Decapitate +0x2C, ButtonReleaseCancelTimer +0x30, ButtonPressure +0x34,
//                         LastButtonPressure +0x38, BowDrawbackSoundID +0x48
//   CThing                interface bitset +0x20 (sound player 0x10, carrying 0x46, weapon 0x48, hero experience 0x68), interface
//                         map start +0x44 / finish +0x48 (8-byte pairs: type, component), dead bit +0x91 & 1
//   CHeroCombatDef        reference count +4, stat weight +0x78, multiplier floats +0x84 / +0x8C / +0x90 / +0x94 / +0x98
// Each case fills these at random, records every callee call (with its `this` and arguments) and compares the call sequence and
// the whole component with the independent model in RunModel().
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CTCHERORANGEDATTACKS_GETSHOTBUILDUPTIME_OUT_OF_LINE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_LOADING
#define FABLE_RETAIL_CTCWEAPON_AUTOLOADING
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSOUNDEVENT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CPlayer.h"
#include "engine/ThingAlive.h"
#include "engine/GFTanh.h"
#include "engine/C3DVector.h"
#include "engine/CTCHeroExperience.h"
#include <math.h>
#pragma intrinsic(fabs)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---- globals the row object reads ----
long CMainGameComponent::ConstantFPS = 15;                    // 0x01375550 (the test varies it)
extern const float DELTA_NOTIONAL_ZERO = 0.0001f;             // 0x0129BA3C (retail value)
// The static is protected: a never-instantiated derived class gives the test its address.
class CFpsAccess : public CMainGameComponent { public: static long& Ref() { return ConstantFPS; } };

// A fault (a mutant following a pointer the model never offers) ends the run at once instead of waiting for a debugger.
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall OnFault(void*) { ExitProcess(3); return 0; }

template<class T> static T& At(void* p, int off) { return *(T*)((char*)p + off); }

static unsigned g_seed;
static unsigned Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static int RndN(int n) { return (int)(Rnd() % (unsigned)n); }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() & 0xFFFF) / 65535.0f; }

// ---- call log ----
static char g_log[256]; static int g_logn;
static void Log(char c) { if (g_logn < 250) g_log[g_logn++] = c; g_log[g_logn] = 0; }

// ---- the scene of one case ----
static unsigned char g_self[0x4C];
static unsigned char g_parent[0x100], g_carried[0x100];
static unsigned char g_soundplayer[8], g_carrying[8], g_weapon[8], g_heroexp[8], g_player[8], g_decoy[8];
static unsigned char g_def[0xA0];
static struct { long type; void* tc; } g_parent_map[8], g_carried_map[8];
static int  g_able_seq[3], g_able_k;
static void* g_primary;                 // what GetThingInPrimarySlot returns
static bool g_loaded, g_auto, g_isproj;
static long g_mode;
static unsigned long g_sound_id;
static float g_stat_param;
static bool g_fire_sets_queued; static float g_fire_sets_pressure; static bool g_load_sets_loading;

// ---- out-of-line callees of the row, under their retail names ----
bool CTCHeroRangedAttacks::IsAbleToUseRangedWeapon() const { Log((void*)this == (void*)g_self ? 'Q' : 'X'); return g_able_seq[g_able_k++] != 0; }
void CTCHeroRangedAttacks::AttemptToFireRangedWeapon() { Log((void*)this == (void*)g_self ? 'A' : 'X'); }
void CTCHeroRangedAttacks::CancelAimingWeapon() { Log((void*)this == (void*)g_self ? 'C' : 'X'); }
// Logs the Loading flag it is entered with ('L' set, 'K' clear), then overwrites it.
void CTCHeroRangedAttacks::LoadRangedWeapon() { Log((void*)this != (void*)g_self ? 'X' : At<unsigned char>(this, 0x17) ? 'L' : 'K'); At<unsigned char>(this, 0x17) = g_load_sets_loading; }
void CTCHeroRangedAttacks::FireRangedWeapon()
{
    Log((void*)this == (void*)g_self ? 'F' : 'X');
    At<unsigned char>(this, 0x16) = g_fire_sets_queued;      // FireQueued, read after the call
    At<float>(this, 0x34) = g_fire_sets_pressure;            // ButtonPressure, copied to LastButtonPressure after the call
}
CCharString::CCharString(const char* text, long length)
{
    Log(strcmp(text, "SE_DRAW_BOW") == 0 && length == -1 ? 's' : 'X');
    *(const char**)this = text;
}
CCharString::~CCharString() { Log('d'); }
unsigned long CTCSoundPlayer::PlayAttachedSoundEvent(const CCharString& name, bool a, bool b, bool c)
{
    Log((void*)this == (void*)g_soundplayer && strcmp(*(const char**)&name, "SE_DRAW_BOW") == 0 && !a && !b && !c ? 'S' : 'X');
    return g_sound_id;
}
CThing* CTCCarrying::GetThingInPrimarySlot() const { Log((void*)this == (void*)g_carrying ? 'G' : 'X'); return (CThing*)g_primary; }
bool CTCWeapon::IsProjectileWeaponLoaded() const { Log((void*)this == (void*)g_weapon ? 'l' : 'X'); return g_loaded; }
bool CTCWeapon::IsProjectileWeaponAutoLoading() const { Log((void*)this == (void*)g_weapon ? 'a' : 'X'); return g_auto; }
bool CTCWeapon::IsProjectileWeapon() const { Log((void*)this == (void*)g_weapon ? 'p' : 'X'); return g_isproj; }
CPlayer& CThing::GetPlayer() { Log((void*)this == (void*)g_parent ? 'P' : 'X'); return *(CPlayer*)g_player; }
EPlayerMode CPlayer::GetCurrentMode() const { Log((void*)this == (void*)g_player ? 'm' : 'X'); return (EPlayerMode)g_mode; }
void CPlayer::AddMode(EPlayerMode mode, bool flag) { Log((void*)this == (void*)g_player && (long)mode == 0x22 && !flag ? 'M' : 'X'); }
float CHeroAbilityStatWeight::GetParameter(const CTCHeroExperience& experience) const
{
    Log((void*)this == (void*)(g_def + 0x78) && (void*)&experience == (void*)g_heroexp ? 'h' : 'X');
    return g_stat_param;
}

// ---- independent model (plain C on the raw fields; double arithmetic as the x87 code at 53-bit precision) ----
struct Model {
    unsigned char o[0x4C]; char log[256]; int n, k; bool invalid;
    void L(char c) { log[n++] = c; log[n] = 0; }
    bool Able() { L('Q'); return g_able_seq[k++] != 0; }
    template<class T> T& F(int off) { return *(T*)(o + off); }
};
static bool Has(const unsigned char* thing, int type) { return (At<unsigned long>((void*)thing, 0x20 + 4 * (type >> 5)) >> (type & 31)) & 1; }
static bool Alive(const unsigned char* thing) { return thing != 0 && !(thing[0x91] & 1); }
static void RunModel(Model& m)
{
    memcpy(m.o, g_self, 0x4C); m.n = 0; m.k = 0; m.log[0] = 0; m.invalid = false;
    float& PullBackAmount = m.F<float>(0x20); long& AimingTimer = m.F<long>(0x18); long& PullBackTimer = m.F<long>(0x1C);
    float& ButtonPressure = m.F<float>(0x34); float& LastButtonPressure = m.F<float>(0x38);
    unsigned char& FireCycle = m.F<unsigned char>(0x14); unsigned char& Aiming = m.F<unsigned char>(0x15);
    unsigned char& FireQueued = m.F<unsigned char>(0x16); unsigned char& Loading = m.F<unsigned char>(0x17);
    m.F<float>(0x28) = 0.0f; m.F<float>(0x24) = 0.0f; m.F<unsigned char>(0x2C) = 0;
    if (m.Able() && FireCycle && !Aiming && !FireQueued) { m.L('A'); return; }
    if (m.Able() && FireCycle && (Aiming || FireQueued)) {
        if (AimingTimer >= 0 && Has(g_parent, 0x10) && m.F<unsigned long>(0x48) == 0) {
            m.L('s'); m.L('S'); m.L('d'); m.F<unsigned long>(0x48) = g_sound_id;
        }
        if (ButtonPressure >= LastButtonPressure) {
            if (AimingTimer >= 0) { AimingTimer++; PullBackTimer++; }
            PullBackAmount = (float)tanh((double)PullBackTimer * (double)0.05f);
        } else {
            if (ButtonPressure < 0.3f) {
                long t = m.F<long>(0x30) + 1; m.F<long>(0x30) = t;
                if (t > 2) { m.L('C'); return; }
            }
            double v = ((double)PullBackAmount * 3.0 + (double)ButtonPressure) * 0.25;
            PullBackAmount = (float)v;
            if (v < 0.0) PullBackAmount = 0.0f; else if (v > 1.0) PullBackAmount = 1.0f;
        }
        // The root of pressure - tanh(floor(x) * 0.05) on [0, 10000): just below the first whole n whose curve value reaches the
        // pressure (bisection to 0.001), so a timer of n or more drops to n - 1.
        long n = 1;
        while ((float)tanh((double)n * (double)0.05f) < ButtonPressure) ++n;
        if (PullBackTimer >= n) PullBackTimer = n - 1;
        if (PullBackAmount > 0.95f) m.F<unsigned char>(0x2C) = 1;
        if (AimingTimer >= 0) {
            float buildup = (float)((double)AimingTimer / (double)CMainGameComponent::GetConstantFPS());
            double frac = 1.0;
            if (Has(g_parent, 0x68)) { m.L('h'); frac = g_stat_param; }
            float sup = (float)((double)buildup * (double)At<float>(g_def, 0x94));
            double comp1 = tanh((double)buildup * (double)At<float>(g_def, 0x8C)) * (double)At<float>(g_def, 0x90);
            double comp2 = (double)sup * (double)sup * (double)At<float>(g_def, 0x98);
            m.F<float>(0x28) = (float)(((double)At<float>(g_def, 0x84) * frac + 1.0) * (comp1 + comp2));
        }
        m.F<float>(0x24) = 100.0f;
        if (Has(g_parent, 0x46)) {
            m.L('G');
            const unsigned char* pc = (const unsigned char*)g_primary;
            if (Alive(pc) && Has(pc, 0x48)) {
                m.L('l');
                if (g_loaded && AimingTimer < 0) { AimingTimer = 0; PullBackTimer = 0; Loading = 0; }
            }
        }
        m.L(Loading ? 'L' : 'K'); Loading = g_load_sets_loading;
        m.L('F'); FireQueued = g_fire_sets_queued; ButtonPressure = g_fire_sets_pressure;
        LastButtonPressure = ButtonPressure;
        if (!FireQueued) Aiming = 0;
        return;
    }
    if (m.Able()) {
        const unsigned char* pc = (const unsigned char*)g_primary;
        if (!Has(g_parent, 0x46) || pc == 0 || !Has(pc, 0x48)) { m.invalid = true; return; }   // unchecked GetTC results in retail
        m.L('G'); m.L('l');
        if (g_loaded) { Loading = 0; return; }
        m.L('a');
        if (g_auto) { m.L(Loading ? 'L' : 'K'); Loading = g_load_sets_loading; return; }
    } else {
        if (!Has(g_parent, 0x46)) { m.invalid = true; return; }
        m.L('G');
        const unsigned char* pc = (const unsigned char*)g_primary;
        if (Alive(pc) && Has(pc, 0x48)) {
            m.L('p');
            if (g_isproj) {
                m.L('l');
                if (!g_loaded) {
                    m.L('a');
                    if (g_auto) { m.L('P'); m.L('m'); if (g_mode == 1) m.L('M'); }
                }
            }
        }
    }
    m.L('C');
}

static void SetBit(unsigned char* thing, int type) { At<unsigned long>(thing, 0x20 + 4 * (type >> 5)) |= 1UL << (type & 31); }
static int failures, g_case, g_ran, g_arm[8];

static void BuildCase()
{
    int i;
    for (i = 0; i < 0x4C; ++i) g_self[i] = (unsigned char)Rnd();
    memset(g_parent, 0, sizeof g_parent); memset(g_carried, 0, sizeof g_carried); memset(g_def, 0, sizeof g_def);
    At<void*>(g_self, 4) = g_parent; At<void*>(g_self, 0x10) = g_def;
    At<unsigned char>(g_self, 0x14) = RndN(4) != 0; At<unsigned char>(g_self, 0x15) = RndN(2) != 0;
    At<unsigned char>(g_self, 0x16) = RndN(3) == 0; At<unsigned char>(g_self, 0x17) = RndN(2) != 0;
    At<long>(g_self, 0x18) = RndN(4) == 0 ? -1 - RndN(3) : RndN(5) == 0 ? 0 : RndN(300);
    At<long>(g_self, 0x1C) = RndN(260) - 3;
    At<float>(g_self, 0x20) = RndN(6) == 0 ? RndF(-0.6f, 0.0f) : RndN(5) == 0 ? RndF(1.0f, 1.6f) : RndF(0.0f, 1.0f);
    At<long>(g_self, 0x30) = RndN(5) - 1;
    float bp = RndN(3) == 0 ? RndF(0.03f, 0.3f) : RndF(0.03f, 0.97f);
    At<float>(g_self, 0x34) = bp;
    At<float>(g_self, 0x38) = RndN(5) == 0 ? bp : RndF(0.03f, 0.97f);
    At<unsigned long>(g_self, 0x48) = RndN(3) == 0 ? 77UL + RndN(9) : 0UL;
    for (i = 0; i < 3; ++i) g_able_seq[i] = RndN(4) != 0; g_able_k = 0;
    // parent thing: sorted interface map with decoys round the entries the row looks up
    int n = 0; bool sp = RndN(4) != 0, ca = RndN(8) != 0, he = RndN(3) == 0;
    if (RndN(2)) { g_parent_map[n].type = 0x05; g_parent_map[n++].tc = g_decoy; SetBit(g_parent, 0x05); }
    if (sp) { g_parent_map[n].type = 0x10; g_parent_map[n++].tc = g_soundplayer; SetBit(g_parent, 0x10); }
    if (RndN(2)) { g_parent_map[n].type = 0x11; g_parent_map[n++].tc = g_decoy; SetBit(g_parent, 0x11); }
    if (RndN(2)) { g_parent_map[n].type = 0x45; g_parent_map[n++].tc = g_decoy; SetBit(g_parent, 0x45); }
    if (ca) { g_parent_map[n].type = 0x46; g_parent_map[n++].tc = g_carrying; SetBit(g_parent, 0x46); }
    if (RndN(2)) { g_parent_map[n].type = 0x47; g_parent_map[n++].tc = g_decoy; SetBit(g_parent, 0x47); }
    if (he) { g_parent_map[n].type = 0x68; g_parent_map[n++].tc = g_heroexp; SetBit(g_parent, 0x68); }
    if (RndN(2)) { g_parent_map[n].type = 0x70; g_parent_map[n++].tc = g_decoy; SetBit(g_parent, 0x70); }
    At<void*>(g_parent, 0x44) = g_parent_map; At<void*>(g_parent, 0x48) = g_parent_map + n; At<void*>(g_parent, 0x4C) = g_parent_map + 8;
    // carried thing
    n = 0; bool we = RndN(6) != 0;
    if (RndN(2)) { g_carried_map[n].type = 0x46; g_carried_map[n++].tc = g_decoy; SetBit(g_carried, 0x46); }
    if (RndN(2)) { g_carried_map[n].type = 0x47; g_carried_map[n++].tc = g_decoy; SetBit(g_carried, 0x47); }
    if (we) { g_carried_map[n].type = 0x48; g_carried_map[n++].tc = g_weapon; SetBit(g_carried, 0x48); }
    if (RndN(2)) { g_carried_map[n].type = 0x49; g_carried_map[n++].tc = g_decoy; SetBit(g_carried, 0x49); }
    At<void*>(g_carried, 0x44) = g_carried_map; At<void*>(g_carried, 0x48) = g_carried_map + n; At<void*>(g_carried, 0x4C) = g_carried_map + 8;
    g_carried[0x91] = RndN(7) == 0 ? 1 : (RndN(2) ? 0xFE : 0);
    g_primary = RndN(9) == 0 ? (void*)0 : (void*)g_carried;
    g_loaded = RndN(2) != 0; g_auto = RndN(3) != 0; g_isproj = RndN(4) != 0;
    static const long modes[4] = { 1, 1, 0, 0x22 }; g_mode = modes[RndN(4)];
    g_sound_id = 1000UL + RndN(1000); g_stat_param = RndF(0.0f, 2.0f);
    g_fire_sets_queued = RndN(2) != 0; g_fire_sets_pressure = RndF(0.03f, 0.97f); g_load_sets_loading = RndN(2) != 0;
    static const long fps[3] = { 15, 30, 7 }; CFpsAccess::Ref() = fps[RndN(3)];
    // the combat def: reference count +4 and the five multiplier floats
    At<long>(g_def, 4) = 1 + RndN(5);
    At<float>(g_def, 0x84) = RndF(0.0f, 1.0f); At<float>(g_def, 0x8C) = RndF(0.05f, 0.6f); At<float>(g_def, 0x90) = RndF(0.5f, 3.0f);
    At<float>(g_def, 0x94) = RndF(0.01f, 0.2f); At<float>(g_def, 0x98) = RndF(0.0f, 1.5f);
}

static void Fail(const char* what) { if (++failures <= 12) printf("FAIL case %d: %s\n", g_case, what); }

int main()
{
    static Model m;
    SetUnhandledExceptionFilter((void*)OnFault);
    g_seed = 0x78A690u;
    for (g_case = 0; g_case < 40000; ++g_case) {
        BuildCase();
        RunModel(m);
        if (m.invalid) continue;
        long refs = At<long>(g_def, 4);
        unsigned char parent0[0x100], carried0[0x100]; memcpy(parent0, g_parent, 0x100); memcpy(carried0, g_carried, 0x100);
        g_logn = 0; g_log[0] = 0; g_able_k = 0;
        ((CTCHeroRangedAttacks*)(void*)g_self)->CTCHeroRangedAttacks::FrameUpdate();
        ++g_ran;
        if (strcmp(g_log, m.log) != 0) { Fail("call sequence"); if (failures <= 12) printf("  got %s want %s\n", g_log, m.log); }
        // DamageMultiplier: the same-TU callee's own arithmetic; equal to a few ulp, everything else bit-exact
        float got = At<float>(g_self, 0x28), want = m.F<float>(0x28);
        if (fabs((double)got - (double)want) > 1e-5 * (fabs((double)want) + 1e-3)) { Fail("DamageMultiplier"); if (failures <= 12) printf("  got %g want %g\n", got, want); }
        At<float>(g_self, 0x28) = want;
        if (memcmp(g_self, m.o, 0x4C) != 0) {
            Fail("object state");
            if (failures <= 12) for (int i = 0; i < 0x4C; ++i) if (g_self[i] != m.o[i]) printf("  +%02X got %02X want %02X (log %s)\n", i, g_self[i], m.o[i], g_log);
        }
        if (At<long>(g_def, 4) != refs) Fail("def reference count not restored");
        if (memcmp(parent0, g_parent, 0x100) != 0 || memcmp(carried0, g_carried, 0x100) != 0) Fail("a thing was written");
        ++g_arm[g_log[1] == 'A' ? 0 : strchr(g_log, 'F') ? 1 : strchr(g_log, 'M') ? 2 : strchr(g_log, 'C') ? 3 : 4];
    }
    printf("ran %d cases: fire %d, aim %d, add-mode %d, cancel %d, load-only %d\n", g_ran, g_arm[0], g_arm[1], g_arm[2], g_arm[3], g_arm[4]);
    if (g_ran < 20000 || g_arm[0] < 500 || g_arm[1] < 2000 || g_arm[2] < 50 || g_arm[3] < 2000 || g_arm[4] < 500) Fail("coverage");
    if (failures == 0) printf("0078a690_TEST PASS\n"); else printf("0078a690_TEST FAIL (%d)\n", failures);
    return failures != 0;
}