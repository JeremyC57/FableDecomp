// std::__copy<CFadeInFadeOutBase*, CFadeInFadeOutBase*, int>(first, last, result, const random_access_iterator_tag&, int*) -- counted forward copy of 28-byte fade records
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x005926CE, 51 bytes, `ret 0Ch` (`/Gr` `__fastcall`: first in ecx, last in edx;
// result, the empty tag reference and the null distance pointer on the stack; returns the advanced result).
// Callers: the copy forwarder 0x00592D68 (no callers of its own) and vector<CFadeInFadeOutBase>::erase(first, last)
// @ 0x0059485B (`copy(last, _M_finish, first)`), reached from resize(n, x) 0x00594CEE <- resize(n) 0x00594DF4 (a
// default CFadeInFadeOutBase() @ 0x00436560 in the frame) <- the CTCInventory-TU setup at 0x00590EAF (vector at +0x160).
// Label: the catalog "_Copy_impl<CDegradableInfo*,CDegradableInfo*>" is a propagated Dinkumware spelling of a
// byte-identical body: here it is STLport 4.x `__copy` for random-access iterators over the 28-byte (0x1C)
// CFadeInFadeOutBase.
// Shape: the element count `last - first` is computed once (`push 1Ch ; cdq ; pop ecx ; idiv`, first kept in ebx),
// each pass copies one record with `push 7 ; pop ecx ; rep movsd` (the trivial assignment of a 28-byte class) while
// the result slot itself is advanced in memory (`add [esp+10h],1Ch`) and re-read at the end.
// Model: engine/CFadeInFadeOutBase.h (Ego_r.pdb layout). Size TU: `#pragma optimize("s", on)`, frameless.
#pragma optimize("s", on)
#include "engine/CFadeInFadeOutBase.h"

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

template CFadeInFadeOutBase* __fastcall __copy(CFadeInFadeOutBase*, CFadeInFadeOutBase*, CFadeInFadeOutBase*,
                                               const random_access_iterator_tag&, int*);
}