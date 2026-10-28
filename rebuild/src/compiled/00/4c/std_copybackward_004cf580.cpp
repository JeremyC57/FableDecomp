// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (std::pair<float, CCharString>*, std::pair<float, CCharString>*, int) STLport random-access backward copy: `for (n = last - first; n > 0; --n) *--result = *--last;`, returns the moved-back result
// Leaf: ??$__copy_backward@PAU?$pair@MVCCharString@@@std@@PAU12@H@std@@YIPAU?$pair@MVCCharString@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CF580, 64 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element std::pair<float, CCharString> (8 bytes); evidence: its callers (the small out-of-line copy_backward wrappers 0x004CFE90, 0x004D0640, 0x004D0B30, and 0x004D0F80) are sort helpers of CSubtitledMessage's TU -- 0x004D0F80 (__linear_insert) is reached through the insertion sort 0x004D14E0 and 0x004D24F0 from CSubtitledMessage::CSubtitledMessage 0x004CE550, and it compares float keys (`fcomp`) before calling it and then assigns the CCharString half -- Ego_r.pdb's CSubtitledMessage::AnimationTags, CVectorMap<float, CCharString>, the class's only container of a 4-byte key and a CCharString
// Label: the manifest's "_Copy_backward<std::pair<float,CCharString>*,std::pair<float,class_CCharString>*>" is propagated (family H070: six backward copies over a 4-byte field followed by a member with an
//   out-of-line operator=; retail has no identical-code folding, so each address is one element type). Two shapes: 8-byte elements
//   (`sar eax,3`) and 16-byte ones (`sar eax,4`).
// Callee: CCharString::operator= 0x0099EFB0 on `second`.
// Shape: the element's implicit operator= inline -- the first dword through eax, then the member's operator= on +4.
class CCharString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                        // +0x00
    _T2 second;                                       // +0x04
};
}

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template std::pair<float, CCharString>* __fastcall __copy_backward<std::pair<float, CCharString>*, std::pair<float, CCharString>*, int>(std::pair<float, CCharString>*, std::pair<float, CCharString>*, std::pair<float, CCharString>*,
                                                                         const random_access_iterator_tag&, int*);
}   // namespace std