// std::deque::_M_push_back_aux_v(const CBezierCurve& t) -- STLport deque<CBezierCurve> slow push_back: copy t, make room in the node map, allocate the next 2-element buffer, construct the copy at the old finish cursor and move the finish iterator onto the new buffer
// Leaf: ?_M_push_back_aux_v@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00913D60, 272 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: the catalog's "PathBuilder_AppendBezierCurveNode" is descriptive. Callers: the flying-thing path code
//   0x00912B8C / 0x00912D22 / 0x009130AE (inlined push_back) and the out-of-line deque<CBezierCurve>::push_back
//   0x00913E70 (landed, call 0x00913E98). Element: CBezierCurve (Ego_r.pdb, 48 bytes = four packed C3DVectors), the
//   flight-path deque of CAIStateGroup_FlyingThingBase; STLport buffer = 128 / 48 = 2 elements (0x60 bytes).
// Callees: deque::_M_reallocate_map(nodes_to_add, add_at_front) 0x00913A80, malloc 0x00BFEA0E.
// Shape: STLport 4.6 `_M_push_back_aux_v` (as the landed deque<CCrimeWaitingForBribe> twin 0x007B4F70): `__t_copy`
//   built in the frame, _M_reserve_map_at_back inlined (`2 > map_size - (finish.node - map)`, unsigned), the node
//   buffer from allocator::allocate(buffer_size()) folded to `malloc(60h)`, the placement-new copy under its null test,
//   then _M_set_node(node + 1) and `cur = first`. `_Construct` must be __forceinline: with a plain inline VC7.1 keeps it
//   an out-of-line call here (the TU's push_back 0x00913E70 does call an out-of-line _Construct, 0x00913590).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

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
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class T> struct _Deque_iterator {
    T*  _M_cur;                                             // +0x00
    T*  _M_first;                                           // +0x04
    T*  _M_last;                                            // +0x08
    T** _M_node;                                            // +0x0c
    static unsigned int _S_buffer_size() { return 128 / sizeof(T); }
    void _M_set_node(T** __new_node)
    {
        _M_node = __new_node;
        _M_first = *__new_node;
        _M_last = _M_first + _S_buffer_size();
    }
};

template <class T> class deque {
public:
    typedef T value_type;
    typedef unsigned int size_type;
    _Deque_iterator<T> _M_start;                            // +0x00
    _Deque_iterator<T> _M_finish;                           // +0x10
    T**                _M_map;                              // +0x20
    size_type          _M_map_size;                         // +0x24
    static size_type buffer_size() { return 128 / sizeof(T); }
    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);   // 0x00913A80
    void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add + 1 > this->_M_map_size - (this->_M_finish._M_node - this->_M_map))
            _M_reallocate_map(__nodes_to_add, false);
    }
    void _M_push_back_aux_v(const value_type& __t);
};

template <class T> void deque<T>::_M_push_back_aux_v(const value_type& __t)
{
    value_type __t_copy = __t;
    _M_reserve_map_at_back();
    *(this->_M_finish._M_node + 1) = allocator<T>().allocate(buffer_size());
    _Construct(this->_M_finish._M_cur, __t_copy);
    this->_M_finish._M_set_node(this->_M_finish._M_node + 1);
    this->_M_finish._M_cur = this->_M_finish._M_first;
}

template void deque<CBezierCurve>::_M_push_back_aux_v(const CBezierCurve&);
}