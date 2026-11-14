// std::char_traits<char>::copy(char* s1, const char* s2, size_t n) -- copy n characters (none when n is 0) and return the destination
// Leaf: ?copy@?$char_traits@D@std@@SAPADPADPBDI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00564D50, 42 bytes, two `ret` (0x00564D5C, 0x00564D79). The catalog row 0x00564D5D (29 bytes,
//   labelled _Copy_impl<CAppearanceModifierGraphics::CEntry*, ...>) is this function's tail after the `jne 0x00564D5D` at 0x00564D56.
// Name: FableWin.pdb ?copy@?$char_traits@D@std@@SAPADPADPBDI@Z + the STLport 4.6.2 char_traits.h body (zero-length test, memcpy, s1 returned).
#define FABLE_RETAIL_STLP_CHAR_TRAITS_COPY_OUTOFLINE
#include "stlport/fable_string.h"

char* __cdecl std::char_traits<char>::copy(char* s1, const char* s2, size_t n)
{
    return (n == 0 ? s1 : (char*)memcpy(s1, s2, n * sizeof(char)));
}
