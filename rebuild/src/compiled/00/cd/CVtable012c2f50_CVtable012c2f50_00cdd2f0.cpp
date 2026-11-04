// CVtable_012c2f50::CVtable_012c2f50(void*) -- base CScriptBase(), the argument stored at +0x40, then this vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDD2F0, oracle length 27 bytes.
// Leaf: ??0CVtable_012c2f50@@QAE@PAX@Z
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CScriptBase::CScriptBase ; mov eax,[esp+8] ; mov [esi+0x40],eax ; mov dword ptr [esi],offset CVtable_012c2f50::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor: base CScriptBase() @ 0x00CB8110, the argument kept at +0x40 and
// this class's vtable (0x012C2F50, named through rtti-vtables.tsv). The argument type is not
// recoverable from the body; `void*` models it. One of 15 bodies of this shape.
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase : CBaseClassNonCopyable : CBaseClass,
//   vtable 0x012C1648, 5 slots, 0x40); 0x00CB8110 is ??0CScriptBase@@QAE@XZ (catalog CScriptBase::CScriptBase + FableWin.pdb; naming wave
//   35d: was the local stand-in CBase_00cb8110, whose 0x3c-byte body pad the header's members now fill).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct CVtable_012c2f50 : CScriptBase {
    void* m_argument;                      // +0x40
    virtual void v00();
    CVtable_012c2f50(void* argument);
};

CVtable_012c2f50::CVtable_012c2f50(void* argument)
    : CScriptBase()
    , m_argument(argument)
{
}