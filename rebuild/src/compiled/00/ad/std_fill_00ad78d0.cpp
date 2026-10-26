// std::fill(QuadricOptimiserInternals::COptimisedVertex* first, QuadricOptimiserInternals::COptimisedVertex* last, const QuadricOptimiserInternals::COptimisedVertex& value) -- (QuadricOptimiserInternals::COptimisedVertex) STLport fill: assign value to every element of [first, last) through the element's assignment (COptimisedVertex::operator= 0x00AD7050)
// Leaf: ??$fill@PAVCOptimisedVertex@QuadricOptimiserInternals@@V12@@std@@YIXPAVCOptimisedVertex@QuadricOptimiserInternals@@0ABV12@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD78D0, 40 bytes (+ int3 padding), `__fastcall` (first in ecx, last in edx, the value
//   reference on the stack), `ret 4`.
// Identity (proven): std::fill<QuadricOptimiserInternals::COptimisedVertex*, QuadricOptimiserInternals::COptimisedVertex >; evidence: the only caller is the landed vector<COptimisedVertex>::_M_fill_insert 0x00AD85D0 (its resize 0x00AD87A0); stride 0xB8 = Ego_r QuadricOptimiserInternals::COptimisedVertex (192) with its three 16-byte trees at 12 bytes (FaceList, Groups, PairContractions: 180, 8-aligned by CQuadric's doubles -> 184); the assignment 0x00AD7050 copies three trees (landed note); Ego_r has vector<QuadricOptimiserInternals::COptimisedVertex> (stride 0xB8 asserted)
// Label: the manifest's "fill<CArray<unsigned_short>*,CArray<unsigned_short>_>" is propagated from 0x00F3B850 (five byte-similar fill bodies; retail has no identical-code folding,
//   so each address is its own element type).
// Callee: QuadricOptimiserInternals::COptimisedVertex::operator= (??4COptimisedVertex@QuadricOptimiserInternals@@QAEAAV01@ABV01@@Z, the PDB spelling) 0x00AD7050.
// Body: STLport 4.6 `for ( ; __first != __last; ++__first) *__first = __val;` (explicit instantiation of the inline template).
class C3DVector { public: float X, Y, Z; };
class CQuadric { public: double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD; double _pad; };   // Ego_r 88 bytes
namespace QuadricOptimiserInternals {
class COptimisedVertex {                                // retail 0xB8 (Ego_r 192 with 16-byte set/maps; STLport's are 12)
public:
    C3DVector     Position;                             // +0x00
    C3DVector     Normal;                               // +0x0C
    C3DVector     TextureCoord;                         // +0x18
    CQuadric      PositionError;                        // +0x28 (8-aligned)
    float         Cost;                                 // +0x80
    unsigned char FaceList[0xC];                        // +0x84 CIndexList (12-byte STLport tree)
    bool          Exists;                               // +0x90
    unsigned long UniqueIndex;                          // +0x94
    unsigned long OutputIndex;                          // +0x98
    unsigned char Groups[0xC];                          // +0x9C map<unsigned long, float>
    unsigned char PairContractions[0xC];                // +0xA8 map<unsigned long, CContractionReference>
    COptimisedVertex& operator=(const COptimisedVertex& other);   // 0x00AD7050
};
}
namespace std {
template <class _ForwardIter, class _Tp>
inline void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}   // namespace std
template void __fastcall std::fill<QuadricOptimiserInternals::COptimisedVertex*, QuadricOptimiserInternals::COptimisedVertex >(QuadricOptimiserInternals::COptimisedVertex*, QuadricOptimiserInternals::COptimisedVertex*, const QuadricOptimiserInternals::COptimisedVertex&);