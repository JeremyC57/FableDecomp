// CFileInstaller::CFileInstaller() -- empty buffer/file containers, the critical section, four auto-reset events, cleared counters and flags, the ".biz" -> ".big" compressed-file mapping, and the read and decompression worker threads
// Grade: DIFFER(389v389) -- scheduling only: the four CreateEventW argument pushes interleave with the flag stores differently, and the ".biz" push / PauseFinishEvent store order; real STLport 4.6 _Vector_base/_STLP_alloc_proxy/_List_base/_Rb_tree models fixed the push edi placement (59->57 lines); tried: temporary CCharString for the mapping (385, worse), flags before InitializeCriticalSection (prior sweep, worse)
// Leaf: ??0CFileInstaller@@QAE@XZ
//   oracle_extra): 18 diff lines, scheduling only (push edi after the first vector store, the first CreateEventW's push
//   ebx interleaved with the flag stores)
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
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };

// STLport 4.6 containers (namespace std, node allocator = malloc).
namespace std {
template <class T> struct less { };
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef allocator<_List_node<_Tp> > _Node_alloc;
    _List_base(const _Alloc&) : _M_node(_Node_alloc(), (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, _Node_alloc> _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    typedef allocator<_Rb_tree_node<_Tp> > _Node_alloc;
    _Rb_tree_base(const _Alloc&) : _M_header(_Node_alloc(), (_Rb_tree_node<_Tp>*)0) { _M_header._M_data = _M_header.allocate(1); }
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, _Node_alloc> _M_header;
};
template <class _Key, class _Value, class _Compare, class _Alloc> class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    _Rb_tree(const _Compare& __comp = _Compare(), const _Alloc& __a = _Alloc())
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;     // _S_rb_tree_red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    unsigned long _M_node_count;
    _Compare _M_key_compare;
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    set() : _M_t(_Compare(), _Alloc()) { }
    _Rb_tree<_Key, _Key, _Compare, _Alloc> _M_t;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    map() : _M_t(_Compare(), _Alloc()) { }
    _Tp& operator[](const _Key& __k);                // 0x009AC700
    _Rb_tree<_Key, pair<const _Key, _Tp>, _Compare, _Alloc> _M_t;
};
}
template <class T> class CArray : public std::vector<T> { };
template <class T> class CCountedPointer { public: T* Data; void* Info; };
void __cdecl FileInstallerDecompressionThread(void* installer);   // 0x009C4420

class CFileInstaller {
public:
    class CBufferChunk;
    class CFileInfo;
    class CActiveFile { public: unsigned char _body[0xb0]; };
    class CActiveRead;
    CFileInstaller();
    virtual ~CFileInstaller();
    static void __cdecl ThreadMain(void* installer);   // 0x009C5870
    _RTL_CRITICAL_SECTION        CriticalSection;       // +0x04
    CArray<CBufferChunk>         BufferChunks;          // +0x1C
    unsigned char*               PReadBuffer;           // +0x28
    unsigned char*               PWriteBuffer;          // +0x2C
    long                         AvailableBuffers;      // +0x30
    long                         AllocatedInputBuffers; // +0x34
    std::list<CCountedPointer<const CFileInfo> > FilesToCopy;     // +0x38
    std::list<CCountedPointer<const CFileInfo> > FilesToFinish;   // +0x3C
    std::list<CActiveFile>       ActiveFiles;           // +0x40
    std::set<CCharString>        CompletedFiles;        // +0x44
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
    std::map<CCharString, CCharString> CompressedFileTypes;   // +0x94
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
