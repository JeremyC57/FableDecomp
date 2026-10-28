// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*, int) STLport random-access backward copy: `for (n = last - first; n > 0; --n) *--result = *--last;`, returns the moved-back result
// Leaf: ??$__copy_backward@PAUCPaletteEntry@CEngineTexturePalette@@PAU12@H@std@@YIPAUCPaletteEntry@CEngineTexturePalette@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF8E90, 64 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element CEngineTexturePalette::CPaletteEntry (8 bytes); evidence: its callers 0x00BF8FD0 and 0x00BF95D0 are the grow / fill-insert under resize 0x00BF97A0, which CEngineTexturePalette::Load 0x00BF9290 and GetTexturePaletteIndex 0x00BF9150 call on this+4 -- Ego_r.pdb's CEngineTexturePalette::Palette (+4), CArray<CEngineTexturePalette::CPaletteEntry>, and CPaletteEntry is the struct {unsigned long Index; CCharString Name}; FableWin.pdb emits _Copy_backward<CEngineTexturePalette::CPaletteEntry*>
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
class CEngineTexturePalette {
public:
    struct CPaletteEntry {                           // 8 bytes (Ego_r.pdb)
        unsigned long Index;                         // +0x00
        CCharString   Name;                          // +0x04
    };
};

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

template CEngineTexturePalette::CPaletteEntry* __fastcall __copy_backward<CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*, int>(CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*,
                                                                         const random_access_iterator_tag&, int*);
}   // namespace std