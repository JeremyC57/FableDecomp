// Self-contained behaviour test for 0x00859EF0 CCombatActionBase::OnAnimationLoaded (the copy vland links): the row's source text,
// verbatim below, compiled in this TU over the same stand-ins, fixtures and scenarios as test/00859ef0_realobj.cpp (which links the
// REAL row object at mut.py's fixed compile path and carries the mutant run). The row object cannot be linked from an arbitrary path:
// its GetAllThings instance calls the anonymous-namespace CThingFilter_IsValidCombatSpline::operator(), whose ?A0x<hash> name depends on
// the compile path (the 0x006F2DE0 / 0x0055F710 precedent). Here the filter's operator() is defined in the same TU as a stand-in.
// ---- row source (verbatim) ----
// CCombatActionBase::OnAnimationLoaded() -- once the action's animation is loaded: set up the strike events / interrupts / action times; tell a live target's hit-notification component who is attacking; record the animation's end handedness on the creature; count a boss's strikes (strike actions) or reset the combo chain (others); face a live target at the start; then, for a melee-knockdown animation, give a live target a flourish-target component and, from the melee ability's knockdown-effects def, start a scripted spline camera on a random valid combat spline and blend in the environment theme
// Leaf: ?OnAnimationLoaded@CCombatActionBase@@MAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00859EF0, 1196 bytes (plain `ret`). Ego_r.pdb ?OnAnimationLoaded@CCombatActionBase@@MAEXXZ.
// Grade: RELOCATION_MATCH (1196/1196). No flag, no pragma, no inline device. Text: Ego_d.pdb's locals and scopes (this, ptc_hit_not, combat_anim,
//   pcombat, panim, phand; { ptc_boss } { tc_combat } { direction } { ptc_flourish_target { pkofxdef { things, pcam_man { pspline, pcamera_point } } } }).
// The handedness argument is CAnimComponentCombatHandedness::GetEndHandedness() (Ego_d.pdb ?GetEndHandedness@..@@QBE?AW4ECombatAnimationHandedness@@XZ,
//   inline; opt-in FABLE_RETAIL_CANIMCOMPONENTCOMBATHANDEDNESS_GETTERS): retail reads phand->EndHandedness before `PCreature->` (0x00859FAC, pushed
//   ahead of the GetPItem call 0x00859FB9). The direct member read with a cast, `(ECombatAnimationHandedness)phand->EndHandedness`, is evaluated
//   after it and grades DIFFER(1195v1196) with the vtable / lea register names shifted to 0x0085A26B. Ego_d line 1530 (30 B at /Od) fits the call.
// Callees (two sources each; NAME_FIXES.tsv for the landed spellings): SetupCombatStrikeEvents 0x00859A70, SetupCombatInterrupts 0x00858A20,
//   InitialiseActionTimes 0x00856A20, UpdateFacingDirection 0x00856B20, CTCCreatureHitNotification::SetHitNotification 0x008651B0,
//   CThingCreatureBase::SetLastActionHandedness 0x006619F0, CTCBoss::IncrementStrikesSinceHitHero 0x007AED30, CTCCombat::ResetComboChainPos
//   0x00733210, CTCFlourishTarget::SetAttacker 0x0085AB00, GetComponent<..> 0x00669CB0 / 0x00738A00, GetDef<CMeleeCombatKnockdownEffects>
//   0x0085ABB0, GetAllThings<CThing, vector<CThing*>, CThingFilter_IsValidCombatSpline> 0x0085B0D0 (filter operator() 0x0085AFD0),
//   NCameraModes::CScriptModuleSpline(long, long) 0x00852710, SetPCameraPoint 0x0072A4A0, SetPThingOrigin 0x00852750,
//   CGameCameraManager::SetGameScriptedCameraMode 0x00697B70, CEnvironment::TransitionToThemeAllChannels 0x006B38D0.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKTHINGSEARCHTOOLS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETHANDEDNESS
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT_BODY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_CANIMCOMPONENTCOMBATHANDEDNESS_GETTERS
#define FABLE_RETAIL_CTCBOSS_INCREMENTSTRIKESSINCEHITHERO
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_RESETCOMBO
#define FABLE_RETAIL_CTCFLOURISHTARGET_SETTERS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SETGAMESCRIPTEDMODE
#define FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULEBASE_SETTERS
#define FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULESPLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThingSearchTools.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentCombatHandedness.h"
#include "engine/CAnimComponentCombatMisc.h"
#include "engine/CTCBoss.h"
#include "engine/CTCCombat.h"
#include "engine/CTCFlourishTarget.h"
#include "engine/CThingSearchTools.h"
#include "engine/GFRandom.h"
#include "engine/NCameraModes_CScriptModuleBase.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"

