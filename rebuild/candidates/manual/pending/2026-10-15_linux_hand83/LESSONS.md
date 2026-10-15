# Chunk 83 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport deque `_M_pop_front_aux` / `_M_pop_back_aux` = an inline null-tested deallocate, an inline
  `_M_set_node(node +/- 1)` (node, first, last = first + 128/sizeof(T)), then `_M_cur = _M_first` (re-reads the member)
  or `_M_cur = _M_last - 1` (reuses the register); the element size only changes the `add eax,N` / `sub eax,N`
  constants, so one model covers the whole propagated "CTCCarryable::OnKill" family (0x0040F560, 0x009138B0,
  0x0059AD35 size TU with `pop ecx`, 0x00A0E620).
- A by-value C3DVector return as three alternating `mov edx/eax` dword copies through ecx = a MEMBERWISE user copy
  constructor over dword members; an implicit POD copy pulls in esi and float members go through the x87 (0x0075C1E0).
- A GetInterface component used as `this` of a virtual call and loaded straight into ecx (`mov ecx,[eax+4]`) needs the
  explicit three-way entry select `if (it == end) sel = end; else if (it->type <= type) sel = it; else sel = end;`;
  the out-parameter GetInterface and a `?:` select both leave it in eax and copy it (0x007EDB80).
- The out-parameter GetInterface template must be `__forceinline` when its caller also tests a global first
  (else VC7.1 emits it out of line) (0x0075D8F0); an out-pointer local of a later call is homed in a dead pointer
  argument slot (0x0075DC00).
- A linear `find`-like loop over a member vector must be a `__forceinline` member returning end() on a miss; the
  caller's `!= end()` then re-reads `last` (0x006A1060, 0x006A19B0).
- `if (!empty()) { pop_back(); if (!empty()) Apply(back()); return true; } return false;` keeps `last` in edx with
  `add edx,-4`; the early-`return false` form flips the blocks and gives `add dword ptr [ecx+10h],-4` (0x00771510).
- `ClearBoth` of two STLport vectors: `clear()` = `_M_finish = copy(_M_finish, _M_finish, _M_start)`; pointer
  elements go through `__copy_trivial` (`(last == first) ? result : memmove(...) + n`, the `cmp eax,eax ; jne` on the
  twice-loaded finish), an element with a user assignment through the counted `__copy` loop (`sar edx,3`) (0x00AA5BD0).
- Retail CThingAICreature sits 0x30 below the Ego_r.pdb offsets from PBrain onwards (PBrain +0x1a8, the bool
  bitfield +0x1c8, Suspenders +0x1cc) (0x00833BA0).
- Confirmed lane gap (GOTCHAS 24l): a retail 4-byte vector push_back passing ONE argument slot as both the value
  reference and the empty tag (`lea edx,[esp+0Ch]` then `lea edx,[esp+10h]` after one more push) does not reproduce
  for a converted rvalue, a by-value parameter, an lvalue parameter, nor through an inline `push_back(const T&)` front
  end over the by-value parameter (the 0x00747450 trick only covers find with a key read after the tag) -- five rows
  parked (0x005202B0, 0x006BC4A0, 0x00778B30, 0x007F7810, 0x007FEF40).

Review at landing:
- Rows that needed 4 grades (all parked): 006a1330 (IsSlotPrimarySlot), 009f0570 (CNameRegistry Contains). 0075cd90
  used 3; 006bc4a0 used 2 (one extra probe by the resuming agent); the other 24l rows 1 each.
- Local models next to existing engine headers: engine/CThingAICreature.h exists but follows the PDB offsets (0x30 too
  high for retail), so 00833ba0 models the class locally with the PDB names; 004d3d0e and 004d4f94 model the
  polymorphic base as a local CTCBase_Calls because engine/CTCBase.h is a layout-only POD (`void* __vftable`) -- the
  strict header check will flag these.
- Catalog rows with wrong (propagated) labels keep the real `Class::Name` on line 1 and pick the symbol with
  `// Leaf:` (see NOTES.md, Label corrections); finish.py derives module/name from line 1, so the payload carries the
  corrected names, not the catalog ones.
- No catalog row in this chunk holds several functions; no errors noticed in already-landed sources.
