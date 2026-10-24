// _global::Copy_SListHandle04_0076ff90_00770230(const SListHandle04_0076ff90* first, const SListHandle04_0076ff90* last, SListHandle04_0076ff90* result, const random_access_iterator_tag&, int*) -- STLport __copy<SListHandle04_0076ff90*>: counted forward assignment of 4-byte handle elements, returns the advanced result
// Leaf: Copy_SListHandle04_0076ff90_00770230
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00770230, length 53 bytes, `ret 0Ch`.
// Callees: SListHandle04_0076ff90::operator= @ 0x0076FF90.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (sar 2 (/4)), then one out-of-line assignment per element (`push esi ; mov ecx,edi ; call`),
// both pointers stepped by 0x4; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is a 4-byte record assigned by 0x0076FF90 (catalog "std::vector<CLipSyncPhoneme>::operator=", unverified); no direct callers (reached through a jump or dead).
struct SListHandle04_0076ff90 {                  // 4 bytes (0x4); its own assignment copies the body
    unsigned char _body[0x4];
    SListHandle04_0076ff90& operator=(const SListHandle04_0076ff90& other);   // 0x0076FF90
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SListHandle04_0076ff90* __fastcall Copy_SListHandle04_0076ff90_00770230(const SListHandle04_0076ff90* first, const SListHandle04_0076ff90* last, SListHandle04_0076ff90* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}