class CTCCreatureHitNotification : public CTCBase {                   // no engine header (header debt); Ego_r.pdb names
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CREATURE_HIT_NOTIFICATION; }
    void SetHitNotification(const CThing& attacker);                  // 0x008651B0 ?SetHitNotification@CTCCreatureHitNotification@@QAEXABVCThing@@@Z
};

class CMeleeCombatKnockdownEffects : public CParentDefClassBase { public:   // RTTI : CParentDefClassBase; Ego_r.pdb members (retail offsets)
    long BlendInFrames;                                     // +0x3c
    long BlendOutFrames;                                    // +0x40
    long AttackerLightingChannel;                           // +0x44
    long TargetLightingChannel;                             // +0x48
    long EnvironmentTheme;                                  // +0x4c
    bool CameraFOVChange;                                   // +0x50
    bool CameraPan;                                         // +0x51
    float AttackerAndTargetSpeedMultiplier;                 // +0x54
    bool PauseEntities;                                     // +0x58
    bool NonAttackingEntities;                              // +0x59
    bool AttackerInvulnerable;                              // +0x5a
};
class CMeleeCombatAbilityDef : public CParentDefClassBase { public:   // RTTI : CParentDefClassBase
    unsigned char _pad3c[0x60 - 0x3c];                      // FlourishAnimName .. ValidWeaponClasses, Decapitate
    long KnockDownEffects;                                  // +0x60
};

namespace {
class CThingFilter_IsValidCombatSpline {                     // Ego_r.pdb `anonymous namespace'::CThingFilter_IsValidCombatSpline (8 bytes)
public:
    CThingFilter_IsValidCombatSpline(long anim_index, CThing& thing) : AnimIndex(anim_index), Thing(&thing) { }
    bool operator()(const CThing& thing) const;             // 0x0085AFD0
private:
    long AnimIndex;                                         // +0x00
    CThing* Thing;                                          // +0x04
};
}

// Fidelity: same-TU strong definition of 0x00859EC0 (retail TU-mate)
// The landed body differs only in one member spelling: HasPerformAction (+0xDE, the MELEE header) for the landed local model's m_keepActionTime (NAME_FIXES.tsv).
void CCombatActionBase::SetUpCombatAnimationNotes()
{
    SetupCombatStrikeEvents();
    SetupCombatInterrupts();
    if (StrikeEvents.begin() != StrikeEvents.end() && !HasPerformAction)
        ActionTime = -1;
}

