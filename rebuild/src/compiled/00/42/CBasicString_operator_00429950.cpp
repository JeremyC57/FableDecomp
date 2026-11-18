// CBasicString<char>::operator<(const CBasicString<char>& other) const -- lexicographic less-than of two strings
// Leaf: ??M?$CBasicString@D@@QBE_NABV0@@Z
// Retail: Fable.exe @ 0x00429950, 88 bytes, __thiscall `ret 4`. An inlined three-way Compare walks both buffers in
// step (one pointer plus the constant distance between them), signed char order; operator< is `Compare(...) < 0`,
// which VC7.1 tail-duplicates into each of the three return paths without folding the constant (`xor ecx,ecx ;
// test eax,eax ; setl cl`). Taking the other string's pointer first makes it the loop induction pointer (this
// string is read through the constant distance), as retail.
// NF80 (q10763 / q10923): the one definition now lives in fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY, a template member
//   definition); this row explicitly instantiates it. No local CBasicString model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#include "fable_string.h"

template bool CBasicString<char>::operator<(const CBasicString<char>& other) const;
