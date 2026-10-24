// _STL::fill(QuadricOptimiserInternals::COptimisedFace* first, COptimisedFace* last, const COptimisedFace& val) -- assign val to every 72-byte optimiser face of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD04A0, length 40 bytes (no padding), __fastcall (first in ecx -> eax,
//   last in edx, the value reference on the stack, `ret 4`).
// Identity: both callers (0x00AD5597, 0x00AD55F2) are in vector<T>::_M_fill_insert 0x00AD54F0 of the QuadricOptimiser
//   TU (72-byte element: the 0x38E38E39 /72 divide, `lea [n+n*8] ; shl 3`), filling the opened gap from the
//   `x_copy` frame copy; the 72-byte element of that module is COptimisedFace (Ego_r.pdb, 0x48 bytes). The manifest's
//   "_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>" is propagated and wrong (CHeroMarriageInfo is 24 bytes).
// Shape: STLport `for (; first != last; ++first) *first = val;` with the implicit assignment of the POD face copied as
//   an 18-dword `rep movsd`; the pointer is advanced and compared before the copy (scheduled).
class C3DVector {
public:
    float X, Y, Z;
};

namespace QuadricOptimiserInternals {
class COptimisedFace {                             // 72 bytes (Ego_r.pdb)
public:
    unsigned long VertexIndices[3];                // +0x00
    unsigned long UniqueVertexIndices[3];          // +0x0c
    long          Edges[3];                        // +0x18
    C3DVector     UTangent;                        // +0x24
    C3DVector     VTangent;                        // +0x30
    bool          Exists;                          // +0x3c
    unsigned long OutputIndex;                     // +0x40
    long          Segment;                         // +0x44
};
}
typedef QuadricOptimiserInternals::COptimisedFace COptimisedFace;

namespace _STL {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill(COptimisedFace*, COptimisedFace*, const COptimisedFace&);
}