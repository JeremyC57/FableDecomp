// Behaviour test for 0x00518C00: every record of [first, last) is copy-constructed into result -- the Identifier through its copy
// constructor and the Maps array as an exactly-sized deep copy (null pointers for an empty array); the end of the
// constructed range is returned and an empty request constructs nothing.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_strcopies;
class CCharString {
public:
    const char* PString;
    CCharString(const char* s) : PString(s) {}
    CCharString(const CCharString& other) : PString(other.PString) { ++g_strcopies; }
};

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
                           : ((char*)std::memmove(result, first, ((const char*)last - (const char*)first)))
                               + ((const char*)last - (const char*)first);
}
template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* first, const _Tp* last, _Tp* result, const __true_type&)
{
    return (_Tp*)__copy_trivial(first, last, result);
}
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator {
public:
    allocator() {}
    allocator(const allocator<T>&) {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
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
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector<T>& x) : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __true_type());
    }
};

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result, const __false_type&)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T> class CArray : public vector<T> {};

class CStaticMapGenerationVisibiilityInfo {
public:
    struct CSeaBodyDesc {
        CCharString  Identifier;               // +0x0
        CArray<long> Maps;                     // +0x4
    };
};
typedef CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc CSeaBodyDesc;


static void init(CSeaBodyDesc& d, const char* name, int n, long base)
{
    d.Identifier.PString = name;
    d.Maps._M_start = n ? (long*)std::malloc(n * sizeof(long)) : 0;
    for (int i = 0; i < n; ++i) d.Maps._M_start[i] = base + i;
    d.Maps._M_finish = d.Maps._M_start + n; d.Maps._M_end_of_storage._M_data = d.Maps._M_start + n + 1;
}
static int same(const CSeaBodyDesc& a, const CSeaBodyDesc& b)
{
    int n = (int)(b.Maps._M_finish - b.Maps._M_start);
    if (a.Identifier.PString != b.Identifier.PString) return 0;
    if (n == 0) return a.Maps._M_start == 0 && a.Maps._M_finish == 0 && a.Maps._M_end_of_storage._M_data == 0;
    return a.Maps._M_start != b.Maps._M_start && a.Maps._M_finish == a.Maps._M_start + n
        && a.Maps._M_end_of_storage._M_data == a.Maps._M_start + n && std::memcmp(a.Maps._M_start, b.Maps._M_start, n * sizeof(long)) == 0;
}
int main()
{
    static double rs[32], rd[32];
    CSeaBodyDesc* src = (CSeaBodyDesc*)rs; CSeaBodyDesc* dst = (CSeaBodyDesc*)rd;
    init(src[0], "Lake", 3, 10); init(src[1], "Pond", 0, 0); init(src[2], "Sea", 5, 100);
    __false_type ft;
    int ok = 1;
    CSeaBodyDesc* end = __uninitialized_copy(src, src + 3, dst, ft);
    ok &= end == dst + 3 && g_strcopies == 3;
    for (int i = 0; i < 3; ++i) ok &= same(dst[i], src[i]);
    ok &= __uninitialized_copy(src, src, dst + 3, ft) == dst + 3 && g_strcopies == 3;
    if (!ok) { std::printf("00518c00_TEST FAIL\n"); return 1; }
    std::printf("00518c00_TEST PASS\n");
    return 0;
}