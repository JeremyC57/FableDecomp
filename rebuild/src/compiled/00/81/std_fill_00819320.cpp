// std::fill(CTextureManagerPoolInfo* first, CTextureManagerPoolInfo* last, const CTextureManagerPoolInfo& val) -- STLport fill of a range of 20-byte texture-pool descriptors with copies of val
// Leaf: ??$fill@
// Retail: Fable.exe @ 0x00819320, 64 bytes, `ret 4`: __fastcall (the algorithm TUs are /Gr), first in ecx, last in edx,
// the value reference on the stack.
// Callers: 0x0081D16A and 0x0081D1C7, the vector fill-insert 0x0081D0B0 of the 20-byte element.
// Identity: the catalog label `fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>` fits the stride: Ego_r.pdb
// CTextureManagerPoolInfo is 20 bytes (ulong Width, ulong Height, CPixelFormat PixelFormat, bool UseMipmaps, ulong
// Count); a byte-identical fill of any 20-byte POD would carry the same bytes, so the type is plausible, not proven.
// Shape: STLport `fill` -- `for (; first != last; ++first) *first = val;` -- the implicit trivial assignment is a
// five-dword block copy through esi/edi (the bool slot copied as a whole dword with its padding).
class CPixelFormat {
public:
    long FormatIndex;                             // +0x0
};
class CTextureManagerPoolInfo {
public:
    unsigned long Width;                          // +0x00
    unsigned long Height;                         // +0x04
    CPixelFormat  PixelFormat;                    // +0x08
    bool          UseMipmaps;                     // +0x0C
    unsigned long Count;                          // +0x10
};
namespace std {
template <class ForwardIter, class Tp>
void __fastcall fill(ForwardIter first, ForwardIter last, const Tp& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CTextureManagerPoolInfo*, CTextureManagerPoolInfo*, const CTextureManagerPoolInfo&);
}