// std::__uninitialized_copy(CBrainBehaviour* first, CBrainBehaviour* last, CBrainBehaviour* result, const __false_type&) -- copy-construct [first, last) into raw storage at result, return the end
// Leaf: ??$__uninitialized_copy@PAVCBrainBehaviour@@PAV1@@std@@YIPAVCBrainBehaviour@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00885ca0, 56 bytes, /Gr `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity (probable): called by the grow insert 0x00889010 and the fill insert 0x008890F0 of the vector whose resize is 0x00889440 (family L078, vector<CBrainBehaviour>, probable through the landed 0x00887330's 12-byte CBrainBehaviour value); bound only as the stand-in ??$__uninitialized_copy@UElement@N008890f0@@....
// Body: STLport 4.6 `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- the element's implicit copy
//   constructor is a three-dword block copy (ebx through esi/edi), the placement new null-tests the destination.
// Shape (family L079).
class CDefString {                                         // Ego_r.pdb, 4 bytes
public:
    long TablePos;                                        // +0x00
};
class CBrainBehaviour {                                    // Ego_r.pdb, 12 bytes (TLC stores Priority as a float)
public:
    CDefString Name;                                      // +0x00
    float      Priority;                                  // +0x04
    long       ScriptAIPriority;                          // +0x08
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
template CBrainBehaviour* __fastcall __uninitialized_copy(CBrainBehaviour*, CBrainBehaviour*, CBrainBehaviour*, const __false_type&);
}   // namespace std