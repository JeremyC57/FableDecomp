// std::vector<CTexture>::push_back(const CTexture&) -- placement-copy at the finish when there is room, else _M_insert_overflow for one element at the end
// Leaf: ?push_back@?$vector@VCTexture@@V?$allocator@VCTexture@@@std@@@std@@QAEXABVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B6B0, 59 bytes, thiscall `ret 4`.
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call CTexture::CTexture(const CTexture&) ; S: add dword ptr [esi+4],0x8 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call _M_insert_overflow ; pop esi ; ret 4
// Identity (agentNF54 name fix; was the descriptive stand-in CRetailVector_CTexture::PushBack_CTexture_0049b6b0): STLport 4.6
//   vector<CTexture>::push_back, Ego_r.pdb 0x004A5E80 + FableWin.pdb 0x018B5770
//   ?push_back@?$vector@VCTexture@@V?$allocator@VCTexture@@@std@@@std@@QAEXABVCTexture@@@Z, the spelling its only retail caller
//   CProgressDisplay::InitialiseTextures 0x0049A0B0 already binds. 8-byte elements (engine/CTexture.h, out-of-line copy
//   constructor 0x009F9D00); the growth path is the out-of-line _M_insert_overflow 0x0049B480 for one element at the end, whose
//   __false_type tag temporary (user constructor, never zero-filled) lives in this function's dead argument slot.
// Shape: the header's STLport push_back body, explicitly instantiated.
#define FABLE_RETAIL_CTEXTURE
#include "engine/CTexture.h"
#include "stlport/fable_containers.h"

template void std::vector<CTexture>::push_back(const CTexture& x);
