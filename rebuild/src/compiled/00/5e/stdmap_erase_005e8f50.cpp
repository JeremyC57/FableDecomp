// std::map<long, unsigned long>::erase(iterator position) -- STLport 4.6 _tree.h erase(iterator) through the map: _Rebalance_for_erase (0x0042A34E)
// on the header's parent / leftmost / rightmost, free the unlinked node, drop the count
// Leaf: ?erase@?$map@JKU?$less@J@std@@V?$allocator@U?$pair@$$CBJK@std@@@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBJK@std@@U?$_Nonconst_traits@U?$pair@$$CBJK@std@@@2@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E8F50, 50 bytes, thiscall, `ret 4`.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; lea ecx,[eax+0xc] ; push ecx ; lea edx,[eax+0x8] ; push edx ; add eax,0x4 ; push eax ; mov eax,[esp+14h] ; push eax ; call 0x0042A34E ; add esp,10h ; test eax,eax ; je K ; push eax ; call free ; add esp,4 ; K: dec dword ptr [esi+4] ; pop esi ; ret 4
// NF39 (2026-11-16): moved off its `CRemovalTable_005e8f50::Remove_005e8f50` stand-in (a generated shape model; catalog label propagated) onto the
//   shared stlport/fable_map.h map (opt-ins FABLE_RETAIL_STLP_MAP / _TREE_ERASE / _TREE_ERASE_BODY: erase(iterator) with its body visible, the
//   landed ?_Rebalance_for_erase@?$_Rb_global@_N@std@@... 0x0042A34E). Identity: its one retail call site, 0x005E6937 in the landed
//   0x005E5F30 (xref.py), already binds map<long, unsigned long>::erase(iterator); the body is that function exactly (no key, the iterator's node).
// Model: explicit instantiation emits the member as its own COMDAT, as retail kept every used inline member.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

template void std::map<long, unsigned long>::erase(std::map<long, unsigned long>::iterator);