void CCombatActionBase::OnAnimationLoaded()
{
    SetUpCombatAnimationNotes();
    InitialiseActionTimes();
    CTCCreatureHitNotification* ptc_hit_not;
    if (IsStrikeAction() && IS_THING_ALIVE(PTarget) && PTarget->GetTC(&ptc_hit_not))
        ptc_hit_not->SetHitNotification(*PCreature);
    const CAnimationEntry* panim = PAnimEntry.Get();
    const CAnimComponentCombatHandedness* phand;
    panim->GetComponent(&phand);
    PCreature->SetLastActionHandedness(phand->GetEndHandedness());
    if (IsStrikeAction()) {
        CTCBoss* ptc_boss;
        if (PCreature->GetTC(&ptc_boss))
            ptc_boss->IncrementStrikesSinceHitHero();
    } else {
        CTCCombat* tc_combat;
        if (PCreature->GetTC(&tc_combat))
            tc_combat->ResetComboChainPos();
    }
    if (IsToFaceTargetAtStart() && IS_THING_ALIVE(PTarget) && PCreature.GetPItem() != PTarget.GetPItem()) {
        C3DVector direction = PTarget->GetPos() - PCreature->GetPos();
        PCreature->GetPhysicsTC()->SetFacingAngleXY(direction.GetAngleXY());
    }
    UpdateFacingDirection();
    const CAnimationEntry* combat_anim = PAnimEntry.Get();
    const CAnimComponentCombatMisc* pcombat;
    combat_anim->GetComponent(&pcombat);
    if (pcombat->MeleeKnockdown) {
        CTCFlourishTarget* ptc_flourish_target = NULL;
        if (IS_THING_ALIVE(PTarget)) {
            PTarget->RemoveTC(TCI_FLOURISH_TARGET);
            ptc_flourish_target = (CTCFlourishTarget*)PTarget->AddTC("CTCFlourishTarget", true, NULL);
            ptc_flourish_target->SetAttacker(*PCreature);
        }
        if (!PMeleeAbilityDef.IsNull() && PMeleeAbilityDef->KnockDownEffects >= 0) {
            CDefPointer<const CMeleeCombatKnockdownEffects> pkofxdef;
            PeekDefinitionManager().GetDef(PMeleeAbilityDef->KnockDownEffects, &pkofxdef);
            if (pkofxdef->CameraPan && !PCreature->HasTC(TCI_BULLET_TIME)) {
                CGameCameraManager* pcam_man;
                GetWorld().GetGameCamera()->GetCameraManager(&pcam_man);
                std::vector<CThing*> things;
                PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_IsValidCombatSpline(GetAnimationIndex(), *PCreature), things);
                if (!things.empty() && IS_THING_ALIVE(PTarget)) {
                    CThing* pcamera_point = things[GFRandom(things.size(), GetWorld().GetWorldSeed())];
                    NCameraModes::CScriptModuleSpline* pspline = new NCameraModes::CScriptModuleSpline(0, -1);
                    pspline->SetPCameraPoint(*pcamera_point);
                    pspline->SetPThingOrigin(*PCreature);
                    pcam_man->SetGameScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(pspline), true);
                    PCameraPoint = pcamera_point;
                    if (ptc_flourish_target) {
                        ptc_flourish_target->SetAsCullAllYAxisMovement();
                        ptc_flourish_target->SetAsCullNegativeXAxisMovement();
                    }
                }
            }
            if (pkofxdef->EnvironmentTheme > 0)
                GetWorld().GetEnvironment().TransitionToThemeAllChannels(2, pkofxdef->EnvironmentTheme, (float)pkofxdef->BlendInFrames);
        }
    }
}

// ---- fixtures ----
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("@CallRow@8", "?OnAnimationLoaded@CCombatActionBase@@MAEXXZ")
#pragma ALT("?SetupCombatStrikeEvents@CCombatActionBase@@IAEXXZ", "@SetupStrike@8")
#pragma ALT("?SetupCombatInterrupts@CCombatActionBase@@IAEXXZ", "@SetupInterrupts@8")
#pragma ALT("?InitialiseActionTimes@CCombatActionBase@@IAEXXZ", "@InitTimes@8")
#pragma ALT("?UpdateFacingDirection@CCombatActionBase@@IAEXXZ", "@UpdateFacing@8")
#pragma ALT("?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ", "@IPGet@8")
#pragma ALT("??1CBaseIntelligentPointer@@UAE@XZ", "@IPDtor@8")
#pragma ALT("??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@PBVCThing@@@Z", "@IPCtor@12")
#pragma ALT("??4?$CIntelligentPointer@$$CBVCThing@@@@QAEAAV0@ABV0@@Z", "@IPAssign@12")
#pragma ALT("?SetHitNotification@CTCCreatureHitNotification@@QAEXABVCThing@@@Z", "@SetHitNot@12")
#pragma ALT("?SetLastActionHandedness@CThingCreatureBase@@QAEXW4ECombatAnimationHandedness@@@Z", "@SetHand@12")
#pragma ALT("?IncrementStrikesSinceHitHero@CTCBoss@@QAEXXZ", "@IncStrikes@8")
#pragma ALT("?ResetComboChainPos@CTCCombat@@QAEXXZ", "@ResetCombo@8")
#pragma ALT("?GetPos@CThing@@QBEABVC3DVector@@XZ", "@GetPos@8")
#pragma ALT("?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z", "@RemoveTC@12")
#pragma ALT("??0CCharString@@QAE@PBDJ@Z", "@CSCtor@16")
#pragma ALT("??1CCharString@@QAE@XZ", "@CSDtor@8")
#pragma ALT("?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z", "@AddTC@20")
#pragma ALT("?SetAttacker@CTCFlourishTarget@@QAEXABVCThing@@@Z", "@SetAttacker@12")
#pragma ALT("?PeekDefinitionManager@CCreatureActionBase@@IBEABVCGameDefinitionManager@@XZ", "@PeekDefMan@8")
#pragma ALT("?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z", "@GetPDef@16")
#pragma ALT("?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ", "@GetWorld@8")
#pragma ALT("?GetWorldSeed@CWorld@@QBEPAKXZ", "@GetSeed@8")
#pragma ALT("?GFRandom@@YIKKPAK@Z", "@Random@8")
#pragma ALT("?PeekThingSearchTools@CCreatureActionBase@@IBEABVCThingSearchTools@@XZ", "@PeekTools@8")
#pragma ALT("?PeekClassList@CThingSearchTools@@IBEABV?$list@PAVCClass@@V?$allocator@PAVCClass@@@std@@@std@@XZ", "@PeekList@8")
#pragma ALT("??0CScriptModuleSpline@NCameraModes@@QAE@JJ@Z", "@SplineCtor@16")
#pragma ALT("?SetPCameraPoint@CScriptModuleBase@NCameraModes@@QAEXAAVCThing@@@Z", "@SetCamPoint@12")
#pragma ALT("?SetPThingOrigin@CScriptModuleBase@NCameraModes@@QAEXABVCThing@@@Z", "@SetOrigin@12")
#pragma ALT("??0?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@QAE@PAVCScriptModuleBase@NCameraModes@@@Z", "@CPCtor@12")
#pragma ALT("?SetGameScriptedCameraMode@CGameCameraManager@@QAEXV?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@_N@Z", "@SetMode@20")
#pragma ALT("?TransitionToThemeAllChannels@CEnvironment@@QAEXJJM@Z", "@Theme@20")
#pragma ALT("?RegisteredComponentDefaults@CAnimationEntry@@2V?$CVectorMap@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@V?$CKeyPairCompareLess@W4EAnimComponent@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@@@@A", "_g_componentDefaults")

