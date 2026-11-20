// Behaviour test for 0x0079D360 CTCBulletTime::CTCBulletTime(CThing&) (links the REAL row object with what it carries: the
// CDefClassBase::GetSubDef<CSpecialEffectsDef> and CSpecialEffectsDef::GetEffectFromString bodies, the bypass vector's push_back,
// the interface-map lookups). The classes come from the shared headers only so that the out-of-line callees defined here get
// their retail names; every object is raw native-layout storage and every offset used is one the row reads or writes:
//   CTCBulletTime   BulletTimeRemainingTicks +0x0C, BulletTimeAbilityLevel +0x10, DisplacementEffectPrimitive +0x18, PDef +0x20,
//                   m_nLevel +0x24, m_nTimeMultiplier +0x28, m_fDuration +0x2C, m_tpLoopingEffect +0x30, LoopSound +0x38,
//                   PItemRemoved +0x3C, HitCreatureBlur +0x44, RadialBlurKey +0x4C, ScreenFilterKey +0x50, the three flags +0x54..
//   def             vtable slot 14 GetPSubDefInfo, count +4, PDefManager +0x1C, SlowTimeDuration +0x3C, BulletTimeSlowdownMultiplier
//                   +0x4C, InitSound +0xC8, LoopSound +0xCC, the seven screen-filter vectors +0x130.., ScreenFilterFadeInSecs +0x184
//   CThing          interface bitset +0x20 (abilities 0x6F, carrying 0x46), interface map +0x44; CWorld environment +0x18,
//                   bullet-time manager +0x68; CEnvironment interpolator +0x1964; emitter dead bit +0x40 & 0x20, AlwaysUpdate +0x44 & 0x10
// Every callee logs a letter (smart-pointer calls also say which pointer); the test compares the whole call sequence, every
// argument and every member the constructor must set (the buffer starts as 0xCC) with what the scene dictates.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
#define FABLE_RETAIL_CTCBULLETTIME_UNPAUSE
#define FABLE_RETAIL_CTCCARRYING_SETBYPASSFILTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADETO
#define FABLE_RETAIL_CTCBULLETTIME_CTOR
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_SETSLOWDOWN
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"
#include "engine/CParticleEmitter.h"
#include "engine/NUISystem_CManager.h"            // the full PDB EEngine2DLayer (ENGINE_2D_LAYER_SPELLS = 82)
#include "engine/CParticleEmitterManager.h"
#include "engine/ThingAlive.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCCarrying.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatToLongCeil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>

// A fault ends the run at once instead of waiting for a debugger.
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall OnFault(void*) { ExitProcess(3); return 0; }

template<class T> static T& At(void* p, int off) { return *(T*)((char*)p + off); }
static unsigned g_seed;
static unsigned Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static int RndN(int n) { return (int)(Rnd() % (unsigned)n); }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() & 0xFFFF) / 65535.0f; }

// ---- call log ----
static char g_log[1024]; static int g_logn;
static void Log(char c) { if (g_logn < 1000) g_log[g_logn++] = c; g_log[g_logn] = 0; }
static void LogS(const char* s) { while (*s) Log(*s++); }

// ---- the scene ----
static unsigned char g_obj[0x58], g_thing[0x100], g_def[0x190], g_effects[0x40], g_mgr[8], g_gdm[8], g_inv[8], g_carrying[8];
static unsigned char g_world[0x80], g_env[0x1970], g_interp[8], g_btm[8], g_pem[8], g_emitter[0x50];
static float g_arr[9][4]; static struct { float* s; float* f; float* e; } g_vec[7];
static struct { long type; void* tc; } g_map[4];
static struct { long DefIndex, OriginalParentDefIndex; } g_subdef;
static struct RbNode { long color; RbNode* parent; RbNode* left; RbNode* right; unsigned long key; long value; } g_hdr, g_cast;
static const char* g_names[8] = { "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7" };
static long g_level, g_def_index; static bool g_has_inv, g_has_sub, g_has_cast, g_has_carrying; static int g_emitter_kind;   // 0 none 1 dead 2 alive
static void* g_key;
static struct { float time; long prio; float a, s, c, b, tint[4]; int n; void* thing; unsigned char bypass; } g_fade;
static struct { long effect; float x, y; long layer; bool flag; float set[6]; } g_create;
static struct { long prio; void* key; float mult; } g_slow;

