// CChunkedFile::~CChunkedFile() -- own vtable, destroy FileComment, release the root chunk's count block
// (destroy + free on the last reference, re-reading the block through `this` after the decrement), null the
// pointer pair and tail-jump into CBase::~CBase.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7DF0, length 74 bytes.
// Callees: CCharString::~CCharString @ 0x0099EAE0, the block's destroy function (+4) on its object (+8),
// operator delete @ 0x00BFE9BC, CBase::~CBase @ 0x0099A300 (jmp).
// CBase: the engine's polymorphic root (vtable @ 0x0129A7C4 in the ctor, 0x01231710 in the dtor);
// both members are out of line in retail.
struct CBase {
    virtual void VSlot0();
    CBase();                                    // 0x0099A2F0
    ~CBase();                                   // 0x0099A300
};
struct CChunkedFileSuperChunk;
struct SCountBlock {                            // CCPPointerInfo: {references, destroy function, object}
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
};
struct CCountedSuperChunkPointer {              // CCountedPointer<CChunkedFileSuperChunk>
    CChunkedFileSuperChunk* Data;               // +0
    SCountBlock*            Info;               // +4
    CCountedSuperChunkPointer(const CCountedSuperChunkPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info != 0)
            ++Info->m_references;
    }
    ~CCountedSuperChunkPointer()
    {
        if (Info != 0) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CChunkedFile : CBase {                   // PDB layout: RootChunk +4, Version +0xc, FileComment +0x10
    CCountedSuperChunkPointer RootChunk;        // +0x04
    unsigned long             Version;          // +0x0c
    CCharString               FileComment;      // +0x10
    CChunkedFile(const CCountedSuperChunkPointer& rootChunk);
    ~CChunkedFile();
};

CChunkedFile::~CChunkedFile()
{
}