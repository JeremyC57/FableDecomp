// std::fill(CTextureManagerPoolInfo* first, CTextureManagerPoolInfo* last, const CTextureManagerPoolInfo& value) -- assign value to every 20-byte pool record of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085D680, length 61 bytes (+3 int3). STLport `fill`, compiled /Gr like the
//   other SGI algorithm instantiations: `__fastcall` (first in ecx, last in edx, the value reference on the stack, `ret 4`).
// Mangled (Ego_r.pdb): ??$fill@PAVCTextureManagerPoolInfo@@V1@@std@@YIXPAVCTextureManagerPoolInfo@@0ABV1@@Z.
// Callers: vector<CTextureManagerPoolInfo>::_M_fill_insert 0x0085E570 (landed; calls at 0x0085E62A, 0x0085E687).
// Layout: CTextureManagerPoolInfo (Ego_r.pdb, 20 bytes): Width, Height, PixelFormat (engine/CPixelFormat.h), UseMipmaps, Count;
//   UseMipmaps is a PDB `bool` but the retail memberwise copy moves its whole word, so it is modelled as a dword.
// Shape: the implicit copy assignment (a block copy: the source/destination re-based into esi/edi per element, words through
//   ebx); the class's user memberwise copy constructor (needed by the uninitialized_fill_n 0x0085D7A0) does not change it.
#include "engine/CPixelFormat.h"
class CTextureManagerPoolInfo {
public:
    unsigned long Width;                                // +0x00
    unsigned long Height;                               // +0x04
    CPixelFormat  PixelFormat;                          // +0x08
    unsigned long UseMipmaps;                           // +0x0C (bool in Ego_r.pdb; the retail copy moves the whole word)
    unsigned long Count;                                // +0x10
    CTextureManagerPoolInfo(const CTextureManagerPoolInfo& other)
        : Width(other.Width), Height(other.Height), PixelFormat(other.PixelFormat), UseMipmaps(other.UseMipmaps), Count(other.Count) {}
};
namespace std {
template<class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill<CTextureManagerPoolInfo*, CTextureManagerPoolInfo>(CTextureManagerPoolInfo*, CTextureManagerPoolInfo*,
                                                                                 const CTextureManagerPoolInfo&);
}