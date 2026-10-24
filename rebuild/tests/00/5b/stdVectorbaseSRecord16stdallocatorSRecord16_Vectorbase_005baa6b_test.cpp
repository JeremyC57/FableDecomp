// behaviour test for 005baa6b / STLport _Vector_base(n, alloc) sized constructor (16-byte records, inlined allocate)
#include <cstdio>
#include <cstdlib>
#include <stddef.h>
struct SRecord20 { int Value[4]; };
static size_t g_lastAlloc = 0;
static int g_calls = 0;
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(size_t n, const void* = 0) { ++g_calls; g_lastAlloc = n * sizeof(T); return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
};
template <class Value, class MaybeReboundAlloc>
class alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
    alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) {}
};
template <class T, class Alloc>
class VectorBaseModel {
public:
    VectorBaseModel(size_t n, const Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;
    T* _M_finish;
    alloc_proxy<T*, Alloc> _M_end_of_storage;
};
int main()
{
    int bad = 0;
    allocator<SRecord20> a;
    VectorBaseModel<SRecord20, allocator<SRecord20> > v(7, a);
    if (v._M_start == 0 || v._M_finish != v._M_start || v._M_end_of_storage._M_data != v._M_start + 7) ++bad;
    if ((char*)v._M_end_of_storage._M_data - (char*)v._M_start != 112 || g_lastAlloc != 112 || g_calls != 1) ++bad;
    std::free(v._M_start);
    VectorBaseModel<SRecord20, allocator<SRecord20> > e(0, a);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage._M_data != 0 || g_calls != 2) ++bad;
    if (bad) { std::printf("005baa6b_TEST FAIL\n"); return 1; }
    std::printf("005baa6b_TEST PASS\n");
    return 0;
}