// ---- strings: a CCharString is one pointer to a data block whose CBasicString<char> starts with the text pointer and the length ----
struct StrData { const char* PData; unsigned long StringLength; unsigned long DataLength; long NoRefs; };
static StrData g_strpool[64]; static int g_strn;
static const char* Text(const CCharString& s) { return (*(StrData**)&s)->PData; }
CCharString::CCharString(const char* text, long length)
{
    StrData* d = &g_strpool[g_strn++ & 63]; d->PData = text; d->StringLength = (unsigned long)strlen(text); *(StrData**)this = d;
    Log(length == -1 ? 's' : 'X');
}
CCharString::~CCharString() { Log('d'); }
const char CCharString::NullChar = 0;
unsigned long FABLE_FASTCALL CCRC::Calc(unsigned long seed, const void* data, unsigned long n)
{
    Log(seed == 0 && n == 4 && memcmp(data, "CAST", 4) == 0 ? 'r' : 'X'); return 0xCA57UL;
}

// ---- bases and smart pointers ----
CTCBase::CTCBase(CThing& thing) { Log((void*)this == (void*)g_obj && (void*)&thing == (void*)g_thing ? 'B' : 'X'); At<void*>(this, 4) = &thing; }
CTCBase::~CTCBase() { }
CBaseClass::CBaseClass() { if ((void*)this != (void*)g_obj) Log((void*)this == (void*)(g_obj + 0x18) ? 'c' : 'X'); }   // silent as this test's own CTCBase base
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CWideString::CWideString() { }
CWideString::~CWideString() { }                                           // members of the string table object below
static char Where(const void* p)
{
    long off = (long)((const unsigned char*)p - g_obj);
    return off == 0x30 ? 'l' : off == 0x3c ? 'p' : off == 0x44 ? 'h' : 't';       // looping effect, item removed, hit blur, a temporary
}
CBaseIntelligentPointer::CBaseIntelligentPointer() { Log('('); Log(Where(this)); At<void*>(this, 4) = (void*)0xDDDDDDDD; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { Log(')'); Log(Where(this)); }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item)
{
    Log('='); Log(Where(this)); Log(item == 0 ? '0' : (void*)item == (void*)g_thing ? 'T' : '?'); At<void*>(this, 4) = item;
    // when the looping effect is cleared, the level-0 duration and multiplier are already in place
    if (Where(this) == 'l' && (At<float>(g_obj, 0x2c) != At<float>(g_def, 0x3c) || At<long>(g_obj, 0x28) != At<long>(g_def, 0x4c))) Log('!');
}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { Log('?'); return At<CBaseObject*>((void*)this, 4); }