static int g_fail = 0, g_scn = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (scenario %d)\n", line, g_scn); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

static char g_trace[2048];
static void Tr(const char* s) { if (strlen(g_trace) + strlen(s) + 2 < sizeof(g_trace)) { strcat(g_trace, s); strcat(g_trace, " "); } }

struct FxVec { float x, y, z; };
struct FxIP { void* vtbl; void* item; };
struct FxTCPair { long type; void* tc; };
struct FxThing {                                   // retail CThing offsets the row and its COMDATs read
    unsigned char b[0x100];
    FxVec pos;
    FxTCPair tcs[4];
    int ntc;
};
static unsigned long& Word(FxThing& t, int off) { return *(unsigned long*)(t.b + off); }
static void AddTCEntry(FxThing& t, long type, void* tc)
{
    int i = t.ntc++;
    while (i > 0 && t.tcs[i - 1].type > type) { t.tcs[i] = t.tcs[i - 1]; --i; }
    t.tcs[i].type = type; t.tcs[i].tc = tc;
    unsigned long* bits = (unsigned long*)(t.b + 0x20);
    bits[type / 32] |= 1ul << (type % 32);
    *(FxTCPair**)(t.b + 0x44) = t.tcs; *(FxTCPair**)(t.b + 0x48) = t.tcs + t.ntc; *(FxTCPair**)(t.b + 0x4C) = t.tcs + 4;
}
static void InitThing(FxThing& t, float x, float y)
{
    memset(&t, 0, sizeof t);
    t.pos.x = x; t.pos.y = y; t.pos.z = 0.0f;
    *(FxTCPair**)(t.b + 0x44) = t.tcs; *(FxTCPair**)(t.b + 0x48) = t.tcs; *(FxTCPair**)(t.b + 0x4C) = t.tcs + 4;
    t.b[0x91] = 0x0C;                            // included in thing scans + activated, alive, not in limbo
}

// fixtures
static unsigned char g_action[0x100];
static void* g_actionVtbl[80];
static FxThing g_creature, g_target, g_spline[4];
static unsigned char g_hitTC[8], g_bossTC[8], g_combatTC[8], g_flourish[0x18];
static unsigned char g_physics[8]; static void* g_physicsVtbl[70];
static unsigned char g_hand[0x10], g_misc[0x10];
static FxTCPair g_compEntries[2];
extern "C" { void* g_componentDefaults[4]; }
static unsigned char g_entry[0x10];
static struct FxDef { void** vtbl; long refs; unsigned char p08[0x34]; long blendIn; unsigned char p40[0xC]; long theme; unsigned char p50;
                    bool cameraPan; unsigned char p52[0xE]; } g_kodef;
