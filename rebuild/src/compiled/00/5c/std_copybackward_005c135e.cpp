// std::__copy_backward(NInventory::CItem* first, NInventory::CItem* last, NInventory::CItem* result, const random_access_iterator_tag&, int*) -- STLport random-access copy_backward over inventory items: assign each item from the back through CItem::operator=, return the new start of the destination
// Leaf: ??$__copy_backward@
// Retail: Fable.exe @ 0x005C135E, 50 bytes, `ret 0Ch`: __fastcall (/Gr algorithm TU) -- first in ecx, last in edx (kept in
// esi), result, the iterator-tag reference and the distance-type pointer on the stack. Callers: vector<CItem> insert/erase
// helpers @ 0x005C1495 and 0x005C28A7. The catalog label `_Copy_backward<CArray<unsigned_short>*,...>` is propagated: the
// element is NInventory::CItem (0x14 bytes, `push 14h ; pop ecx ; idiv`) and the callee is its assignment.
// Callees: NInventory::CItem::operator=(const CItem&) @ 0x005C0C24 (ItemType, NumberOfItems, the CCountedPointer<CPerItemData>
// operator= @ 0x005C077F, AverageBoughtPrice; out of line in this size TU; manifest label "CEnginePrimitive2DText::operator="
// is a byte twin).
// Body: STLport _algobase.h
//     for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;
// Shape: size TU (`push 14h ; pop ecx` divisor, `sub dword ptr [esp+0Ch],14h`): `result` is decremented IN its argument slot
// and re-read for ecx before `last` (esi) steps back -- the left operand of the assignment is evaluated first.
#pragma optimize("s", on)
#include "engine/CItem.h"
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
}
struct CItem_Assignable : CItem {
    CItem_Assignable& operator=(const CItem_Assignable& other);       // 0x005C0C24
};
namespace std {
template <class RandomAccessIter, class BidirectionalIter, class Distance>
BidirectionalIter __fastcall __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                             const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
template CItem_Assignable* __fastcall __copy_backward(CItem_Assignable*, CItem_Assignable*, CItem_Assignable*, const random_access_iterator_tag&, int*);
}