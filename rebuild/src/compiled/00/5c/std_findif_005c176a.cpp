// std::__find_if(NInventory::CItem* first, NInventory::CItem* last, CIsSameItemType pred, const random_access_iterator_tag&) -- STLport random-access find_if over a category's items: the first item the CIsSameItemType predicate accepts, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe @ 0x005C176A, 174 bytes, `ret 0Ch`: __fastcall (/Gr template TU) -- first in ecx, last in edx (spilled to
// the frame word [ebp-4]), the 8-byte CIsSameItemType {ItemType, OnlyFindNonFullSlots} BY VALUE and the unused tag on the
// stack. Only caller: the NON-const find_if forwarder @ 0x005C190B (landed), used by NInventory::CCategory::RemoveItem
// 0x005BF237 with CIsSameItemType(type, false). Byte twin of the const instantiation 0x005C1818.
// Callees: CIsSameItemType::operator()(const NInventory::CItem&) const @ 0x005C150B (out of line: `push esi ; lea ecx,[ebp+8] ;
// call` -- the predicate is addressed in its own by-value argument slot).
// Body: STLport 4.x _algo.c __find_if (four predicate calls per trip, the 3/2/1 remainder switch); size TU (`push 14h ; pop
// ebx` stride, `idiv`, ebp frame for the slot-relative `lea`).
#pragma optimize("s", on)
#include "engine/CItem.h"
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
// STLport 4.x _algo.c: the random-access __find_if, four predicate calls per trip, then the 3/2/1 remainder switch.
template <class RandomAccessIter, class Predicate>
RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred, const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
}
class CIsSameItemType {
public:
    long ItemType;                                        // +0x00
    bool OnlyFindNonFullSlots;                            // +0x04
    bool operator()(const CItem& item) const;             // 0x005C150B
};
template CItem* __fastcall std::__find_if(CItem*, CItem*, CIsSameItemType, const std::random_access_iterator_tag&);