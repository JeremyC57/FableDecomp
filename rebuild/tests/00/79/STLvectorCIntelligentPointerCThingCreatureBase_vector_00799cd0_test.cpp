#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __fastcall
#endif

// Model: vector(n) allocates n elements (none for n == 0), fills them with null pointers through the
// __uninitialized_fill_n stand-in, and destroys its default element temporary.
static int g_live = 0;

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
    CBaseIntelligentPointer(const CBaseIntelligentPointer& o) : m_pItem(o.m_pItem) { ++g_live; }
    ~CBaseIntelligentPointer() { --g_live; }
};
typedef CBaseIntelligentPointer CCreaturePointer;

struct __false_type { __false_type() { } };
inline void* operator new(size_t, void* where) { return where; }

static CCreaturePointer* __fastcall __uninitialized_fill_n(CCreaturePointer* first, unsigned int n, const CCreaturePointer& x,
                                                           const __false_type&)
{
    CCreaturePointer* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CCreaturePointer(x);
    return cur;
}
template <class T> inline T* uninitialized_fill_n(T* first, unsigned int n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}
template <class T> struct allocator {
    T* allocate(unsigned int n) const { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>&, T* p) : _M_data(p) { }
};
template <class T> struct _Vector_base {
    T*                   _M_start;
    T*                   _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(unsigned int n, const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};
template <class T> struct vector : _Vector_base<T> {
    explicit vector(unsigned int n) : _Vector_base<T>(n, allocator<T>())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};

int main()
{
    vector<CCreaturePointer> v(4);
    bool ok = v._M_finish - v._M_start == 4 && v._M_end_of_storage._M_data == v._M_start + 4 && g_live == 4;
    for (int i = 0; i < 4; ++i)
        ok = ok && v._M_start[i].m_pItem == 0;
    vector<CCreaturePointer> e(0);
    ok = ok && e._M_start == 0 && e._M_finish == 0 && e._M_end_of_storage._M_data == 0 && g_live == 4;
    for (int i = 0; i < 4; ++i)
        v._M_start[i].~CBaseIntelligentPointer();
    std::free(v._M_start);
    if (!ok) {
        std::printf("00799cd0_TEST FAIL\n");
        return 1;
    }
    std::printf("00799cd0_TEST PASS\n");
    return 0;
}