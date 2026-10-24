// Behaviour test for 00a5e900 (std::__destroy_aux over SArrayHolderEntry): each element's members are destroyed in reverse declaration order and its array storage is freed once (a null array frees nothing).
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_frees = 0;
static void* g_lastFreed = 0;
static int g_events = 0;
static int g_order[16];
static void test_free(void* p) { ++g_frees; g_lastFreed = p; std::free(p); }
namespace std2 {
struct __true_type { };
struct __false_type { };
template <class T> inline void __destroy_trivial(T*, T*, const __true_type&) { }
template <class T> inline void _Destroy(T* first, T* last) { __destroy_trivial(first, last, __true_type()); }
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
        if (_M_start != 0) {
            g_order[g_events++] = 2;
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
        }
    }
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
template <class T> struct CArray : std2::vector<T> { };
class CBaseClass {
public:
    virtual ~CBaseClass() { g_order[g_events++] = 1; }   // stands in for 0x0099A2E0
};
class CArrayHolder : public CBaseClass {
public:
    CArray<long> m_array;
};
typedef std2::pair<unsigned long, CArrayHolder> SArrayHolderEntry;
void __destroy_aux(SArrayHolderEntry* first, SArrayHolderEntry* last, const std2::__false_type&)
{
    for (; first != last; ++first)
        std2::_Destroy(&*first);
}
union UStorage { double d; void* p; char bytes[sizeof(SArrayHolderEntry) * 2]; };
int main()
{
    UStorage storage;
    SArrayHolderEntry* items = reinterpret_cast<SArrayHolderEntry*>(storage.bytes);
    for (int i = 0; i < 2; ++i)
        new (&items[i]) SArrayHolderEntry();
    long* values = static_cast<long*>(std::malloc(3 * sizeof(long)));
    items[0].second.m_array._M_start = values;
    items[0].second.m_array._M_finish = values + 3;
    items[0].second.m_array._M_end_of_storage._M_data = values + 3;
    void* block = items[0].second.m_array._M_start;
    std2::__false_type tag;
    __destroy_aux(items, items, tag);
    if (g_events != 0 || g_frees != 0) { std::printf("00a5e900_TEST FAIL\n"); return 1; }
    __destroy_aux(items, items + 2, tag);
    // element 0: array freed (2) then the base destructor (1); element 1: only the base destructor
    if (g_frees != 1 || g_lastFreed != block || g_events != 3) { std::printf("00a5e900_TEST FAIL\n"); return 1; }
    if (g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 1) { std::printf("00a5e900_TEST FAIL\n"); return 1; }
    std::printf("00a5e900_TEST PASS\n");
    return 0;
}