// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (CEngineTexturePalette::CPaletteEntry)
// Leaf: ??$__copy@PAUCPaletteEntry@CEngineTexturePalette@@PAU12@H@std@@YIPAUCPaletteEntry@CEngineTexturePalette@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bf8e50, 61 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) ??$__copy@PAUCPaletteEntry@CEngineTexturePalette@@PAU12@H@std@@YIPAUCPaletteEntry@CEngineTexturePalette@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z; callers the landed vector<CPaletteEntry>::erase 0x00BF9420 and fill_n 0x00BF8F70.
// Callee: the second member's operator= 0x0099EFB0 (thiscall).
// Shape (family L049): n = (last - first) >> 3; per element the leading dword copied through eax, then one thiscall
//   assignment of the member at +4; returns the advanced result.
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
struct random_access_iterator_tag { };
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
}
template CEngineTexturePalette::CPaletteEntry* __fastcall std::__copy(CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*, const std::random_access_iterator_tag&, int*);