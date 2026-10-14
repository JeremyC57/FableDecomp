// CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&) -- CBase shell, vtable
// 0x0129E4E8 (RTTI: CChunkedFile), the root chunk pointer copied (shared count block incremented when
// present), Version = 0 and an empty FileComment.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7DB0, length 63 bytes.
// Callees: CBase::CBase @ 0x0099A2F0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E).
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

CChunkedFile::CChunkedFile(const CCountedSuperChunkPointer& rootChunk)
    : CBase(), RootChunk(rootChunk), Version(0), FileComment("", -1)
{
}
