// CAsyncLoadingMemoryPool::RemoveLoadingCallback(CAsyncLoadingProcessCallback* callback) -- erase a callback from the static async-loading callback list
// Leaf: ?RemoveLoadingCallback@CAsyncLoadingMemoryPool@@SIXPAVCAsyncLoadingProcessCallback@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14930, 81 bytes, plain `ret` (static __fastcall, callback in ecx).
// Identity: PDB `?RemoveLoadingCallback@CAsyncLoadingMemoryPool@@SIXPAVCAsyncLoadingProcessCallback@@@Z`; callers are
//   the CEngineResourceManager destructors. The catalog label "_global::StackAllocator_Pop" is a descriptive auto label.
// Shape: `find` (out-of-line STLport `__find` with the random-access tag, 0x00A14AC0, __fastcall: first/last in
//   ecx/edx, &value and &tag on the stack, `ret 8`) then the inline single-element `erase` through `__copy_ptrs`/`__copy_trivial`
//   (memmove 0x00BFEAE6). `_M_finish` stays cached in esi across the `__find` call only because `__find`'s body is
//   visible in the same TU (VC7.1 same-TU mod/ref): with a bare declaration the lane reloads it after the call.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);

namespace std {
struct __true_type { __true_type() {} };
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}
struct random_access_iterator_tag { random_access_iterator_tag() {} };
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val, const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    typedef _Tp* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, __true_type());
        --this->_M_finish;
        return __position;
    }
};
}

class CAsyncLoadingProcessCallback {
public:
    virtual void ProcessLoading() = 0;
};

class CAsyncLoadingMemoryPool {
public:
    static void __fastcall RemoveLoadingCallback(CAsyncLoadingProcessCallback* callback);
    static std::vector<CAsyncLoadingProcessCallback*> LoadingCallbacks;
};

void __fastcall CAsyncLoadingMemoryPool::RemoveLoadingCallback(CAsyncLoadingProcessCallback* callback)
{
    std::vector<CAsyncLoadingProcessCallback*>::iterator it = std::find(LoadingCallbacks.begin(), LoadingCallbacks.end(), callback);
    if (it != LoadingCallbacks.end())
        LoadingCallbacks.erase(it);
}