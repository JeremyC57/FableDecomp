// std::vector<CWideString>::resize(unsigned int newSize) -- STLport resize with a default value: erase the excess tail or fill-insert copies of an empty CWideString at the end
// Leaf: ?resize@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00412B70, 99 bytes, thiscall `ret 4`. Callees: CWideString::CWideString()
// 0x0099AED0, vector<CWideString>::erase(first, last) 0x00412450, vector<CWideString>::_M_fill_insert(pos, n, const T&)
// 0x004126E0, CWideString::~CWideString 0x0099B510.
// Retail bytes:  push ecx ; push ebx ; push esi ; mov esi,ecx ; push edi ; lea ecx,[tmp] ;
//   call CWideString() ; mov edi,[esi+4] ; mov edx,[esi] ; mov ecx,[newSize] ; mov ebx,edi ;
//   sub ebx,edx ; sar ebx,2 ; cmp ecx,ebx ; jae G ; lea eax,[edx+ecx*4] ; push edi ; push eax ;
//   mov ecx,esi ; call erase ; ~tmp ; ... ret 4 ; G: mov edi,[esi+4] ; push eax (&tmp, the ctor's
//   returned this) ; mov eax,edi ; sub eax,edx ; sar eax,2 ; sub ecx,eax ; push ecx ; push edi ;
//   mov ecx,esi ; call _M_fill_insert ; ~tmp ; ... ret 4
// Shape: STLport 4.6 `void resize(size_type __new_size) { resize(__new_size, _Tp()); }` over the inline `resize(n, x) { if (n <
//   size()) erase(begin() + n, end()); else _M_fill_insert(end(), n - size(), x); }`; the default value is a temporary built first
//   and destroyed in both arms. This TU calls erase(first, last) out of line (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE).
// Name fix (NF232, landing nf231 ruling 2: retail is a VC7.1 STLport build): the STLport spelling the landed binder 0x00501D30 and
//   the table line emit; the callees move onto the STLport spellings the other landed binders emit (erase 0x00412450: 14 rows;
//   _M_fill_insert 0x004126E0: 0x00412B20). The old row was the local stand-in CRetailVector<CWideString>::resize.
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
template void std::vector<CWideString>::resize(std::vector<CWideString>::size_type);
