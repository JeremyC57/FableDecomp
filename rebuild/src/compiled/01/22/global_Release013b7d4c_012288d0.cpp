// Release_013b7d4c() -- drops the static counted pointer @ 0x013B7D4C: Reset(0) inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012288D0, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[0x13b7d50] ; push esi ; xor esi,esi ; cmp eax,esi ; je Z ; dec dword ptr [eax] ; mov eax,[0x13b7d50] ; cmp [eax],esi ; jne Z ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[0x13b7d50] ; push eax ; call operator delete ; add esp,4 ; Z: mov [0x13b7d50],esi ; mov [0x13b7d4c],esi ; pop esi ; ret
// A static CCountedPointer {object @ 0x013B7D4C, count block @ 0x013B7D50} being released: the
// inlined Reset(0) decrements the block, destroys the object through the block's function pointer
// and frees the block (operator delete @ 0x00BFE9BC) on the last reference, then nulls both
// fields (the "adopt new object" half folds away for a null argument). One of 25 bodies.
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};

struct CCountedPointerStatic {
    void* m_pObject;
    SCountBlock* m_pCount;
    __forceinline void Reset(void* object)
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
    }
};

extern CCountedPointerStatic g_pointer_013b7d4c;   // 0x013B7D4C

void Release_013b7d4c()
{
    g_pointer_013b7d4c.Reset(0);
}