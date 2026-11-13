// NScript::CQ_BountyHuntScript::WatchForBoasts() -- per script frame: read the boasts made this frame (Game->MsgOnBoastsMade);
// the time-limit boast (15) arms BoastActiveTimeLimit and spawns WatchForQuestInfoDisplayConditions, the decapitation boast (16) arms
// BoastActiveDecapitation; once the time-limit boast's timer has run out, set MasterScriptData->BountyHuntTimeLimitExceeded.
// Retail: Fable.exe 0x00D17A80, 785 bytes, plain `ret`, no frame pointer, no EH.
// Callees: CCharString() 0x0099E4B0 / operator= 0x0099EFB0 / (const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   IsActiveThreadTerminating 0x00CB7940; _Vector_base<pair<long,CCharString> >(const allocator&) 0x0074F680 (called: the vector
//   ctor round it is expanded); ??_G pair<long,CCharString> 0x004DA68A, ~_Vector_base 0x004D9C5D, free 0x00BFEA14;
//   operator new 0x00BFEA1A, CSpawnedFuncBase(const CCharString&, ulong) 0x00CDD450 (CSpawnedFunc<CQ_BountyHuntScript> vtable
//   0x012C9EDC, Func 0x00D17DA0 WatchForQuestInfoDisplayConditions), AddSpawnedFunction 0x00CB7E50;
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 34 MsgOnBoastsMade, 90 GetTimer (CTimer::GetTime via pDSTGame 0x0143E8F8).
// Shape (why the six exits destroy the boast vector differently): retail's inliner took c2's BUDGET path here, not the
// frequency heuristic. c1 marks a function for c2's heuristic inliner only when /Og is on the COMMAND LINE; with global
// optimisation switched on by #pragma optimize instead, c2 (inline.c, 13.10.3077) inlines by budget: 1000 units per function, each
// inline call site gets remaining / remaining-sites, and a callee over 40 IL instructions is expanded only if that share covers it.
// The share grows from site to site, so the earliest sites get least: the vector ctor's _Vector_base ctor (58) is called; the
// for-top return calls ??_G pair (41) and ~_Vector_base (74); the two boast-arm returns call only ~_Vector_base; the expiry,
// post-loop and normal exits expand everything. The split needs the function's real number of inline call sites: the two
// master-data reads go through CLevelScriptBase<T>::GetMasterData() and the two Game calls through one more empty inline
// accessor (ScriptGameInterface below: a descriptive, made-up name -- no accessor for Game is in the PDBs).
// Header opt-in (overlay): FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS (declares WatchForBoasts / WatchForQuestInfoDisplayConditions).
// Flags: /Og- /d2vol10
// Why /d2vol10 (a c2-internal switch, kept only because the retail bytes need it): on the budget path c2 first applies the
// old inliner's body check (c2.dll 0x107284D5; limit = (args + 2, +1 when optimising for speed) * the -vol value, default 0).
// At the default that check rejects ~vector<pair<long,CCharString> > itself, so no exit gets an inline ~vector expansion:
// graded with `/Og-` alone this source is 496 bytes against retail's 785, every destruction of the boast vector being one
// `lea ecx,[esp+0x20]; call ~vector`. The retail bytes it supplies are the inline element loops + free at the normal end of
// the frame block (0x00D17C27..0x00D17C5F) and at the expiry and post-loop returns (0x00D17D2E / 0x00D17D51, free at
// 0x00D17D6F), and the partly expanded for-top (0x00D17C88, ??_G loop) and boast-arm (0x00D17CBE, 0x00D17CF9) returns.
// The threshold is 8: vol 7 still fails (675 bytes, only part of the expansions), vol 8, 9, 10, 14, 17 and 20 all give the
// retail bytes; any value from 8 up is equivalent here and 10 carries no meaning of its own.
// cl.exe has no documented way to pass a volume, so HOW retail's build set it is unknown: the flag emulates an unidentified
// build setting of the original TU and is not a claim about the original command line. /Og- only takes /Og off the command
// line (the pragma below turns global optimisation back on), so that c1 does not flag the function for the heuristic inliner.
// Leaf: ?WatchForBoasts@CQ_BountyHuntScript@NScript@@QAEXXZ
#pragma optimize("g", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_BountyHuntScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
template <class T> inline T* CLevelScriptBase<T>::GetMasterData() { return MasterScriptData; }
inline CGameScriptInterfaceBase* ScriptGameInterface(CGameScriptInterfaceBase* game) { return game; }

void NScript::CQ_BountyHuntScript::WatchForBoasts()
{
    CCharString boastName;
    while (!IsActiveThreadTerminating()) {
        {
            std::vector<std::pair<long, CCharString> > boasts;
            ScriptGameInterface(Game)->MsgOnBoastsMade(boasts);
            for (unsigned long i = 0; i != boasts.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                long boast = boasts[i].first;
                boastName = boasts[i].second;
                if (boast == 15) {
                    if (!BoastActiveTimeLimit) {
                        if (IsActiveThreadTerminating())
                            return;
                        BoastActiveTimeLimit = true;
                        AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>("WatchForQuestInfoDisplayConditions", 0, this, &CQ_BountyHuntScript::WatchForQuestInfoDisplayConditions), CCharString(""));
                    }
                } else if (boast == 16) {
                    if (!BoastActiveDecapitation) {
                        if (IsActiveThreadTerminating())
                            return;
                        BoastActiveDecapitation = true;
                    }
                }
                if (BoastActiveTimeLimit && BoastTime.GetTime() <= 0 && !GetMasterData()->BountyHuntTimeLimitExceeded) {
                    if (IsActiveThreadTerminating())
                        return;
                    GetMasterData()->BountyHuntTimeLimitExceeded = true;
                }
            }
            if (IsActiveThreadTerminating())
                return;
        }
        ScriptGameInterface(Game)->NewScriptFrame();
    }
}