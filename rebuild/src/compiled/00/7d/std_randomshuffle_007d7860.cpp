// std::random_shuffle(CCard** first, CCard** last) -- STLport random_shuffle with the default generator: for each i after first, swap *i with first[rand() % (i - first + 1)]
// Flags: /Gr
// Leaf: ??$random_shuffle@PAPAVCCard@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D7860, 64 bytes, `__fastcall` (first ecx, last edx), plain `ret`.
// Identity (probable): callers CTCTavernGameCardPairs::ShuffleCards 0x007D71E0 and CTCTavernGameCardBase::ShuffleCards 0x008F10B0, both on this+0x134 = Ego_r.pdb CTCTavernGameCardBase::Cards (vector<CCard*>, PDB +0x140).
// Callee: rand 0x00BFEB16 (msvcr71 import thunk).
// Shape (family K127, representative 0x005131D0): STLport 4.6 `if (first == last) return; for (i = first + 1; i != last; ++i)
//   iter_swap(i, first + __random_number((i - first) + 1));` with __random_number = `rand() % n` and __iter_swap inline
//   (the K-family's landed random_shuffle 0x00513210 without the functor).
extern "C" int __cdecl rand(void);                       // 0x00BFEB16
class CCard;
namespace std {
template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap(ForwardIter1 i1, ForwardIter2 i2, T*)
{
    T tmp = *i1;
    *i1 = *i2;
    *i2 = tmp;
}
template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 i1, ForwardIter2 i2)
{
    __iter_swap(i1, i2, i1);
}
template <class _Distance> inline _Distance __random_number(_Distance __n) { return rand() % __n; }
template <class RandomAccessIter>
void __fastcall random_shuffle(RandomAccessIter first, RandomAccessIter last)
{
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        iter_swap(i, first + __random_number((i - first) + 1));
}
template void __fastcall random_shuffle<CCard**>(CCard**, CCard**);
}