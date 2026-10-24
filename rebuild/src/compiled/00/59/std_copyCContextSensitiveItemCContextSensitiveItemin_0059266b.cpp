// std::__copy<CContextSensitiveItem*, CContextSensitiveItem*, int>(first, last, result, const random_access_iterator_tag&, int*) -- counted forward copy of 12-byte context-sensitive item records
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059266B, 45 bytes, `ret 0Ch` (`/Gr` `__fastcall`: first in ecx, last in edx;
// result, the empty tag reference and the null distance pointer on the stack; returns the advanced result).
// Callers: the `__copy_aux`/copy forwarder 0x00592D0A (no callers of its own) and vector<CTCInventory::
// CContextSensitiveItem>::erase(iterator) @ 0x0059310D (`copy(position + 1, _M_finish, position)`), which the landed
// RemoveContextSensitivePreferredItem 0x005829CD calls on the static list @ 0x013B8B40.
// Label: the catalog "_Move<CMusicSetEntry*,CMusicSetEntry*>" is a propagated Dinkumware spelling of a byte-identical
// body: here it is STLport 4.x `__copy` for random-access iterators over the 12-byte PDB record
// CTCInventory::CContextSensitiveItem {ThingDefIndex, ButtonIndex, Priority}.
// Shape: the element count `last - first` is computed once (`sub ; push 0Ch ; cdq ; pop esi ; idiv`), each pass
// copies one record with three `movsd` while the result slot itself is advanced in memory (`add [esp+0Ch],0Ch`) and
// re-read at the end. Size TU: `#pragma optimize("s", on)`, frameless.
#pragma optimize("s", on)

struct CContextSensitiveItem {                       // CTCInventory::CContextSensitiveItem, 12 bytes (Ego_r.pdb)
    long ThingDefIndex;                              // +0x00
    long ButtonIndex;                                // +0x04
    long Priority;                                   // +0x08
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template CContextSensitiveItem* __fastcall __copy(CContextSensitiveItem*, CContextSensitiveItem*, CContextSensitiveItem*,
                                                  const random_access_iterator_tag&, int*);
}