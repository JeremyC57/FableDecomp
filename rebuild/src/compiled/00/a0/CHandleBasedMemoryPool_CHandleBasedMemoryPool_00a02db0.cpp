// CHandleBasedMemoryPool::~CHandleBasedMemoryPool() -- uninitialise the pool, then destroy Name and the Handles array, then ~CBaseClassNonCopyable
// Leaf: ??1CHandleBasedMemoryPool@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02DB0, 52 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): stores vtable 0x0129C964 first (RTTI .?AVCHandleBasedMemoryPool@@ : CBaseClassNonCopyable : CBaseClass);
//   callers are the derived classes' destructors: CAsyncLoadingMemoryPool's ??_G 0x00A14B40 (landed), the tail jump of the
//   derived dtor 0x00A148F0 (stores 0x0129CA6C first) and the `??_G` 0x00A62F90. Ego_r.pdb (0x00B6DFA0) and FableWin.pdb
//   (0x0319A080) spell ??1CHandleBasedMemoryPool@@UAE@XZ; the body is the landed CHandleBasedMemoryPool `??_G` 0x00A03AA0's
//   inlined destructor. The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: CHandleBasedMemoryPool::Uninitialise 0x00A02190 (?Uninitialise@CHandleBasedMemoryPool@@QAEXXZ, bound),
//   CCharString::~CCharString 0x0099EAE0, free 0x00BFEA14, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Shape: user destructor `{ Uninitialise(); }`, then the members in reverse order (Name; Handles = CArray over the STLport
//   vector: trivial _Destroy + ~_Vector_base's null-tested free), then the base.
// Layout: Ego_r.pdb members at the same offsets up to Handles (+0x438); FirstUnusedHandle +0x444, Name +0x448,
//   CriticalSection +0x44C (12-byte STLport CArray). engine/CHandleBasedMemoryPool.h flattens the vptr (header debt).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

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
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

class CFailedAllocationHandler;
struct _RTL_CRITICAL_SECTION_DEBUG;
struct _RTL_CRITICAL_SECTION {                              // winnt.h, 0x18
    _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;
    long  LockCount;
    long  RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};

class CHandleBasedMemoryPool : public CBaseClassNonCopyable {   // vtable 0x0129C964, 0x464 bytes
public:
    class CHeader;
    class CHandleInfo {                                     // Ego_r.pdb: 4 bytes (a union)
    public:
        CHandleInfo();
        void* Data;
    };
    CHandleBasedMemoryPool();
    virtual ~CHandleBasedMemoryPool();
    void Uninitialise();                                    // 0x00A02190
    void*                     Buffer;                       // +0x04
    void*                     UnalignedBuffer;              // +0x08
    long                      PoolSize;                     // +0x0c
    long                      AllocatedSize;                // +0x10
    long                      NumAllocations;               // +0x14
    long                      LockCount;                    // +0x18
    long                      LargestFreeArea;              // +0x1c
    unsigned long             MemoryFlags;                  // +0x20
    bool                      MultiThreaded;                // +0x24
    bool                      ShutdownFlag;                 // +0x25
    bool                      EnableDefrag;                 // +0x26
    bool                      EnableFullDefragIfPartialFails; // +0x27
    unsigned long             MaxFreeSizeBeforeDefrag;      // +0x28
    unsigned long             MaxFreeSizeBeforeFailure;     // +0x2c
    long                      MaxBlocksToTryOnPartialDefrag; // +0x30
    CFailedAllocationHandler* FailureHandler;               // +0x34
    CHeader*                  FreeLists[256];               // +0x38
    CArray<CHandleInfo>       Handles;                      // +0x438
    long                      FirstUnusedHandle;            // +0x444
    CCharString               Name;                         // +0x448
    _RTL_CRITICAL_SECTION     CriticalSection;              // +0x44c
};

CHandleBasedMemoryPool::~CHandleBasedMemoryPool()
{
    Uninitialise();
}