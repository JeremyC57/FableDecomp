// CFileInstaller::PostFileReadCompletion(CFileInstaller::CActiveRead* read) -- under the installer's critical section, account a finished read against its active file, copy the read buffer into the read's chunk, then queue it for decompression (signal the decompression event, keep the read) or as a pending write (delete the read), and signal the worker event
// Leaf: ?PostFileReadCompletion@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C4120, 258 bytes, thiscall `ret 4`. Callers: ProcessFileReadCompletion
//   0x009C4312 and two more (0x009C4F42, 0x009C5753). Name: Ego_r.pdb
//   ?PostFileReadCompletion@CFileInstaller@@IAEXPAVCActiveRead@1@@Z. The chunk label "FileReadCompletion_ProcessData" is
//   descriptive.
// Layout (retail; Ego_r.pdb names, offsets re-derived for STLport containers): CFileInstaller CriticalSection +0x04,
//   PReadBuffer +0x28, NoActiveReads +0x58, Event +0x84, DecompressionEvent +0x88; CActiveRead (0x28) {PActiveFile,
//   FilePos, TransferSize, PBufferChunk, OverlappedInfo, PFileInstaller +0x24}; CActiveFile NoBytesRead +0x0C, PFileInfo
//   +0x1C, PendingWrites +0x90, PendingDecompression +0x94; CFileInfo Decompress +0x0D; CBufferChunk Mem +0.
// Callees: CCriticalSectionLock ctor @ 0x00A62920 / dtor @ 0x00A629A0, SetEvent (IAT 0x0143FDE0), 0x00BFEA0E malloc (list
//   nodes), 0x00BFE9BC operator delete.
// Shape: no EH frame; memcpy is the intrinsic (`rep movsd ; rep movsb`); the push_backs are STLport insert(end(), x)
//   with the placement-new null test; TransferSize is read once into a local (ebx) for the count, the copy and the
//   pending write, which pushes the deleteRead flag into the dead `read` argument slot; the CPendingWrite temporary keeps FilePos in its stack slot.
extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);   // IAT 0x0143FDE0
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __tmp = _M_node.allocate(1);
        _Construct(&__tmp->_M_data, __x);
        return __tmp;
    }
    iterator end() { return iterator(_M_node._M_data); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

}

struct SCriticalSection { unsigned char _body[0x18]; };            // RTL_CRITICAL_SECTION

class CCriticalSectionLock {                        // 8 bytes on the stack
public:
    SCriticalSection* CriticalSection;
    bool Locked;
    CCriticalSectionLock(SCriticalSection* section, bool tryOnly);  // 0x00A62920
    ~CCriticalSectionLock();                        // 0x00A629A0
};

struct SOverlapped { unsigned long Internal, InternalHigh, Offset, OffsetHigh; void* hEvent; };

