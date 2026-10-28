// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (CQuestCardItem*, CQuestCardItem*, int) STLport random-access forward copy: `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`, returns the advanced result
// Leaf: ??$__copy@PAVCQuestCardItem@@PAV1@H@std@@YIPAVCQuestCardItem@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00623150, 61 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element CQuestCardItem (8 bytes); evidence: its callers are 0x006235B0 and 0x006249E0, the erase(first, last) that the vector<CQuestCardItem>::resize 0x006253D0 (CTCInventoryQuests::OnCreate 0x0061A6A0 resizes this+0x140 to 6) calls to shrink -- Ego_r.pdb's CTCInventoryQuests::QuestCardItemList, vector<CQuestCardItem>, CQuestCardItem being {long ItemIndex; CWideString ItemText}; H070's 0x00623260 is the same vector's backward copy
// Label: the manifest's "_Copy_impl<CTCSkeletalMorph::CSkeletalMorph*,CTCSkeletalMorph::CSkeletalMorph*>" is propagated (family H081: six forward copies over a 4-byte field followed by a member with an
//   out-of-line operator=; retail has no identical-code folding, so each address is one element type). Two shapes: 8-byte elements
//   (`sar edx,3`) and 16-byte ones (`sar edx,4`).
// Callee: CWideString::operator= 0x0099B7D0 on ItemText.
// Shape: the element's implicit operator= inline -- the first dword through eax, then the member's operator= on +4.
class CWideString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    const CWideString& operator=(const CWideString& other);  // 0x0099B7D0
};
class CQuestCardItem {                               // 8 bytes (Ego_r.pdb)
public:
    long        ItemIndex;                            // +0x00
    CWideString ItemText;                             // +0x04
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template CQuestCardItem* __fastcall __copy<CQuestCardItem*, CQuestCardItem*, int>(CQuestCardItem*, CQuestCardItem*, CQuestCardItem*,
                                                                const random_access_iterator_tag&, int*);
}   // namespace std