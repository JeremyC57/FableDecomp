// std::deque::clear() -- STLport deque<CBezierCurve>::clear: frees every full interior node buffer, then the finish buffer when start and finish differ, and sets finish = start
// Leaf: ?clear@?$deque@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00913A20, 94 bytes (96 with padding), __fastcall.
// Identity: STLport 4.6 deque<CBezierCurve, allocator<CBezierCurve> >::clear; single caller 0x00912BB4 (the flying-thing path
//   builder). The catalog label "PathBuffer_ClearAndReset" is an autoname.
// Callees: free 0x00BFEA14 (allocator::deallocate's null-tested free).
// Element: CBezierCurve (Ego_r.pdb, 48 bytes = four packed C3DVectors), the flight-path deque of CAIStateGroup_FlyingThingBase
//   (Ego_r.pdb AddRandomBezierCurveToPath / CalculatePrettyPath take `deque<CBezierCurve>&`); the AI bodies at 0x00912B8C,
//   0x00912D22, 0x009130AE call this TU's _M_push_back_aux_v 0x00913D60. STLport buffer = 128 / 48 = 2 elements (0x60 bytes).
// Layout (STLport 4.6 _Deque_base, 0x28 bytes): _M_start {cur, first, last, node} +0x00, _M_finish +0x10, _M_map +0x20,
//   _M_map_size +0x24 (the map/node allocator proxies).
// Shape: the STLport body: the _Destroy calls over CBezierCurve* ranges (empty destructor) inline away, leaving the node
//   loop (`*node` freed when non-null, finish node re-read each pass) and the finish-buffer free; `_M_finish = _M_start` is
//   the iterator's implicit operator= (four dword moves through `lea ecx,[edi+10h]`).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);          // 0x00BFEA14

inline void* operator new(size_t, void* p) { return p; }

#pragma pack(push, 1)
class C3DVector {                               // retail: byte-packed, memberwise user copy ctor
public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

class CBezierCurve {                            // Ego_r.pdb, 48 bytes
public:
    C3DVector Point1;                           // +0x00
    C3DVector Point2;                           // +0x0C
    C3DVector Point3;                           // +0x18
    C3DVector Point4;                           // +0x24
};

namespace std {

struct __false_type {                           // STLport empty tag; a user ctor keeps the temporary store-free
    __false_type() { }
};

template <class _Tp> class allocator {          // STLport allocator over the CRT malloc/free
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, size_t) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> struct _Deque_iterator_base {
    enum _Constants { _blocksize = 128, __buffer_size = (sizeof(_Tp) < (size_t)_blocksize ? ((size_t)_blocksize / sizeof(_Tp)) : size_t(1)) };
    typedef _Tp** _Map_pointer;
    _Tp* _M_cur;                                // +0x00
    _Tp* _M_first;                              // +0x04
    _Tp* _M_last;                               // +0x08
    _Map_pointer _M_node;                       // +0x0C
    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    void _M_increment()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }
    void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + ptrdiff_t(__buffer_size);
    }
};

template <class _Tp> struct _Nonconst_traits { };

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    _Deque_iterator() { }
    _Deque_iterator(const _Self& __x) : _Deque_iterator_base<_Tp>(__x) { }
    _Tp& operator*() const { return *this->_M_cur; }
    _Self& operator++() { this->_M_increment(); return *this; }
};

template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator!=(const _Deque_iterator<_Tp, _Traits1>& __x, const _Deque_iterator<_Tp, _Traits2>& __y)
{
    return __x._M_cur != __y._M_cur;
}

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(__first);
}

template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _STLP_alloc_proxy<size_t, _Tp, _Alloc> _Alloc_proxy;
    typedef _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _Map_alloc_proxy;
    static size_t buffer_size() { return (size_t)_Deque_iterator_base<_Tp>::__buffer_size; }
    enum { _S_initial_map_size = 8 };
    void _M_initialize_map(size_t);
    void _M_create_nodes(_Tp** __nstart, _Tp** __nfinish);

    iterator _M_start;                          // +0x00
    iterator _M_finish;                         // +0x10
    _Map_alloc_proxy _M_map;                    // +0x20
    _Alloc_proxy _M_map_size;                   // +0x24
};

template <class _Tp, class _Alloc>
void _Deque_base<_Tp, _Alloc>::_M_create_nodes(_Tp** __nstart, _Tp** __nfinish)
{
    _Tp** __cur;
    for (__cur = __nstart; __cur < __nfinish; ++__cur)
        *__cur = _M_map_size.allocate(buffer_size());
}

template <class _Tp, class _Alloc>
void _Deque_base<_Tp, _Alloc>::_M_initialize_map(size_t __num_elements)
{
    size_t __num_nodes = __num_elements / buffer_size() + 1;
    _M_map_size._M_data = (max)((size_t)_S_initial_map_size, __num_nodes + 2);
    _M_map._M_data = _M_map.allocate(_M_map_size._M_data);
    _Tp** __nstart = _M_map._M_data + (_M_map_size._M_data - __num_nodes) / 2;
    _Tp** __nfinish = __nstart + __num_nodes;
    _M_create_nodes(__nstart, __nfinish);
    _M_start._M_set_node(__nstart);
    this->_M_finish._M_set_node(__nfinish - 1);
    _M_start._M_cur = _M_start._M_first;
    this->_M_finish._M_cur = this->_M_finish._M_first + __num_elements % buffer_size();
}

template <class _T1, class _T2> void __fastcall _Construct(_T1* __p, const _T2& __val);   // 0x00913590 (out of line)

template <class _Tp, class _Alloc = allocator<_Tp> >
class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _Tp** _Map_pointer;
    void push_back(const value_type& __t)
    {
        if (this->_M_finish._M_cur != this->_M_finish._M_last - 1) {
            _Construct(this->_M_finish._M_cur, __t);
            ++this->_M_finish._M_cur;
        }
        else
            _M_push_back_aux_v(__t);
    }
    void clear();
    protected:
    void _M_push_back_aux_v(const value_type& __t);     // 0x00913D60
    public:
};

template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::clear()
{
    for (_Map_pointer __node = this->_M_start._M_node + 1; __node < this->_M_finish._M_node; ++__node) {
        _Destroy(*__node, *__node + this->buffer_size());
        this->_M_map_size.deallocate(*__node, this->buffer_size());
    }
    if (this->_M_start._M_node != this->_M_finish._M_node) {
        _Destroy(this->_M_start._M_cur, this->_M_start._M_last);
        _Destroy(this->_M_finish._M_first, this->_M_finish._M_cur);
        this->_M_map_size.deallocate(this->_M_finish._M_first, this->buffer_size());
    }
    else
        _Destroy(this->_M_start._M_cur, this->_M_finish._M_cur);
    this->_M_finish = this->_M_start;
}

}

template void std::deque<CBezierCurve, std::allocator<CBezierCurve> >::clear();