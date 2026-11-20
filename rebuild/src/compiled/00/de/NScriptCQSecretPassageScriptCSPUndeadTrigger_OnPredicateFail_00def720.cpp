// NScript::CQ_SecretPassageScript::CSP_UndeadTrigger::OnPredicateFail() -- the undead trigger ignores predicate failures (empty override)
// Leaf: ?OnPredicateFail@CSP_UndeadTrigger@CQ_SecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEF720, 1 byte: ret.
// Identity (two sources): slot 5 of the RTTI-less entity vtable 0x012DDF3C (slot 1 the landed ?Main@CSP_UndeadTrigger@... 0x00DEF760, slot 3
//   GetParentScript 0x00DEF700); Ego_r.pdb / FableWin.pdb ?OnPredicateFail@CSP_UndeadTrigger@CQ_SecretPassageScript@NScript@@EAEXXZ (private
//   virtual; the CEntityScriptBase default 0x00CDEBD0 is overridden here).
//   Name-fix NF186 (q4875): was a `struct T` gapscan stand-in (?Hook@T@@QAEXXZ, no Leaf line; build catalog module _gapscan).
// Class: engine/NScript_CQ_SecretPassageScript.h (shared header; NF186 opt-in FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_UNDEADTRIGGER).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_UNDEADTRIGGER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/NScript_CQ_SecretPassageScript.h"
namespace NScript {
void CQ_SecretPassageScript::CSP_UndeadTrigger::OnPredicateFail()
{
}
}
