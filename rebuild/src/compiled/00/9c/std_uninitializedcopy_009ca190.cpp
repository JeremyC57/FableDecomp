// std::__uninitialized_copy(NGameText::CTextTag* first, NGameText::CTextTag* last, NGameText::CTextTag* result, const __false_type&) -- STLport placement copy-construct each record (the scalar, then the member's out-of-line copy constructor) and return the advanced result
// Leaf: ??$__uninitialized_copy@PAVCTextTag@NGameText@@PAV12@@std@@YIPAVCTextTag@NGameText@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009ca190, 54 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (probable): callers the _M_insert_overflow 0x009CAD20 and the fill insert 0x009CAF60 that the landed std::vector<NGameText::CTextTag>::resize(n) 0x009CB7D0 (Ego_r.pdb ?resize@?$vector@VCTextTag@NGameText@@...; CTextEntry::Tags) calls; Ego_r.pdb NGameText::CTextTag {long Pos; CCharString Name}; bound only as a FableVector stand-in pair<int, CCharString>.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (bound).
// Shape (family L011): STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` verbatim; the record's
//   implicit copy constructor under placement new's null test; stride 8.
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {             // STLport pair: implicit copy constructor
    _T1 first;
    _T2 second;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}
class CCharString {
public:
    void* PStringData;                                    // +0x00
    CCharString(const CCharString& other);                // 0x0099EC30
};
namespace NGameText {
class CTextTag {                                           // Ego_r.pdb, 8 bytes
public:
    long Pos;                                             // +0x00
    CCharString Name;                                     // +0x04
};
}
template NGameText::CTextTag* __fastcall std::__uninitialized_copy<NGameText::CTextTag*, NGameText::CTextTag*>(NGameText::CTextTag*, NGameText::CTextTag*, NGameText::CTextTag*, const std::__false_type&);