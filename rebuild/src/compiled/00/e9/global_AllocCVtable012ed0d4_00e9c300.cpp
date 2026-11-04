// Alloc_CVtable_012ed0d4(void*, void*) -- factory: new CVtable_012ed0d4(first, second) with the constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E9C300, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; push edi ; push 0x5c ; mov edi,edx ; mov ebx,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; mov ecx,esi ; call CScriptBase::CScriptBase ; mov [esi+0x40],edi ; pop edi ; mov [esi+0x44],ebx ; mov dword ptr [esi],offset CVtable_012ed0d4::`vftable' ; mov eax,esi ; pop esi ; pop ebx ; ret ; N: pop edi ; pop esi ; xor eax,eax ; pop ebx ; ret
// A __fastcall factory (ecx = first, edx = second): allocates sizeof(CVtable_012ed0d4) = 0x5c bytes
// (operator new @ 0x00BFEA1A, landed-trusted), null-checks the block and runs the inlined
// two-argument constructor: base CScriptBase() @ 0x00CB8110, second argument stored at +0x40,
// first at +0x44, vtable 0x0X12ED0D4 (named through rtti-vtables.tsv). The constructor body
// is the same shape the out-of-line family lands as CVtable_012ed0d4::CVtable_012ed0d4. One of 75 bodies.
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase : CBaseClassNonCopyable : CBaseClass,
//   vtable 0x012C1648, 5 slots, 0x40); 0x00CB8110 is ??0CScriptBase@@QAE@XZ (catalog CScriptBase::CScriptBase + FableWin.pdb; naming wave
//   35d: was the local stand-in CBase_00cb8110, whose 0x3c-byte body pad the header's members now fill).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct CVtable_012ed0d4 : CScriptBase {
    void* m_second;                           // +0x40
    void* m_first;                           // +0x44
    unsigned char _tail[0x14];
    virtual void v00();
    CVtable_012ed0d4(void* first, void* second) : CScriptBase(), m_second(second), m_first(first) { }
};

CVtable_012ed0d4* __fastcall Alloc_CVtable_012ed0d4(void* first, void* second)
{
    return new CVtable_012ed0d4(first, second);
}