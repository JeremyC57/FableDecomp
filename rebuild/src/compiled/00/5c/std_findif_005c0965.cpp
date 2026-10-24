// std::__find_if(const NInventory::CItem* first, const NInventory::CItem* last, std::const_mem_fun_ref_t<bool, NInventory::CItem> pred, const random_access_iterator_tag&) -- STLport random-access find_if over a category's items: the first item for which the bound const member predicate returns true, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe @ 0x005C0965, 145 bytes, `ret 8`: __fastcall (/Gr template TU) -- first in ecx, last in edx (spilled to the
// `push ecx` frame word), the 4-byte predicate (a pointer to a const member function) BY VALUE and the unused tag on the
// stack. Only caller: the find_if forwarder @ 0x005C0C86 (catalog "_Umove<EWeaponClass*>" propagated), called from
// NInventory::CCategory::GetFirstEmptySlot 0x005BD96B with mem_fun_ref(&CItem::<IsEmpty-style predicate> @ 0x005BC46C) over
// ItemsInCategory. Ego_r.pdb names the instantiation `_Find_if<const NInventory::CItem*, const_mem_fun_ref_t<bool, CItem>>`.
// Body: STLport 4.x _algo.c __find_if: trip_count = (last - first) >> 2 (`idiv` by the 0x14 CItem size under optimize("s"),
// the stride kept in ebp: `push 14h ; pop ebp`), four inlined `(x.*f)()` calls per trip (`mov ecx,esi ; call ebx` -- the
// single-inheritance member pointer is a plain code address), then the 3/2/1 remainder switch.
// Shape: size TU; no frame pointer here because ebp holds the stride (the binder2nd twins 0x005C1153/0x005C11FA keep one).
// Element: NInventory::CItem (engine/CItem.h agrees: 0x14 bytes, ItemType +0); the predicate's target is declared only.
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
namespace std {
template <class R, class T> class const_mem_fun_ref_t {
    typedef R (T::*fun_type)() const;
    fun_type f;
public:
    explicit const_mem_fun_ref_t(fun_type pf) : f(pf) {}
    R operator()(const T& x) const { return (x.*f)(); }
};
}
template const CItem* __fastcall std::__find_if(const CItem*, const CItem*, std::const_mem_fun_ref_t<bool, CItem>, const std::random_access_iterator_tag&);