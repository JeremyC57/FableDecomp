// std::vector<CDateAndTime>::erase(CDateAndTime* first, CDateAndTime* last) -- STLport range erase: copy the tail [last, finish) down onto first, shrink finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCDateAndTime@@V?$allocator@VCDateAndTime@@@std@@@std@@QAEPAVCDateAndTime@@PAV3@0@Z
// Retail: Fable.exe @ 0x00411B00, 38 bytes, thiscall `ret 8`: `push 0 ; lea eax,[first] ; mov esi,ecx ; mov edx,[esi+4] ;
// mov ecx,[last] ; push eax ; push edi(first) ; call 0x0040E350 ; mov [esi+4],eax ; mov eax,edi ; ret 8`.
// Shape: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _IsPODType()); _Destroy(__i, _M_finish); _M_finish = __i;
//   return __first;`. CDateAndTime (28 bytes, engine/CDateAndTime.h) is not declared POD, so __copy_ptrs takes the __false_type arm into
//   the random-access __copy, which this TU calls as the __fastcall COMDAT 0x0040E350 (first in ecx, last in edx, result, the tag's
//   address and the null distance pointer on the stack; FABLE_RETAIL_STLP_COPY_NOINLINE; landed 0x0059AEAF binds it as
//   ??$__copy@PAVCDateAndTime@@PAV1@H@std@@YIPAVCDateAndTime@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z); _Destroy of the
//   trivially destructible element is empty.
// Name fix (NF232, landing nf231 ruling 2: retail is a VC7.1 STLport build): the STLport spelling all four other landed binders emit
//   (0x0047FE83 CTCInGameMenu::GetFileNameList, 0x004861F7 vector<CDateAndTime>::resize, 0x00597DF7 CFrontEndManager::GetSavedGameNames,
//   0x0042EBB6 CNewFrontendGameComponent::Shutdown) and the table line (witness 0x004861F7). The PDBs (Dinkumware debug STL) spell the member
//   with _Vector_iterator. The old self row was the stand-in __fastcall vector_push_back_copy / Vector_CopyImpl.
// Opt: /O2 (16-aligned entry; the HEAD stand-in's size pragma gives a frame pointer on this body).
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#include "stlport/fable_containers.h"
#include "engine/CDateAndTime.h"
template std::vector<CDateAndTime>::iterator std::vector<CDateAndTime>::erase(std::vector<CDateAndTime>::iterator, std::vector<CDateAndTime>::iterator);
