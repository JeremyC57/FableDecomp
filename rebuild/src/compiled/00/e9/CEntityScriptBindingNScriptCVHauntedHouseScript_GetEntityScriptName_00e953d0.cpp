// CEntityScriptBinding<NScript::CV_HauntedHouseScript>::GetEntityScriptName() const -- the registered entity-script name (EntityScriptName, +0x04)
// Leaf: ?GetEntityScriptName@?$CEntityScriptBinding@VCV_HauntedHouseScript@NScript@@@@UBEABVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E953D0, 4 bytes: lea eax,[ecx+4]; ret.
// Identity (naming wave 35, family gesn): Anniversary naming 2026-11-04, wave2 medium: Anniversary wave2 (medium, strict): Anniversary RTTI CEntityScriptBinding<NScript::CV_HauntedHouseScript> matched to this vtable; TLC proof agrees: vtable 0x012ECC98 slot 0 = GetEntityScriptName of CEntityScriptBinding<NScript::CV_HauntedHouseScript>; TLC: 2-slot vtable, slot0 body lea eax,[ecx+4];ret (returns EntityScriptName @+4, PDB layout), slot1 CEntityScriptBinding<NScript::CV_HauntedHouseScript>::CreateActiveEntityScript; stored by CEntityScriptBinding<NScript::CQ_EndGameFocalSitesScript>::CEntityScriptBinding (alloc 0x1c = PDB sizeof 28); registers 0 Alloc fns e.g. []; PDB has GetEntityScriptName of this exact class; function dword-referenced once (this vtable); was sub_00e953d0 (placeholder).
//   ?GetEntityScriptName@?$CEntityScriptBinding@VCV_HauntedHouseScript@NScript@@@@UBEABVCCharString@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_waves23.tsv).
// Was: Address-of-member accessor (`lea`) at this+0x4. __fastcall this=ecx. (catalog stand-in _gapscan).
// Shape: engine/CEntityScriptBinding.h defines the member inline (slot 0 of every binding vtable); this TU instantiates only it. (slot 1 CreateActiveEntityScript is the landed row 0x00E95F80)
// CCharString / CCountedPointer<T> / CScriptThing first (the engine/CEntityScriptBinding.h contract), then the binding template.
// NF336 (2026-11-24): moved off the row-local class model onto the shared engine headers (acctB Gate 0 prerequisite). The family
//   preamble below is shared by the CV_HauntedHouseScript object-mate rows (a per-row departure is marked where it occurs).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS
#define FABLE_RETAIL_CM_UNDEADHERE_MAIN
#define FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
#define FABLE_RETAIL_CHHUNDEAD_MEMBERS
#define FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_HauntedHouseScript.h"
#include "engine/CEntityScriptBinding.h"
template const CCharString& CEntityScriptBinding<NScript::CV_HauntedHouseScript>::GetEntityScriptName() const;
