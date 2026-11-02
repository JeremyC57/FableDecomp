// CActiveEntityScriptBase::OnPredicateFail() -- forward the predicate failure to the bound entity script (PEntityScript, slot 5 OnPredicateFail), if one is bound
// Leaf: ?OnPredicateFail@CActiveEntityScriptBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE1090, 13 bytes, thiscall: `mov ecx,[ecx+34h] ; test ; je ; mov eax,[ecx] ;
//   jmp [eax+14h] ; ret` (the virtual call is the tail jump).
// Identity (proven): slot 5 of the CActiveEntityScriptBase vtable 0x012C3594 (stored by the ctor 0x00CE1110) and inherited unchanged
//   in slot 5 of every CActiveEntityScript<T> vtable (121 .rdata references, e.g. 0x012C358C / 0x012C6970); its sibling slot 6
//   0x00CE10A0 forwards to +0x18 (OnInterrupted). Ego_r.pdb 0x00CC45A0 and FableWin.pdb 0x03B3A170 both spell
//   ?OnPredicateFail@CActiveEntityScriptBase@@UAEXXZ (donor order ctor, OnPredicateFail, OnInterrupted, dtor; engine/CActiveEntityScriptBase.h
//   names this slot). The catalog row was the gap-scan placeholder sub_00ce1090.
// Layout: engine/CActiveEntityScriptBase.h (Ego_r.pdb = retail): PEntityScript (CCountedPointer<CEntityScriptBase>) +0x34, its Data
//   word first; CEntityScriptBase slot 5 = OnPredicateFail (+0x14, engine/CEntityScriptBase.h).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "fable_string.h"                                   // CCharString for CSpawnedFuncBase (header contract)
#include "engine/CActiveEntityScriptBase.h"
#include "engine/CEntityScriptBase.h"

void CActiveEntityScriptBase::OnPredicateFail()
{
    if (PEntityScript.Get())
        PEntityScript->OnPredicateFail();
}