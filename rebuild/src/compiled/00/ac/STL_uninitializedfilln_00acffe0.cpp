// _STL::__uninitialized_fill_n(QuadricOptimiserInternals::COptimisedEdge* first, unsigned int n, const COptimisedEdge& x, const __false_type&) -- copy-construct n copies of a 16-byte optimiser edge into raw storage and return the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ACFFE0, length 58 bytes, __fastcall (first in ecx -> eax, n in edx, the
//   value and the empty tag reference on the stack, `ret 8`), returns the end in eax.
// Identity: the only caller (0x00AD4D98) is vector<COptimisedEdge>::_M_insert_overflow 0x00AD4D10, whose push_back
//   0x00AD5CE0 is called by COptimisedPrimitive::JoinAdjacentFaces (0x00AC8C48) on the primitive's Edges (+0x28 retail)
//   with a {FaceA, FaceB, UniqueVertex1, UniqueVertex2} record (Ego_r.pdb COptimisedEdge, 16 bytes). The manifest's
//   "fill<C3DClothQuad*,C3DClothQuad>" is propagated and wrong.
// Shape: STLport 4.x `for (; n > 0; --n, ++cur) _Construct(&*cur, x)`: unsigned count (`jbe`), placement-new null test;
//   the implicit copy of a POD of four dwords goes through three registers (source esi, destination edi, data ebx).
inline void* operator new(unsigned int, void* p) { return p; }

namespace QuadricOptimiserInternals {
class COptimisedEdge {                             // 16 bytes (Ego_r.pdb)
public:
    unsigned long FaceA;                           // +0x0
    unsigned long FaceB;                           // +0x4
    unsigned long UniqueVertex1;                   // +0x8
    unsigned long UniqueVertex2;                   // +0xc
};
}
typedef QuadricOptimiserInternals::COptimisedEdge COptimisedEdge;

namespace _STL {
struct __false_type { __false_type() {} };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template COptimisedEdge* __fastcall __uninitialized_fill_n(COptimisedEdge*, unsigned int, const COptimisedEdge&,
                                                           const __false_type&);
}