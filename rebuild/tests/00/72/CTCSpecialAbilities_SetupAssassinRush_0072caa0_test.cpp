// Behaviour test for 0x0072CAA0 CTCSpecialAbilities::SetupAssassinRush (links the REAL row object; NF270 rewrite of the standalone host
// model, PassPattern kept). The row is reached through its private PDB leaf by an explicit instantiation (access is not checked there,
// C++03 14.7.2/8; test-only scaffolding). The objects it reads are raw storage at the retail offsets the row documents (Creature
// reference +0x18; CThing UsingComponents +0x20, Components start/finish +0x44/+0x48, ThingTypeFlags +0x6C, Dead +0x91; CPlayer ZTargeting
// +0x20A; targeting slot 33). Its out-of-line callees are test probes: SetCurrentAction / PeekPlayer as members, and the two
// CCreatureAction_CastAssassinRush constructors and ~CCombatActionBase as __fastcall probes bound by /alternatename (test only).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_USE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_UNTARGETEDCTOR
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_INITIALISE
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CPlayer.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_CastAssassinRush.h"
#include <stdio.h>
#include <string.h>
#pragma comment(linker, "/alternatename:??0CCreatureAction_CastAssassinRush@@QAE@AAVCThingCreatureBase@@AAVCThing@@_NJV?$CCountedPointer@VCCreatureActionBase@@@@@Z=@ProbeTargetedCtor@32")
#pragma comment(linker, "/alternatename:??0CCreatureAction_CastAssassinRush@@QAE@AAVCThingCreatureBase@@JV?$CCountedPointer@VCCreatureActionBase@@@@@Z=@ProbeUntargetedCtor@24")
#pragma comment(linker, "/alternatename:??1CCombatActionBase@@UAE@XZ=@ProbeBaseDtor@8")
static int g_targeted, g_untargeted, g_dtors, g_sets;
static void* g_target_arg; static void* g_creature_arg; static int g_flag_arg; static long g_level_arg;
static const void* g_set_this; static const void* g_set_action; static void* g_built; static void* g_destroyed;
static unsigned char g_player[0x220];
extern "C" void* __fastcall ProbeTargetedCtor(void* self, int, void* creature, void* target, int flag, long level, unsigned long, unsigned long)
{
    memset(self, 0, sizeof(CCreatureAction_CastAssassinRush));
    ++g_targeted; g_creature_arg = creature; g_target_arg = target; g_flag_arg = flag & 0xff; g_level_arg = level; g_built = self;
    return self;
}
extern "C" void* __fastcall ProbeUntargetedCtor(void* self, int, void* creature, long level, unsigned long, unsigned long)
{
    memset(self, 0, sizeof(CCreatureAction_CastAssassinRush));
    ++g_untargeted; g_creature_arg = creature; g_level_arg = level; g_built = self;
    return self;
}
extern "C" void __fastcall ProbeBaseDtor(void* self, int) { ++g_dtors; g_destroyed = self; }
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action) { ++g_sets; g_set_this = this; g_set_action = &action; return true; }
const CPlayer& CThing::PeekPlayer() const { return *(const CPlayer*)g_player; }
typedef bool (CTCSpecialAbilities::*TSetup)();
static TSetup g_setup;
template<TSetup P> struct CSetupPeek { CSetupPeek() { g_setup = P; } static CSetupPeek Instance; };
template<TSetup P> CSetupPeek<P> CSetupPeek<P>::Instance;
template struct CSetupPeek<&CTCSpecialAbilities::SetupAssassinRush>;
class CProbe { public: CThing* Target() const { return (CThing*)g_target_arg_slot; } static void* g_target_arg_slot; };
void* CProbe::g_target_arg_slot = 0;
typedef CThing* (CProbe::*TProbe)() const;
static void* ProbeAddress(TProbe probe) { union { TProbe member; void* address; } u; u.member = probe; return u.address; }
static unsigned char g_creature[0x400], g_target[0x100], g_abilities[0x28];
static void* g_table[0x40];
static unsigned char g_targeting[0x10];
static unsigned long g_components[2];
static void SetWord(unsigned char* p, int off, unsigned long v) { memcpy(p + off, &v, 4); }
static void Reset()
{
    memset(g_creature, 0, sizeof(g_creature)); memset(g_target, 0, sizeof(g_target)); memset(g_player, 0, sizeof(g_player));
    g_targeted = g_untargeted = g_dtors = g_sets = 0; g_built = g_destroyed = 0; g_set_this = g_set_action = 0; g_flag_arg = -1; g_level_arg = -1;
    void* c = g_creature; memcpy(g_abilities + 0x18, &c, 4);
}
static bool Run() { return (((CTCSpecialAbilities*)g_abilities)->*g_setup)(); }
int main()
{
    int ok = 1;
    // 1: neither 0xA9 nor 0xA8, no targeting component (bit 8 clear) -> the untargeted action, level 0, set on the creature, destroyed.
    Reset();
    bool r1 = Run();
    ok &= r1 && g_untargeted == 1 && g_targeted == 0 && g_creature_arg == g_creature && g_level_arg == 0 && g_sets == 1 &&
          g_set_this == g_creature && g_set_action == g_built && g_dtors == 1 && g_destroyed == g_built;
    // 2: interface 0xA9 (assassin rush) present -> false, nothing built.
    Reset(); SetWord(g_creature, 0x34, 1ul << 9);
    bool r2 = Run(); ok &= !r2 && g_untargeted == 0 && g_targeted == 0 && g_sets == 0;
    // 3: interface 0xA8 (battle charge) present -> false, nothing built.
    Reset(); SetWord(g_creature, 0x34, 1ul << 8);
    bool r3 = Run(); ok &= !r3 && g_untargeted == 0 && g_targeted == 0 && g_sets == 0;
    // 4: a targeting component (interface 8) whose slot 33 gives a live creature target; player-controlled hero Z-targeting -> targeted, flag 1.
    Reset(); SetWord(g_creature, 0x20, 1ul << 8);
    g_components[0] = 8; g_components[1] = (unsigned long)g_targeting;
    SetWord(g_creature, 0x44, (unsigned long)&g_components[0]); SetWord(g_creature, 0x48, (unsigned long)&g_components[2]);
    for (int i = 0; i < 0x40; ++i) g_table[i] = 0;
    g_table[33] = ProbeAddress(&CProbe::Target); SetWord(g_targeting, 0, (unsigned long)g_table);
    CProbe::g_target_arg_slot = g_target; SetWord(g_target, 0x6C, 8);
    SetWord(g_creature, 0x6C, 0x20); g_player[0x20A] = 1;
    bool r4 = Run();
    ok &= r4 && g_targeted == 1 && g_untargeted == 0 && g_target_arg == g_target && g_flag_arg == 1 && g_level_arg == 0 && g_sets == 1 &&
          g_dtors == 1 && g_destroyed == g_built;
    // 5: same, not player-controlled -> flag 0.
    SetWord(g_creature, 0x6C, 0); g_targeted = g_sets = g_dtors = 0;
    bool r5 = Run(); ok &= r5 && g_targeted == 1 && g_flag_arg == 0;
    // 6: the target is dead -> untargeted.
    g_target[0x91] = 0xff; g_targeted = g_untargeted = 0;
    bool r6 = Run(); ok &= r6 && g_untargeted == 1 && g_targeted == 0;
    // 7: the target is not a creature (type flag 8 clear) -> untargeted.
    g_target[0x91] = 0; SetWord(g_target, 0x6C, 0); g_targeted = g_untargeted = 0;
    bool r7 = Run(); ok &= r7 && g_untargeted == 1 && g_targeted == 0;
    // 8: no target -> untargeted.
    CProbe::g_target_arg_slot = 0; g_targeted = g_untargeted = 0;
    bool r8 = Run(); ok &= r8 && g_untargeted == 1 && g_targeted == 0;
    if (ok) { printf("0072caa0_TEST PASS\n"); return 0; }
    printf("0072caa0_TEST FAIL %d%d%d%d%d%d%d%d\n", r1, r2, r3, r4, r5, r6, r7, r8);
    return 1;
}
