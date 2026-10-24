// std::uninitialized_fill_n(CQuestCardItem* first, unsigned int n, const CQuestCardItem& x) -- STLport placement fill of quest-card items (item index plus a wide-string copy); returns the end of the filled range
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006236C0, 50 bytes (+14 int3), `ret 4` (/Gr TU: first in ecx -> esi,
//   n in edx -> edi, x on the stack -> ebx).
// Caller: vector<CQuestCardItem>::_M_fill_insert 0x00624E40 (call at 0x00624EE0; reached from the vector's resize
//   0x006252D0), the CTCInventoryQuests::QuestCardItemList vector (Ego_r.pdb: CQuestCardItem is ItemIndex (long) +0,
//   ItemText (CWideString) +4 -- the only 8-byte PDB record with a CWideString at +4 outside the pair templates).
// Callee: CWideString::CWideString(const CWideString&) 0x0099B720 (ItemText).
// Label correction: the catalog `_Fill_n<unsigned long*,unsigned int,unsigned long>` is a propagated Dinkumware name; the
//   body is STLport `uninitialized_fill_n(first, n, x)` with its `__uninitialized_fill_n(first, n, x, __false_type)`
//   worker inlined (`for (; n > 0; --n, ++cur) _Construct(&*cur, x);`, the unsigned count tested with `jbe`).
// Shape: speed TU (the loop head is aligned with a `nop`); the implicit CQuestCardItem copy constructor stores the
//   index, then calls the string copy on &cur->ItemText; the placement new null-tests the destination.
#include <new>

struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);          // 0x0099B720
};

struct CQuestCardItem {                             // 8 bytes (Ego_r.pdb)
    long        ItemIndex;                          // +0x00
    CWideString ItemText;                           // +0x04
};

namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class ForwardIter, class Size, class T>
inline ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template CQuestCardItem* __fastcall uninitialized_fill_n<CQuestCardItem*, unsigned int, CQuestCardItem>(
    CQuestCardItem*, unsigned int, const CQuestCardItem&);
}