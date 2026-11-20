// NUISystem::CList::$E_GPreviousSelected() -- dynamic initializer of the static std::list<NUISystem::CList*> NUISystem::CList::GPreviousSelected: the inline STLport 4.6 list() constructor (malloc the 0xc sentinel node, link it to itself), then its atexit destructor is registered
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A6AC (unaligned: the CList TU is size-optimised and packed), 31 bytes, in
//   `.text$yc`, reached from the .CRT$XCU table entry 0x01374300, plain `ret`: `push 0Ch ; call malloc ; mov [eax],eax ;
//   mov [eax+4],eax ; mov [0x013B8A98],eax ; mov dword ptr [esp],0x01228E44 ; call _atexit ; pop ecx`.
// Identity (proven, two sources): Ego_r.pdb 0x0100DFCD / FableWin.pdb 0x03FE51B1 `dynamic initializer for
//   'NUISystem::CList::GPreviousSelected'', and the atexit pair 0x0101BC4A / 0x0400F312; FableWin.pdb's static accessor
//   ?GetPreviousSelected@CList@NUISystem@@SIAAV?$list@PAVCList@NUISystem@@... proves the type list<NUISystem::CList*>.
// agentNF187 (q4154): ported off its local NUISystem::CList / STLport model onto engine/NUISystem_CList.h (FABLE_RETAIL_CLIST) and
//   stlport/fable_containers.h. The compiler emits this initializer (`_$E1`) and the atexit destructor 0x01228E44 (`_$E2`) from the one
//   definition of the static; this source is the 0x01228E44 row's text (both rows land from the same form, so they define the same
//   GPreviousSelected symbol: the known LNK2005 of a static's initializer / atexit-destructor pair, EXTRA_RULES 2026-11-20).
//   ~list is declared only (FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY): the TU calls the landed out-of-line copy 0x0053C9BC.
// Data symbol: the header's default (public, 2V) spelling, which 16 landed rows bind; the PDB access is protected (1V): header claim
//   queued by NF156 / NF179 (flipping the default branch moves this row and 0x01228E44 together).
// Pragma: `#pragma optimize("s", on)` -- the retail TU is size-optimised: it reuses the malloc argument slot for the atexit argument
//   (`mov [esp],thunk ; ... ; pop ecx`).
#pragma optimize("s", on)
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY
#include "engine/NUISystem_CList.h"

std::list<NUISystem::CList*> NUISystem::CList::GPreviousSelected;   // 0x013B8A98
