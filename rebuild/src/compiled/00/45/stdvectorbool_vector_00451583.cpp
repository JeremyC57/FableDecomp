// std::vector<bool>::vector(unsigned int n, const bool& value, const allocator<bool>& a) -- STLport fill constructor: _Vector_base(n, a) allocates n bytes, then uninitialized_fill_n writes value into each, finish = start + n
// Leaf: ??0?$vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00451583, length 49 bytes, `ret 0Ch` (thiscall: n, const bool&, const allocator&).
// Callees: std::_Vector_base<bool>::_Vector_base(unsigned int, const allocator<bool>&) @ 0x00450CCC (out of line, this chunk).
// Label: the catalog "VertexListInsertVertices" is behavioural. Callers include CPlayerInventoryDef::CPlayerInventoryDef
// (0x00459AF3, `IsUsable(6, false)`) and eleven more sites; the element is 1 byte (retail STLport's vector<bool> is a
// plain byte vector, as the landed CPlayerInventoryDef::operator= 0x00462E2D models it).
// Shape: frame pointer kept with `push dword ptr [ebp+N]` pass-through arguments and no spills = size TU with /Oy off:
// #pragma optimize("y", off) + ("s", on). The fill is STLport fill_n (`for (; n > 0; --n, ++first) *first = value;`),
// the value re-read through its reference every pass (a bool store may alias it).
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {
template <class T> struct allocator {
    allocator() { }
};

template <class P, class T, class A> struct _STLP_alloc_proxy : A {
    P _M_data;
};

template <class T, class A> struct _Vector_base {
    T*                              _M_start;            // +0
    T*                              _M_finish;           // +4
    _STLP_alloc_proxy<T*, T, A>     _M_end_of_storage;   // +8
    _Vector_base(unsigned int n, const A& a);            // 0x00450CCC
};

template <class OutputIter, class Size, class T>
__forceinline OutputIter fill_n(OutputIter first, Size n, const T& value)
{
    for (; n > 0; --n, ++first)
        *first = value;
    return first;
}

template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    vector(unsigned int n, const T& value, const A& a);
};

template <class T, class A> vector<T, A>::vector(unsigned int n, const T& value, const A& a)
    : _Vector_base<T, A>(n, a)
{
    this->_M_finish = fill_n(this->_M_start, n, value);
}

template class vector<bool, allocator<bool> >;
}