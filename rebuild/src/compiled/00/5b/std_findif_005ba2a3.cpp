// std::__find_if(SKeyedRecord24* first, SKeyedRecord24* last, CIsRecordKeyEqualTo pred, const random_access_iterator_tag&) -- STLport random-access find_if over 24-byte records: the first record whose leading key equals the predicate's key, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe @ 0x005BA2A3, 105 bytes, `ret 8`: __fastcall (/Gr template TU) -- first in ecx, last in edx (kept in ebx),
// the 4-byte predicate BY VALUE and the unused category tag on the stack. Only caller: the find_if forwarder @ 0x005BA778
// (catalog label "_Umove<EWeaponClass*>" is propagated), called from CTCInventoryClothing (vtable slot 11 @ 0x005B9641) right
// after the 12-byte twin 0x005BA23A with the same tree-node key.
// Body: STLport 4.x _algo.c __find_if: trip_count = (last - first) >> 2 (`idiv` by 24 under optimize("s"), stride register
// edi = 24), four inlined predicate calls per trip, then the 3/2/1 remainder switch falling through to `return last`.
// Shape: byte-identical to 0x005BA23A except the stride immediate; size TU (`push 18h ; pop edi`); the one-member predicate
// keeps its key in esi (`cmp [ecx],esi`). Element/predicate names are descriptive.
#pragma optimize("s", on)
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
struct SKeyedRecord24 {
    long Key;                   // +0x00
    long Value[5];              // +0x04
};
struct CIsRecordKeyEqualTo {
    long Key;                   // +0x00
    bool operator()(const SKeyedRecord24& record) const { return record.Key == Key; }
};
template SKeyedRecord24* __fastcall std::__find_if(SKeyedRecord24*, SKeyedRecord24*, CIsRecordKeyEqualTo, const std::random_access_iterator_tag&);