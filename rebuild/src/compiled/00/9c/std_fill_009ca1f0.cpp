// std::fill(NGameText::CTextTag* first, NGameText::CTextTag* last, const NGameText::CTextTag& value) -- STLport fill: assign value to every element of [first, last)
// Leaf: ??$fill@PAVCTextTag@NGameText@@V12@@std@@YIXPAVCTextTag@NGameText@@0ABV12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009ca1f0, 46 bytes, /Gr __fastcall (first in ecx, last in edx, &value on the stack), `ret 4`.
// Identity (proven): its only caller is vector<NGameText::CTextTag>::_M_fill_insert 0x009CAF60 (bound ?_M_fill_insert@?$vector@VCTextTag@NGameText@@@std@@...); 8-byte element {long Pos; CCharString Name} (family L011's __uninitialized_copy 0x009CA190 serves the same vector).
// Callee: the second member's operator= 0x0099EFB0 (thiscall).
// Shape (family L052): `for (; first != last; ++first) *first = value;` -- the element's implicit assignment copies the
//   leading dword through eax and calls the member assignment with &value + 4 kept in ebp; stride 8.
class CCharString {
public:
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
private:
    void* PStringData;                                    // +0x00
};
namespace NGameText {
class CTextTag {                                           // Ego_r.pdb, 8 bytes
public:
    long Pos;                                             // +0x00
    CCharString Name;                                     // +0x04
};
}
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
template void __fastcall std::fill(NGameText::CTextTag*, NGameText::CTextTag*, const NGameText::CTextTag&);