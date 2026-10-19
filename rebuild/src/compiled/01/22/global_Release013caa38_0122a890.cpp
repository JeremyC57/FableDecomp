// Release_013caa38() -- drops the static counted pointer @ 0x013CAA38: Reset(0) inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A890, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[0x13caa3c] ; push esi ; xor esi,esi ; cmp eax,esi ; je Z ; dec dword ptr [eax] ; mov eax,[0x13caa3c] ; cmp [eax],esi ; jne Z ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[0x13caa3c] ; push eax ; call operator delete ; add esp,4 ; Z: mov [0x13caa38],esi ; mov [0x13caa3c],esi ; pop esi ; ret
// A static CCountedPointer {object @ 0x013CAA38, count block @ 0x013CAA3C} being released: the
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

extern CCountedPointerStatic g_pointer_013caa38;   // 0x013CAA38

void Release_013caa38()
{
    g_pointer_013caa38.Reset(0);
}