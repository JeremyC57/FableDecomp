// std::__uninitialized_copy(const C2DBoxF* first, const C2DBoxF* last, C2DBoxF* result, const __false_type&) -- copy-construct [first, last) into raw slots at result, return the end
// Leaf: ??$__uninitialized_copy@PBVC2DBoxF@@PAV1@@std@@YIPAVC2DBoxF@@PBV1@0PAV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7EA70, 56 bytes, `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity (proven): callers 0x00A7FA50 (vector<C2DBoxF>::_M_allocate_and_copy<const C2DBoxF*>) and 0x00A81790 (vector<C2DBoxF>::operator=, called by CNavQuadTree::Initialise 0x00A7A911 on DetailedAreas, retail +0x5C); no binding yet.
// Callees: none (placement copy inlined behind its null test).
// Shape (family M010, representative 0x0040F5E0): STLport 4.6 `__uninitialized_copy(__false_type)` -- `cur = result`, the
//   first == last early test, `_Construct(&*cur, *first)` (null test, four dword moves through esi: the memberwise copy),
//   `++first, ++cur`; returns cur in eax.
inline void* operator new(unsigned int, void* p) { return p; }
class C2DBoxF {                                          // Ego_r.pdb, 16 bytes
public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DBoxF& other) : TLX(other.TLX), TLY(other.TLY), BRX(other.BRX), BRY(other.BRY) { }
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template C2DBoxF* __fastcall __uninitialized_copy<const C2DBoxF*, C2DBoxF*>(const C2DBoxF*, const C2DBoxF*, C2DBoxF*, const __false_type&);
}