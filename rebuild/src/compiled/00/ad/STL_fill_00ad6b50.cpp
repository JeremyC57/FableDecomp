// _STL::fill(QuadricOptimiserInternals::CUniqueVertex* first, QuadricOptimiserInternals::CUniqueVertex* last, const QuadricOptimiserInternals::CUniqueVertex& value) -- assign one mesh-optimiser unique-vertex record (exists flag, vertex and face index sets) to every element of a range
// Leaf: ??$fill@PAVCUniqueVertex@QuadricOptimiserInternals@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD6B50, 73 bytes, `__fastcall` (first ecx, last edx, value pointer
//   on the stack), `ret 4`; `last` is spilled to the frame (`push ecx` slot) for the loop test.
// Identity: 28-byte stride = {bool +0, 12-byte member +4, 12-byte member +0x10}, both members assigned through
//   0x00AD3720, an STLport _Rb_tree operator= (clear through the set<unsigned long> _M_erase 0x004CF810, then
//   _M_copy 0x00AD3230): Ego_r.pdb QuadricOptimiserInternals::CUniqueVertex {bool Exists; CIndexList VertexIndices;
//   CIndexList FaceList}, CIndexList = {set<unsigned long> Indices}. Its fill-insert caller 0x00AD7E80 is also called
//   from the COptimisedPrimitive constructor 0x00ACDB80 (UniqueVertices). The manifest's "pair<CCharString,
//   CCharString>" is propagated. One caller, 0x00AD7F86.
// Callees: _Rb_tree<unsigned long, ...>::operator= @ 0x00AD3720 (twice).
// Shape: STLport fill with CUniqueVertex's implicit operator= inline (the bool, then the two CIndexList implicit
//   operator= -> set::operator= inline -> the tree's operator= out of line); both member addresses of val hoisted.

namespace _STL {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _Key, class _Value, class _Compare> class _Rb_tree {   // 12 bytes
public:
    void*        _M_header;                              // +0x00
    unsigned int _M_node_count;                          // +0x04
    _Compare     _M_key_compare;                         // +0x08
    _Rb_tree& operator=(const _Rb_tree& __x);            // 0x00AD3720 for unsigned long
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    _Rb_tree<_Key, _Key, _Compare> _M_t;
    set& operator=(const set& __x) { _M_t = __x._M_t; return *this; }
};
}

namespace QuadricOptimiserInternals {
class CIndexList {                                       // 12 bytes
public:
    _STL::set<unsigned long> Indices;                    // +0x00
};
class CUniqueVertex {                                    // 28 bytes
public:
    bool       Exists;                                   // +0x00
    CIndexList VertexIndices;                            // +0x04
    CIndexList FaceList;                                 // +0x10
};
}
typedef QuadricOptimiserInternals::CUniqueVertex CUniqueVertex;

namespace _STL {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall _STL::fill<CUniqueVertex*, CUniqueVertex>(CUniqueVertex*, CUniqueVertex*, const CUniqueVertex&);