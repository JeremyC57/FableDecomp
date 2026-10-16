// CActiveFile::CActiveFile(const CCountedPointer<const CFileInstaller::CFileInfo>& fileInfo) -- constructor: the
// offsets and byte counters zeroed, the file info shared (reference count bumped), the zlib decompressor built,
// two empty pending lists and the decompression state cleared.
// Leaf: ??0CActiveFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C4330, length 172 bytes (+ int3 padding), `ret 4`.
// Callees: CCompressorZlib::GetWorkspaceSize @ 0x00A3A7D0 (static, `xor eax,eax ; ret`; manifest
// "CEngineInternalPrimitiveBase::AddChildPrimitive" is propagated), CACompressor::CACompressor(long) @ 0x00A3AEA0 (landed),
// CCompressorZlib::Init @ 0x00A3AA10 (manifest "CVO_OracleMarker::Init" propagated), malloc @ 0x00BFEA0E (the two list
// sentinels: 0x14- and 0xc-byte nodes). Vtable 0x01245974 = CCompressorZlib.
// Layout: Ego_r.pdb CActiveFile up to Decompressor (+0x38); retail CCompressorZlib is 0x58 bytes and the STLport lists
// are one sentinel pointer each, so PendingWrites +0x90, PendingDecompression +0x94, DecompressSrcPos +0x98,
// DecompressDstPos +0x9c, PDecompressSrcBuffer +0xa0, PDecompressDstBuffer +0xa4, DecompressDstUsed +0xa8,
// DecompressFinish +0xac (the engine header's PDB offsets do not apply past +0x38, so a local model).
// Shape: init-list members with ebx zero; the counted-pointer copy bumps the count when set; the decompressor's inline
// ctor (base with the static workspace size, vtable, Init) and the landed sentinel-list ctor (head = 0, node linked to
// itself, head = node).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CCPPointerInfo { long m_references; };

struct CFileInfo;
struct CFileInfoPointer {                           // CCountedPointer<const CFileInstaller::CFileInfo>
    const CFileInfo* Data;                          // +0
    CCPPointerInfo*  Info;                          // +4
    CFileInfoPointer(const CFileInfoPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};

struct CACompressor {                               // landed 0x00A3AEA0 (vtable 0x0129B424)
    virtual void v00();
    unsigned char _body[0x1c - 4];
    CACompressor(long workspaceSize);               // 0x00A3AEA0
};

struct CCompressorZlib : CACompressor {             // vtable 0x01245974, retail 0x58 bytes
    unsigned char _zlib[0x58 - 0x1c];
    static long GetWorkspaceSize();                 // 0x00A3A7D0
    void Init();                                    // 0x00A3AA10
    virtual void v00();
    CCompressorZlib() : CACompressor(GetWorkspaceSize()) { Init(); }
};

template<unsigned int NodeSize> struct CSentinelList {   // STLport list: the sentinel pointer only
    struct node { node* next; node* prev; unsigned char data[NodeSize - 8]; };
    node* head;
    CSentinelList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

struct CBufferChunk;

struct CActiveFile {
    void*            SrcFileHandle;                 // +0x00
    void*            DstFileHandle;                 // +0x04
    unsigned long    CurrentSrcOffset;              // +0x08
    unsigned long    NoBytesRead;                   // +0x0c
    unsigned long    NoBytesWritten;                // +0x10
    unsigned long    InputSize;                     // +0x14
    unsigned long    OutputSize;                    // +0x18
    CFileInfoPointer PFileInfo;                     // +0x1c
    unsigned long    MinWriteSize;                  // +0x24
    unsigned long    MinReadSize;                   // +0x28
    unsigned char    _pad_0x2c[4];
    double           StartTime;                     // +0x30
    CCompressorZlib  Decompressor;                  // +0x38
    CSentinelList<0x14> PendingWrites;              // +0x90 list<CPendingWrite>
    CSentinelList<0xc>  PendingDecompression;       // +0x94 list<CActiveRead*>
    unsigned long    DecompressSrcPos;              // +0x98
    unsigned long    DecompressDstPos;              // +0x9c
    CBufferChunk*    PDecompressSrcBuffer;          // +0xa0
    CBufferChunk*    PDecompressDstBuffer;          // +0xa4
    unsigned long    DecompressDstUsed;             // +0xa8
    bool             DecompressFinish;              // +0xac
    CActiveFile(const CFileInfoPointer& fileInfo);
};

CActiveFile::CActiveFile(const CFileInfoPointer& fileInfo)
    : CurrentSrcOffset(0)
    , NoBytesRead(0)
    , NoBytesWritten(0)
    , InputSize(0)
    , OutputSize(0)
    , PFileInfo(fileInfo)
    , MinWriteSize(0)
    , MinReadSize(0)
    , Decompressor()
    , PendingWrites()
    , PendingDecompression()
    , DecompressSrcPos(0)
    , DecompressDstPos(0)
    , PDecompressSrcBuffer(0)
    , PDecompressDstBuffer(0)
    , DecompressDstUsed(0)
    , DecompressFinish(false)
{
}