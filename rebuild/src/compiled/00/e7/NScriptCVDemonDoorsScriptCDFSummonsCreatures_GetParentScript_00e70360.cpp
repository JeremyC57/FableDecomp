// NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CDF_SummonsCreatures@CV_DemonDoorsScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E70360, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity: slot 3 of the CDF_SummonsCreatures vtable 0x012E8E80 (slot 0 = the landed `scalar deleting destructor' 0x00E70370,
//   slot 1 = Main 0x00E74D70; the vtable is stored by Alloc 0x00E702C0, the "DF_SummonsCreatures" factory bound at
//   CV_DemonDoorsScript::Main 0x00E6E559); the only reference to this address is that slot (0x012E8E8C). FableWin.pdb
//   ?GetParentScript@CDF_SummonsCreatures@CV_DemonDoorsScript@NScript@@UBEPAVCScriptBase@@XZ (the name set's entry follows the
//   class's ??0 and Alloc, as for every CDF_ sibling). Split from the 1828-byte ledger row 0x00E6FC40 (chunk 781).
// Model: the landed sibling CDF_Dragon::GetParentScript 0x00E6FC10 (same 4 bytes): engine/CV_DemonDoorsScript.h, nested class
//   CDF_SummonsCreatures (ParentClass is a CV_DemonDoorsScript& at +0x14, PMasterData +0x18).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_DemonDoorsScript.h"
CScriptBase* NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::GetParentScript() const
{
    return &ParentClass;
}