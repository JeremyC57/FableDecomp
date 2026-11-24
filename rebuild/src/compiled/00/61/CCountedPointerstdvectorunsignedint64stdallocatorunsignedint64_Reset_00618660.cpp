// CCountedPointer_006174f0::Reset(T*) -- drop the shared count block (destroying the object on last reference), then adopt the new object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00618660, oracle length 103 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je A ; dec dword ptr [eax] ; mov eax,[esi+4] ; cmp dword ptr [eax],0 ; jne A ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+4] ; push eax ; call operator delete ; add esp,4 ; A: mov eax,[esp+8] ; test eax,eax ; mov dword ptr [esi+4],0 ; mov [esi],eax ; je Z ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov ecx,[esi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset Destroy ; mov [eax+8],ecx ; mov [esi+4],eax ; pop esi ; ret 4 ; N: xor eax,eax ; mov [esi+4],eax ; Z: pop esi ; ret 4
// Intrusive-count smart pointer {object, count block}: the 12-byte count block holds the
// reference count, the per-type destroy function (0x006174F0, called with the object in ecx)
// and the object. Reset() decrements the old block (re-reading it after the decrement: the count
// is touched through a reference), destroys + frees it on zero, then adopts the new object by
// allocating a fresh block (operator new @ 0x00BFEA1A, null-checked; delete @ 0x00BFE9BC). The
// instantiation is named from the manifest label when unique, else from the destroy function.
// One of 74 bodies of this shape.
// Naming (NF309, 2026-11-23): the count block's deleter 0x006174F0 now binds its PDB spelling ?DeleteData@?$CCountedPointer@V?$vector@_KV?$allocator@_K@std@@@std@@@@SIXPAX@Z (FableWin.pdb name set; retail stores
//   0x006174F0 as the count block's DeleteFunc here; the self row's Leaf), declared by the shared engine/CCountedPointer.h under
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (only its address is stored here, T is only declared; the NF284 0x00BB64A0 form);
//   was the free stand-in ?Destroy_006174f0@@YIXPAX@Z. Object otherwise unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(&CCountedPointer<std::vector<unsigned __int64, std::allocator<unsigned __int64> > >::DeleteData), m_pObject(object) { }
};

struct CCountedPointer_006174f0 {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    void Reset(void* object);
};

void CCountedPointer_006174f0::Reset(void* object)
{
    if (m_pCount) {
        --m_pCount->m_references;
        if (m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }
    }
    m_pCount = 0;
    m_pObject = object;
    if (object)
        m_pCount = new SCountBlock(m_pObject);   // re-read through `this` (retail loads [esi] for the block), not the argument
}