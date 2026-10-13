// _Destroy_range<CCountedPointer*> -- run the inlined CCountedPointer destructor on [first, last)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2BD20, oracle length 76 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je E ; L: mov eax,[esi+4] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+4] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+4] ; push eax ; call operator delete ; add esp,4 ; N: mov dword ptr [esi],0 ; mov dword ptr [esi+4],0 ; add esi,8 ; cmp esi,edi ; jne L ; E: pop edi ; pop esi ; ret 4
// Dinkumware `_Destroy_range` (`__fastcall`: first in ecx, last in edx, the by-value allocator
// on the stack) for CCountedPointer elements {object, count block}: each element's destructor is
// inlined -- decrement the block's count, at zero call its destroy function (slot +4, object in
// ecx) and delete the block through the flag-tested operator delete @ 0x00BFE9BC, then null
// both fields. One of 14 bodies of this shape.
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CRetailAllocator { };

void __fastcall DestroyRange_CountedPointer_00a2bd20(CCountedPointer* first, CCountedPointer* last, CRetailAllocator allocator)
{
    for (; first != last; ++first)
        first->~CCountedPointer();
}
