// Behaviour test for 009b8070 (std::__destroy_aux over SDefClassInfoEntry): each element's members are destroyed in reverse declaration order and its array storage is freed once (a null array frees nothing).
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
template <class T> class CArray : public std2::vector<T> { public: };
class CCharString { public:
    void* m_pData;
    CCharString() : m_pData(0) { }
    ~CCharString() { g_order[g_events++] = 1; }    // stands in for 0x0099EAE0
};
class CDefClassBase;
class CDefinitionManager { public:
    class CDefClassInfo { public:
        CCharString Name;
        CDefClassBase* (*AllocFunc)();
        CArray<unsigned long> GlobalIndices;
    };
};
typedef std2::pair<unsigned long, CDefinitionManager::CDefClassInfo> SDefClassInfoEntry;
void __destroy_aux(SDefClassInfoEntry* first, SDefClassInfoEntry* last, const std2::__false_type&)
{
    for (; first != last; ++first)
        std2::_Destroy(&*first);
}
union UStorage { double d; void* p; char bytes[sizeof(SDefClassInfoEntry) * 2]; };
int main()
{
    UStorage storage;
    SDefClassInfoEntry* items = reinterpret_cast<SDefClassInfoEntry*>(storage.bytes);
    for (int i = 0; i < 2; ++i)
        new (&items[i]) SDefClassInfoEntry();
    unsigned long* indices = static_cast<unsigned long*>(std::malloc(4 * sizeof(unsigned long)));
    items[0].second.GlobalIndices._M_start = indices;
    items[0].second.GlobalIndices._M_finish = indices + 2;
    items[0].second.GlobalIndices._M_end_of_storage._M_data = indices + 4;
    void* block = items[0].second.GlobalIndices._M_start;
    std2::__false_type tag;
    __destroy_aux(items, items, tag);
    if (g_events != 0 || g_frees != 0) { std::printf("009b8070_TEST FAIL\n"); return 1; }
    __destroy_aux(items, items + 2, tag);
    // element 0: array freed (2) then the name (1); element 1: only the name
    if (g_frees != 1 || g_lastFreed != block || g_events != 3) { std::printf("009b8070_TEST FAIL\n"); return 1; }
    if (g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 1) { std::printf("009b8070_TEST FAIL\n"); return 1; }
    std::printf("009b8070_TEST PASS\n");
    return 0;
}