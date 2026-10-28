// CFileInstaller::~CFileInstaller() -- under the installer's critical section: clear the buffers, flag the worker threads to exit, signal both events and spin (releasing the lock around Sleep(0)) until both threads report finished, close the four events; then delete the critical section and destroy the containers
// Grade: DIFFER(416v410) -- retail holds a zero in ebx from the prologue (`xor ebx,ebx`: `push ebx` for the lock's false and Sleep's 0, the thread flags tested `cmp byte ptr [esi+78h], bl`) and &CriticalSection in ebp (pushed for the lock and again for DeleteCriticalSection), with a 0x10 frame; ours pushes immediates, tests the flags `mov al,[..] ; test al,al`, keeps &CriticalSection in ebx and zeroes ebx only for the container teardown (0x0c frame). Likely the flags' real (thread-shared) qualifier, which the purity rules do not allow here. 2 grades (probe/c5170_a, _b).
// Leaf: ??1CFileInstaller@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C5170, 410 bytes, thiscall, ebp frame aligned to 8 (`and esp,-8`, the 8-aligned
//   lock local), plain `ret`. Its only caller is 0x009C7903 (the scalar deleting destructor).
// Identity: Ego_r.pdb `??1CFileInstaller@@UAE@XZ`; the body stores vtable 0x0129B43C (CFileInstaller) on entry and
//   0x0129B414 (CTBaseSingleton<CFileInstaller>) on exit; it calls the landed 0x009C5110 (bound as the invented
//   `?FreeBuffers@CFileInstaller@@QAEXXZ`; Ego_r.pdb names the lock-guarded buffer release ClearBuffers, and donor order
//   ClearBuffers, InitBuffers, ~CFileInstaller brackets the retail rows 0x009C5110 / 0x009C5170 / 0x009C5310). The
//   manifest label "CPatchTesselationEdgeStrip_DestructorHelper" is propagated.
// Callees: CCriticalSectionLock(_RTL_CRITICAL_SECTION&, bool) 0x00A62920 / Release 0x00A62960 / Lock 0x00A62980 / dtor
//   0x00A629A0, ClearBuffers 0x009C5110, SetEvent / Sleep / CloseHandle / DeleteCriticalSection imports 0x0143FDE0 /
//   0x0143FE1C / 0x0143FE30 / 0x0143FDE4, the map<CCharString,CCharString> _M_erase 0x009AA830, the set<CCharString>
//   _M_erase 0x004ABA70, the two out-of-line list clears 0x009C71A0 / 0x009C6CC0, the CBufferChunk __destroy_aux 0x009C6D60,
//   free 0x00BFEA14.
// Layout: Ego_r.pdb CFileInstaller with retail STLport sizes (list 4, set/map 12, CArray 12): CriticalSection +0x04,
//   BufferChunks +0x1c, FilesToCopy +0x38, FilesToFinish +0x3c, ActiveFiles +0x40, CompletedFiles +0x44, the thread flags
//   +0x78..+0x7a, the four events +0x84..+0x90, CompressedFileTypes +0x94 (0xa0 bytes).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
extern "C" __declspec(dllimport) int  __stdcall SetEvent(void* event);                         // import 0x0143FDE0
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);             // import 0x0143FE1C
extern "C" __declspec(dllimport) int  __stdcall CloseHandle(void* handle);                     // import 0x0143FE30
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* section);   // import 0x0143FDE4

struct __declspec(align(8)) CCriticalSectionLock {         // 8 bytes, 8-aligned on the stack (`and esp,-8`)
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool                   Locked;
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                               // 0x00A629A0
    void Release();                                        // 0x00A62960
    void Lock();                                           // 0x00A62980
};

