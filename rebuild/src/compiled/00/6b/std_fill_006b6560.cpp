// std::fill(CThemeChannel* first, CThemeChannel* last, const CThemeChannel& value) -- STLport fill over 496-byte environment theme channels: one out-of-line assignment per element
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6560, 37 bytes (+ int3 padding), `ret 4`.
// Callees: CThemeChannel::operator= @ 0x006B6180 (landed; catalog "CBlendedEnvironmentTheme::operator=" is propagated).
// Shape: STLport 4.x `fill(first, last, const T& val)` from a /Gr TU (`__fastcall`: first in ecx, last in edx, the
//   value reference on the stack): `for (; first != last; ++first) *first = val;`. The element is 0x1F0 bytes -- the
//   Ego_r CThemeChannel (520 bytes) with its three CIDrawEnvironmentTextureSet pairs of CArrays shrunk to the retail
//   12-byte STLport vectors -- and its compiler-generated operator= is out of line, so the loop is `push ebx ;
//   mov ecx,esi ; call`.
// Label correction: the catalog `fill<CArray<unsigned short>*,CArray<unsigned short> >` is propagated; the stride
//   (0x1F0) and the callee (the landed CThemeChannel::operator=) name the element.
struct CThemeChannel {                                  // 496 bytes (0x1F0): colours, scalers, three texture sets
    unsigned char _body[0x1f0];
    CThemeChannel& operator=(const CThemeChannel& other);   // 0x006B6180
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}

template void __fastcall fill<CThemeChannel*, CThemeChannel>(CThemeChannel*, CThemeChannel*, const CThemeChannel&);
}