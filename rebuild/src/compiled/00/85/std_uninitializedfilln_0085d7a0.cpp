// std::__uninitialized_fill_n(CTextureManagerPoolInfo* first, unsigned int n, const CTextureManagerPoolInfo& value, const __false_type&) -- copy-construct n pool records from value at first; returns the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085D7A0, length 58 bytes. STLport `__uninitialized_fill_n` (the non-POD
//   branch), compiled /Gr: `__fastcall` (first in ecx, n in edx, value and the empty tag on the stack, `ret 8`); the result in eax.
// Callers: vector<CTextureManagerPoolInfo> fill/insert paths 0x0085DBBA, 0x0085E29F, 0x0085E648 (the landed 0x0085E570 models
//   this callee the same way).
// Layout: CTextureManagerPoolInfo (Ego_r.pdb, 20 bytes): Width, Height, PixelFormat (engine/CPixelFormat.h), UseMipmaps, Count;
//   UseMipmaps is a PDB `bool` but the retail memberwise copy moves its whole word, so it is modelled as a dword.
// Shape: `_Construct` is a placement new (VC7.1 null-tests its result); the class's user memberwise copy constructor gives
//   the direct dword moves through esi (the implicit one is a block copy re-based into esi/edi); `n` is unsigned (`test edx,edx ; jbe`) and counts down.
#include <new>
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
struct __false_type {};
template<class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template<class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CTextureManagerPoolInfo* __fastcall __uninitialized_fill_n<CTextureManagerPoolInfo*, unsigned int, CTextureManagerPoolInfo>(
    CTextureManagerPoolInfo*, unsigned int, const CTextureManagerPoolInfo&, const __false_type&);
}