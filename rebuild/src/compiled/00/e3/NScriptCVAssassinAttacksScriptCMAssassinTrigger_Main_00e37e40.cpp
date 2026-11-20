// NScript::CV_AssassinAttacksScript::CM_AssassinTrigger::Main() -- the assassin-attack trigger marker: unless the hero is in Knothole Glade on the ransom quest, wait until the hero is within AssassinTriggerProximity of the marker; then, each time the spawn marker named by Me's data string has been off screen for AssassinCameraOffScreenTime, if the current region has no active assassin and is not the region of the most recent kill, spawn a CREATURE_ASSASSIN ("AttackingAssassin") at the marker, mark the region and give it the next AssassinObjects item
// Leaf: ?Main@CM_AssassinTrigger@CV_AssassinAttacksScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E37E40, 1103 bytes, thiscall, plain `ret`, no EH frame, default flags.
// Identity (proven, two sources): slot 1 (Main) of the RTTI-less entity vtable 0x012E36B8 (its only reference, 0x012E36BC), which the landed
//   CM_AssassinTrigger::Alloc 0x00E37D50 stores (slot 4 = the landed OnPersist 0x00E38D80); Ego_r.pdb
//   ?Main@CM_AssassinTrigger@CV_AssassinAttacksScript@NScript@@EAEXXZ (private virtual) and the Ego_d.pdb proc of the same name.
// Source shape: Ego_d.pdb line table (lines 209..306, ref egod_lines/) and locals: function scope `_Fail_Val` (TRetVoid), `ScriptTriggered`
//   (bool, line 213 = 4 bytes: `=false`; line 301 = 4 bytes: `=true` near the end); one block {data (CCharString), timer (CTimer), spawn_mkr
//   (CScriptThing), spawn_pos (C3DVector), {assassin (CScriptThing), {object (CCharString)}}}. Generated quest-script text as the landed
//   Mains 0x00E377D0 / 0x00DDAE40: `return _Fail_Val.Return();`, each wait loop with its trailing terminate test on one line.
// Callees: CScriptThing copy 0x004ABE90 (into the CIsThingAlive filter) / ~CScriptThing 0x004AA840, CEntityScriptBase::SetPredicate
//   0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, ~CIsThingAlive 0x00E241B0, CCharString(const char*, long) 0x0099EBF0 / copy
//   0x0099EC30 / operator= 0x0099EFB0 / operator!= 0x0099E900 / ~ 0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall),
//   CTimer::CTimer 0x00CD4450 / ~CTimer 0x00CD4470 (and expanded: pDSTGame 0x0143E8F8 slots 88 DeregisterTimer, 89 SetTimer (operator=),
//   90 GetTimer (GetTime)), CV_AssassinAttacksScript::GetRegionIdForRegion 0x00E38290 (by-value CCharString); Game slots 7 NewScriptFrame,
//   12 IsRegionLoaded, 15 GetRegionName, 70 GetHero, 72 GetThingWithScriptName, 91 CreateCreature, 284 IsQuestActive, 421
//   IsCameraPosOnScreen, 585 AddItemToContainer; CScriptThing slot 3 GetDataString (on Me) and slot 6 GetPos expanded on the exact-type
//   local (NullCoord 0x0143E8E0). Data: pGlobalScriptDef 0x0143E90C (AssassinTriggerProximity +0x39C, AssassinCameraOffScreenTime +0x3A8,
//   AssassinObjects +0x3C0); literals KnotholeGlade 0x012C513C, Q_RansomVictim 0x012C48AC, AttackingAssassin 0x012E3688,
//   CREATURE_ASSASSIN 0x012E36D4. Layout: Ego_r.pdb CM_AssassinTrigger (ParentClass +0x14, regionName +0x1C, regionId +0x20) and
//   CV_AssassinAttacksScript (AssassinObjectCount +0x4C, RegionOfMostRecentKill +0x54, RegionInfo[5] +0x58, 16 bytes each).
// Inline split (c2 static frequency, docs/pipeline/INLINE_DEPTH_REVISIT.md; measured with the c2 inline trace): ~CTimer is plain inline;
//   retail expands it at the loop-head exit (0x00E380F8, f 0.156) and the first wait loop's exit (0x00E3810C, f 0.078) and calls 0x00CD4470
//   at every other exit (f <= 0.023); the ctor (f 0.031) stays a call.
// Inferred: (1) the outer loop is `for(;;)` with the terminate check at its head as the statement macro `do{..}while(0);` (lever 4 of
//   INLINE_DEPTH_REVISIT: `while(!IsActiveThreadTerminating()){..}` gives the loop exit f 0.008, a call, and expands the second wait loop's
//   exit instead, 1104v1103; `for(;;)` with a plain head check the same); (2) `!ScriptTriggered&&` heads the trigger condition (the Ego_d
//   local; without it VC7.1 hoists the flag's `mov [esp+0Fh],0` above the `jne` -- retail keeps `jne ; mov 1 ; jmp ; mov 0`; the dead
//   `mov [esp+1Ch],0` at entry comes with either text); (3) `timer.GetTime()!=0` (`test eax,eax ;
//   je`); (4) `ScriptTriggered=true;` placed before the final return (dead; Ego_d line 301).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CV_ASSASSINATTACKSSCRIPT_GETREGIONIDFORREGION
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/CV_AssassinAttacksScript.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
// Retail expands these at this row's sites (header contract: defined inline after the include).
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CV_AssassinAttacksScript::CM_AssassinTrigger::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
    bool ScriptTriggered=false;
    if(!ScriptTriggered&&(!Game->IsRegionLoaded("KnotholeGlade")||!Game->IsQuestActive("Q_RansomVictim"))){
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        CCharString data=Me.GetDataString();
        CScriptThing spawn_mkr=Game->GetThingWithScriptName(data);
        C3DVector spawn_pos=spawn_mkr.GetPos();
        while(!IsDistanceBetweenThingsUnder(Game->GetHero(),Me,(float)pGlobalScriptDef->AssassinTriggerProximity)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
        CTimer timer;
        for(;;){
            do{if(IsActiveThreadTerminating())return _Fail_Val.Return();}while(0);   // Inferred: (1) loop-head statement macro
            while(Game->IsCameraPosOnScreen(spawn_pos)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
            timer=pGlobalScriptDef->AssassinCameraOffScreenTime;
            while(Game->IsCameraPosOnScreen(spawn_pos)&&timer.GetTime()!=0){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(!Game->IsCameraPosOnScreen(spawn_pos)){
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
                regionName=Game->GetRegionName();
                regionId=ParentClass.GetRegionIdForRegion(regionName);
                if(ParentClass.RegionInfo[regionId].assassinActive)return _Fail_Val.Return();
                if(regionName!=ParentClass.RegionOfMostRecentKill){
                    if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    CScriptThing assassin=Game->CreateCreature("CREATURE_ASSASSIN",spawn_pos,"AttackingAssassin",false);
                    ParentClass.RegionInfo[regionId].assassinActive=true;
                    {
                        CCharString object=pGlobalScriptDef->AssassinObjects[ParentClass.AssassinObjectCount];
                        Game->AddItemToContainer(assassin,object);
                        ParentClass.AssassinObjectCount++;
                        if(ParentClass.AssassinObjectCount>=pGlobalScriptDef->AssassinObjects.size())ParentClass.AssassinObjectCount=0;
                    }
                }
                ScriptTriggered=true;
                return _Fail_Val.Return();
            }
            Game->NewScriptFrame();
        }
    }
}