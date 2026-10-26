// _global::Copy_CSubtitledMessage_00719f00(const CSubtitledMessage* first, const CSubtitledMessage* last, CSubtitledMessage* result, const random_access_iterator_tag&, int*) -- STLport __copy<CSubtitledMessage*>: counted forward assignment of 68-byte elements, returns the advanced result
// Leaf: Copy_CSubtitledMessage_00719f00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00719F00, length 67 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CSubtitledMessage::operator= @ 0x00719E80.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x78787879 ; sar 5 (/68)), then one out-of-line assignment per element (`push esi ; mov ecx,edi ; call`),
// both pointers stepped by 0x44; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is CSubtitledMessage (its landed operator= @ 0x00719E80; catalog calls that body a copy constructor).
class CSubtitledMessage { public:                       // 68 bytes (0x44); its own assignment copies the body
    unsigned char _body[0x44];
    CSubtitledMessage& operator=(const CSubtitledMessage& other);   // 0x00719E80
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CSubtitledMessage* __fastcall Copy_CSubtitledMessage_00719f00(const CSubtitledMessage* first, const CSubtitledMessage* last, CSubtitledMessage* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}