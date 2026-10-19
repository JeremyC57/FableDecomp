// Release_013ba854() -- drops the static counted pointer @ 0x013BA854: Reset(0) inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01229190, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[0x13ba858] ; push esi ; xor esi,esi ; cmp eax,esi ; je Z ; dec dword ptr [eax] ; mov eax,[0x13ba858] ; cmp [eax],esi ; jne Z ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[0x13ba858] ; push eax ; call operator delete ; add esp,4 ; Z: mov [0x13ba854],esi ; mov [0x13ba858],esi ; pop esi ; ret
// A static CCountedPointer {object @ 0x013BA854, count block @ 0x013BA858} being released: the
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
        m_pObject = object;                 // retail stores the object word first
        m_pCount = 0;
    }
};

extern CCountedPointerStatic g_pointer_013ba854;   // 0x013BA854

void Release_013ba854()
{
    g_pointer_013ba854.Reset(0);
}