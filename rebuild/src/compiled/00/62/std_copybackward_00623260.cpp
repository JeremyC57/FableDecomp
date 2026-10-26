// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CQuestCardItem*, CQuestCardItem*, int) STLport random-access backward copy: `for (n = last - first; n > 0; --n) *--result = *--last;`, returns the moved-back result
// Leaf: ??$__copy_backward@PAVCQuestCardItem@@PAV1@H@std@@YIPAVCQuestCardItem@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00623260, 64 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element CQuestCardItem (8 bytes); evidence: its callers 0x006236A0 and 0x00624E40 are the grow and fill-insert of the vector that CTCInventoryQuests::OnCreate 0x0061A6A0 resizes to 6 (0x006253D0) at this+0x140 -- Ego_r.pdb's CTCInventoryQuests::QuestCardItemList, vector<CQuestCardItem> (+0x15C behind 16-byte debug vectors), and CQuestCardItem is {long ItemIndex; CWideString ItemText}
// Label: the manifest's "_Copy_backward<std::pair<float,CCharString>*,std::pair<float,class_CCharString>*>" is propagated (family H070: six backward copies over a 4-byte field followed by a member with an
//   out-of-line operator=; retail has no identical-code folding, so each address is one element type). Two shapes: 8-byte elements
//   (`sar eax,3`) and 16-byte ones (`sar eax,4`).
// Callee: CWideString::operator= 0x0099B7D0 on ItemText.
// Shape: the element's implicit operator= inline -- the first dword through eax, then the member's operator= on +4.
class CWideString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    CWideString& operator=(const CWideString& other);  // 0x0099B7D0
};
class CQuestCardItem {                               // 8 bytes (Ego_r.pdb)
public:
    long        ItemIndex;                            // +0x00
    CWideString ItemText;                             // +0x04
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template CQuestCardItem* __fastcall __copy_backward<CQuestCardItem*, CQuestCardItem*, int>(CQuestCardItem*, CQuestCardItem*, CQuestCardItem*,
                                                                         const random_access_iterator_tag&, int*);
}   // namespace std