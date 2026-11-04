// CEntityScriptBinding<NScript::Cguy_scriptScript>::GetEntityScriptName() const -- the registered entity-script name (EntityScriptName, +0x04)
// Leaf: ?GetEntityScriptName@?$CEntityScriptBinding@VCguy_scriptScript@NScript@@@@UBEABVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFA0A0, 4 bytes: lea eax,[ecx+4]; ret.
// Identity (naming wave 35, family gesn): binding vtable 012f699c slot 0: stored at every CEntityScriptBinding registration in Cguy_scriptScript (00efa010); PDB ?GetEntityScriptName@?$CEntityScriptBinding@VCguy_scriptScript@NScript@@@@; refs=1; body lea eax,[ecx+4];ret; was sub_00efa0a0.
//   ?GetEntityScriptName@?$CEntityScriptBinding@VCguy_scriptScript@NScript@@@@UBEABVCCharString@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Address-of-member accessor (`lea`) at this+0x4. __fastcall this=ecx. (catalog stand-in _gapscan).
// Shape: engine/CEntityScriptBinding.h defines the member inline (slot 0 of every binding vtable); this TU instantiates only it. (slot 1 CreateActiveEntityScript is the landed row 0x00EFA210)
// CCharString / CCountedPointer<T> / CScriptThing first (the engine/CEntityScriptBinding.h contract), then the binding template.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBinding.h"
namespace NScript { class Cguy_scriptScript; }   // the level script; only T* enters the binding's layout
template const CCharString& CEntityScriptBinding<NScript::Cguy_scriptScript>::GetEntityScriptName() const;
