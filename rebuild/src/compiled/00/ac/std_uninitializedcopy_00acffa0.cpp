// std::__uninitialized_copy(first, last, result, const __false_type&) -- (QuadricOptimiserInternals::COptimisedEdge* -> QuadricOptimiserInternals::COptimisedEdge*) STLport placement copy-construct of a run of 16-byte QuadricOptimiserInternals::COptimisedEdge elements into raw storage, returns the end of the constructed run
// Leaf: ??$__uninitialized_copy@PAVCOptimisedEdge@QuadricOptimiserInternals@@PAV12@@std@@YIPAVCOptimisedEdge@QuadricOptimiserInternals@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ACFFA0, 62 bytes (+ int3 padding to 64), `ret 8`: /Gr STLport TU (first ecx, last edx;
//   result and the const __false_type& tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): std::__uninitialized_copy<QuadricOptimiserInternals::COptimisedEdge*, QuadricOptimiserInternals::COptimisedEdge*>; evidence: its only caller is 0x00AD4D10 (twice: prefix and remainder), the _M_insert_overflow of push_back 0x00AD5CE0, which COptimisedPrimitive::JoinAdjacentFaces 0x00AC8AE0 calls at 0x00AC8C48 on this+0x28 -- Ego_r.pdb's COptimisedPrimitive::Edges (+0x34 behind three 16-byte debug CArrays; retail 4 + 3 * 12 = 0x28), CArray<QuadricOptimiserInternals::COptimisedEdge>, 16 bytes {FaceA, FaceB, UniqueVertex1, UniqueVertex2}; pointer iterators, hence <T*, T*>
// Label: the manifest's "_Copy_impl<C2DLineF*,C2DLineF*>" is propagated (family H076: six 16-byte-element __uninitialized_copy COMDATs; retail has no
//   identical-code folding, so each address is one instantiation). `ret 8` + the placement-new null test = __uninitialized_copy.
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);`; the element's implicit copy constructor
//   copies four dwords through ebx (esi = source, edi = destination); /O2 aligns the loop head (`jmp` over `lea ecx,[ecx]`).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace QuadricOptimiserInternals {
class COptimisedEdge {                                  // 16 bytes (Ego_r.pdb class)
public:
    unsigned long FaceA;
    unsigned long FaceB;
    unsigned long UniqueVertex1;
    unsigned long UniqueVertex2;
};
}

namespace std {

struct __false_type { __false_type() { } };      // STLport empty tag; passed by const reference (one stack slot)

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template QuadricOptimiserInternals::COptimisedEdge* __fastcall __uninitialized_copy<QuadricOptimiserInternals::COptimisedEdge*, QuadricOptimiserInternals::COptimisedEdge*>(QuadricOptimiserInternals::COptimisedEdge*, QuadricOptimiserInternals::COptimisedEdge*, QuadricOptimiserInternals::COptimisedEdge*, const __false_type&);

}   // namespace std