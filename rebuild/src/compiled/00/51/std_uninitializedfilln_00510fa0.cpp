// std::__uninitialized_fill_n(CNavigationTransitionPoint* first, unsigned int n, const CNavigationTransitionPoint& x, const __false_type&) -- copy-construct x into n raw slots, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCNavigationTransitionPoint@@IV1@@std@@YIPAVCNavigationTransitionPoint@@PAV1@IABV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00510FA0, 52 bytes, `__fastcall` (first ecx, n edx, x and the tag on the stack), `ret 8`.
// Identity (proven): the catalog binding of 0x00510FA0 is this symbol (recorded by its caller 0x00517AC0 (vector<CNavigationTransitionPoint>::_M_insert_overflow, landed)); the element's 16-byte
//   memberwise copy matches the PDB copy constructor. The manifest label is propagated/behavioural.
// Callees: none (placement copy inlined behind its null test).
// Shape (family M007, representative 0x0040DE00): STLport 4.6 `__uninitialized_fill_n(__false_type)` -- `cur` walks from
//   first, `_Construct(&*cur, x)` (placement new's null test, then four dword moves through esi), `--n, ++cur`; VC7.1 turns the
//   `n > 0` count into the `test edx,edx ; jbe` guard plus a `dec edx ; jne` loop with an aligned head.
inline void* operator new(unsigned int, void* p) { return p; }
class C2DVector {                                         // Ego_r.pdb, 8 bytes
public:
    float X;
    float Y;
};
class CNavigationTransitionPoint {                        // Ego_r.pdb, 16 bytes
public:
    C2DVector Position;                                   // +0x0
    long      Layer1;                                     // +0x8
    long      Layer2;                                     // +0xC
    CNavigationTransitionPoint(const CNavigationTransitionPoint& other)   // PDB ??0CNavigationTransitionPoint@@QAE@ABV0@@Z
        : Position(other.Position), Layer1(other.Layer1), Layer2(other.Layer2) { }
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CNavigationTransitionPoint* __fastcall __uninitialized_fill_n<CNavigationTransitionPoint*, unsigned int, CNavigationTransitionPoint>(CNavigationTransitionPoint*, unsigned int, const CNavigationTransitionPoint&, const __false_type&);
}