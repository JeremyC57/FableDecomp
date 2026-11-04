// CEntityScriptBinding<NScript::CQ_GuildTrainingMeleeScript>::GetEntityScriptName() const -- the registered entity-script name (EntityScriptName, +0x04)
// Leaf: ?GetEntityScriptName@?$CEntityScriptBinding@VCQ_GuildTrainingMeleeScript@NScript@@@@UBEABVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D565B0, 4 bytes: lea eax,[ecx+4]; ret.
// Identity (naming wave 35, family gesn): binding vtable 012d0304 slot 0: stored at every CEntityScriptBinding registration in CQ_GuildTrainingMeleeScript (00d55e90); PDB ?GetEntityScriptName@?$CEntityScriptBinding@VCQ_GuildTrainingMeleeScript@NScript@@@@; refs=1; body lea eax,[ecx+4];ret; was sub_00d565b0.
//   ?GetEntityScriptName@?$CEntityScriptBinding@VCQ_GuildTrainingMeleeScript@NScript@@@@UBEABVCCharString@@XZ in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Address-of-member accessor (`lea`) at this+0x4. __fastcall this=ecx. (catalog stand-in _gapscan).
// Shape: engine/CEntityScriptBinding.h defines the member inline (slot 0 of every binding vtable); this TU instantiates only it. (slot 1 CreateActiveEntityScript is the landed row 0x00D58300)
// CCharString / CCountedPointer<T> / CScriptThing first (the engine/CEntityScriptBinding.h contract), then the binding template.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBinding.h"
namespace NScript { class CQ_GuildTrainingMeleeScript; }   // the level script; only T* enters the binding's layout
template const CCharString& CEntityScriptBinding<NScript::CQ_GuildTrainingMeleeScript>::GetEntityScriptName() const;
