// CFileInstaller::CActiveFile::~CActiveFile() -- destroy the members in reverse order (strings / containers, then the counted pointer's inline release)
// Leaf: ??1CActiveFile@CFileInstaller@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C6DE0, 79 bytes, thiscall, plain `ret`.
// Identity (proven): its callers are CActiveFile's deleting destructor 0x009C6EE0 and the list destructor tail 0x009C7020; it runs the out-of-line list destructors 0x009C67D0 (+0x94) / 0x009C6730 (+0x90), ~CCompressorZlib 0x00A3ACC0 (+0x38) and releases PFileInfo +0x1C -- Ego_r.pdb CFileInstaller::CActiveFile (PendingDecompression, PendingWrites, Decompressor, PFileInfo) and ??1CActiveFile@CFileInstaller@@QAE@XZ.
// Callees: the members' out-of-line destructors, the count block's deleter and operator delete 0x00BFE9BC.
// Shape (family M103, representative 0x0050F910): an empty user destructor -- the implicit member destruction in reverse
//   declaration order; the counted pointer's `if (Info) { if (--RefCount == 0) { DeleteFunc(Data); delete Info; } } Data =
//   Info = 0;` inline (edi = 0 for the stores).
void __cdecl operator delete(void* block);               // 0x00BFE9BC
class CCPPointerInfo { public:                          // the shared count block
    long RefCount;                                      // +0x0
    void (__fastcall *DeleteFunc)(void*);               // +0x4
    void* Data;                                         // +0x8
};
template <class T> class CCountedPointer {             // 8 bytes: object, shared count block
public:
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    T* Data;                                            // +0x0
    CCPPointerInfo* Info;                               // +0x4
};
class CCompressorZlib {
public:
    virtual ~CCompressorZlib();                           // 0x00A3ACC0
    unsigned char _body[0x54];
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    ~list();                                              // out of line
    void* _M_node;
};
}
class CFileInstaller {
public:
    class CFileInfo;
    class CPendingWrite;
    class CActiveRead;
    class CActiveFile {                                   // Ego_r.pdb, 200 bytes
    public:
        ~CActiveFile();
        void* SrcFileHandle;                              // +0x0
        void* DstFileHandle;                              // +0x4
        unsigned long CurrentSrcOffset;                   // +0x8
        unsigned long NoBytesRead;                        // +0xc
        unsigned long NoBytesWritten;                     // +0x10
        unsigned long InputSize;                          // +0x14
        unsigned long OutputSize;                         // +0x18
        CCountedPointer<const CFileInfo> PFileInfo;       // +0x1c
        unsigned long MinWriteSize;                       // +0x24
        unsigned long MinReadSize;                        // +0x28
        double StartTime;                                 // +0x30
        CCompressorZlib Decompressor;                     // +0x38
        std::list<CPendingWrite> PendingWrites;           // +0x90
        std::list<CActiveRead*> PendingDecompression;     // +0x94
        unsigned long Rest[7];                            // +0x98 DecompressSrcPos .. DecompressFinish
    };
};
CFileInstaller::CActiveFile::~CActiveFile()
{
}