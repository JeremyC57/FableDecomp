// CFileInstaller::CFileInstaller() -- empty buffer/file containers, the critical section, four auto-reset events, cleared counters and flags, the ".biz" -> ".big" compressed-file mapping, and the read and decompression worker threads
// Grade: DIFFER(ours v retail) -- scheduling only (same length): `push edi` after the first vector store, the first CreateEventW's two `push ebx` interleaved after the flag stores, and the last event store before the string-literal push
// Leaf: ??0CFileInstaller@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C5890, length 389 bytes (`ret`). Vtable 0x0129B43C.
// Callees: malloc @ 0x00BFEA0E (the STLport list sentinels and red-black tree headers), InitializeCriticalSection / CreateEventW
//   (IAT 0x0143FEE4 / 0x0143FDC8), CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0 and operator=(const char*) @ 0x0099EFE0,
//   map<CCharString, CCharString>::operator[] @ 0x009AC700, _beginthread @ 0x00BFED3A with CFileInstaller::ThreadMain
//   @ 0x009C5870 and the decompression thread entry @ 0x009C4420.
// Layout: Ego_r.pdb CFileInstaller names at the retail offsets (12-byte vectors, one-word lists, 12-byte sets/maps):
//   CriticalSection +4, BufferChunks +0x1C, PReadBuffer +0x28 .. AllocatedInputBuffers +0x34, FilesToCopy +0x38,
//   FilesToFinish +0x3C, ActiveFiles +0x40 (0xB8-byte nodes), CompletedFiles +0x44, PCurrentReadFile +0x50 ..
//   PCompletedRead +0x74, ThreadFinished +0x78 .. IgnoreTimeStampWhenCopying +0x81, the events +0x84 .. +0x90,
//   CompressedFileTypes +0x94.
// Shape: STLport container constructors inline (list: sentinel word zeroed, node allocated and linked to itself; tree:
//   header word zeroed, allocated, count zeroed, then colour/root/leftmost/rightmost through the re-read header); the body
//   sets the thread flags, creates the events, then clears the counters (Reset-style, some flags written twice).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // IAT 0x0143FEE4
extern "C" __declspec(dllimport) void* __stdcall CreateEventW(void* attributes, int manualReset, int initialState, const unsigned short* name);   // IAT 0x0143FDC8
extern "C" unsigned long __cdecl _beginthread(void (__cdecl* start)(void*), unsigned int stackSize, void* argument);   // 0x00BFED3A
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
template<unsigned int NodeSize> struct CSentinelList {  // STLport list: the sentinel pointer only
    struct node { node* next; node* prev; unsigned char data[NodeSize - 8]; };
    node* head;
    CSentinelList() : head(0)
    {
        node* n = (node*)malloc(sizeof(node));
        n->next = n;
        n->prev = n;
        head = n;
    }
};
template<unsigned int NodeSize> struct CRbTree {        // STLport _Rb_tree: header, count, comparator
    struct node { unsigned char color; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[NodeSize - 16]; };
    node*         header;
    unsigned long count;
    unsigned char compare;
    CRbTree() : header(0)
    {
        header = (node*)malloc(sizeof(node));
        count = 0;
        header->color = 0;
        header->parent = 0;
        header->left = header;
        header->right = header;
    }
};
struct CCompressedFileTypes : CRbTree<0x18> {           // map<CCharString, CCharString>
    CCharString& operator[](const CCharString& key);    // 0x009AC700
};
template<class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CBufferChunk;
struct CActiveFile;
struct CActiveRead;
void __cdecl FileInstallerDecompressionThread(void* installer);   // 0x009C4420

struct CFileInstaller {
    CRITICAL_SECTION_Fable       CriticalSection;       // +0x04
    CRetailVector<CBufferChunk>  BufferChunks;          // +0x1C
    unsigned char*               PReadBuffer;           // +0x28
    unsigned char*               PWriteBuffer;          // +0x2C
    long                         AvailableBuffers;      // +0x30
    long                         AllocatedInputBuffers; // +0x34
    CSentinelList<0x10>          FilesToCopy;           // +0x38
    CSentinelList<0x10>          FilesToFinish;         // +0x3C
    CSentinelList<0xb8>          ActiveFiles;           // +0x40
    CRbTree<0x14>                CompletedFiles;        // +0x44
    CActiveFile*                 PCurrentReadFile;      // +0x50
    CActiveFile*                 PCurrentWriteFile;     // +0x54
    long                         NoActiveReads;         // +0x58
    long                         NoActiveWrites;        // +0x5C
    bool                         WriteMode;             // +0x60
    unsigned long                BufferChunkSize;       // +0x64
    unsigned long                NoBufferChunks;        // +0x68
    long                         NoTotalBytesWritten;   // +0x6C
    long                         TotalSizeOfAllFiles;   // +0x70
    CActiveRead*                 PCompletedRead;        // +0x74
    bool                         ThreadFinished;        // +0x78
    bool                         DecompressionThreadFinished;   // +0x79
    bool                         ExitThread;            // +0x7A
    bool                         Running;               // +0x7B
    unsigned long                Paused;                // +0x7C
    bool                         Completed;             // +0x80
    bool                         IgnoreTimeStampWhenCopying;    // +0x81
    void*                        Event;                 // +0x84
    void*                        DecompressionEvent;    // +0x88
    void*                        PauseStartEvent;       // +0x8C
    void*                        PauseFinishEvent;      // +0x90
    CCompressedFileTypes         CompressedFileTypes;   // +0x94
    CFileInstaller();
    virtual ~CFileInstaller();
    static void __cdecl ThreadMain(void* installer);   // 0x009C5870
};

CFileInstaller::CFileInstaller()
{
    InitializeCriticalSection(&CriticalSection);
    DecompressionThreadFinished = false;
    ThreadFinished = false;
    ExitThread = false;
    Running = false;
    Completed = true;
    Paused = 0;
    Event = CreateEventW(0, 0, 0, 0);
    DecompressionEvent = CreateEventW(0, 0, 0, 0);
    PauseStartEvent = CreateEventW(0, 0, 0, 0);
    PauseFinishEvent = CreateEventW(0, 0, 0, 0);
    PCompletedRead = 0;
    ThreadFinished = false;
    PCurrentReadFile = 0;
    PCurrentWriteFile = 0;
    PReadBuffer = 0;
    PWriteBuffer = 0;
    BufferChunkSize = 0;
    NoBufferChunks = 0;
    AvailableBuffers = 0;
    AllocatedInputBuffers = 0;
    NoTotalBytesWritten = 0;
    TotalSizeOfAllFiles = 0;
    ExitThread = false;
    NoActiveReads = 0;
    NoActiveWrites = 0;
    WriteMode = false;
    IgnoreTimeStampWhenCopying = false;
    {
        CCharString extension(".biz");
        CompressedFileTypes[extension] = ".big";
    }
    _beginthread(ThreadMain, 0x3000, this);
    _beginthread(FileInstallerDecompressionThread, 0x3000, this);
}
