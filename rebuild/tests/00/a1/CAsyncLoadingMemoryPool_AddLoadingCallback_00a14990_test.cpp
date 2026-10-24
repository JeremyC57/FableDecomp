// Behaviour test for CAsyncLoadingMemoryPool::AddLoadingCallback (0x00A14990): push_back on the static callback
// vector, in place while capacity remains, through _M_insert_overflow (grow, copy, append) when it is full.
#include <cstdio>
#include <cstdlib>
#include <cstring>

inline void* operator new(size_t, void* where, int) { return where; }

namespace _STL {
struct __true_type { __true_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p, 0) _T1(__val); }
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend)
    {
        unsigned int __old_size = (unsigned int)(_M_finish - _M_start);
        unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
        _Tp* __new_start = __len ? (_Tp*)std::malloc(__len * sizeof(_Tp)) : 0;
        _Tp* __new_finish = __new_start;
        if (__position != _M_start) {
            std::memmove(__new_start, _M_start, (char*)__position - (char*)_M_start);
            __new_finish += __position - _M_start;
        }
        for (unsigned int i = 0; i < __fill_len; ++i) *__new_finish++ = __x;
        if (!__atend && _M_finish != __position) {
            std::memmove(__new_finish, __position, (char*)_M_finish - (char*)__position);
            __new_finish += _M_finish - __position;
        }
        if (_M_start) std::free(_M_start);
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage._M_data = __new_start + __len;
    }
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
    static void AddLoadingCallback(CAsyncLoadingProcessCallback* callback);
    static _STL::vector<CAsyncLoadingProcessCallback*> LoadingCallbacks;
};

_STL::vector<CAsyncLoadingProcessCallback*> CAsyncLoadingMemoryPool::LoadingCallbacks;

void CAsyncLoadingMemoryPool::AddLoadingCallback(CAsyncLoadingProcessCallback* callback)
{
    LoadingCallbacks.push_back(callback);
}

class CDummyCallback : public CAsyncLoadingProcessCallback {
public:
    virtual void ProcessLoading() {}
};

int main()
{
    CDummyCallback cb[5];
    int ok = 1;
    _STL::vector<CAsyncLoadingProcessCallback*>& v = CAsyncLoadingMemoryPool::LoadingCallbacks;
    v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;

    CAsyncLoadingMemoryPool::AddLoadingCallback(&cb[0]);          // empty -> overflow path, capacity 1
    if (v._M_finish - v._M_start != 1 || v._M_start[0] != &cb[0]) ok = 0;
    for (int i = 1; i < 5; ++i) CAsyncLoadingMemoryPool::AddLoadingCallback(&cb[i]);
    if (v._M_finish - v._M_start != 5) ok = 0;
    for (int i = 0; i < 5 && ok; ++i) if (v._M_start[i] != &cb[i]) ok = 0;

    // room left -> in-place construct, no reallocation
    CAsyncLoadingProcessCallback* storage[4];
    v._M_start = storage; v._M_finish = storage; v._M_end_of_storage._M_data = storage + 4;
    CAsyncLoadingMemoryPool::AddLoadingCallback(&cb[2]);
    CAsyncLoadingMemoryPool::AddLoadingCallback(&cb[3]);
    if (v._M_start != storage || v._M_finish != storage + 2 || storage[0] != &cb[2] || storage[1] != &cb[3]) ok = 0;

    if (!ok) { std::printf("00a14990_TEST FAIL\n"); return 1; }
    std::printf("00a14990_TEST PASS\n");
    return 0;
}