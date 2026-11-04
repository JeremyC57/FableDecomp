// CEntityScriptBinding<NScript::CV_HauntedHouseScript>::GetEntityScriptName() const -- the registered entity-script name (EntityScriptName, +0x04)
// Leaf: ?GetEntityScriptName@?$CEntityScriptBinding@VCV_HauntedHouseScript@NScript@@@@UBEABVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E953D0, 4 bytes: lea eax,[ecx+4]; ret.
// Identity (naming wave 35, family gesn): Anniversary naming 2026-11-04, wave2 medium: Anniversary wave2 (medium, strict): Anniversary RTTI CEntityScriptBinding<NScript::CV_HauntedHouseScript> matched to this vtable; TLC proof agrees: vtable 0x012ECC98 slot 0 = GetEntityScriptName of CEntityScriptBinding<NScript::CV_HauntedHouseScript>; TLC: 2-slot vtable, slot0 body lea eax,[ecx+4];ret (returns EntityScriptName @+4, PDB layout), slot1 CEntityScriptBinding<NScript::CV_HauntedHouseScript>::CreateActiveEntityScript; stored by CEntityScriptBinding<NScript::CQ_EndGameFocalSitesScript>::CEntityScriptBinding (alloc 0x1c = PDB sizeof 28); registers 0 Alloc fns e.g. []; PDB has GetEntityScriptName of this exact class; function dword-referenced once (this vtable); was sub_00e953d0 (placeholder).
//   ?GetEntityScriptName@?$CEntityScriptBinding@VCV_HauntedHouseScript@NScript@@@@UBEABVCCharString@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_waves23.tsv).
// Was: Address-of-member accessor (`lea`) at this+0x4. __fastcall this=ecx. (catalog stand-in _gapscan).
// Shape: engine/CEntityScriptBinding.h defines the member inline (slot 0 of every binding vtable); this TU instantiates only it. (slot 1 CreateActiveEntityScript is the landed row 0x00E95F80)
// CCharString / CCountedPointer<T> / CScriptThing first (the engine/CEntityScriptBinding.h contract), then the binding template.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBinding.h"
namespace NScript { class CV_HauntedHouseScript; }   // the level script; only T* enters the binding's layout
template const CCharString& CEntityScriptBinding<NScript::CV_HauntedHouseScript>::GetEntityScriptName() const;
