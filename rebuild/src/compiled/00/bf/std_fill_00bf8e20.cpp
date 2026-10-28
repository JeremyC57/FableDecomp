// std::fill(CEngineTexturePalette::CPaletteEntry* first, CEngineTexturePalette::CPaletteEntry* last, const CEngineTexturePalette::CPaletteEntry& value) -- STLport fill: assign value to every element of [first, last)
// Leaf: ??$fill@PAUCPaletteEntry@CEngineTexturePalette@@U12@@std@@YIXPAUCPaletteEntry@CEngineTexturePalette@@0ABU12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bf8e20, 46 bytes, /Gr __fastcall (first in ecx, last in edx, &value on the stack), `ret 4`.
// Identity (proven): its only caller is vector<CEngineTexturePalette::CPaletteEntry>::_M_fill_insert 0x00BF95D0 (bound ?_M_fill_insert@?$vector@UCPaletteEntry@CEngineTexturePalette@@@std@@...); 8-byte element {unsigned long Index; CCharString Name}.
// Callee: the second member's operator= 0x0099EFB0 (thiscall).
// Shape (family L052): `for (; first != last; ++first) *first = value;` -- the element's implicit assignment copies the
//   leading dword through eax and calls the member assignment with &value + 4 kept in ebp; stride 8.
class CCharString {
public:
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
private:
    void* PStringData;                                    // +0x00
};
class CEngineTexturePalette {
public:
    struct CPaletteEntry {                                 // Ego_r.pdb (struct), 8 bytes
        unsigned long Index;                              // +0x00
        CCharString Name;                                 // +0x04
    };
};
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
template void __fastcall std::fill(CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*, const CEngineTexturePalette::CPaletteEntry&);