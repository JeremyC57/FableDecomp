// vector<pair<unsigned __int64, list<C2DLineF> > >::_M_insert_overflow(value_type* position, const value_type& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD vector growth for CNavigationLayer's (UID, line list) records: allocate old + max(old, n), copy the head, construct the n new copies, copy the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@_KV?$list@VC2DLineF@@
//
// Retail 0x0051C1F0, 208 bytes, thiscall `ret 14h`.  Callees: 0x00BFEA0E malloc, 0x0051A810 __uninitialized_copy (twice),
//   0x005199E0 pair copy constructor (the fill_len == 1 _Construct; this chunk), 0x0051A880 __uninitialized_fill_n,
//   0x00519EF0 __destroy_aux (landed), 0x00BFEA14 free.
// Caller: the vector's push_back 0x0051C850 (landed).
// Element: the 16-byte (unsigned __int64 key, list<C2DLineF>) record of Ego_r.pdb CNavigationLayer's
//   CVectorMap<unsigned __int64, list<C2DLineF>, ...> (see 0x005199E0).
// Shape: the 0x0051C110 template -- STLport `_M_insert_overflow(..., const __false_type&, ...)` with const-reference max,
//   `len ? malloc(len * 16) : 0`, head copy, `fill_len == 1` _Construct (null test, then the out-of-line pair copy) or
//   __uninitialized_fill_n, tail copy unless atend, _M_clear (__destroy_aux + `if (_M_start) free`) and _M_set.
// Lever: as 0x0051C110 -- the `__false_type()` temporaries overlay the live atend slot only when the receiving
//   templates are same-TU __declspec(noinline) bodies with the tag unnamed; the pair copy constructor is declared only.
// Label: catalog "Vector_InsertRangeGeneric_CMusicSetEntry" (propagated).
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class C2DVector {
public:
    float X;                                   // +0x0
    float Y;                                   // +0x4
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}   // FableWin.pdb ??0C2DVector@@QAE@ABV0@@Z
};
class C2DLineF {
public:
    C2DVector Point1;                          // +0x0
    C2DVector Point2;                          // +0x8
};

namespace std {
struct __false_type { __false_type() {} };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
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
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result,
                                                                  const __false_type&)   // 0x0051A810 (same-TU body)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x,
                                                                    const __false_type&)   // 0x0051A880 (same-TU body)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator first, _ForwardIterator last,
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
    pair(const pair<T1, T2>& o);               // 0x005199E0 (out of line)
};

template <class T> class vector : public _Vector_base<T> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
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
}

typedef std::pair<unsigned __int64, std::list<C2DLineF> > CLineListRecord;

template void std::vector<CLineListRecord>::_M_insert_overflow(CLineListRecord*, const CLineListRecord&,
                                                              const std::__false_type&, unsigned int, bool);