// ---- the out-of-line callees, under their retail names ----
CGameDefinitionManager& FABLE_FASTCALL CGameDefinitionManager::Get() { Log('G'); return *(CGameDefinitionManager*)(void*)g_gdm; }
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& name, long index) const
{
    Log((void*)this == (void*)g_gdm && strcmp(Text(name), "SPECIAL_ABILITIES_BULLET_TIME_DEF") == 0 && index == 1 ? 'I' : 'X'); return g_def_index;
}
template<> bool CDefinitionManager::GetDef<CSpecialAbilitiesBulletTimeDef>(long index, CDefPointer<const CSpecialAbilitiesBulletTimeDef>* out) const
{
    Log((void*)this == (void*)g_gdm && index == g_def_index && (void*)out == (void*)(g_obj + 0x20) ? 'D' : 'X');
    *(void**)out = g_def; ++At<long>(g_def, 4); return true;
}
long CTCInventoryAbilities::GetAbilityLevel(EHeroAbility ability) const { Log((void*)this == (void*)g_inv && (long)ability == 2 ? 'a' : 'X'); return g_level; }
void CThingCreatureBase::MakeNoise(const CCharString& name) { Log((void*)this == (void*)g_thing && strcmp(Text(name), "SIMPLE_NOISE") == 0 ? 'N' : 'X'); }
CDefStringTable GDefStringTable;
CCharString CDefStringTable::GetString(long id) const { Log((void*)this == (void*)&GDefStringTable && id >= 0 && id < 8 ? 'g' : 'X'); return CCharString(g_names[id & 7], -1); }
unsigned long CTCBulletTime::PlaySoundW(const CCharString& name, bool loop)
{
    int id = (int)(Text(name)[1] - '0'); Log((void*)this == (void*)g_obj && loop ? 'W' : 'X'); Log((char)('0' + id)); return 500UL + id;
}
static void* __fastcall Def_GetPSubDefInfo(void* self, int, const CCharString& type_name)
{
    Log(self == (void*)g_def && strcmp(Text(type_name), "CSpecialEffectsDef") == 0 ? 'u' : 'X'); return g_has_sub ? (void*)&g_subdef : (void*)0;
}
static void* g_def_vtbl[32];
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{
    Log((void*)this == (void*)g_mgr && index == g_subdef.DefIndex ? 'x' : 'X');
    ++At<long>(g_effects, 4);                                              // the returned handle's reference
    CDefPointer<CDefClassBase> handle; *(void**)&handle = g_effects;
    return handle;
}
namespace std {
typedef _Rb_tree<unsigned long, pair<const unsigned long, long>, _Select1st<pair<const unsigned long, long> >, less<unsigned long>, allocator<pair<const unsigned long, long> > > EffectTree;
template<> EffectTree::const_iterator EffectTree::find(const unsigned long& key) const
{
    Log(key == 0xCA57UL ? 'f' : 'X');
    At<void*>((void*)this, 0) = &g_hdr;                                    // the tree's header node: end()
    return const_iterator((_Rb_tree_node<pair<const unsigned long, long> >*)(void*)(g_has_cast ? &g_cast : &g_hdr));
}
}
namespace NParticleEngine { CParticleEmitterManager& __fastcall GetParticleEmitterManager(); }
NParticleEngine::CParticleEmitterManager& __fastcall NParticleEngine::GetParticleEmitterManager() { Log('M'); return *(CParticleEmitterManager*)(void*)g_pem; }
float __fastcall GFVXToAX(float x) { Log('x'); return x * 0.25f + 1.0f; }
float __fastcall GFVYToAY(float y) { Log('y'); return y * 0.125f - 2.0f; }
double VIRTUAL_SCREEN_WIDTH = 640.0, VIRTUAL_SCREEN_HEIGHT = 480.0;
NParticleEngine::CParticleEmitter* NParticleEngine::CParticleEmitterManager::Create2DEmitter(long effect, const C2DVector& pos, EEngine2DLayer layer, bool flag, const CRightHandedSet& set)
{
    Log((void*)this == (void*)g_pem ? 'E' : 'X');
    g_create.effect = effect; g_create.x = At<float>((void*)&pos, 0); g_create.y = At<float>((void*)&pos, 4); g_create.layer = (long)layer; g_create.flag = flag;
    memcpy(g_create.set, &set, 24);
    return g_emitter_kind == 0 ? 0 : (NParticleEngine::CParticleEmitter*)(void*)g_emitter;
}
CWorld& CTCBase::GetWorld() const { Log((void*)this == (void*)g_obj ? 'w' : 'X'); return *(CWorld*)(void*)g_world; }
void CBulletTimeManager::SetBulletTimeSlowdownMultiplier(long priority, void* key, float multiplier)
{
    Log((void*)this == (void*)g_btm ? 'L' : 'X'); g_slow.prio = priority; g_slow.key = key; g_slow.mult = multiplier;
}
void* CEnvironmentSpellEffectInterpolator::ScreenFilterFadeTo(float time, NScreenEffect::ScreenEffectPriority priority, float alpha, float saturation,
    float contrast, float brightness, const CRGBFloatColour& tint, const NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers& things)
{
    Log((void*)this == (void*)g_interp ? 'F' : 'X');
    g_fade.time = time; g_fade.prio = (long)priority; g_fade.a = alpha; g_fade.s = saturation; g_fade.c = contrast; g_fade.b = brightness;
    memcpy(g_fade.tint, &tint, 16);
    unsigned char* b = At<unsigned char*>((void*)&things, 0); unsigned char* e = At<unsigned char*>((void*)&things, 4);
    g_fade.n = (int)(e - b); g_fade.thing = g_fade.n >= 9 ? At<void*>(b, 4) : 0; g_fade.bypass = g_fade.n >= 9 ? b[8] : 0;
    return g_key;
}
void CTCCarrying::SetAllCarriedThingsAsBypassFilter(void* key) { Log((void*)this == (void*)g_carrying && key == g_key ? 'C' : 'X'); }
long CMainGameComponent::ConstantFPS = 15;
class CFpsAccess : public CMainGameComponent { public: static long& Ref() { return ConstantFPS; } };

