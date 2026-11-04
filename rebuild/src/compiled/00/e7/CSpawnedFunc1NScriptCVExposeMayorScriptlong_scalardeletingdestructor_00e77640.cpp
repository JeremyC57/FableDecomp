// CSpawnedFunc1<NScript::CV_ExposeMayorScript,long>::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CSpawnedFunc1 (no member destructor: Par1 is long), i.e. the CSpawnedFuncBase base; free when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc1@VCV_ExposeMayorScript@NScript@@J@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E77640, 30 bytes: push esi; mov esi,ecx; call ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Identity (naming wave 35, family sf1): Anniversary naming 2026-11-04, wave2 medium_shifted: Anniversary wave2 (medium, shift-corrected): Anniversary row put CSpawnedFunc<NScript::CV_ExposeMayorScript> here, TLC proves CSpawnedFunc1<NScript::CV_ExposeMayorScript,long>, which Anniversary RTTI also has (neighbouring vtable): vtable 0x012E9924 slot 0 = sdd of CSpawnedFunc1<NScript::CV_ExposeMayorScript,long>; TLC: 7-slot vtable, slots 5/6 = CSpawnedFuncBase pair 00cdd410/00cdd420; alloc 0x40 = PDB sizeof; sdd member dtor none; CEM_FlashLightPoint::Main spawns CV_ExposeMayorScript::HeroFlashesLamp ['ParentClass.']; PDB member signature args ('long',); PDB has sdd of this exact class; function dword-referenced once (this vtable); was CVtable_012e9924::`scalar deleting destructor' (placeholder).
//   rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_waves23.tsv; PDB: FableWin.pdb ??_G?$CSpawnedFunc1@VCV_ExposeMayorScript@NScript@@J@@UAEPAXI@Z; FableWin.pdb ??1?$CSpawnedFunc1@VCV_ExposeMayorScript@NScript@@J@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012e9924, Leaf `vector_deleting_destructor').
// Shape: the wave 29 sfg emitter (DECISIONS_W29_RESOLVED D7, 26 landed CSpawnedFunc1 ??_G rows): MakeOne constructs the class, so the TU
//   emits the vtable and its ??_G; the implicit ~CSpawnedFuncBase (engine/CSpawnedFuncBase.h) is called out of line.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc1.h"
namespace NScript {
class CV_ExposeMayorScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CV_ExposeMayorScript* pclass, void (NScript::CV_ExposeMayorScript::*func)(long), long par1)
{
    return new CSpawnedFunc1<NScript::CV_ExposeMayorScript, long>(name, id, pclass, func, par1);
}