typedef char DefCheck[(size_t)&((FxDef*)0)->blendIn == 0x3C && (size_t)&((FxDef*)0)->theme == 0x4C && (size_t)&((FxDef*)0)->cameraPan == 0x51 ? 1 : -1];
static void* g_defVtbl[4];
static unsigned char g_meleeDef[0x64];
static unsigned char g_defMan[4], g_tools[4], g_world[0x40], g_env[4], g_camMan[4];
static unsigned char g_camera[4]; static void* g_cameraVtbl[12];
static unsigned long g_seed = 77;
struct FxNode { FxNode* next; FxNode* prev; void* data; };
static FxNode g_listHead, g_nodes[4];
static FxNode g_strikeHead, g_strikeNode;
static unsigned char* g_newSpline;
static float g_angle; static int g_angleCalls;
static bool P_strike, P_face, P_filter[4]; static long P_animIndex; static unsigned long P_random;

extern "C" {
void __fastcall CallRow(void* a, int);
void __fastcall SetupStrike(void* a, int) { CHECK(a == g_action); Tr("SE"); }
void __fastcall SetupInterrupts(void* a, int) { CHECK(a == g_action); Tr("SI"); }
void __fastcall InitTimes(void* a, int) { CHECK(a == g_action); Tr("IT"); }
void __fastcall UpdateFacing(void* a, int) { CHECK(a == g_action); Tr("UF"); }
void* __fastcall IPGet(FxIP* ip, int) { return ip->item; }
void __fastcall IPDtor(FxIP* ip, int) { Tr("ipdtor"); }
FxIP* __fastcall IPCtor(FxIP* ip, int, void* item) { ip->vtbl = 0; ip->item = item; Tr("ipctor"); return ip; }
FxIP* __fastcall IPAssign(FxIP* ip, int, FxIP* other) { CHECK(ip == (FxIP*)(g_action + 0xB0)); ip->item = other->item; Tr("ipassign"); return ip; }
void __fastcall SetHitNot(void* tc, int, FxThing* attacker) { CHECK(tc == g_hitTC && attacker == &g_creature); Tr("hit"); }
void __fastcall SetHand(FxThing* c, int, long h) { CHECK(c == &g_creature); char s[16]; sprintf(s, "hand%ld", h); Tr(s); }
void __fastcall IncStrikes(void* tc, int) { CHECK(tc == g_bossTC); Tr("boss"); }
void __fastcall ResetCombo(void* tc, int) { CHECK(tc == g_combatTC); Tr("combo"); }
FxVec* __fastcall GetPos(FxThing* t, int) { return &t->pos; }
void __fastcall RemoveTC(FxThing* t, int, long type) { CHECK(t == &g_target && type == 0x100); Tr("removetc"); }
void* __fastcall CSCtor(void* cs, int, const char* s, long n) { CHECK(n == -1); *(const char**)cs = s; return cs; }
void __fastcall CSDtor(void* cs, int) { Tr("csdtor"); }
void* __fastcall AddTC(FxThing* t, int, void* cs, int force, void* params)
{
    CHECK(t == &g_target && strcmp(*(const char**)cs, "CTCFlourishTarget") == 0 && (force & 0xFF) == 1 && params == 0);
    Tr("addtc"); return g_flourish;
}
void __fastcall SetAttacker(void* tc, int, FxThing* attacker) { CHECK(tc == g_flourish && attacker == &g_creature); Tr("attacker"); }
void* __fastcall PeekDefMan(void* a, int) { CHECK(a == g_action); return g_defMan; }
void** __fastcall GetPDef(void* man, int, void** ret, long index)
{
    CHECK(man == g_defMan); char s[16]; sprintf(s, "getdef%ld", index); Tr(s);
    *ret = index == 5 ? &g_kodef : 0;
    if (*ret) ++g_kodef.refs;
    return ret;
}
void __fastcall DefDestroy(void* d, int) { Tr("defdestroy"); }
void* __fastcall GetWorld(void* a, int) { CHECK(a == g_action); return g_world; }
unsigned long* __fastcall GetSeed(void* w, int) { CHECK(w == g_world); return &g_seed; }
unsigned long __fastcall Random(unsigned long range, unsigned long* seed) { CHECK(seed == &g_seed); char s[16]; sprintf(s, "rand%lu", range); Tr(s); return P_random; }
void* __fastcall PeekTools(void* a, int) { CHECK(a == g_action); return g_tools; }
FxNode** __fastcall PeekList(void* tools, int) { CHECK(tools == g_tools); static FxNode* head; head = &g_listHead; return &head; }
bool __fastcall Filter(long* f, int, FxThing* t)
{
    CHECK(f[0] == P_animIndex && (FxThing*)f[1] == &g_creature);
    int i = (int)(t - g_spline); CHECK(i >= 0 && i < 4);
    char s[16]; sprintf(s, "filter%d", i); Tr(s); return P_filter[i];
}
void* __fastcall SplineCtor(unsigned char* p, int, long a, long b) { CHECK(a == 0 && b == -1); g_newSpline = p; Tr("spline"); return p; }
void __fastcall SetCamPoint(void* s, int, FxThing* t) { CHECK(s == g_newSpline); char b[16]; sprintf(b, "campoint%d", (int)(t - g_spline)); Tr(b); }
void __fastcall SetOrigin(void* s, int, FxThing* t) { CHECK(s == g_newSpline && t == &g_creature); Tr("origin"); }
void** __fastcall CPCtor(void** cp, int, void* p) { CHECK(p == g_newSpline); cp[0] = p; cp[1] = (void*)0x5150; Tr("cpctor"); return cp; }
struct FxCP { void* p; void* q; };
void __fastcall SetMode(void* man, int, FxCP cp, int flag) { CHECK(man == g_camMan && cp.p == g_newSpline && cp.q == (void*)0x5150 && (flag & 0xFF) == 1); Tr("setmode"); free(cp.p); }
void __fastcall Theme(void* env, int, long channel, long theme, float frames) { CHECK(env == g_env); char s[48]; sprintf(s, "theme%ld,%ld,%g", channel, theme, frames); Tr(s); }
}
static long __fastcall VAnimIndex(void* a, int) { CHECK(a == g_action); return P_animIndex; }
static bool __fastcall VIsStrike(void* a, int) { CHECK(a == g_action); Tr("isstrike"); return P_strike; }
static bool __fastcall VFace(void* a, int) { CHECK(a == g_action); Tr("isface"); return P_face; }
static void __fastcall VSetFacing(void* p, int, float angle) { CHECK(p == g_physics); g_angle = angle; ++g_angleCalls; Tr("facing"); }
static bool __fastcall VGetCamMan(void* cam, int, void** out) { CHECK(cam == g_camera); *out = g_camMan; Tr("cammanager"); return true; }

