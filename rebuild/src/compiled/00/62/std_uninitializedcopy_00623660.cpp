// std::__uninitialized_copy(CQuestCardItem* first, CQuestCardItem* last, CQuestCardItem* result, const __false_type&) -- STLport placement copy-construct each record (the scalar, then the member's out-of-line copy constructor) and return the advanced result
// Leaf: ??$__uninitialized_copy@PAVCQuestCardItem@@PAV1@@std@@YIPAVCQuestCardItem@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00623660, 54 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): callers the landed std::vector<CQuestCardItem>::_M_insert_overflow 0x00624B90 (head and tail copies, non-const pointers) and that vector's fill insert 0x00624E40; Ego_r.pdb CQuestCardItem {long ItemIndex; CWideString ItemText}; bound only as a FableVector stand-in pair<int, CWideString>.
// Callees: CWideString::CWideString(const CWideString&) 0x0099B720 (bound).
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
class CWideString {
public:
    void* PStringData;                                    // +0x00
    CWideString(const CWideString& other);                // 0x0099B720
};
class CQuestCardItem {                                     // Ego_r.pdb, 8 bytes
public:
    long ItemIndex;                                       // +0x00
    CWideString ItemText;                                 // +0x04
};
template CQuestCardItem* __fastcall std::__uninitialized_copy<CQuestCardItem*, CQuestCardItem*>(CQuestCardItem*, CQuestCardItem*, CQuestCardItem*, const std::__false_type&);