// NScript::CV_ExposeMayorScript::HeroFlashesLamp(long flashes) -- recursive lamp-flash watcher: with no flashes left, note whether the hero stands within MaxFlashDistFromStables of EM_FlashLightPoint (lights flashed correctly) or reset the lamp puzzle; otherwise start a three-second timer and, while it runs, wait for the hero's hand lamp to change state (lit -> unlit, or unlit -> lit), then watch for one flash fewer; a timeout resets the puzzle
// Grade: DIFFER(1613v1612) -- 10 lines, 1 byte: the shared expanded ~CTimer exit block of the second inlined recursion copy is laid out at the end of the copy (retail after its first arm); chunk 864 (11 grades): per-row inline controls do not touch it (/d2inlS2, S6, T80 and inline_depth(2..4) identical; S12, T20, T40, inline_depth(1) change the expansion mix badly), so it is a block-layout residue, outside the 2026-11-08 inline-control rule; 111 earlier grades
// Next idea: block layout of a tail-merged return block; no device known
// Leaf: ?HeroFlashesLamp@CV_ExposeMayorScript@NScript@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E76E90, 1612 bytes, thiscall `ret 4`, no EH frame.
// Identity (two sources): Ego_r.pdb / FableWin.pdb name set ?HeroFlashesLamp@CV_ExposeMayorScript@NScript@@QAEXJ@Z (one long);
//   the body calls itself (0x00E76E90) with flashes - 1 and reads the quest's MaxFlashDistFromStables +0x9c /
//   HeroFlashedLightsCorrectly +0x50 / ResetLamp +0x52 that the landed CEM_FlashLightPoint::Main 0x00E76920 and CheckLampSlot
//   0x00E76CA0 use; engine/CV_ExposeMayorScript.h declares it.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   ~CScriptThing 0x004AA840; IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall, the distance on the stack); CTimer ctor
//   0x00CD4450 / dtor 0x00CD4470 (plain inline: VC7.1 expands RegisterTimer / DeregisterTimer at most sites and calls the COMDAT
//   at the rest); HeroFlashesLamp 0x00E76E90 (the depth-2 recursion; the depth-1 call is expanded once, VC7.1's
//   inline_recursion(off) default); Game (+0x40) slots 7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 185
//   IsHeroHandLampLit; pDSTGame 0x0143E8F8 slots 87 RegisterTimer, 88 DeregisterTimer, 89 SetTimer, 90 GetTimer.
// Literal: "EM_FlashLightPoint" 0x012E97BC.
// Layout (engine/CV_ExposeMayorScript.h, Ego_r.pdb - 0x14): Game +0x40, HeroFlashedLightsCorrectly +0x50, ResetLamp +0x52,
//   MaxFlashDistFromStables +0x9c.
// Shape: `if (flashes == 0) { distance check } else { CTimer timer; timer = 3; if (lit) { wait for unlit } else { wait for lit } }`
//   with the translator's terminate check in front of every block; the recursive call inside each wait loop is expanded one
//   level, so its own zero-flashes arm and timer appear inline with their returns jumping to the outer loop's latch.
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
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance); // 0x00CBE2FF
extern CGameScriptInterfaceBase* pDSTGame;                                                                      // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long n) { pDSTGame->SetTimer(TimerIndex, n); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

#ifdef _MSC_VER
#pragma inline_recursion(on)
#endif
void NScript::CV_ExposeMayorScript::HeroFlashesLamp(long flashes)
{
    if (flashes == 0) {
        if (IsActiveThreadTerminating()) return;
        CScriptThing point = Game->GetThingWithScriptName("EM_FlashLightPoint");
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), point, MaxFlashDistFromStables)) {
            if (IsActiveThreadTerminating()) return;
            HeroFlashedLightsCorrectly = true;
        } else {
            if (IsActiveThreadTerminating()) return;
            ResetLamp = true;
        }
    } else {
        CTimer timer;
        timer = 3;
        if (Game->IsHeroHandLampLit()) {
            if (IsActiveThreadTerminating()) return;
            while (timer.GetTime() > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (!Game->IsHeroHandLampLit()) {
                    if (IsActiveThreadTerminating()) return;
                    HeroFlashesLamp(flashes - 1);
                }
            }
            if (IsActiveThreadTerminating()) return;
        } else {
            if (IsActiveThreadTerminating()) return;
            while (timer.GetTime() > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (Game->IsHeroHandLampLit()) {
                    if (IsActiveThreadTerminating()) return;
                    HeroFlashesLamp(flashes - 1);
                }
            }
            if (IsActiveThreadTerminating()) return;
        }
        ResetLamp = true;
    }
}
