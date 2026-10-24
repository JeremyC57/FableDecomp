// std::fill(CGameMapCell* first, CGameMapCell* last, const CGameMapCell& value) -- STLport fill over the 7-byte game-map cells: block-assign the value to every cell
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B78E0, 50 bytes (+ int3 padding), `ret 4`.
// Callers: one site, 0x006B810A (vector<CGameMapCell>::_M_fill_insert / resize helper of the CGameMap TU).
// Types (Ego_r.pdb): CGameMapCell = AtmosThemes uchar[3] +0, AtmosBlends uchar[2] +3, SoundTheme schar +5, Music uchar +6
//   (7 bytes, alignment 1; the landed CGameMap ctor 0x006B7760 gives it an empty user default constructor).
// Shape: STLport 4.x `fill(first, last, const T& val)` from a /Gr TU (`__fastcall`: first in ecx, last in edx, the value
//   reference on the stack): `for (; first != last; ++first) *first = val;` with the implicit 7-byte assignment as a
//   dword + word + byte block copy through copies of both pointers (`mov eax,esi ; mov edi,ecx`), aligned loop head.
// Label correction: the catalog `fill<C3DTriangle2*,C3DTriangle2>` is propagated; the 7-byte stride and the callers in
//   the CGameMap vector helpers name the element.
struct CGameMapCell {                                   // 7 bytes
    unsigned char AtmosThemes[3];                       // +0
    unsigned char AtmosBlends[2];                       // +3
    signed char   SoundTheme;                           // +5
    unsigned char Music;                                // +6
    CGameMapCell() { }
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}

template void __fastcall fill<CGameMapCell*, CGameMapCell>(CGameMapCell*, CGameMapCell*, const CGameMapCell&);
}