// the CTCBase virtuals the row object's vtable names (never called here)
void CTCBase::OnSerialise(CPersistContext&, bool) { } void CTCBase::OnRemove() { } void CTCBase::ConstructFromParams(const CTCParamsBase&) { }
void CTCBase::OnCreate() { } void CTCBase::InitialActivate() { } void CTCBase::OnInitialActivate() { } void CTCBase::RenderUpdate(float) { }
bool CTCBase::HasFrameUpdate() const { return false; } void CTCBase::FrameUpdate() { } void CTCBase::BulletTimeFrameUpdate() { }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { } void CTCBase::Copy(const CTCBase&) { }
void CTCBase::OnDie() { } void CTCBase::OnKill() { } void CTCBase::OnLevelUnload() { } void CTCBase::OnSetInLimbo(bool) { }
bool CTCBase::Validate(CCharString&) const { return true; } bool CTCBase::IsFreeForDeletion() const { return true; }
bool CTCBase::IsSerialiseCreatable() const { return true; } bool CTCBase::IsSerialisableForHeroState() const { return true; }
bool CTCBase::IsComponentSerialisable() const { return true; } bool CTCBase::IsDriver() const { return false; } bool CTCBase::IsCopyable() const { return true; }
void CTCBase::GlobalUpdate(bool) { } void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) { }

// The class is abstract on this define set (its two pure-virtual overrides are not declared): a derived shell constructs it.
class CTestBulletTime : public CTCBulletTime {
public:
    CTestBulletTime(CThing& thing) : CTCBulletTime(thing) { }
    virtual ETCInterfaceType GetInterfaceType() const { return TCI_BULLET_TIME; }
    virtual CCharString GetClassTypeName() const { return CCharString("CTCBulletTime", -1); }
};

// ---- the model: what the constructor must have called and stored ----
static char g_want[1024]; static int g_wantn;
static void W(const char* s) { while (*s) g_want[g_wantn++] = *s++; g_want[g_wantn] = 0; }
static long CeilModel(float v)
{
    double r = floor((double)v + 0.5 + 0.5);                               // round to nearest of v + 0.5 ..
    if (r - ((double)v + 0.5) == 0.5 && fmod(r, 2.0) != 0.0) r -= 1.0;     // .. ties to even
    if ((float)(r - 1.0) == v) r -= 1.0;                                   // an exact whole number is its own ceiling
    return (long)r;
}
static int failures, g_case, g_cov[6];
static void Fail(const char* what) { if (++failures <= 12) printf("FAIL case %d: %s\n", g_case, what); }
static bool Near(float a, float b) { return fabs((double)a - (double)b) <= 1e-5 * (fabs((double)b) + 1e-3); }

