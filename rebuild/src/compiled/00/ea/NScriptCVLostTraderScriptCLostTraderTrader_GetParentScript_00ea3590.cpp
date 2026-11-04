// NScript::CV_LostTraderScript::CLostTraderTrader::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CLostTraderTrader@CV_LostTraderScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA3590, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00ea3247 "LostTraderTrader" in 00ea2e80; Alloc 00ea34f0; vtable 012edda8 slot 3; PDB ?GetParentScript@CLostTraderTrader@CV_LostTraderScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ea3590.
//   ?GetParentScript@CLostTraderTrader@CV_LostTraderScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_LostTraderScript.h (shared header: nested class CLostTraderTrader, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CV_LostTraderScript.h"
CScriptBase* NScript::CV_LostTraderScript::CLostTraderTrader::GetParentScript() const
{
    return &ParentClass;
}
