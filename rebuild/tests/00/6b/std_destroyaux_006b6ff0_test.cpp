// Behaviour test for 006b6ff0 (std::__destroy_aux over vector<SThemeBlock>): each vector destroys its blocks through the element __destroy_aux, then frees its storage (an empty vector frees nothing).
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_frees = 0;
static int g_blockRanges = 0;
static int g_blocksDestroyed = 0;
static void test_free(void* p) { ++g_frees; std::free(p); }
struct SThemeBlock {
    unsigned char _data[0x1f4];
};
namespace std2 {
struct __false_type { };
void __destroy_aux(SThemeBlock* first, SThemeBlock* last, const __false_type&)   // stands in for 0x006B67A0
{
    ++g_blockRanges;
    g_blocksDestroyed += (int)(last - first);
}
inline void _Destroy(SThemeBlock* first, SThemeBlock* last)
{
    __false_type tag;
    __destroy_aux(first, last, tag);
}
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> struct allocator {
    void deallocate(T* p, unsigned int) { test_free(p); }
};
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
void __destroy_aux(vector<SThemeBlock>* first, vector<SThemeBlock>* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}
union UStorage { double d; void* p; char bytes[sizeof(std2::vector<SThemeBlock>) * 2]; };
int main()
{
    UStorage storage;
    std2::vector<SThemeBlock>* v = reinterpret_cast<std2::vector<SThemeBlock>*>(storage.bytes);
    new (&v[0]) std2::vector<SThemeBlock>();
    new (&v[1]) std2::vector<SThemeBlock>();
    SThemeBlock* blocks = static_cast<SThemeBlock*>(std::malloc(3 * sizeof(SThemeBlock)));
    v[0]._M_start = blocks;
    v[0]._M_finish = blocks + 3;
    v[0]._M_end_of_storage._M_data = blocks + 3;
    std2::__false_type tag;
    std2::__destroy_aux(v, v, tag);
    if (g_blockRanges != 0 || g_frees != 0) { std::printf("006b6ff0_TEST FAIL\n"); return 1; }
    std2::__destroy_aux(v, v + 2, tag);
    if (g_blockRanges != 2 || g_blocksDestroyed != 3 || g_frees != 1) { std::printf("006b6ff0_TEST FAIL\n"); return 1; }
    std::printf("006b6ff0_TEST PASS\n");
    return 0;
}