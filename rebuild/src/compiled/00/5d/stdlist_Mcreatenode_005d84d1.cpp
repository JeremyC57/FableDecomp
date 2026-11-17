// std::list<std::pair<CWideString, long> >::_M_create_node(const value_type&) -- malloc a 0x10-byte node and copy-construct the 8-byte value at +0x8 in place
// Leaf: ?_M_create_node@?$list@U?$pair@VCWideString@@J@std@@V?$allocator@U?$pair@VCWideString@@J@std@@@2@@std@@IAEPAU?$_List_node@U?$pair@VCWideString@@J@std@@@2@ABU?$pair@VCWideString@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D84D1, 33 bytes, size-optimised TU, thiscall `ret 4` (`this` unused).
// Retail bytes:  push esi ; push 0x10 ; call malloc ; mov esi,eax ; pop ecx ; lea ecx,[esi+0x8] ; test ecx,ecx ; je S ; push [esp+8] ;
//   call T::T(const T&) ; S: mov eax,esi ; pop esi ; ret 4
// Identity (NF56, round 7, queue q10468): STLport 4.6 list<pair<CWideString, long> >::_M_create_node (protected, stl/_list.h: allocate one
//   node, placement-copy the value -- the null test on the value address), the node factory its only landed caller, list::insert
//   0x005D8626 (thiscall, hidden iterator result, `ret 0Ch`), calls with `this` passed through. NF54 had moved the value onto
//   std::pair<CWideString, long> (copy constructor 0x005B12F4) but kept the made-up owner CRetailList_node0x10::Buynode; the queue
//   entry carried that stand-in spelling, replaced here by the STLport member of the shared stlport/fable_containers.h list (REJECTED.tsv).
#pragma optimize("s",on)
#include "fable_string.h"
#include "stlport/fable_containers.h"

template std::_List_node<std::pair<CWideString, long> >* std::list<std::pair<CWideString, long> >::_M_create_node(const std::pair<CWideString, long>& x);