class CCharString { public: void* PStringData; };
template <class T> class CCountedPointer { public: T* Data; void* Info; };

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x009C6D60
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
template <class _Tp> struct _Identity { };
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    _Rb_tree_node<_Value>* _M_header;
    unsigned int           _M_node_count;
    _Compare               _M_key_compare;
    void _M_erase(_Rb_tree_node<_Value>* __x);             // out of line
    _Rb_tree_node<_Value>* _M_root() const { return (_Rb_tree_node<_Value>*)_M_header->_M_parent; }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    ~_Rb_tree()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _M_t;
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set { public:
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
};
template <class _Tp> struct _List_node { _List_node* _M_next; _List_node* _M_prev; _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
public:
    _List_node<_Tp>* _M_node;
    void clear();                                          // out of line
    ~_List_base()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { };
}

template <class T> class CArray {                          // retail CArray<T>: first / last / end of storage
public:
    T* Data;
    T* DataEnd;
    T* StorageEnd;
    ~CArray()
    {
        std::__destroy_aux(Data, DataEnd, std::__false_type());
        if (Data)
            free(Data);
    }
};

template <class T> class CTBaseSingleton {                 // vtable 0x0129B414
public:
    virtual ~CTBaseSingleton() { }
};

class CFileInstaller : public CTBaseSingleton<CFileInstaller> {   // vtable 0x0129B43C, retail 0xa0 bytes
public:
    class CFileInfo;
    class CActiveFile;
    class CActiveRead;
    class CBufferChunk { public: void* Buffer; long Size; ~CBufferChunk(); };
    _RTL_CRITICAL_SECTION   CriticalSection;               // +0x04
    CArray<CBufferChunk>    BufferChunks;                  // +0x1c
    unsigned char*          PReadBuffer;                   // +0x28
    unsigned char*          PWriteBuffer;                  // +0x2c
    long                    AvailableBuffers;              // +0x30
    long                    AllocatedInputBuffers;         // +0x34
    std::list<CCountedPointer<const CFileInfo> > FilesToCopy;     // +0x38
    std::list<CCountedPointer<const CFileInfo> > FilesToFinish;   // +0x3c
    std::list<CActiveFile*> ActiveFiles;                   // +0x40 (element type not modelled)
    std::set<CCharString>   CompletedFiles;                // +0x44
    CActiveFile*            PCurrentReadFile;              // +0x50
    CActiveFile*            PCurrentWriteFile;             // +0x54
    long                    NoActiveReads;                 // +0x58
    long                    NoActiveWrites;                // +0x5c
    bool                    WriteMode;                     // +0x60
    unsigned long           BufferChunkSize;               // +0x64
    unsigned long           NoBufferChunks;                // +0x68
    long                    NoTotalBytesWritten;           // +0x6c
    long                    TotalSizeOfAllFiles;           // +0x70
    CActiveRead*            PCompletedRead;                // +0x74
    bool                    ThreadFinished;                // +0x78
    bool                    DecompressionThreadFinished;   // +0x79
    bool                    ExitThread;                    // +0x7a
    bool                    Running;                       // +0x7b
    unsigned long           Paused;                        // +0x7c
    bool                    Completed;                     // +0x80
    bool                    IgnoreTimeStampWhenCopying;    // +0x81
    void*                   Event;                         // +0x84
    void*                   DecompressionEvent;            // +0x88
    void*                   PauseStartEvent;               // +0x8c
    void*                   PauseFinishEvent;              // +0x90
    std::map<CCharString, CCharString> CompressedFileTypes;   // +0x94
    void ClearBuffers();                                   // 0x009C5110
    virtual ~CFileInstaller();
};

CFileInstaller::~CFileInstaller()
{
    {
        CCriticalSectionLock lock(CriticalSection, false);
        ClearBuffers();
        ExitThread = true;
        SetEvent(Event);
        SetEvent(DecompressionEvent);
        while (!ThreadFinished || !DecompressionThreadFinished) {
            lock.Release();
            Sleep(0);
            lock.Lock();
        }
        CloseHandle(Event);
        CloseHandle(DecompressionEvent);
        CloseHandle(PauseStartEvent);
        CloseHandle(PauseFinishEvent);
    }
    DeleteCriticalSection(&CriticalSection);
}
