// CScriptThing::operator==(const CScriptThing& other) const -- compare the two counted pointers' implementations through the protected virtual IsEqualTo (slot 78)
// Leaf: ??8CScriptThing@@QBE_NABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AAE20, length 22 bytes (32 with padding). 8 callers (0x00CBEF0D,
// 0x00E6B4F6, 0x00E8623E ...); the landed callers bind ??8CScriptThing@@QBE_NABV0@@Z (Ego_r.pdb / FableWin.pdb spelling).
// engine/CScriptThing.h retail branch (FABLE_RETAIL_NSCRIPT): PImp +0x04 (CCountedPointer<CScriptThing>::Data), the pointee's
// vtable slot 78 (+0x138) ?IsEqualTo@CScriptThing@@MBE_NABV1@@Z (FableWin.pdb; rtti-vtables.tsv CScriptThing 0x01238C8C slot 78)
// takes the other implementation and returns the bool straight through the tail jump.
// Shape: `mov ecx,[ecx+4] ; mov eax,[esp+4] ; mov eax,[eax+4] ; mov edx,[ecx] ; mov [esp+4],eax ; jmp [edx+138h]`.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

bool CScriptThing::operator==(const CScriptThing& other) const
{
    return PImp->IsEqualTo(*other.PImp);
}
