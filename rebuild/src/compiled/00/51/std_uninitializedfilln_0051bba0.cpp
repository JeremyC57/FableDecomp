// std::__uninitialized_fill_n(CNavigationLayerUpdate* first, unsigned int n, const CNavigationLayerUpdate& x, const __false_type&) -- copy-construct n navigation layer update records (line list + switchable blockages) from x, returning the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051BBA0, 241 bytes, __fastcall (ecx = first, edx = n; x and the tag on the
//   stack), `ret 8`, returns first + n.
// Identity: the 0x18-byte element is Ego_r.pdb CNavigationLayerUpdate {vector<C2DLineF> LineList;
//   vector<CNavigationAreaUpdate::CSwitchableBlockage> SwitchableBlockages} with 12-byte STLport vectors (the landed
//   __uninitialized_copy 0x0051C590 copies the same record); CSwitchableBlockage is {unsigned __int64 UniqueID;
//   list<C2DLineF> LineList} (16 bytes, landed default ctor 0x005155F0). Callers: the vector's fill-insert paths
//   0x0051D400 / 0x0051D630 (and 0x0051C590's caller). Catalog label "VectorReallocateAndCopyMultipleStructures" is
//   descriptive.
// Callees: malloc 0x00BFEA0E (inline allocate, twice per element), __uninitialized_copy<const CSwitchableBlockage*,
//   CSwitchableBlockage*> 0x0051AC00 (__fastcall, out of line; same-TU noinline stand-in here).
// Shape: STLport 4.6 `for (; n > 0; --n, ++cur) _Construct(&*cur, x);` (unsigned n: `jbe`, the count homed in the
//   `push ecx` slot) over the implicit CNavigationLayerUpdate copy constructor: two vector copy constructors
//   (`_Vector_base(x.size(), x.get_allocator())` zero stores, `n ? malloc(n * 16) : 0`, finish = start, end = start +
//   n), the C2DLineF __uninitialized_copy inlined (null-checked placement copy of four dwords), the blockage copy
//   called out of line with its `__false_type()` temporary homed in the dead x argument slot.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class C2DVector {
public:
    float X;                                   // +0x0
    float Y;                                   // +0x4
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
class C2DLineF {
public:
    C2DVector Point1;                          // +0x0
    C2DVector Point2;                          // +0x8
};

namespace std {
struct __false_type { __false_type() {} };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator {
public:
    allocator() {}
    allocator(const allocator&) {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result, const __false_type&)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};

template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef const T* const_pointer;
    A get_allocator() const { return A(); }
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    vector(const vector<T, A>& x) : _Vector_base<T, A>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)x._M_start, (const_pointer)x._M_finish, this->_M_start,
                                               __false_type());
    }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> class list {
public:
    _List_node_base* _M_node;
    list(const list<T>& x);                     // 0x005152C0-based copy (out of line)
};
}

class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {
    public:
        unsigned __int64       UniqueID;        // +0x00
        std::list<C2DLineF>   LineList;        // +0x08
    };
};
typedef CNavigationAreaUpdate::CSwitchableBlockage CSwitchableBlockage;

namespace std {
__declspec(noinline) CSwitchableBlockage* __fastcall __uninitialized_copy(const CSwitchableBlockage* first,
    const CSwitchableBlockage* last, CSwitchableBlockage* result, const __false_type&)   // 0x0051AC00
{
    CSwitchableBlockage* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
}

class CNavigationLayerUpdate {
public:
    std::vector<C2DLineF>            LineList;              // +0x00
    std::vector<CSwitchableBlockage> SwitchableBlockages;   // +0x0c
};

namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
}

template CNavigationLayerUpdate* __fastcall std::__uninitialized_fill_n(CNavigationLayerUpdate*, unsigned int,
    const CNavigationLayerUpdate&, const std::__false_type&);