// std::vector<float>::push_back(const float& x) -- store at `last` when there is room, else _M_insert_overflow @ 0x004408A0 for one element
// Leaf: ?push_back@?$vector@MV?$allocator@M@std@@@std@@QAEXABM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00568850, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[ecx+4] ; cmp eax,[ecx+8] ; je G ; test eax,eax ; je S ; mov edx,[esp+4] ; mov edx,[edx] ; mov [eax],edx ; S: add dword ptr [ecx+4],4 ; ret 4 ; G: push 1 ; push 1 ; lea edx,[esp+0Ch] ; push edx ; mov edx,[esp+10h] ; push edx ; push eax ; call InsertGrow ; ret 4
// Retail vector push_back for 4-byte scalar elements {first, last, end}: with spare capacity the
// value is placement-copied at `last` (hence the null test) and `last` advances; when full, the
// reallocating insert @ 0x004408A0 is called for one element with `noTail` set, its third
// argument the address of a 1-byte empty-tag temporary homed in the dead argument slot. `this`
// stays in ecx for the call. One of 13 bodies of this shape.
// Name (round 4, NF40): STLport vector<float>::push_back (descriptive-by-library: the PDBs are Dinkumware builds). The callee
//   0x004408A0 is vector<float>::_M_insert_overflow (Codex pass 22, both PDB owner records; queue entry 65), and all five landed
//   callers (0x0057E2F1, 0x007C4430, 0x00A59FD0, 0x00CBFB7D, 0x00D77340) already bind this spelling here. Was the stand-in
//   CRetailVector_dword::push_back over a local long vector; now the explicit instantiation of stlport/fable_containers.h's
//   push_back (its _M_insert_overflow declared, out of line as in retail).
#include "stlport/fable_containers.h"

template void std::vector<float, std::allocator<float> >::push_back(const float&);
