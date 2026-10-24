// std::uninitialized_fill_n(CGameMapCell* first, unsigned int n, const CGameMapCell& x) -- STLport three-argument front end: placement-copy n game-map cells into raw storage, return first + n
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7CC0, 54 bytes, `ret 4`.
// Callers: 0x006B7D9F (vector<CGameMapCell>::vector(n), 0x006B7D60) and 0x006B8131 (the fill-insert helper).
// Types (Ego_r.pdb): CGameMapCell = AtmosThemes uchar[3] +0, AtmosBlends uchar[2] +3, SoundTheme schar +5, Music uchar +6
//   (7 bytes, alignment 1, an empty user default constructor).
// Shape: `ret 4` = the top-level `uninitialized_fill_n(first, n, x)` with `__uninitialized_fill_n(.., __false_type())`
//   inlined (`__forceinline`, GOTCHAS 00440180/007688b0); otherwise the same bytes as the four-argument helper 0x006B7C10
//   (first ecx -> eax as `cur`, n in edx counted down, &x -> esi, placement-new null test, 7-byte block copy).
// Label correction: the catalog `_Fill_n<unsigned long*,unsigned int,unsigned long>` is propagated.
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
__forceinline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template CGameMapCell* __fastcall uninitialized_fill_n<CGameMapCell*, unsigned int, CGameMapCell>(CGameMapCell*, unsigned int, const CGameMapCell&);
}