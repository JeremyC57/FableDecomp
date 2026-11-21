// Behaviour test for NCreatureMode::CModeScorpionKingVulnerable::AnimUpdate (0x008D9B00): links the REAL row object (with its own
// COMDAT copies of CDefClassBase::GetSubDef<CScorpionKingBattleDef> and the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound of the
// inline GetTC). The test defines every import and hand-builds the memory at retail offsets: mode PCreature +0x78, PEyeEmitter +0x94
// (intelligent pointer, item at +4), FrameCount +0xA4, TailCurlFinished +0xB4; creature TC flags +0x2C (sign bit = has TCs), TC map
// +0x44 (begin / finish of {interface, TC*} pairs), PBaseDef +0x70 -> base def (vtable slots 14 / 15 GetPSubDefInfo, PDefManager +0x1C);
// battle def (vtable slot 1 = delete, count +4, TailStrikePrepareTime +0x50); anim set Current / Last +0x1C / +0x20; dead flag +0x91 bit 0.
// Checks against a double reference: the base AnimUpdate first, the tail-curl anim (type 0x21) or nothing, the sub-def lookup by
// type name and its handle balanced (the def deleted only when the handle was its last reference), the frame = clamp(FrameCount / fps /
// TailStrikePrepareTime, 0, 1) with Last = the old Current, then, from an old Current of 0.8, the belly hit location disabled and the
// normal one enabled (looked up on the hit-locations TC 0x7F, or on none when the creature has no TCs) and a live eye emitter killed,
// and from 1.0 TailCurlFinished set.
#include <stdio.h>
#include <math.h>
#include <string.h>
static int failures;
static void check(bool ok, const char* what, int c) { if (!ok) { ++failures; printf("FAIL case %d: %s\n", c, what); } }
class CBaseObject;
class CCharString { public: const char* Text; CCharString(const char* text, long len); ~CCharString(); };
class CAnimSet;
class CAnimateThing { public: CAnimSet* GetFirstCombinationAnimFromType(long type); };
class CModeStandBase { public: virtual void AnimUpdate(CAnimateThing& anim); };
namespace NCreatureMode { class CModeScorpionKingVulnerable { public: virtual void AnimUpdate(CAnimateThing& anim); }; }
class CBaseIntelligentPointer { public: CBaseObject* GetPItem() const; };
class CThing { public: void Kill(bool b); };
namespace NHitLocations { class CHitLocation { public: void SetEnabled(bool on); }; }
class CTCHitLocations { public: NHitLocations::CHitLocation* GetPHitLocationWithDefName(const CCharString& name); };
// CDefinitionManager::GetPDefFromGlobalIndex returns a CDefPointer<CDefClassBase> by hidden pointer (thiscall, `ret 8`). The double is a
// fastcall twin under /alternatename, so the test declares no CDefPointer of its own (its inline members would be COMDATs competing with
// the row object's copies).
#pragma comment(linker, "/alternatename:?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z=@GetPDefFromGlobalIndexDouble@16")
class CMainGameComponent { protected: static long ConstantFPS; friend void set_fps(long fps); };
void set_fps(long fps) { CMainGameComponent::ConstantFPS = fps; }
long CMainGameComponent::ConstantFPS = 30;

// fixtures
static unsigned char mode_mem[0xc0], creature_mem[0x100], base_def[0x40], battle_def[0x60], anim_mem[0x40], eye_mem[0x100], manager_mem[8];
static unsigned char tc_hit[8], tc_other[8], belly_loc[8], normal_loc[8];
static void* base_vtbl[32]; static void* battle_vtbl[4];
static unsigned char animate_thing[4];
static long tc_map[2][2];
static bool eye_alive; static long subdef_index;
static int order, base_at, find_at, n_info, n_getdef, n_delete, n_find, n_cs, n_cs_dtor, n_lookup, n_enable, n_kill;
static long find_type; static long getdef_index; static const char* info_name; static const void* lookup_this[2]; static const char* lookup_name[2];
static const void* enable_this[2]; static bool enable_arg[2]; static bool kill_arg; static const void* kill_this;
static struct { long DefIndex, OriginalParentDefIndex; } subdef_info;
static bool have_anim, have_subdef;

