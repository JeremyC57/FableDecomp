// std::__uninitialized_copy(CUnprovenRecord12_00ae4ea0* first, CUnprovenRecord12_00ae4ea0* last, CUnprovenRecord12_00ae4ea0* result, const __false_type&) -- copy-construct [first, last) into raw storage at result, return the end
// Leaf: ??$__uninitialized_copy@PAVCUnprovenRecord12_00ae4ea0@@PAV1@@std@@YIPAVCUnprovenRecord12_00ae4ea0@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ae4ea0, 56 bytes, /Gr `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity (descriptive): its only callers are the grow insert 0x00AE6C70 (head and tail copies) of a 12-byte-element vector in the particle-engine TU, whose push_back 0x00AE7290 has no direct caller; the element type is unproven.
// Body: STLport 4.6 `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- the element's implicit copy
//   constructor is a three-dword block copy (ebx through esi/edi), the placement new null-tests the destination.
// Shape (family L079).
class CUnprovenRecord12_00ae4ea0 {                       // descriptive: a 12-byte record of three dwords, implicit copy
public:
    unsigned long Word0, Word1, Word2;                    // +0x00
};
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template CUnprovenRecord12_00ae4ea0* __fastcall __uninitialized_copy(CUnprovenRecord12_00ae4ea0*, CUnprovenRecord12_00ae4ea0*, CUnprovenRecord12_00ae4ea0*, const __false_type&);
}   // namespace std