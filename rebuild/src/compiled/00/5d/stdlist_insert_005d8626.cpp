// std::list<std::pair<CWideString, long> >::insert(iterator position, const value_type& x) -- create the node and link it in front of position; return its iterator
// Leaf: ?insert@?$list@U?$pair@VCWideString@@J@std@@V?$allocator@U?$pair@VCWideString@@J@std@@@2@@std@@QAE?AU?$_List_iterator@U?$pair@VCWideString@@J@std@@U?$_Nonconst_traits@U?$pair@VCWideString@@J@std@@@2@@2@U32@ABU?$pair@VCWideString@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D8626, 37 bytes, size-optimised TU, thiscall `ret 0Ch` (hidden iterator result, position, &x).
// Retail bytes:  push [esp+0Ch] ; call 0x005D84D1 ; mov ecx,[esp+8] ; mov edx,[ecx+4] ; mov [eax],ecx ; mov [eax+4],edx ; mov [edx],eax ;
//   mov [ecx+4],eax ; mov ecx,[esp+4] ; mov [ecx],eax ; mov eax,ecx ; ret 0Ch
// Name fix (NF56, round 7, queue q10468): the node factory call 0x005D84D1 binds STLport's list<pair<CWideString, long> >::_M_create_node
//   (the respelled self of 0x005D84D1). The July row was a __stdcall free stand-in (ListNode_InsertBefore_005d8626 over a local node);
//   the body is STLport 4.6 list::insert(iterator, const T&) (stl/_list.h) of the shared stlport/fable_containers.h, explicitly
//   instantiated: `this` passes straight through to _M_create_node.
#pragma optimize("s",on)
#include "fable_string.h"
#include "stlport/fable_containers.h"

template std::list<std::pair<CWideString, long> >::iterator std::list<std::pair<CWideString, long> >::insert(std::list<std::pair<CWideString, long> >::iterator position, const std::pair<CWideString, long>& x);
