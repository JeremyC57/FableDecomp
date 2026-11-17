// std::vector<unsigned long>::push_back(const unsigned long&) -- placement-copy at the finish when there is room, else _M_insert_overflow for one element at the end
// Leaf: ?push_back@?$vector@KV?$allocator@K@std@@@std@@QAEXABK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005178A0, 50 bytes, thiscall `ret 4`.
// Retail bytes:  mov eax,[ecx+4] ; cmp eax,[ecx+8] ; je G ; test eax,eax ; je S ; mov edx,[esp+4] ; mov edx,[edx] ; mov [eax],edx ; S: add dword ptr [ecx+4],4 ; ret 4 ; G: push 1 ; push 1 ; lea edx,[esp+0Ch] ; push edx ; mov edx,[esp+10h] ; push edx ; push eax ; call _M_insert_overflow ; ret 4
// Identity (agentNF54 name fix; was the descriptive stand-in CRetailVector_dword::PushBack_dword_005178a0): STLport 4.6
//   vector<unsigned long>::push_back. Its growth path is 0x00464ACE, the landed vector<unsigned long>::_M_insert_overflow (POD,
//   __true_type), not the vector<long> one (0x00440990, called by vector<long>::push_back 0x00485FF3). Its retail callers push
//   through std::vector<unsigned long> references: CGameScriptThing::MsgGetThingsKilled 0x008D3DD0 (Ego_r.pdb 0x0090B290 + FableWin.pdb
//   0x02B05E00 ?MsgGetThingsKilled@CGameScriptThing@@UBE_NAAV?$vector@KV?$allocator@K@std@@@std@@@Z) and the NUISystem::CList scrolling
//   children (CComponent::ShapeChildren, Ego_r.pdb vector<unsigned long> at +0x104). FableWin.pdb carries this spelling
//   (0x01CA02D0). Not CArray<unsigned long>::push_back: a vector reference cannot call the CArray wrapper.
// Shape: the header's STLport push_back body, explicitly instantiated; the __true_type tag temporary (user constructor, never
//   zero-filled) lives in the dead argument slot.
#include "stlport/fable_containers.h"

template void std::vector<unsigned long>::push_back(const unsigned long& x);
