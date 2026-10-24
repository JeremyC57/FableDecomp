// Behaviour test for 0x0051C1F0 vector<pair<unsigned __int64, list<C2DLineF> > >::_M_insert_overflow: inserting one
// record mid-vector reallocates to old + max(old, 1) slots (head, deep copy of x, tail); every old record's list is torn
// down (nodes and sentinel freed) and the old block freed; a 2-record fill at the end with atend set copies no tail.
#include <cstdio>
#include <cstdlib>
#include <new>
#ifdef _MSC_VER
#define K64(x) x##ui64
#else
#define __fastcall
#define __int64 long long
#define K64(x) x##ULL
#endif
static int g_frees, g_paircopies;
class C2DVector {
public:
    C2DVector() {}
    float X;                                   // +0x0
    float Y;                                   // +0x4
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}   // FableWin.pdb ??0C2DVector@@QAE@ABV0@@Z
};
class C2DLineF {
public:
    C2DLineF() {}
    C2DVector Point1;                          // +0x0
    C2DVector Point2;                          // +0x8
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
                                                                  const __false_type&)   // 0x0051A810 (same-TU body)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x,
                                                                    const __false_type&)   // 0x0051A880 (same-TU body)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator first, _ForwardIterator last,
                                                   const __false_type&)   // 0x00519EF0 (same-TU body)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator first, _ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> class _List_base {
public:
    _List_node<T>* _M_node;                    // sentinel (alloc proxy data)
    ~_List_base()
    {
        clear();
        allocator<_List_node<T> >().deallocate(_M_node, 1);
    }
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            _Destroy(&tmp->_M_data);
            allocator<_List_node<T> >().deallocate(tmp, 1);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
template <class T> class list : public _List_base<T> {
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first)
    {
        ++g_paircopies;
        second._M_node = (_List_node<C2DLineF>*)std::malloc(sizeof(_List_node<C2DLineF>));
        second._M_node->_M_next = second._M_node->_M_prev = second._M_node;
        for (_List_node_base* p = o.second._M_node->_M_next; p != o.second._M_node; p = p->_M_next) {
            _List_node<C2DLineF>* n = (_List_node<C2DLineF>*)std::malloc(sizeof(_List_node<C2DLineF>));
            new (&n->_M_data) C2DLineF(((_List_node<C2DLineF>*)p)->_M_data);
            n->_M_next = second._M_node; n->_M_prev = second._M_node->_M_prev;
            second._M_node->_M_prev->_M_next = n; second._M_node->_M_prev = n;
        }
    }
};

template <class T> class vector : public _Vector_base<T> {
public:
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


typedef pair<unsigned __int64, list<C2DLineF> > R;
typedef vector<R> V;
typedef _List_node<C2DLineF> N;
static void put(R* r, unsigned __int64 key, int lines)
{
    r->first = key;
    r->second._M_node = (N*)std::malloc(sizeof(N)); r->second._M_node->_M_next = r->second._M_node->_M_prev = r->second._M_node;
    for (int i = 0; i < lines; ++i) {
        N* n = (N*)std::malloc(sizeof(N)); n->_M_data.Point1.X = (float)(int)((key & 0xFF) + i);
        n->_M_next = r->second._M_node; n->_M_prev = r->second._M_node->_M_prev;
        r->second._M_node->_M_prev->_M_next = n; r->second._M_node->_M_prev = n;
    }
}
static int is(const R& r, unsigned __int64 key, int lines)
{
    if (r.first != key) return 0;
    _List_node_base* p = r.second._M_node->_M_next;
    for (int i = 0; i < lines; ++i, p = p->_M_next)
        if (p == r.second._M_node || ((N*)p)->_M_data.Point1.X != (float)(int)((key & 0xFF) + i)) return 0;
    return p == r.second._M_node;
}
int main()
{
    V v;
    R* blk = (R*)std::malloc(2 * sizeof(R));
    put(blk, K64(0x100000001), 2); put(blk + 1, 7, 1);
    v._M_start = blk; v._M_finish = blk + 2; v._M_end_of_storage._M_data = blk + 2;
    char xr[sizeof(R)]; R* xp = (R*)xr; put(xp, 3, 3);
    __false_type ft;
    v._M_insert_overflow(v._M_start + 1, *xp, ft, 1, false);
    int ok = v._M_start != blk && v._M_finish == v._M_start + 3 && v._M_end_of_storage._M_data == v._M_start + 4;
    ok &= is(v._M_start[0], K64(0x100000001), 2) && is(v._M_start[1], 3, 3) && is(v._M_start[2], 7, 1);
    ok &= g_paircopies == 3 && g_frees == 2 + 1 + 2 + 1;   // old lists: 2 nodes + sentinel, 1 node + sentinel; + block
    g_frees = 0; g_paircopies = 0;
    v._M_insert_overflow(v._M_finish, *xp, ft, 2, true);
    ok &= v._M_finish == v._M_start + 5 && v._M_end_of_storage._M_data == v._M_start + 6;
    ok &= is(v._M_start[3], 3, 3) && is(v._M_start[4], 3, 3) && v._M_start[3].second._M_node != xp->second._M_node;
    ok &= g_paircopies == 5;
    if (!ok) { std::printf("0051c1f0_TEST FAIL\n"); return 1; }
    std::printf("0051c1f0_TEST PASS\n");
    return 0;
}