static void Setup()
{
    memset(g_action, 0, sizeof g_action);
    *(void***)g_action = g_actionVtbl;
    g_actionVtbl[7] = (void*)VAnimIndex; g_actionVtbl[15] = (void*)VIsStrike; g_actionVtbl[74] = (void*)VFace;
    InitThing(g_creature, 1.0f, 2.0f); InitThing(g_target, 4.0f, 6.0f);
    ((FxIP*)(g_action + 0x08))->item = &g_creature;
    ((FxIP*)(g_action + 0xA8))->item = &g_target;
    *(unsigned char**)(g_action + 0x74) = g_entry;
    g_strikeHead.next = &g_strikeHead; *(FxNode**)(g_action + 0xBC) = &g_strikeHead;
    *(long*)(g_action + 0x14) = 1234;
    memset(g_entry, 0, sizeof g_entry);
    g_compEntries[0].type = 3; g_compEntries[0].tc = g_hand; g_compEntries[1].type = 6; g_compEntries[1].tc = g_misc;
    g_componentDefaults[0] = g_compEntries; g_componentDefaults[1] = g_compEntries + 2; g_componentDefaults[2] = g_compEntries + 2;
    memset(g_hand, 0, sizeof g_hand); *(long*)(g_hand + 0xC) = 2;
    memset(g_misc, 0, sizeof g_misc);
    memset(g_flourish, 0, sizeof g_flourish);
    *(void**)(g_creature.b + 0x60) = g_physics; *(void***)g_physics = g_physicsVtbl; g_physicsVtbl[66] = (void*)VSetFacing;
    memset(&g_kodef, 0, sizeof g_kodef); g_kodef.vtbl = g_defVtbl; g_defVtbl[1] = (void*)DefDestroy; g_kodef.refs = 1;
    memset(g_meleeDef, 0, sizeof g_meleeDef); *(long*)(g_meleeDef + 0x60) = 5;
    *(void**)(g_action + 0xB8) = 0;
    *(void**)(g_world + 0x34) = g_camera; *(void***)g_camera = g_cameraVtbl; g_cameraVtbl[9] = (void*)VGetCamMan;
    *(void**)(g_world + 0x18) = g_env;
    g_listHead.next = g_listHead.prev = &g_listHead;
    for (int i = 0; i < 4; ++i) { InitThing(g_spline[i], 0, 0); P_filter[i] = false; }
    P_strike = P_face = false; P_animIndex = 17; P_random = 0;
    g_angleCalls = 0; g_newSpline = 0; g_trace[0] = 0;
}
static void SetList(int n)
{
    FxNode* prev = &g_listHead;
    for (int i = 0; i < n; ++i) { g_nodes[i].data = &g_spline[i]; prev->next = &g_nodes[i]; g_nodes[i].prev = prev; prev = &g_nodes[i]; }
    prev->next = &g_listHead; g_listHead.prev = prev;
}
static void Run(int scn, const char* expected)
{
    g_scn = scn;
    CallRow(g_action, 0);
    if (strcmp(g_trace, expected) != 0) { g_fail = 1; printf("scenario %d trace\n  got  %s\n  want %s\n", scn, g_trace, expected); }
}


