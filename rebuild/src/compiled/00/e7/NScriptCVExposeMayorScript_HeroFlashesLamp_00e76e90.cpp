// NScript::CV_ExposeMayorScript::HeroFlashesLamp(long TimesLeft) -- recursive lamp-flash watcher: with no flashes left, note whether the hero stands within MaxFlashDistFromStables of EM_FlashLightPoint (lights flashed correctly) or reset the lamp puzzle; otherwise start a three-second timer and, while it runs, wait for the hero's hand lamp to change state (lit -> unlit, or unlit -> lit), then watch for one flash fewer; a timeout resets the puzzle
// Inline-budget: `#pragma inline_recursion(on)` (inside `#ifdef _MSC_VER`, the owner-approved guard of the 7 landed inline_recursion rows; owner decision 2026-11-08 on inline controls) -- retail expands the self call HeroFlashesLamp(TimesLeft-1) one level in each wait loop (0x00E76FE9 / 0x00E7728A `dec ebp ; jne`) and calls 0x00E76E90 from inside the copies; VC7.1's default (inline_recursion off) calls it at both sites: DIFFER(556v1612), and no flag expands a self call (/d2inlT200, /d2inlS30: DIFFER(514v1612)). Plus FABLE_FORCEINLINE CTimer::operator= / GetTime (header contract, as 99 / 87 landed rows): plain operator= DIFFER(1596v1612), plain GetTime or both DIFFER(1589v1612), both plain with /d2inlT60 / T90 DIFFER(1525 / 1589 v1612).
// Leaf: ?HeroFlashesLamp@CV_ExposeMayorScript@NScript@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E76E90, 1612 bytes, thiscall `ret 4`, no EH frame, default flags.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?HeroFlashesLamp@CV_ExposeMayorScript@NScript@@QAEXJ@Z (one long) and the Ego_d.pdb proc
//   of the same name; the body calls itself (0x00E76E90) with TimesLeft - 1 and reads MaxFlashDistFromStables +0x9C / writes
//   HeroFlashedLightsCorrectly +0x50 / ResetLamp +0x52, the members the landed CEM_FlashLightPoint::Main 0x00E76920 and CheckLampSlot 0x00E76CA0 use.
// Source shape: Ego_d.pdb line table (lines 170..218, ref egod_lines/) and locals: parameter `TimesLeft`; function scope `_Fail_Val` (TRetVoid) and
//   `LampTimer` (CTimer, so the zero-flashes arm ends in a return -- line 185, 20 bytes -- and `CTimer LampTimer;` follows at function scope,
//   line 189); one block {TargetPosition (CScriptThing)}. Generated quest-script text as the landed Mains (0x00E377D0, 0x00DDAE40):
//   `return _Fail_Val.Return();`, each terminate check on its statement's line.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; ~CScriptThing
//   0x004AA840; IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); CTimer::CTimer 0x00CD4450 / ~CTimer 0x00CD4470 (plain inline: expanded at
//   some sites, called at others -- c2 site frequency); operator= / GetTime expanded at every site (FABLE_FORCEINLINE, as 99 / 87 landed rows;
//   retail has no out-of-line copy of either; plain inline calls them in the recursion copies, DIFFER(1589v1612)); HeroFlashesLamp 0x00E76E90;
//   Game (+0x40) slots 7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 185 IsHeroHandLampLit; pDSTGame 0x0143E8F8 slots 87
//   RegisterTimer, 88 DeregisterTimer, 89 SetTimer, 90 GetTimer. Literal "EM_FlashLightPoint" 0x012E97BC.
// Inferred: (1) `LampTimer.GetTime()>0` (retail `test eax,eax ; jle` / `jg`); (2) the literal 3 seconds (`push 3`).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ExposeMayorScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
// Retail expands these at this row's sites (header contract: defined after the include).
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#ifdef _MSC_VER
#pragma inline_recursion(on)
#endif
void NScript::CV_ExposeMayorScript::HeroFlashesLamp(long TimesLeft)
{
    TRetVoid _Fail_Val;
    if(TimesLeft==0){if(IsActiveThreadTerminating())return _Fail_Val.Return();
        CScriptThing TargetPosition=Game->GetThingWithScriptName("EM_FlashLightPoint");
        if(IsDistanceBetweenThingsUnder(Game->GetHero(),TargetPosition,MaxFlashDistFromStables)){if(IsActiveThreadTerminating())return _Fail_Val.Return();
            HeroFlashedLightsCorrectly=true;
        }
        else{if(IsActiveThreadTerminating())return _Fail_Val.Return();
            ResetLamp=true;
        }
        return _Fail_Val.Return();
    }
    CTimer LampTimer;
    LampTimer=3;
    if(Game->IsHeroHandLampLit()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
        while(LampTimer.GetTime()>0){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(!Game->IsHeroHandLampLit()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                HeroFlashesLamp(TimesLeft-1);
            }
        }
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
    }
    else{if(IsActiveThreadTerminating())return _Fail_Val.Return();
        while(LampTimer.GetTime()>0){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(Game->IsHeroHandLampLit()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                HeroFlashesLamp(TimesLeft-1);
            }
        }
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
    }
    ResetLamp=true;
}