// CAsyncLoadingMemoryPool::AddLoadingCallback(CAsyncLoadingProcessCallback* callback) -- append a callback to the static async-loading callback list
// Leaf: ?AddLoadingCallback@CAsyncLoadingMemoryPool@@SIXPAVCAsyncLoadingProcessCallback@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14990, 72 bytes, plain `ret` (static __fastcall, callback in ecx).
// Identity: PDB `?AddLoadingCallback@CAsyncLoadingMemoryPool@@SIXPAVCAsyncLoadingProcessCallback@@@Z`; callers are the
//   CEngineResourceManager constructors, which register their CAsyncLoadingProcessCallback base (this+0x0C). The
//   catalog label "_global::StackAllocator_Push" is a descriptive auto label.
// Shape: STLport `vector<CAsyncLoadingProcessCallback*>::push_back` inlined on the static LoadingCallbacks
//   (0x013D2770): `_Construct` = placement new with its null test, else `_M_insert_overflow(_M_finish, x, tag, 1,
//   true)` (0x00A14CB0) with the empty tag at frame byte 3 and the homed callback at frame word 1.
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};
}

class CAsyncLoadingProcessCallback {
public:
    virtual void ProcessLoading() = 0;
};

class CAsyncLoadingMemoryPool {
public:
    static void __fastcall AddLoadingCallback(CAsyncLoadingProcessCallback* callback);
    static std::vector<CAsyncLoadingProcessCallback*> LoadingCallbacks;
};

void __fastcall CAsyncLoadingMemoryPool::AddLoadingCallback(CAsyncLoadingProcessCallback* callback)
{
    LoadingCallbacks.push_back(callback);
}