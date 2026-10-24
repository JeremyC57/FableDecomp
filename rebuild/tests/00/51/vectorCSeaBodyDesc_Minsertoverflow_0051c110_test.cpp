// Behaviour test for 0x0051C110 vector<CSeaBodyDesc>::_M_insert_overflow: inserting one record mid-vector reallocates to
// old + max(old, 1) slots holding head, the new copy, then the tail; the old records are destroyed and their block freed;
// a 3-record fill at the end with atend set grows to old + max(old, 3) and copies no tail.
#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_strdtors, g_frees;
class CCharString {
public:
    const char* PString;
    CCharString(const char* s) : PString(s) {}
    CCharString(const CCharString& other) : PString(other.PString) {}
    ~CCharString() { ++g_strdtors; }
};

struct __false_type { __false_type() {} };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) { ++g_frees; std::free(p); } }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result,
                                                                  const __false_type&)   // 0x00518C00 (same-TU body)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x,
                                                                    const __false_type&)   // 0x00518C90 (same-TU body)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator first, _ForwardIterator last,
                                                   const __false_type&)   // 0x00519EB0 (same-TU body)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator first, _ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template <class T> class vector : public _Vector_base<T> {
public:
    vector() { this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0; }
    vector(const vector<T>& x)
    {
        unsigned int n = x.size();
        this->_M_start = n ? (T*)std::malloc(n * sizeof(T)) : 0;
        for (unsigned int i = 0; i < n; ++i) this->_M_start[i] = x._M_start[i];
        this->_M_finish = this->_M_start + n; this->_M_end_of_storage._M_data = this->_M_finish;
    }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template <class T>
void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);
    T* new_start = this->_M_end_of_storage.allocate(len);
    T* new_finish = new_start;
    new_finish = __uninitialized_copy(this->_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, this->_M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template <class T> class CArray : public vector<T> {};

class CStaticMapGenerationVisibiilityInfo {
public:
    struct CSeaBodyDesc {
        CCharString  Identifier;               // +0x0
        CArray<long> Maps;                     // +0x4
        CSeaBodyDesc(const char* n) : Identifier(n) {}
    };
};
typedef CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc CSeaBodyDesc;


typedef vector<CSeaBodyDesc> V;
static void put(CSeaBodyDesc* p, const char* name, long m0)
{
    new (p) CSeaBodyDesc(name);
    p->Maps._M_start = (long*)std::malloc(2 * sizeof(long)); p->Maps._M_start[0] = m0; p->Maps._M_start[1] = m0 + 1;
    p->Maps._M_finish = p->Maps._M_start + 2; p->Maps._M_end_of_storage._M_data = p->Maps._M_finish;
}
static int is(const CSeaBodyDesc& d, const char* name, long m0)
{
    return d.Identifier.PString == name && d.Maps._M_finish - d.Maps._M_start == 2 && d.Maps._M_start[0] == m0 && d.Maps._M_start[1] == m0 + 1;
}
int main()
{
    static const char a[] = "A", b[] = "B", x[] = "X", y[] = "Y";
    V v;
    CSeaBodyDesc* blk = (CSeaBodyDesc*)std::malloc(2 * sizeof(CSeaBodyDesc));
    put(blk, a, 10); put(blk + 1, b, 20);
    v._M_start = blk; v._M_finish = blk + 2; v._M_end_of_storage._M_data = blk + 2;
    char xr[sizeof(CSeaBodyDesc)]; CSeaBodyDesc* xp = (CSeaBodyDesc*)xr; put(xp, x, 30);
    __false_type ft;
    v._M_insert_overflow(v._M_start + 1, *xp, ft, 1, false);
    int ok = v._M_start != blk && v._M_finish == v._M_start + 3 && v._M_end_of_storage._M_data == v._M_start + 4;
    ok &= is(v._M_start[0], a, 10) && is(v._M_start[1], x, 30) && is(v._M_start[2], b, 20);
    ok &= g_strdtors == 2 && g_frees == 3;   // two old Maps blocks + the old record block
    char yr[sizeof(CSeaBodyDesc)]; CSeaBodyDesc* yp = (CSeaBodyDesc*)yr; put(yp, y, 40);
    g_strdtors = 0; g_frees = 0;
    v._M_insert_overflow(v._M_finish, *yp, ft, 3, true);
    ok &= v._M_finish == v._M_start + 6 && v._M_end_of_storage._M_data == v._M_start + 6;
    ok &= is(v._M_start[0], a, 10) && is(v._M_start[1], x, 30) && is(v._M_start[2], b, 20);
    for (int i = 3; i < 6; ++i) ok &= is(v._M_start[i], y, 40) && v._M_start[i].Maps._M_start != yp->Maps._M_start;
    ok &= g_strdtors == 3 && g_frees == 4;
    if (!ok) { std::printf("0051c110_TEST FAIL\n"); return 1; }
    std::printf("0051c110_TEST PASS\n");
    return 0;
}