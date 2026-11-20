// NScript::CV_BanditTollScript::CTollBandit::Should_I_Flee(CScriptGameResourceObjectScriptedThingBase&) -- once the bandits flee, take scripting control of the bandit, run him to the escape marker until he is close to it, then fade him out and kill him
// Leaf: ?Should_I_Flee@CTollBandit@CV_BanditTollScript@NScript@@AAEXAAVCScriptGameResourceObjectScriptedThingBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4DF20, 433 bytes (448 with padding), `ret 4`.
// Identity (two sources): Ego_r.pdb + FableWin.pdb
//   ?Should_I_Flee@CTollBandit@CV_BanditTollScript@NScript@@AAEXAAVCScriptGameResourceObjectScriptedThingBase@@@Z (private; ghidra_out/
//   bsim_port_audit_final.tsv gives the same spelling); the landed CTollBandit::Main 0x00E4B5B0 calls it with seh_me at the end of every state
//   loop and binds this spelling through engine/CV_BanditTollScript.h (FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT declares it).
//   Name-fix NF186 (q6528): was a global `class CTollBandit` local model with local CEntityScriptBase / CV_BanditTollScript /
//   CScriptGameResourceObjectScriptedThingBase stand-ins (leaf ?Should_I_Flee@CTollBandit@@QAEXAAVCScriptGameResourceObjectScriptedThingBase@@@Z;
//   catalog CTollBandit::Should_I_Flee).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, the handle's IsValid 0x00CD23B9 / Release
//   0x00CD2770 and virtuals MoveToPosition (slot 4), GetScriptThing (slot 12), IsPerformingScriptTask (slot 26),
//   IsDistanceFromThingToPositionOver 0x00CBE45C and FadeOutAndKillEntity 0x00CBE716 (`__fastcall`), ~CScriptThing
//   0x004AA840; through the interface (+4) NewScriptFrame (slot 7), StartScriptingEntity (slot 8, SCRIPT_AI_PRIORITY_HIGHEST = 4);
//   BanditEscapeMarker's GetPos (CScriptThing slot 6, virtual on the member). Float 2.0f 0x0122DCDC.
// Layout: engine/CV_BanditTollScript.h (CTollBandit: ParentClass +0x14, BanditEscapeMarker +0x28; CV_BanditTollScript BanditsFlee +0x4f);
//   CScriptDef TollBanditCloseToExit +0x43c, TollBanditTimeToFade +0x440 (retail; PDB +0x470/+0x474).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_BanditTollScript.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/EScriptEntityMoveType.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_fade.h"                 // FadeOutAndKillEntity 0x00CBE716
#include "engine/fable_script_def_global.h"           // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>)
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
namespace NScript {
void CV_BanditTollScript::CTollBandit::Should_I_Flee(CScriptGameResourceObjectScriptedThingBase& bandit)
{
    if (!ParentClass.BanditsFlee)
        return;
    if (IsActiveThreadTerminating())
        return;
    if (bandit.IsValid())
        bandit.Release();
    while (!Game->StartScriptingEntity(Me, bandit, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    float closeToExit = pGlobalScriptDef->TollBanditCloseToExit;
    C3DVector exitPos = BanditEscapeMarker.GetPos();
    float range = closeToExit + 2.0f;
    while (IsDistanceFromThingToPositionOver(bandit.GetScriptThing(), exitPos, range)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        bandit.MoveToPosition(exitPos, closeToExit, ENTITY_MOVE_RUN, false, true);
        while (bandit.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    FadeOutAndKillEntity(Me, pGlobalScriptDef->TollBanditTimeToFade, true, true);
}
}
