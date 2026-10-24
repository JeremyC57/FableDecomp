// std::__uninitialized_fill_n(CGameMapCell* first, unsigned int n, const CGameMapCell& x, const __false_type&) -- placement-copy n game-map cells into raw storage, return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7C10, 54 bytes, `ret 8`.
// Callers: one site, 0x006B8006 (vector<CGameMapCell>::_M_insert_overflow, the n > 1 branch of 0x006B7F70).
// Types (Ego_r.pdb): CGameMapCell = AtmosThemes uchar[3] +0, AtmosBlends uchar[2] +3, SoundTheme schar +5, Music uchar +6
//   (7 bytes, alignment 1, an empty user default constructor -- so STLport sees a non-POD and takes __false_type).
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)` from a /Gr TU (`__fastcall`: first in
//   ecx -> eax as `cur`, n in edx counted down, &x and the empty tag reference on the stack, `ret 8`): a placement-new
//   null test per cell and the implicit 7-byte copy (dword + word + byte) from x.
// Label correction: the catalog `_Fill_n<unsigned long*,unsigned int,unsigned long>` is propagated; `ret 8` with a tag
//   and the placement null test name the four-argument STLport helper (its `ret 4` front end is 0x006B7CC0).
inline void* operator new(unsigned int, void* p) { return p; }

struct __false_type { __false_type() { } };

struct CGameMapCell {                                   // 7 bytes
    unsigned char AtmosThemes[3];                       // +0
    unsigned char AtmosBlends[2];                       // +3
    signed char   SoundTheme;                           // +5
    unsigned char Music;                                // +6
    CGameMapCell() { }
};

namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}

template CGameMapCell* __fastcall __uninitialized_fill_n<CGameMapCell*, unsigned int, CGameMapCell>(CGameMapCell*, unsigned int, const CGameMapCell&, const __false_type&);
}