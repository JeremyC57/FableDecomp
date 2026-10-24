// std::fill_n<unsigned long*, unsigned int, unsigned long>(unsigned long*, unsigned int, const unsigned long&) -- store the value into n consecutive words, return the end
// Leaf: fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043E060, length 30 bytes, `ret 4` (__fastcall: first in ecx, n in edx, &value on the
// stack), returns first + n.
// Name: STLport 4.x `fill_n(first, n, val)` (it returns the iterator; Dinkumware's _Fill_n returns void, so the donor PDB's
// `??$_Fill_n@...` names are not this STL); single caller 0x00451701, the POD path of vector<4-byte T>(size_type n, const T& val)
// (`_M_finish = uninitialized_fill_n(_M_start, n, val)` -> fill_n for a POD type). The catalog's "_Fill_n<...>" is close.
// Shape: /O2; STLport verbatim -- `for (; n > 0; --n, ++first) *first = val; return first;` with the unsigned size test `test edx,edx
// ; jbe`, the value re-read every pass (it may alias the range) and the 3-byte `lea ecx,[ecx]` loop-alignment pad; Fable builds its
// templates __fastcall (the lane default is __cdecl, hence the explicit keyword).
namespace std {
template <class OutputIter, class Size, class T>
OutputIter __fastcall fill_n(OutputIter first, Size n, const T& val)
{
    for (; n > 0; --n, ++first)
        *first = val;
    return first;
}
template unsigned long* __fastcall fill_n<unsigned long*, unsigned int, unsigned long>(unsigned long*, unsigned int, const unsigned long&);
}