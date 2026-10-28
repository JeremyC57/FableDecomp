// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (NGameText::CTextTag*, NGameText::CTextTag*, int) STLport random-access backward copy: `for (n = last - first; n > 0; --n) *--result = *--last;`, returns the moved-back result
// Leaf: ??$__copy_backward@PAVCTextTag@NGameText@@PAV12@H@std@@YIPAVCTextTag@NGameText@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C9DD0, 64 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element NGameText::CTextTag (8 bytes); evidence: its callers 0x009CA1D0 and 0x009CAF60 are the grow / fill-insert under resize 0x009CB7D0, which CTextEntry::ReadFromDataStream 0x009C8DE0 calls on this+0x2C -- Ego_r.pdb's CTextEntry::Tags (+0x2C), CArray<NGameText::CTextTag>, and CTextTag is {long Pos; CCharString Name}; FableWin.pdb emits _Copy_backward<NGameText::CTextTag*>
// Label: the manifest's "_Copy_backward<std::pair<float,CCharString>*,std::pair<float,class_CCharString>*>" is propagated (family H070: six backward copies over a 4-byte field followed by a member with an
//   out-of-line operator=; retail has no identical-code folding, so each address is one element type). Two shapes: 8-byte elements
//   (`sar eax,3`) and 16-byte ones (`sar eax,4`).
// Callee: CCharString::operator= 0x0099EFB0 on Name.
// Shape: the element's implicit operator= inline -- the first dword through eax, then the member's operator= on +4.
class CCharString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
namespace NGameText {
class CTextTag {                                     // 8 bytes (Ego_r.pdb)
public:
    long        Pos;                                  // +0x00
    CCharString Name;                                 // +0x04
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

template NGameText::CTextTag* __fastcall __copy_backward<NGameText::CTextTag*, NGameText::CTextTag*, int>(NGameText::CTextTag*, NGameText::CTextTag*, NGameText::CTextTag*,
                                                                         const random_access_iterator_tag&, int*);
}   // namespace std