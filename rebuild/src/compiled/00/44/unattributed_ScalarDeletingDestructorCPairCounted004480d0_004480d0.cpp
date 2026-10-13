// CPairCounted_004480d0::`scalar deleting destructor'(unsigned int) -- non-virtual: inlined CCountedPointer member destructor at +0x4, then delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004480D0, oracle length 74 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0x8] ; test eax,eax ; je Z ; dec dword ptr [eax] ; mov eax,[esi+0x8] ; cmp dword ptr [eax],0 ; jne Z ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0x8] ; push eax ; call operator delete ; add esp,4 ; Z: test byte ptr [esp+8],1 ; mov dword ptr [esi+0x4],0 ; mov dword ptr [esi+0x8],0 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// VC7.1 also emits a (non-virtual) scalar deleting destructor for a non-polymorphic class with a
// non-trivial destructor whenever a translation unit applies `delete` to it: the inlined
// ~CCountedPointer of the member at +0x4 (release the count block: decrement, destroy the object
// through the block's function pointer and free the block on zero; then null both fields) followed
// by the flag-tested operator delete @ 0x00BFE9BC. The manifest label (`_Dest_val<...pair<unsigned
// long,CCountedPointer<...>>>`) names the pair; the instantiation is not recoverable. One of 15 bodies.
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};

struct CCountedPointer {
    void* m_pObject;
    SCountBlock* m_pCount;
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

struct CPairCounted_004480d0 {
    unsigned char _pad[0x4];
    CCountedPointer m_pointer;             // +0x4
};

// The delete-expression that makes the compiler emit CPairCounted_004480d0::`scalar deleting destructor'.
void Destroy_CPairCounted_004480d0(CPairCounted_004480d0* pair)
{
    delete pair;
}