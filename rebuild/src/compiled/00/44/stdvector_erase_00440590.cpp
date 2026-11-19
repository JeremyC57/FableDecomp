// std::vector<long>::erase(long* first, long* last) -- STLport range erase on a POD element: memmove the tail [last, finish) down onto first (skipped when last == finish), set finish to the copy's end, return first
// Leaf: ?erase@?$vector@JV?$allocator@J@std@@@std@@QAEPAJPAJ0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00440590, thiscall `ret 8`.
// Shape: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, __true_type()); _Destroy(__i, _M_finish); _M_finish = __i;
//   return __first;` -- __copy_ptrs on a POD range is the inline __copy_trivial (`last == first ? result : memmove(...) + n`), _Destroy of
//   long is empty.
// Name fix (agentNF94, q11238; descriptive-by-library: STLport instance, not in either PDB, which are Dinkumware builds): the landed callers
//   0x005815D6 (CPersistContext::TransferVector<long>), 0x005BEDCB and others bind ?erase@?$vector@JV?$allocator@J@std@@@std@@QAEPAJPAJ0@Z
//   (reloc_bindings), and the body is STLport's erase(first, last) for a POD element (above). The old row defined a free __fastcall
//   Buffer_Relocate (catalog name, a stand-in) on a local overlay; it is now the explicit instantiation over stlport/fable_containers.h.
#include "stlport/fable_containers.h"
template std::vector<long>::iterator std::vector<long>::erase(std::vector<long>::iterator, std::vector<long>::iterator);
