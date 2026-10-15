// Release_013ca818() -- shut down the file installer singleton: drop the static counted pointer @ 0x013CA818 (Reset(0) inlined)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404490, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// The file installer lives in a static counted pointer (object @ 0x013CA818, count block @ 0x013CA81C)
// that CFileInstallerSingleton::Get creates on first use. GFUninitialise calls this to let it go: we
// drop our reference on the count block, and since the singleton is normally the only holder that
// destroys the installer through the block's deleter and frees the block. Both words are then nulled,
// so a later Get() would build a fresh installer.
//
// Matching: Reset(0) is __forceinline so the "adopt new object" half folds away for a null argument;
// the block pointer is re-read after the decrement; delete @ 0x00BFE9BC. One of 25 bodies of this shape.
// Count block shared by all pointers to one object.
struct SCountBlock {
    long m_references;                       // how many pointers share the object
    void (__fastcall* m_pfnDestroy)(void*);  // deletes the object on the last release
    void* m_pObject;                         // the object handed to the deleter
};

struct CCountedPointerStatic {
    void* m_pObject;                         // the installer
    SCountBlock* m_pCount;                   // its count block
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

// The installer singleton's pointer.
extern CCountedPointerStatic g_pointer_013ca818;   // 0x013CA818

void Release_013ca818()
{
    g_pointer_013ca818.Reset(0);
}