// The filter's operator() (retail 0x0085AFD0) as a stand-in in this TU.
extern "C" bool __fastcall Filter(long* f, int, FxThing* t);
bool CThingFilter_IsValidCombatSpline::operator()(const CThing& thing) const { return Filter((long*)this, 0, (FxThing*)&thing); }

int main()
{
    // 1: neither strike nor facing, the creature has a combat TC; not a knockdown
    Setup(); AddTCEntry(g_creature, 0x30, g_combatTC);
    Run(1, "SE SI IT isstrike hand2 isstrike combo isface UF ");
    CHECK(*(long*)(g_action + 0x14) == 1234);
    // 2: strike events present and no perform action: ActionTime = -1; strike on a live target with a hit TC; boss TC on the creature
    Setup(); g_strikeHead.next = &g_strikeNode; AddTCEntry(g_creature, 0xAD, g_bossTC); AddTCEntry(g_target, 0xCD, g_hitTC);
    P_strike = true; *(long*)(g_hand + 0xC) = 1;
    Run(2, "SE SI IT isstrike hit hand1 isstrike boss isface UF ");
    CHECK(*(long*)(g_action + 0x14) == -1);
    // 3: strike events but HasPerformAction: ActionTime kept; strike, target dead (no hit), creature has no boss TC
    Setup(); g_strikeHead.next = &g_strikeNode; g_action[0xDE] = 1; g_target.b[0x91] |= 1; AddTCEntry(g_target, 0xCD, g_hitTC);
    P_strike = true;
    Run(3, "SE SI IT isstrike hand2 isstrike isface UF ");
    CHECK(*(long*)(g_action + 0x14) == 1234);
    // 4: strike, live target without the hit TC
    Setup(); P_strike = true;
    Run(4, "SE SI IT isstrike hand2 isstrike isface UF ");
    // 5: face a live target: angle of (3, 4) in turns
    Setup(); P_face = true;
    Run(5, "SE SI IT isstrike hand2 isstrike isface facing UF ");
    { float want = (float)atan2(3.0, 4.0) * 0.15915494f; CHECK(g_angleCalls == 1 && fabs(g_angle - want) < 1e-6f); }
    // 6: face a target behind-left: the angle wraps into [0, 1)
    Setup(); P_face = true; g_target.pos.x = -3.0f; g_target.pos.y = -2.0f;
    Run(6, "SE SI IT isstrike hand2 isstrike isface facing UF ");
    { float a = (float)atan2(-4.0, -4.0) * 0.15915494f; a = (float)fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f;
      CHECK(g_angleCalls == 1 && fabs(g_angle - a) < 1e-6f && g_angle >= 0.0f && g_angle < 1.0f); }
    // 7: facing, but the target is the creature itself
    Setup(); P_face = true; ((FxIP*)(g_action + 0xA8))->item = &g_creature;
    Run(7, "SE SI IT isstrike hand2 isstrike isface UF ");
    // 8: facing a dead target
    Setup(); P_face = true; g_target.b[0x91] |= 1;
    Run(8, "SE SI IT isstrike hand2 isstrike isface UF ");
    // 9: knockdown, live target: flourish TC replaced; no melee ability def
    Setup(); g_misc[9] = 1;
    Run(9, "SE SI IT isstrike hand2 isstrike isface UF removetc addtc csdtor attacker ");
    // 10: knockdown, dead target, melee def with no knockdown effects (-1)
    Setup(); g_misc[9] = 1; g_target.b[0x91] |= 1; *(void**)(g_action + 0xB8) = g_meleeDef; *(long*)(g_meleeDef + 0x60) = -1;
    Run(10, "SE SI IT isstrike hand2 isstrike isface UF ");
    // 11: knockdown effects def 5: no camera pan, environment theme 3 blended over 12 frames; the def reference is released again
    Setup(); g_misc[9] = 1; *(void**)(g_action + 0xB8) = g_meleeDef; g_kodef.theme = 3; g_kodef.blendIn = 12;
    Run(11, "SE SI IT isstrike hand2 isstrike isface UF removetc addtc csdtor attacker getdef5 theme2,3,12 ");
    CHECK(g_kodef.refs == 1);
    // 12: theme 0: no transition
    Setup(); g_misc[9] = 1; *(void**)(g_action + 0xB8) = g_meleeDef;
    Run(12, "SE SI IT isstrike hand2 isstrike isface UF removetc addtc csdtor attacker getdef5 ");
    CHECK(g_kodef.refs == 1);
    // 13: dead target, def 5 with a negative theme: no transition; the def's reference count is balanced again (2 -> 2)
    Setup(); g_misc[9] = 1; g_target.b[0x91] |= 1; *(void**)(g_action + 0xB8) = g_meleeDef; g_kodef.refs = 2; g_kodef.theme = -2;
    Run(13, "SE SI IT isstrike hand2 isstrike isface UF getdef5 ");
    CHECK(g_kodef.refs == 2);
    // 14: camera pan, but the creature is in bullet time
    Setup(); g_misc[9] = 1; *(void**)(g_action + 0xB8) = g_meleeDef; g_kodef.cameraPan = true; Word(g_creature, 0x30) |= 0x40000;
    Run(14, "SE SI IT isstrike hand2 isstrike isface UF removetc addtc csdtor attacker getdef5 ");
    // 15: camera pan, no valid spline: things stays empty (one excluded from scans, one dead, one in limbo, one refused by the filter)
    Setup(); g_misc[9] = 1; *(void**)(g_action + 0xB8) = g_meleeDef; g_kodef.cameraPan = true; SetList(4);
    g_spline[0].b[0x91] = 0x08; g_spline[1].b[0x91] = 0x0D; g_spline[2].b[0x91] = 0x2C;
    Run(15, "SE SI IT isstrike hand2 isstrike isface UF removetc addtc csdtor attacker getdef5 cammanager filter3 ");
    CHECK(g_newSpline == 0);
    // 16: camera pan, splines 1 and 3 pass the filter, random index 1 picks spline 3: scripted spline camera; the flourish target culls
    Setup(); g_misc[9] = 1; *(void**)(g_action + 0xB8) = g_meleeDef; g_kodef.cameraPan = true; g_kodef.theme = 4; g_kodef.blendIn = 9;
    SetList(4); P_filter[1] = P_filter[3] = true; P_random = 1; P_animIndex = 42;
    Run(16, "SE SI IT isstrike hand2 isstrike isface UF removetc addtc csdtor attacker getdef5 cammanager filter0 filter1 filter2 filter3 "
            "rand2 spline campoint3 origin cpctor setmode ipctor ipassign ipdtor theme2,4,9 ");
    CHECK(((FxIP*)(g_action + 0xB0))->item == &g_spline[3] && g_flourish[0x15] == 1 && g_flourish[0x14] == 1 && g_kodef.refs == 1);
    // 17: camera pan with valid splines but a dead target: no flourish TC, no spline camera
    Setup(); g_misc[9] = 1; g_target.b[0x91] |= 1; *(void**)(g_action + 0xB8) = g_meleeDef; g_kodef.cameraPan = true;
    SetList(2); P_filter[0] = true;
    Run(17, "SE SI IT isstrike hand2 isstrike isface UF getdef5 cammanager filter0 filter1 ");
    CHECK(g_newSpline == 0 && ((FxIP*)(g_action + 0xB0))->item == 0);
    // 18: one valid spline, random 0; the flourish target exists (live target)
    Setup(); g_misc[9] = 1; *(void**)(g_action + 0xB8) = g_meleeDef; g_kodef.cameraPan = true; SetList(1); P_filter[0] = true;
    Run(18, "SE SI IT isstrike hand2 isstrike isface UF removetc addtc csdtor attacker getdef5 cammanager filter0 "
            "rand1 spline campoint0 origin cpctor setmode ipctor ipassign ipdtor ");
    CHECK(((FxIP*)(g_action + 0xB0))->item == &g_spline[0]);
    if (g_fail) { printf("00859ef0_TEST FAIL\n"); return 1; }
    printf("00859ef0_TEST PASS\n");
    return 0;
}