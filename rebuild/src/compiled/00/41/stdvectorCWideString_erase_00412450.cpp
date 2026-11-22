// std::vector<CWideString>::erase(CWideString* first, CWideString* last) -- STLport range erase: assign the tail down over [first, last), destroy the vacated strings, shrink _M_finish, return first
// Leaf: ?erase@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEPAVCWideString@@PAV3@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00412450, 104 bytes (+8 int3 pad), thiscall `ret 8`. Callees:
// CWideString::operator=(const CWideString&) 0x0099B7D0, CWideString::~CWideString 0x0099B510.
// Retail bytes:  push ebx ; push ebp ; push esi ; mov ebp,ecx ; mov eax,[ebp+4] ; mov ecx,[first] ;
//   push edi ; mov edi,[last] ; sub eax,edi ; sar eax,2 ; test eax,eax ; mov esi,ecx ; jle D ;
//   sub edi,ecx ; mov ebx,eax ; L: lea eax,[edi+esi] ; push eax ; mov ecx,esi ; call operator= ;
//   add esi,4 ; dec ebx ; jne L ; mov ecx,[first] ; D: mov ebx,[ebp+4] ; cmp esi,ebx ; mov edi,esi ;
//   je E ; K: mov ecx,edi ; call ~CWideString ; add edi,4 ; cmp edi,ebx ; jne K ;
//   mov eax,[first] ; pop edi ; mov [ebp+4],esi ; ... ret 8 ; E: pop edi ; mov [ebp+4],esi ; ...
//   mov eax,ecx ; ret 8
// Shape: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _IsPODType()); _Destroy(__i, _M_finish); _M_finish = __i;
//   return __first;`: CWideString is not POD, so the random-access __copy (count `_M_finish - last` once, element operator= out of
//   line) and the _Destroy loop, both expanded.
// Name fix (NF232, landing nf231 ruling 2: retail is a VC7.1 STLport build): the STLport spelling 14 landed binders emit
//   (0x004091C0, 0x0040BCA0, 0x0040C790, 0x00412B20, 0x0042EBB6, 0x0047FE83, 0x00492381, 0x00495830, 0x00495890, 0x004959E0, 0x005955AB,
//   0x00597DF7, 0x005FDAD0, 0x00606300; 0x00412B70 moves with this row, 0x00495780 still binds an allocator-less stand-in) and the table line (witness 0x00412B20). The PDBs (Dinkumware debug STL) spell the member with
//   _Vector_iterator. The old self row was the local stand-in CRetailVector<CWideString>::erase.
#include "stlport/fable_containers.h"
#include "fable_string.h"
template std::vector<CWideString>::iterator std::vector<CWideString>::erase(std::vector<CWideString>::iterator, std::vector<CWideString>::iterator);