class CFileInstaller {
public:
    struct CFileInfo {                              // 0x10 bytes
        void* SrcPath;                              // +0x00 CWideString
        void* DstPath;                              // +0x04 CWideString
        long InputSize;                             // +0x08
        bool Started;                               // +0x0C
        bool Decompress;                            // +0x0D
        bool CreateMarkerFile;                      // +0x0E
    };
    struct CBufferChunk {
        unsigned char* Mem;                         // +0x00
        bool Used;                                  // +0x04
        bool IsInput;                               // +0x05
    };
    struct CPendingWrite {                          // 0x0C bytes
        CBufferChunk* PBufferChunk;                 // +0x00
        unsigned long FilePos;                      // +0x04
        unsigned long WriteLength;                  // +0x08
        CPendingWrite(CBufferChunk* chunk, unsigned long filePos, unsigned long writeLength)
            : PBufferChunk(chunk), FilePos(filePos), WriteLength(writeLength) {}
    };
    struct CActiveRead;
    struct CActiveFile {                            // retail 0xB0 bytes
        void* SrcFileHandle;                        // +0x00
        void* DstFileHandle;                        // +0x04
        unsigned long CurrentSrcOffset;             // +0x08
        unsigned long NoBytesRead;                  // +0x0C
        unsigned long NoBytesWritten;               // +0x10
        unsigned long InputSize;                    // +0x14
        unsigned long OutputSize;                   // +0x18
        const CFileInfo* PFileInfo_Data;            // +0x1C CCountedPointer<const CFileInfo>
        void* PFileInfo_Info;                       // +0x20
        unsigned long MinWriteSize;                 // +0x24
        unsigned long MinReadSize;                  // +0x28
        double StartTime;                           // +0x30
        unsigned char Decompressor[0x58];           // +0x38 CCompressorZlib
        std::list<CPendingWrite> PendingWrites;     // +0x90
        std::list<CActiveRead*> PendingDecompression;   // +0x94
    };
    struct CActiveRead {                            // 0x28 bytes
        CActiveFile* PActiveFile;                   // +0x00
        long FilePos;                               // +0x04
        long TransferSize;                          // +0x08
        CBufferChunk* PBufferChunk;                 // +0x0C
        SOverlapped OverlappedInfo;                 // +0x10
        CFileInstaller* PFileInstaller;             // +0x24
    };

    void PostFileReadCompletion(CActiveRead* read);

    void* __vftable;                                // +0x00
    SCriticalSection CriticalSection;               // +0x04
    unsigned char BufferChunks[0x0C];               // +0x1C CArray<CBufferChunk>
    unsigned char* PReadBuffer;                     // +0x28
    unsigned char* PWriteBuffer;                    // +0x2C
    long AvailableBuffers;                          // +0x30
    long AllocatedInputBuffers;                     // +0x34
    unsigned char FilesToCopy[4];                   // +0x38 list<CCountedPointer<const CFileInfo> >
    unsigned char FilesToFinish[4];                 // +0x3C
    unsigned char ActiveFiles[4];                   // +0x40 list<CActiveFile>
    unsigned char CompletedFiles[0x0C];             // +0x44 set<CCharString>
    CActiveFile* PCurrentReadFile;                  // +0x50
    CActiveFile* PCurrentWriteFile;                 // +0x54
    long NoActiveReads;                             // +0x58
    long NoActiveWrites;                            // +0x5C
    bool WriteMode;                                 // +0x60
    unsigned long BufferChunkSize;                  // +0x64
    unsigned long NoBufferChunks;                   // +0x68
    long NoTotalBytesWritten;                       // +0x6C
    long TotalSizeOfAllFiles;                       // +0x70
    CActiveRead* PCompletedRead;                    // +0x74
    bool ThreadFinished;                            // +0x78
    bool DecompressionThreadFinished;               // +0x79
    bool ExitThread;                                // +0x7A
    bool Running;                                   // +0x7B
    unsigned long Paused;                           // +0x7C
    bool Completed;                                 // +0x80
    bool IgnoreTimeStampWhenCopying;                // +0x81
    void* Event;                                    // +0x84
    void* DecompressionEvent;                       // +0x88
};

void CFileInstaller::PostFileReadCompletion(CActiveRead* read)
{
    CCriticalSectionLock lock(&CriticalSection, false);
    long transferSize = read->TransferSize;
    read->PActiveFile->NoBytesRead += transferSize;
    --read->PFileInstaller->NoActiveReads;
    memcpy(read->PBufferChunk->Mem, PReadBuffer, transferSize);
    bool deleteRead = false;
    if (read->PActiveFile->PFileInfo_Data->Decompress) {
        read->PActiveFile->PendingDecompression.push_back(read);
        SetEvent(read->PFileInstaller->DecompressionEvent);
    } else {
        read->PActiveFile->PendingWrites.push_back(CPendingWrite(read->PBufferChunk, read->FilePos, transferSize));
        deleteRead = true;
    }
    SetEvent(read->PFileInstaller->Event);
    if (deleteRead)
        delete read;
}