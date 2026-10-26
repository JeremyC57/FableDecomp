// _S_merge(SMarkerList& list, SMarkerList& other, CThingCompare_Nearest comp) -- STLport list merge: splice the nodes of the sorted list `other` into the sorted `list` in comparator order (nearest first), then append what remains
// Leaf: _S_merge
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F04C0, length 139 bytes (`ret 0Ch`: the 12-byte comparator by
// value; the two lists in ecx/edx -- the STL algorithm TUs are /Gr, free templates are __fastcall). Callers:
// 0x006F3B01 and 0x006F3B77 (the list sort's merge steps).
// Callee: CThingCompare_Nearest::operator()(CThingMarker*, CThingMarker*) @ 0x006EF290 (`__fastcall` on the
// comparator's own argument slot, non-zero when the first marker is nearer).
// Label: the manifest's "CBinaryTree_Alt::Rebalance" is a placeholder; the shape is STLport's
//   _S_merge(__that, __x, __comp) { first1 = begin, last1 = end, first2 = x.begin, last2 = x.end;
//     while (first1 != last1 && first2 != last2)
//       if (comp(*first2, *first1)) { next = first2; _M_transfer(first1, first2, ++next); first2 = next; }
//       else ++first1;
//     if (first2 != last2) _M_transfer(last1, first2, last2); }
// with list::_M_transfer inlined (the six-store splice under `position != last`). Nodes: next +0, prev +4, value +8.
// Shape: both header nodes are read once (ebx, ebp); the comparator is a 12-byte class (a world position) whose
// slot address is `this` of the out-of-line call.
class CThingMarker;
struct SListNode {
    SListNode*    Next;                       // +0x00
    SListNode*    Prev;                       // +0x04
    CThingMarker* Value;                      // +0x08
};
struct SMarkerList {                          // STLport list<CThingMarker*>: the header node pointer
    SListNode* Node;
};
class CThingCompare_Nearest { public:                // 12 bytes: the reference position
    float X, Y, Z;
    long operator()(CThingMarker* a, CThingMarker* b);   // 0x006EF290
};

inline void _M_transfer(SListNode* position, SListNode* first, SListNode* last)
{
    if (position != last) {
        last->Prev->Next = position;
        first->Prev->Next = last;
        position->Prev->Next = first;
        SListNode* tmp = position->Prev;
        position->Prev = last->Prev;
        last->Prev = first->Prev;
        first->Prev = tmp;
    }
}

void __fastcall _S_merge(SMarkerList& list, SMarkerList& other, CThingCompare_Nearest comp)
{
    SListNode* last1 = list.Node;
    SListNode* last2 = other.Node;
    SListNode* first2 = last2->Next;
    SListNode* first1 = last1->Next;
    while (first1 != last1 && first2 != last2) {
        if (comp(first2->Value, first1->Value)) {
            SListNode* next = first2->Next;
            _M_transfer(first1, first2, next);
            first2 = next;
        } else {
            first1 = first1->Next;
        }
    }
    if (first2 != last2)
        _M_transfer(last1, first2, last2);
}