// std::fill_n<unsigned long*, unsigned int, unsigned long>(unsigned long*, unsigned int, const unsigned long&) -- STLport fill_n: writes n copies of a dword, returns the end
// Leaf: fill_n
// Retail: Fable.exe @ 0x00788C87, 23 bytes, `ret 4` (__fastcall: first in ecx, n in edx, &val on the stack); caller(s) 0x00789046.
// The catalog name std::_Fill_n comes from Ego_r.pdb's Dinkumware build (??$_Fill_n@PAKIK@std@@YIXPAKIABK@Z, returning void);
// retail is STLport, whose fill_n returns the advanced iterator: the loop runs on eax (`mov eax,ecx`) and only the
// returning form reproduces that. One copy per TU (no ICF across these four).
// Shape: size TU (val loaded once before the loop, no loop alignment padding).
#pragma optimize("s", on)
namespace std {
template <class OutputIter, class Size, class T> inline
OutputIter __fastcall fill_n(OutputIter first, Size n, const T& val)
{
    for (; n > 0; --n, ++first)
        *first = val;
    return first;
}
template unsigned long* __fastcall fill_n<unsigned long*, unsigned int, unsigned long>(unsigned long*, unsigned int, const unsigned long&);
}