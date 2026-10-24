// std::__find_if(SKeyedRecord12* first, SKeyedRecord12* last, CIsRecordKeyEqualTo pred, const random_access_iterator_tag&) -- STLport random-access find_if over 12-byte records: the first record whose leading key equals the predicate's key, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe @ 0x005BA23A, 105 bytes, `ret 8`: __fastcall (/Gr template TU) -- first in ecx, last in edx (kept in ebx),
// the 4-byte predicate BY VALUE and the unused category tag on the stack. Only caller: the find_if forwarder @ 0x005BA765
// (`lea eax,[ebp+0Bh] ; push eax ; push [ebp+8]`: the tag in the dead high byte of the predicate's slot; catalog label
// "_Umove<EWeaponClass*>" is propagated), itself called from CTCInventoryClothing (vtable slot 11 @ 0x005B9641) with the key
// of a tree node (`push [ebx+10h]`).
// Body: STLport 4.x _algo.c __find_if: trip_count = (last - first) >> 2 (the 12-byte element divide is `idiv` by 12 under
// optimize("s"), the stride register edi = 12 reused for every `add ecx,edi`), four inlined predicate calls per trip, then
// switch (last - first) { case 3/2/1 fall through; default: return last; } (`dec eax ; je` chain, second divide via ebp).
// Shape: size TU (`push 0Ch ; pop edi`); the predicate holds the key as its only member, so it lives in esi and every test is
// `cmp [ecx],esi` (a `const long&` __find would dereference the slot). Element/predicate names are descriptive (no PDB
// instantiation of this STLport body survives).
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
struct SKeyedRecord12 {
    long Key;                   // +0x00
    long Value[2];              // +0x04
};
struct CIsRecordKeyEqualTo {
    long Key;                   // +0x00
    bool operator()(const SKeyedRecord12& record) const { return record.Key == Key; }
};
template SKeyedRecord12* __fastcall std::__find_if(SKeyedRecord12*, SKeyedRecord12*, CIsRecordKeyEqualTo, const std::random_access_iterator_tag&);