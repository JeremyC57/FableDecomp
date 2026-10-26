// CCountedRecord_00a16d70::~CCountedRecord_00a16d70() -- destroy the name string, then release the shared pointee leaving the pointer null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A16D70, length 61 bytes.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (the string at +0x1c), the pointee's destroy
// function through the count block (`call [eax+4]`, object in ecx), operator delete @ 0x00BFE9BC.
// The manifest label "CBrainOutput::~CBrainOutput" is propagated: CBrainOutput (PDB) has its
// counted pointer at +0x20 and no string; this class holds a CCountedPointer at +0 and a CCharString
// at +0x1c and cannot be named from the body. Members are destroyed in reverse order; the counted
// pointer's inline destructor decrements, re-reads the block, destroys + frees on zero and nulls
// both words (speed form: `test eax,eax`, immediate zero stores).
class CCharString { public:
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


struct CCountedRecord_00a16d70 {
    CCountedPointer_CPointee m_pItem;           // +0x00
    unsigned char            _pad[0x14];        // +0x08..+0x1b
    CCharString              m_name;            // +0x1c
    ~CCountedRecord_00a16d70();
};

CCountedRecord_00a16d70::~CCountedRecord_00a16d70()
{
}