CCharString::CCharString(const char* text, long) { ++n_cs; Text = text; }
CCharString::~CCharString() { ++n_cs_dtor; }
void CModeStandBase::AnimUpdate(CAnimateThing& anim) { base_at = ++order; check((unsigned char*)this == mode_mem && (unsigned char*)&anim == animate_thing, "base AnimUpdate (this, anim)", -1); }
CAnimSet* CAnimateThing::GetFirstCombinationAnimFromType(long type) { ++n_find; find_at = ++order; find_type = type; check((unsigned char*)this == animate_thing, "anim lookup this", -1); return have_anim ? (CAnimSet*)anim_mem : 0; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { check((const unsigned char*)this == mode_mem + 0x94, "GetPItem on PEyeEmitter (+0x94)", -1); return *(CBaseObject* const*)((const char*)this + 4); }
void CThing::Kill(bool b) { ++n_kill; kill_this = this; kill_arg = b; }
void NHitLocations::CHitLocation::SetEnabled(bool on) { if (n_enable < 2) { enable_this[n_enable] = this; enable_arg[n_enable] = on; } ++n_enable; }
NHitLocations::CHitLocation* CTCHitLocations::GetPHitLocationWithDefName(const CCharString& name)
{
    if (n_lookup < 2) { lookup_this[n_lookup] = this; lookup_name[n_lookup] = name.Text; }
    ++n_lookup;
    return (NHitLocations::CHitLocation*)(strcmp(name.Text, "HIT_LOCATION_SCORPION_KING_BELLY") == 0 ? belly_loc : normal_loc);
}
extern "C" void* __fastcall GetPDefFromGlobalIndexDouble(void* manager, int, void* result, long index)
{
    ++n_getdef; getdef_index = index; check((unsigned char*)manager == manager_mem, "GetPDefFromGlobalIndex on the def's manager", -1);
    void* def = battle_def; memcpy(result, &def, 4); ++*(long*)(battle_def + 4); return result;
}
struct FxDef {
    const void* GetInfo(const CCharString& name) const { ++n_info; info_name = name.Text; check((const unsigned char*)this == base_def, "GetPSubDefInfo on the base def", -1); return have_subdef ? &subdef_info : 0; }
    void Delete() { ++n_delete; check((unsigned char*)this == battle_def, "delete the battle def", -1); }
    void Bad() { check(false, "unexpected virtual", -1); }
};
template <class F> static void* code_of(F f) { void* p; memcpy(&p, &f, sizeof p); return p; }
static void put_f(unsigned char* m, int off, float v) { memcpy(m + off, &v, 4); }
static void put_l(unsigned char* m, int off, long v) { memcpy(m + off, &v, 4); }
static void put_p(unsigned char* m, int off, const void* p) { memcpy(m + off, &p, 4); }
static float get_f(const unsigned char* m, int off) { float v; memcpy(&v, m + off, 4); return v; }
static bool near(double a, double b) { return fabs(a - b) <= 1e-5 * (1.0 + fabs(b)); }

static void run(int c, bool anim, long frames, long fps, float prep, float old_cur, bool has_tcs, bool eye, bool eye_dead, long def_count)
{
    order = base_at = find_at = n_info = n_getdef = n_delete = n_find = n_cs = n_cs_dtor = n_lookup = n_enable = n_kill = 0;
    info_name = 0; lookup_this[0] = lookup_this[1] = 0; kill_this = 0;
    have_anim = anim; have_subdef = true;
    memset(mode_mem, 0, sizeof mode_mem);
    put_p(mode_mem, 0x78, creature_mem); put_l(mode_mem, 0xa4, frames);
    put_p(mode_mem, 0x98, eye ? (void*)eye_mem : (void*)0);
    eye_mem[0x91] = eye_dead ? 1 : 0;
    put_l(creature_mem, 0x2c, has_tcs ? (long)0x80000000 : 0);
    tc_map[0][0] = 0x10; tc_map[0][1] = (long)tc_other; tc_map[1][0] = 0x7f; tc_map[1][1] = (long)tc_hit;
    put_p(creature_mem, 0x44, &tc_map[0][0]); put_p(creature_mem, 0x48, &tc_map[2][0]);
    put_p(creature_mem, 0x70, base_def); put_p(base_def, 0, base_vtbl); put_p(base_def, 0x1c, manager_mem);
    put_p(battle_def, 0, battle_vtbl); put_l(battle_def, 4, def_count); put_f(battle_def, 0x50, prep);
    subdef_info.DefIndex = 1234; subdef_info.OriginalParentDefIndex = 7;
    put_f(anim_mem, 0x1c, old_cur); put_f(anim_mem, 0x20, -5.0f);
    set_fps(fps);
    ((NCreatureMode::CModeScorpionKingVulnerable*)mode_mem)->NCreatureMode::CModeScorpionKingVulnerable::AnimUpdate(*(CAnimateThing*)animate_thing);
    check(base_at == 1 && find_at == 2 && find_type == 0x21, "base AnimUpdate, then the 0x21 combination anim", c);
    if (!anim) {
        check(n_info == 0 && n_getdef == 0 && n_lookup == 0 && n_kill == 0 && mode_mem[0xb4] == 0, "no anim set: nothing else", c);
        return;
    }
    check(n_info == 1 && info_name && strcmp(info_name, "CScorpionKingBattleDef") == 0, "sub-def info by type name", c);
    check(n_getdef == 1 && getdef_index == 1234, "def from the sub-def's global index", c);
    check(*(long*)(battle_def + 4) == def_count && n_delete == (def_count == 0 ? 1 : 0), "def handle balanced (deleted only at zero)", c);
    double cur = (double)frames / fps / prep; if (cur < 0.0) cur = 0.0; else if (cur > 1.0) cur = 1.0;
    check(near(get_f(anim_mem, 0x1c), cur), "Current = clamp(frames / fps / prep, 0, 1)", c);
    check(get_f(anim_mem, 0x20) == old_cur, "Last = old Current", c);
    if (old_cur >= 0.8f) {
        const void* tc = has_tcs ? (const void*)tc_hit : 0;
        check(n_lookup == 2 && lookup_this[0] == tc && lookup_this[1] == tc, "both lookups on the hit-locations TC (0x7F) or none", c);
        check(n_lookup == 2 && strcmp(lookup_name[0], "HIT_LOCATION_SCORPION_KING_BELLY") == 0 && strcmp(lookup_name[1], "HIT_LOCATION_SCORPION_KING") == 0, "belly then normal by def name", c);
        check(n_enable == 2 && enable_this[0] == belly_loc && !enable_arg[0] && enable_this[1] == normal_loc && enable_arg[1], "belly disabled, normal enabled", c);
        bool alive = eye && !eye_dead;
        check(alive ? (n_kill == 1 && kill_this == eye_mem && kill_arg) : n_kill == 0, "live eye emitter killed(true)", c);
    } else
        check(n_lookup == 0 && n_enable == 0 && n_kill == 0, "below 0.8: hit locations untouched", c);
    check(n_cs == n_cs_dtor, "string temporaries destroyed", c);
    check(mode_mem[0xb4] == (old_cur >= 1.0f ? 1 : 0), "TailCurlFinished from an old Current of 1.0", c);
}

int main()
{
    for (int i = 0; i < 32; ++i) base_vtbl[i] = code_of(&FxDef::Bad);
    base_vtbl[14] = base_vtbl[15] = code_of(&FxDef::GetInfo);
    for (int i = 0; i < 4; ++i) battle_vtbl[i] = code_of(&FxDef::Bad);
    battle_vtbl[1] = code_of(&FxDef::Delete);
    run(1, false, 10, 30, 2.0f, 0.5f, true, true, false, 1);
    run(2, true, 15, 30, 2.0f, 0.1f, true, true, false, 1);     // 0.25, below 0.8
    run(3, true, 45, 30, 2.0f, 0.85f, true, true, false, 1);    // 0.75, hit locations, live eye
    run(4, true, 120, 30, 2.0f, 0.9f, false, true, true, 2);    // clamped to 1, no TCs, dead eye
    run(5, true, -30, 30, 2.0f, 1.0f, true, false, false, 0);   // clamped to 0, no eye, tail curl finished, def deleted
    run(6, true, 59, 25, 2.5f, 0.79f, true, true, false, 3);    // 0.944, just below 0.8
    run(7, true, 75, 30, 2.5f, 1.25f, true, true, false, 1);    // exactly 1, old above 1
    run(8, true, 30, 30, 2.0f, 0.8f, true, true, false, 1);     // old Current exactly 0.8: the swap happens
    if (failures) { printf("008d9b00_TEST FAIL (%d)\n", failures); return 1; }
    printf("008d9b00_TEST PASS\n");
    return 0;
}