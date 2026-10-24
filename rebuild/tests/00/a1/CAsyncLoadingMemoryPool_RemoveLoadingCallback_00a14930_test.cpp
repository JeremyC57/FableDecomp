// Behaviour test for CAsyncLoadingMemoryPool::RemoveLoadingCallback (0x00A14930): find the callback in the static
// vector and erase it (shift the tail down by one), and leave the vector alone when it is not registered.
#include <cstdio>
#include <cstring>

namespace _STL {
struct __true_type { __true_type() {} };
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)std::memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val, const random_access_iterator_tag&)
{
    int __trip_count = (int)(__last - __first) >> 2;
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
    static void RemoveLoadingCallback(CAsyncLoadingProcessCallback* callback);
    static _STL::vector<CAsyncLoadingProcessCallback*> LoadingCallbacks;
};

_STL::vector<CAsyncLoadingProcessCallback*> CAsyncLoadingMemoryPool::LoadingCallbacks;

void CAsyncLoadingMemoryPool::RemoveLoadingCallback(CAsyncLoadingProcessCallback* callback)
{
    _STL::vector<CAsyncLoadingProcessCallback*>::iterator it = _STL::find(LoadingCallbacks.begin(), LoadingCallbacks.end(), callback);
    if (it != LoadingCallbacks.end())
        LoadingCallbacks.erase(it);
}

class CDummyCallback : public CAsyncLoadingProcessCallback {
public:
    virtual void ProcessLoading() {}
};

int main()
{
    CDummyCallback cb[7];
    CAsyncLoadingProcessCallback* storage[8];
    int ok = 1;
    _STL::vector<CAsyncLoadingProcessCallback*>& v = CAsyncLoadingMemoryPool::LoadingCallbacks;
    for (int i = 0; i < 6; ++i) storage[i] = &cb[i];
    v._M_start = storage; v._M_finish = storage + 6; v._M_end_of_storage._M_data = storage + 8;

    CAsyncLoadingMemoryPool::RemoveLoadingCallback(&cb[6]);            // not registered: unchanged
    if (v._M_finish != storage + 6) ok = 0;
    CAsyncLoadingMemoryPool::RemoveLoadingCallback(&cb[1]);            // middle: tail shifts down
    if (v._M_finish != storage + 5 || storage[0] != &cb[0] || storage[1] != &cb[2] || storage[4] != &cb[5]) ok = 0;
    CAsyncLoadingMemoryPool::RemoveLoadingCallback(&cb[5]);            // last: no move, just shrink
    if (v._M_finish != storage + 4 || storage[3] != &cb[4]) ok = 0;
    CAsyncLoadingMemoryPool::RemoveLoadingCallback(&cb[0]);            // first (found by the unrolled loop)
    if (v._M_finish != storage + 3 || storage[0] != &cb[2] || storage[1] != &cb[3] || storage[2] != &cb[4]) ok = 0;
    CAsyncLoadingMemoryPool::RemoveLoadingCallback(&cb[0]);            // already gone
    if (v._M_finish != storage + 3) ok = 0;

    if (!ok) { std::printf("00a14930_TEST FAIL\n"); return 1; }
    std::printf("00a14930_TEST PASS\n");
    return 0;
}