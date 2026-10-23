// CChunkedFile::`scalar deleting destructor'(unsigned int flags) -- destroy the comment string and the root chunk pointer, run ~CBaseClass, then free when flags & 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA9190, length 95 bytes (+ int3 padding).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (FileComment at +0x10), the root chunk's destroy
// function through its count block (`call [eax+4]`), operator delete @ 0x00BFE9BC (the count block,
// and `this` when flags & 1), CBaseClass::~CBaseClass @ 0x0099A300 (stores vtable 0x01231710).
// Vtable 0x0129E4E8 slot 0 is this function (rtti-vtables.tsv), so the label is trusted. Layout from
// the Ego_r PDB (CChunkedFile, 20 bytes): RootChunk CCountedPointer<CChunkedFileSuperChunk> at +4,
// Version at +0xc, FileComment at +0x10. Members are destroyed in reverse order (string first, then
// the inlined CCountedPointer destructor whose two null stores precede the out-of-line base destructor).
struct CChunkedFileSuperChunk;

struct CCharString {
    void* PStringData;                     // +0 the shared string block
    CCharString(const CCharString& other); // 0x0099EC30
    ~CCharString();                        // 0x0099EAE0
};

struct SCountBlock {                            // the shared count block of a CCountedPointer
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(CChunkedFileSuperChunk*);    // +0x04
    CChunkedFileSuperChunk* m_pObject;                           // +0x08
};

struct CCountedPointer_CChunkedFileSuperChunk {                  // {object, count block}
    CChunkedFileSuperChunk*       m_pObject;                     // +0x00
    SCountBlock* m_pCount;                      // +0x04
    __forceinline ~CCountedPointer_CChunkedFileSuperChunk()      // forced: VC7.1 would otherwise call it out of line
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

namespace N_0099a300 { struct CBaseClass {                             // vtable 0x01231710
    virtual ~CBaseClass();                      // 0x0099A300
}; } using N_0099a300::CBaseClass;

struct CChunkedFile : CBaseClass {              // vtable 0x0129E4E8
    CCountedPointer_CChunkedFileSuperChunk RootChunk;   // +0x04
    unsigned long                          Version;     // +0x0c
    CCharString                            FileComment; // +0x10
    virtual ~CChunkedFile();
};

CChunkedFile::~CChunkedFile()
{
}