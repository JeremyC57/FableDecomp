// CXMVPlayer::~CXMVPlayer() -- XMV movie player destructor (empty user body): clear and free the SubTitles list, release PThreadedFile, then ~CBaseClassNonCopyable
// Leaf: ??1CXMVPlayer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BDB40, int3 padding before and after, thiscall, tail `jmp ~CBaseClassNonCopyable` 0x0099A300.
// Identity (proven, two sources): (1) it stores vtable 0x0129B3BC (RTTI .?AVCXMVPlayer@@ : CThreadedFileCallback :
//   CBaseClassNonCopyable : CBaseClass; slot 0 ??_G 0x009BE1B0, slots 1/2 OnFileReadBegin 0x009BDC00 / OnFileReadCompletion
//   0x009BDC10) and its sole caller is that ??_G; (2) Ego_r.pdb 0x00B22D80 / FableWin.pdb 0x02FE6D50 ??1CXMVPlayer@@UAE@XZ, and the
//   body walks Ego_r's SubTitles +0x58 and PThreadedFile +0x44. The ctor 0x009BDB00 (`ret 8`, list sentinel malloc 0x24) matches
//   ??0CXMVPlayer@@QAE@_NK@Z. The catalog label "CBankFileEntryUpdateData" is wrong.
// Callees: ?clear@?$_List_base@UCSubTitle@@V?$allocator@UCSubTitle@@@std@@@std@@QAEXXZ 0x009BE060, free 0x00BFEA14 (sentinel), the
//   counted pointer's DeleteFunc and operator delete (engine/CCountedPointer.h dtor), ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300 (tail).
// Shape: empty user dtor; members in reverse order (SubTitles: ~list -> ~_List_base { clear(); deallocate(sentinel); },
//   PThreadedFile), then the implicit ~CThreadedFileCallback (empty) leaves the CBaseClassNonCopyable tail.
// Layout: Ego_r.pdb CXMVPlayer with the STLport list (4 bytes), retail sizeof 0x78; CSubTitle per Ego_r (0x1C).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();                                                 // _list.c, out of line (0x009BE060)
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x0: the sentinel node
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    ~list() { }
};
}

template <class T> class CIVCountedPointer {                // Ego_r.pdb: one CIVCountedPointeeBase* (chunk 712 model)
public:
    ~CIVCountedPointer() { Release(); }                     // FableWin ??1?$CIVCountedPointer@...@@QAE@XZ (inline here)
    void Release()                                          // FableWin ?Release@?$CIVCountedPointer@...@@QAEXXZ (inline here)
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;                          // +0x00
};

struct _RTL_CRITICAL_SECTION_DEBUG;
struct _RTL_CRITICAL_SECTION {                              // winnt.h layout, 0x18 bytes (no windows.h in the lane)
    _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;
    long LockCount;
    long RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};

namespace NGameText { class CTextEntry; class CDataBank; }
struct CSubTitle {
    CIVCountedPointer<const NGameText::CTextEntry> TextEntry;   // +0x00
    CWideString FormattedText;                                  // +0x04
    CCharString Sound;                                          // +0x08
    long FrameStart;                                            // +0x0c
    long FrameEnd;                                              // +0x10
    float StartTime;                                            // +0x14
    float EndTime;                                              // +0x18
};

class CThreadedFile;
class CTexture;
enum EThreadedFilePriority;

class CThreadedFileCallback : public CBaseClassNonCopyable {    // RTTI CThreadedFileCallback : CBaseClassNonCopyable : CBaseClass
public:
    CThreadedFileCallback() { }
    virtual bool OnFileReadBegin(void** buffer, EThreadedFilePriority priority);   // slot 1
    virtual void OnFileReadCompletion(bool success);                                // slot 2
};

class CXMVPlayer : public CThreadedFileCallback {           // vtable 0x0129B3BC
public:
    CXMVPlayer(bool allowSkipping, unsigned long flags);
    virtual ~CXMVPlayer();
    virtual bool OnFileReadBegin(void** buffer, EThreadedFilePriority priority);
    virtual void OnFileReadCompletion(bool success);
    _RTL_CRITICAL_SECTION          CriticalSection;         // +0x04
    void*                          ReadCompletionEvent;     // +0x1c
    char*                          InputBuffer;             // +0x20
    char*                          PlaybackBuffer;          // +0x24
    bool                           NewInputReady;           // +0x28
    bool                           ReadyToRead;             // +0x29
    long                           BufferSize;              // +0x2c
    long                           MaxPacketSize;           // +0x30
    long                           CurrentPacketSize;       // +0x34
    long                           NextPacketPosition;      // +0x38
    long                           InputBufferLength;       // +0x3c
    long                           PlaybackBufferLength;    // +0x40
    CCountedPointer<CThreadedFile> PThreadedFile;           // +0x44
    long                           FilePosition;            // +0x4c
    bool                           AllowSkipping;           // +0x50
    long                           LocalXMVPlayerRefCount;  // +0x54
    std::list<CSubTitle>           SubTitles;               // +0x58
    CTexture*                      ExternalWorkingTextureDraw;   // +0x5c
    CTexture*                      ExternalWorkingTextureShow;   // +0x60
    unsigned long                  MyFps;                   // +0x64
    const NGameText::CDataBank*    pTextBank;               // +0x68
    double                         TimeSinceMovieStarted;   // +0x70
};
FABLE_STATIC_ASSERT(sizeof(CXMVPlayer) == 0x78);

CXMVPlayer::~CXMVPlayer()
{
}