int main()
{
    int i, j;
    SetUnhandledExceptionFilter((void*)OnFault);
    for (i = 0; i < 32; ++i) g_def_vtbl[i] = 0;
    g_def_vtbl[14] = (void*)Def_GetPSubDefInfo;
    g_seed = 0x79D360u;
    for (g_case = 0; g_case < 20000; ++g_case) {
        memset(g_obj, 0xCC, sizeof g_obj); memset(g_thing, 0, sizeof g_thing); memset(g_def, 0, sizeof g_def); memset(g_emitter, 0, sizeof g_emitter);
        g_logn = 0; g_log[0] = 0; g_wantn = 0; g_want[0] = 0;
        g_has_inv = RndN(4) != 0; g_has_sub = RndN(5) != 0; g_has_cast = RndN(5) != 0; g_has_carrying = RndN(3) != 0; g_emitter_kind = RndN(4) == 0 ? RndN(2) : 2;
        g_level = RndN(4); g_def_index = 100 + RndN(900); g_key = (void*)(0xF000 + RndN(4000));
        g_subdef.DefIndex = 1000 + RndN(1000); g_cast.value = 1 + RndN(500); g_cast.key = 0xCA57UL;
        CFpsAccess::Ref() = RndN(2) ? 15 : 30;
        VIRTUAL_SCREEN_WIDTH = 320.0 + RndN(700); VIRTUAL_SCREEN_HEIGHT = 240.0 + RndN(500);
        // the def: vtable (slot 14), reference count +4, manager +0x1C, per-level tables, sound ids, screen-filter vectors
        At<void*>(g_def, 0) = g_def_vtbl; At<long>(g_def, 4) = 10; At<void*>(g_def, 0x1c) = g_mgr; At<long>(g_effects, 4) = 20;
        for (j = 0; j < 4; ++j) {
            At<float>(g_def, 0x3c + 4 * j) = RndN(6) == 0 ? (float)(1 + RndN(9)) / 15.0f * 5.0f : RndF(0.5f, 20.0f);   // SlowTimeDuration
            At<long>(g_def, 0x4c + 4 * j) = 2 + RndN(30);                                                           // BulletTimeSlowdownMultiplier
            for (i = 0; i < 7; ++i) g_arr[i][j] = RndF(0.0f, 2.0f);
        }
        At<long>(g_def, 0xc8) = RndN(8); At<long>(g_def, 0xcc) = RndN(8);
        for (i = 0; i < 7; ++i) { g_vec[i].s = g_arr[i]; g_vec[i].f = g_vec[i].e = g_arr[i] + 4; memcpy(g_def + 0x130 + 12 * i, &g_vec[i], 12); }
        At<float>(g_def, 0x184) = RndF(0.1f, 3.0f);
        // the caster: abilities (0x6F) and carrying (0x46) components in a sorted interface map
        int n = 0;
        g_map[n].type = 5; g_map[n++].tc = g_btm; At<unsigned long>(g_thing, 0x20) |= 1UL << 5;
        if (g_has_carrying) { g_map[n].type = 0x46; g_map[n++].tc = g_carrying; At<unsigned long>(g_thing, 0x28) |= 1UL << 6; }
        if (g_has_inv) { g_map[n].type = 0x6f; g_map[n++].tc = g_inv; At<unsigned long>(g_thing, 0x2c) |= 1UL << 15; }
        g_map[n].type = 0x70; g_map[n++].tc = g_btm; At<unsigned long>(g_thing, 0x2c) |= 1UL << 16;
        At<void*>(g_thing, 0x44) = g_map; At<void*>(g_thing, 0x48) = g_map + n; At<void*>(g_thing, 0x4c) = g_map + 4;
        At<void*>(g_world, 0x68) = g_btm; At<void*>(g_world, 0x18) = g_env; At<void*>(g_env, 0x1964) = g_interp;
        if (g_emitter_kind == 1) g_emitter[0x40] = 0x20;                   // dead

        new (g_obj) CTestBulletTime(*(CThing*)(void*)g_thing);

        long lvl = g_has_inv ? g_level : 0;
        long init = At<long>(g_def, 0xc8), loop = At<long>(g_def, 0xcc);
        char buf[64];
        W("Bc(l(p=p0(h=h0");                                               // base, handle, three pointers (two set to null)
        W("sGId" "GD");                                                    // class-name string, index, def
        W("(t=t0?=l0)t");                                                  // looping effect = null through a temporary
        if (g_has_inv) W("a");
        W("sNd");                                                          // the cast noise
        sprintf(buf, "gsW%dd" "gsW%dd", (int)init, (int)loop); W(buf);     // table string, play, release (twice)
        W("sud");                                                          // sub-def type name, lookup
        bool cast = false;
        if (g_has_sub) { W("x" "srfd"); cast = g_has_cast; }               // the effects def, then "CAST" hashed and looked up
        bool alive = cast && g_emitter_kind == 2;
        if (cast) W("yxME");
        W("wL" "(t=tT" "(t?=tT)t" "w" "F");                                // slowdown, the bypass record and its copy, the filter
        if (g_has_carrying) W("C");
        W(")t");                                                           // the vector's one element
        if (strcmp(g_log, g_want) != 0) { Fail("call sequence"); if (failures <= 12) printf("  got  %s\n  want %s\n", g_log, g_want); }
        float dur = At<float>(g_def, 0x3c + 4 * lvl); long mult = At<long>(g_def, 0x4c + 4 * lvl);
        if (At<long>(g_obj, 0x10) != 0 || At<unsigned long>(g_obj, 0x14) != 0xCCCCCCCCUL || At<long>(g_obj, 0x1c) != 0) Fail("ability level / mask / handle");
        if (At<void*>(g_obj, 0x20) != (void*)g_def || At<long>(g_def, 4) != 11) Fail("PDef");
        if (At<long>(g_obj, 0x24) != lvl) Fail("m_nLevel");
        if (At<long>(g_obj, 0x28) != mult) Fail("m_nTimeMultiplier");
        if (At<float>(g_obj, 0x2c) != dur) Fail("m_fDuration");
        if (At<void*>(g_obj, 0x34) != 0 || At<void*>(g_obj, 0x40) != 0 || At<void*>(g_obj, 0x48) != 0) Fail("pointer members not null");
        if (At<unsigned long>(g_obj, 0x38) != 500UL + (unsigned long)loop) Fail("LoopSound");
        if (At<void*>(g_obj, 0x4c) != 0 || At<void*>(g_obj, 0x50) != g_key) Fail("blur / filter keys");
        if (g_obj[0x54] != 1 || g_obj[0x55] != 0 || g_obj[0x56] != 0 || g_obj[0x57] != 0xCC) Fail("flags");
        long ticks = CeilModel((float)((double)CFpsAccess::Ref() * (double)dur));
        if (At<long>(g_obj, 0x0c) != ticks) { Fail("BulletTimeRemainingTicks"); if (failures <= 12) printf("  got %ld want %ld (%g)\n", At<long>(g_obj, 0x0c), ticks, dur); }
        if (At<long>(g_effects, 4) != 20) Fail("effects def reference count");
        if (g_slow.prio != mult || g_slow.key != (void*)g_obj || g_slow.mult != 0.5f) Fail("slowdown arguments");
        if (cast) {
            float x = ((float)VIRTUAL_SCREEN_WIDTH * 0.25f + 1.0f) * 0.5f, y = ((float)VIRTUAL_SCREEN_HEIGHT * 0.125f - 2.0f) * 0.5f;
            static const float set[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
            if (g_create.effect != g_cast.value || !Near(g_create.x, x) || !Near(g_create.y, y) || g_create.layer != 0x52 || !g_create.flag ||
                memcmp(g_create.set, set, 24) != 0) Fail("Create2DEmitter arguments");
            if (((g_emitter[0x44] & 0x10) != 0) != alive) Fail("AlwaysUpdate");
        }
        if (g_fade.time != At<float>(g_def, 0x184) || g_fade.prio != 2 || g_fade.a != g_arr[0][lvl] || g_fade.s != g_arr[1][lvl] || g_fade.c != g_arr[2][lvl] ||
            g_fade.b != g_arr[3][lvl] || g_fade.tint[0] != g_arr[4][lvl] || g_fade.tint[1] != g_arr[5][lvl] || g_fade.tint[2] != g_arr[6][lvl] ||
            g_fade.tint[3] != 1.0f || g_fade.n != 9 || g_fade.thing != (void*)g_thing || g_fade.bypass != 1) Fail("ScreenFilterFadeTo arguments");
        ++g_cov[cast ? (alive ? 0 : 1) : 2]; if (g_has_inv && g_level) ++g_cov[3]; if (g_has_carrying) ++g_cov[4];
    }
    printf("cast alive %d, cast no/dead emitter %d, no cast %d, levelled %d, carrying %d\n", g_cov[0], g_cov[1], g_cov[2], g_cov[3], g_cov[4]);
    if (g_cov[0] < 3000 || g_cov[1] < 500 || g_cov[2] < 2000 || g_cov[3] < 3000 || g_cov[4] < 3000) Fail("coverage");
    if (failures == 0) printf("0079d360_TEST PASS\n"); else printf("0079d360_TEST FAIL (%d)\n", failures);
    return failures != 0;
}