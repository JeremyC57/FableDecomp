// std::__uninitialized_copy(QuadricOptimiserInternals::COptimisedBone* first, QuadricOptimiserInternals::COptimisedBone* last, QuadricOptimiserInternals::COptimisedBone* result, const __false_type&) -- copy-construct [first, last) into raw storage at result
// Leaf: ??$__uninitialized_copy@PAVCOptimisedBone@QuadricOptimiserInternals@@PAV12@@std@@YIPAVCOptimisedBone@QuadricOptimiserInternals@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD16C0, 64 bytes, `__fastcall` (first ecx, last edx), `ret 8`; returns the end of the run.
// Identity (proven): its callers are the mutable-source moves of vector<QuadricOptimiserInternals::COptimisedBone>::_M_insert_overflow 0x00AD5F80 (16-byte elements; the const-source instantiation for the same element is the separate COMDAT 0x00AD1B20); Ego_r.pdb COptimisedBone {CCharString BoneName, long Parent, bool Remapped, long NewIndex}.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the element's implicit copy constructor inline).
// Shape (family M083, representative 0x004B8190): STLport `for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur,
//   *__first); return __cur;` -- the placement new's null test, the string copy, then the scalar members; speed TU.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CCharString {                                         // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    void* PStringData;
};
namespace QuadricOptimiserInternals {
class COptimisedBone {                                    // Ego_r.pdb, 16 bytes
public:
    CCharString BoneName;                                 // +0x0
    long Parent;                                          // +0x4
    bool Remapped;                                        // +0x8
    long NewIndex;                                        // +0xc
};
}
namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
typedef ::QuadricOptimiserInternals::COptimisedBone _E;
template _E* __fastcall __uninitialized_copy<_E*, _E*>(_E*, _E*, _E*, const __false_type&);
}