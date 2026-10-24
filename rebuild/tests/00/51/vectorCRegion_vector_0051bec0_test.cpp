// Behaviour test for 0x0051BEC0 vector<CRegion>(n): allocates exactly n slots (none for n == 0), copy-constructs every
// slot from one default-constructed temporary, and destroys that temporary once.
#include <cstdio>
#include <cstdlib>
#include <new>
using std::malloc;
static int g_defaults, g_copies, g_dtors;
class CRegion {
public:
    int Tag;
    CRegion() : Tag(42) { ++g_defaults; }
    CRegion(const CRegion& other) : Tag(other.Tag + 1) { ++g_copies; }
    ~CRegion() { ++g_dtors; }
};
namespace std {
struct __false_type { __false_type() {} };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(unsigned int n, const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};
template <class T> class vector : public _Vector_base<T> {
public:
    explicit vector(unsigned int n) : _Vector_base<T>(n, allocator<T>())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};
}

int main()
{
    std::vector<CRegion> e(0);
    int ok = e._M_start == 0 && e._M_finish == 0 && e._M_end_of_storage._M_data == 0;
    ok &= g_defaults == 1 && g_copies == 0 && g_dtors == 1;
    std::vector<CRegion> v(3);
    ok &= v._M_start != 0 && v._M_finish == v._M_start + 3 && v._M_end_of_storage._M_data == v._M_start + 3;
    ok &= g_defaults == 2 && g_copies == 3 && g_dtors == 2;
    ok &= v._M_start[0].Tag == 43 && v._M_start[1].Tag == 43 && v._M_start[2].Tag == 43;
    std::free(v._M_start);
    if (!ok) { std::printf("0051bec0_TEST FAIL\n"); return 1; }
    std::printf("0051bec0_TEST PASS\n");
    return 0;
}