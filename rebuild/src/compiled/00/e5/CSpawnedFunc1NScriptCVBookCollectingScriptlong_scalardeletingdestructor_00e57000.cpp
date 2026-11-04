// CSpawnedFunc1<NScript::CV_BookCollectingScript,long>::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CSpawnedFunc1 (no member destructor: Par1 is long), i.e. the CSpawnedFuncBase base; free when bit 0 is set
// Leaf: ??_G?$CSpawnedFunc1@VCV_BookCollectingScript@NScript@@J@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E57000, 30 bytes: push esi; mov esi,ecx; call ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Identity (naming wave 35, family sf1): Anniversary naming 2026-11-04, wave2 tlc_extra: TLC-only (found while verifying; not an Anniversary row): vtable 0x012E6228 slot 0 = sdd of CSpawnedFunc1<NScript::CV_BookCollectingScript,long>; TLC: alloc 0x40 = PDB sizeof CSpawnedFunc1<T,long>; stored by CBS_Teacher::AskForBook (CV_BookCollectingScript entity) spawning "BookReaction" = NScript::CV_BookCollectingScript::BookReaction, PDB ?BookReaction@CV_BookCollectingScript@NScript@@QAEXJ@Z (one long arg); slot4 pushes [esi+0x3c] = 1 arg; PDB has sdd of this exact class; function dword-referenced once (this vtable); was CVtable_012e6228::`scalar deleting destructor' (placeholder).
//   rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_waves23.tsv; PDB: FableWin.pdb ??_E?$CSpawnedFunc1@VCV_BookCollectingScript@NScript@@J@@UAEPAXI@Z; FableWin.pdb ??1?$CSpawnedFunc1@VCV_BookCollectingScript@NScript@@J@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012e6228, Leaf `vector_deleting_destructor').
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
class CV_BookCollectingScript : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::CV_BookCollectingScript* pclass, void (NScript::CV_BookCollectingScript::*func)(long), long par1)
{
    return new CSpawnedFunc1<NScript::CV_BookCollectingScript, long>(name, id, pclass, func, par1);
}
