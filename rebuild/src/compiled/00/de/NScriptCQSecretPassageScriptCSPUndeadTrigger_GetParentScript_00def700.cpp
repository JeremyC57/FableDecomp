// NScript::CQ_SecretPassageScript::CSP_UndeadTrigger::GetParentScript() const -- the owning quest script (ParentClass) as a CScriptBase*
// Leaf: ?GetParentScript@CSP_UndeadTrigger@CQ_SecretPassageScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEF700, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (two sources): slot 3 of the RTTI-less entity vtable 0x012DDF3C (slot 1 the landed ?Main@CSP_UndeadTrigger@... 0x00DEF760, slot 2 the
//   landed ?Init@CSP_UndeadTrigger@... 0x00DEF710); Ego_r.pdb / FableWin.pdb
//   ?GetParentScript@CSP_UndeadTrigger@CQ_SecretPassageScript@NScript@@UBEPAVCScriptBase@@XZ; Ego_r.pdb ParentClass at +0x14.
//   Name-fix NF186 (q4874): was a `struct T` gapscan stand-in (?Get@T@@QAEHXZ, no Leaf line; build catalog module _gapscan).
// Class: engine/NScript_CQ_SecretPassageScript.h (shared header; NF186 opt-in FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_UNDEADTRIGGER declares the
//   entity class). CQ_SecretPassageScript derives from CScriptBase at offset 0 (CLevelScriptBase<T>), so the conversion adds no code.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_UNDEADTRIGGER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/NScript_CQ_SecretPassageScript.h"
namespace NScript {
CScriptBase* CQ_SecretPassageScript::CSP_UndeadTrigger::GetParentScript() const
{
    return ParentClass;
}
}
