// CV_ChapelOfEvilScript::TortureVictim(CScriptThing& victim, CScriptGameResourceObjectScriptedThingBase& victimObject, CScriptGameResourceObjectScriptedThingBase& heroObject) -- record the victim's health, sex, night time, Sunday and guard/bandit/trader flags, pick the torture sound, then play CS_CHAPEL_EVIL_TORTURE with HERO/VICTIM cast and the $SOUND argument while the master data's in-progress flag is set; clear it after one frame unless terminating
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6DA80, 0x389 bytes, `ret 0Ch`.
// Leaf: ?TortureVictim@CV_ChapelOfEvilScript@NScript@@QAEXAAVCScriptThing@@AAVCScriptGameResourceObjectScriptedThingBase@@1@Z
// Callees: CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0 / operator= 0x0099EFB0, __ftol2 0x00BFEA70, malloc 0x00BFEA0E,
//   free 0x00BFEA14, operator delete 0x00BFE9BC, CBaseObject::operator= 0x0099A3B0, the cast map (operator[] 0x00CD3D2E, erase
//   0x00CD344A) and the argument map (operator[] 0x009AC700, erase 0x009AA830), both constructed and destroyed inline,
//   RunCutsceneMacro 0x00CBFB7D (__fastcall), GetVictimIsGuard 0x00E6E050 / IsBandit 0x00E6E120 / IsTrader 0x00E6E1B0 (each
//   takes the victim's GetDefName(), CScriptThing virtual slot 2, by value), GetTortureSoundEffect 0x00E6DE10,
//   IsActiveThreadTerminating 0x00CB7940; Game slots 7, 264 GetHealth, 513 EntityGetSex, 647 GetTimeOfDay, 651 GetDayOfWeek.
// Shape: the landed CQ_OrchardFarmRaidScript::DoMultiplierCutscene 0x00DD1AF0 cast-map model, with __forceinline map
// NF87 (2026-11-17): moved onto the shared engine/CV_ChapelOfEvilScript.h (CLevelScriptBase<CQ_SunnyvaleMasterData>: Game +0x40,
//   MasterScriptData +0x44; Victim* +0x78..+0x84, TortureSound +0x88) with STLport std::map cast / argument maps (stlport/fable_map.h),
//   so the argument map's operator[] binds 0x009AC700 under its real spelling, as the landed shared-header cutscene rows
//   (0x00D6D670, 0x00D33730) spell RunCutsceneMacro and the cast map. The master-data flag at +0x10 is
//   CQ_SunnyvaleMasterData::HenchmanSacrificedBodge (Ego_r.pdb layout; the old row called it TortureInProgress).
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_ChapelOfEvilScript.h"
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_map.h"
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D

void NScript::CV_ChapelOfEvilScript::TortureVictim(CScriptThing& victim, CScriptGameResourceObjectScriptedThingBase& victimObject, CScriptGameResourceObjectScriptedThingBase& heroObject)
{
    VictimHealth = (long)Game->GetHealth(victim);
    VictimGender = Game->EntityGetSex(victim);
    VictimNightTime = Game->GetTimeOfDay() > 2159 || Game->GetTimeOfDay() < 600;
    VictimSunday = Game->GetDayOfWeek() == 6;
    VictimIsGuard = GetVictimIsGuard(victim.GetDefName());
    VictimIsBandit = GetVictimIsBandit(victim.GetDefName());
    VictimIsTrader = GetVictimIsTrader(victim.GetDefName());
    GetTortureSoundEffect(VictimGender);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["HERO"] = heroObject;
    cast["VICTIM"] = victimObject;
    std::map<CCharString, CCharString> args;
    args["$SOUND"] = TortureSound;
    MasterScriptData->HenchmanSacrificedBodge = true;
    RunCutsceneMacro("CS_CHAPEL_EVIL_TORTURE", cast, 0, &args, false, true);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    MasterScriptData->HenchmanSacrificedBodge = false;
}
