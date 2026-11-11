// NScript::CV_TempleOfLightScript::WatchForAcolyteDeath(CScriptThing) -- with an is-alive predicate on the acolyte, wait until it is killed or the hero's hit leaves it without health
// Inline-budget: the by-value acolyte's inline ~CScriptThing (FABLE_FORCEINLINE, as the landed byte-twin WatchForDeputy1Killed 0x00D16570) is expanded only at the shared terminate exit (0x00EDCAF1); scoped `#pragma inline_depth(0)` regions keep the other exits on the out-of-line 0x004AA840 (0x00EDCB47 / 0x00EDCB6B). Plain `inline` loses the zero register (edi) retail keeps for the expanded release, so no /d2inl flag reaches it.
// Revisit: inline-depth -- byte-exact, but reached with #pragma inline_depth regions retail most likely never wrote (same device as the landed 0x00D16570); redo pragma-free once the retail mechanism is found.
// Leaf: ?WatchForAcolyteDeath@CV_TempleOfLightScript@NScript@@QAEXVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDC980, 503 bytes (512 with padding), thiscall, `ret 0Ch` (the by-value CScriptThing).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (the predicate's copy), CScriptBase::SetPredicate 0x00CB7920,
//   ~CIsThingAlive 0x00E241B0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0 ("" 0x0122D70E, "SCRIPT_NAME_HERO" 0x0125D1C8), ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line); Game (+0x40) NewScriptFrame (slot 7), GetHealth
//   (slot 264, +0x420); the thing's slots 18 MsgIsKilledBy, 21 MsgIsHitBy, 43 MsgIsHitByAnyAggressiveSpecialAbilityFrom
//   behind the inlined null-safe wrappers. Float 0.0001f 0x0129BA3C.
// Shape: the landed WatchForRockTrollKilled 0x00EC5010 predicate + yield prologue, then the landed WatchForDeputy1Killed 0x00D16570
//   watch loop (`for (;;)` with the terminate return first, `bool killed` / `bool hit` locals, `break` to one trailing terminate
//   check); the parameter's inline destructor is expanded at the first exits only (retail keeps 0 in edi for it).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_TempleOfLightScript.h"
// Retail expands these at this row's sites: defined inline after the headers (as the landed 0x00E6AF20 / 0x00EC5010).
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
FABLE_FORCEINLINE CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
using NScript::CIsThingAlive;

void NScript::CV_TempleOfLightScript::WatchForAcolyteDeath(CScriptThing acolyte)
{
    SetPredicate(CIsThingAlive(acolyte));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        bool killed = acolyte.MsgIsKilledBy("");
        if (killed) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            break;
        }
        bool hit = acolyte.MsgIsHitBy("SCRIPT_NAME_HERO") || acolyte.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
        if (hit) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            if (Game->GetHealth(acolyte) < 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                break;
            }
        }
        Game->NewScriptFrame();
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
}
#pragma inline_depth()