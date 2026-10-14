// CCountedRecord_00a16e50::`scalar deleting destructor'(unsigned int flags) -- the inlined destructor (string, then the shared pointee), then operator delete when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A16E50, length 81 bytes (+ int3 padding).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (the string at +0x1c), the pointee's destroy
// function through its count block (`call [eax+4]`), operator delete @ 0x00BFE9BC (the block, and
// `this` when flags & 1). Same class as the destructors @ 0x00A16D70 / 0x00A170A0 (a counted
// pointer at +0, a CCharString at +0x1c; the manifest's CBrainOutput label is propagated). The class
// is not polymorphic: VC7.1 emits a non-virtual `??_G` in any TU that `delete`s one (the Destroy
// helper); the flag test is scheduled before the two null stores.
struct CCharString {
    void* m_pStringData;
    ~CCharString();                             // 0x0099EAE0
};

struct CPointee;

struct SCountBlock {                            // the shared count block of a CCountedPointer
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(CPointee*);   // +0x04
    CPointee* m_pObject;             // +0x08
};

struct CCountedPointer_CPointee {    // {object, count block}
    CPointee* m_pObject;             // +0x00
    SCountBlock*         m_pCount;              // +0x04
    __forceinline ~CCountedPointer_CPointee()   // forced: VC7.1 would call it out of line
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


struct CCountedRecord_00a16e50 {
    CCountedPointer_CPointee m_pItem;           // +0x00
    unsigned char            _pad[0x14];        // +0x08..+0x1b
    CCharString              m_name;            // +0x1c
};

// The delete-expression that makes the compiler emit the (implicit-destructor) scalar deleting destructor.
void Destroy(CCountedRecord_00a16e50* p)        // pulls the scalar deleting destructor into the TU
{
    delete p;
}
