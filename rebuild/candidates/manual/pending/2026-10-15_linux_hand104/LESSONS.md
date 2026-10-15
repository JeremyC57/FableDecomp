# Chunk 104 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `or byte ptr [reg+N],imm` (a read-modify-write flag set straight on memory) is a size TU: the speed TU emits
  `mov cl,[..] ; or cl,imm ; mov [..],cl` for both a plain `|=` and a bit-field store; `optimize("s",on)` also fixed
  the eax/edi colouring of the rest of the body (0x00787D4F).
- `std::for_each(begin(), end(), CFunctor(arg))` from a /Gr module: `__fastcall` with the hidden functor result in ecx
  pointing at the caller's dead argument slot (`lea ecx,[esp+10h]`), the iterators built in reserved slots
  (`push ecx ; mov eax,esp ; mov [eax],edx`) need a user copy constructor, the 1-byte functor only a user constructor
  (its temporary is homed in the argument slot and pushed as a dword) (0x0071B2A0 / 0x0071B2D0).
- An erase loop over the retail one-pointer list destroys the element with `push 0 ; call [vt]` = an explicit
  `n->value.~T()` with a virtual destructor, followed by `free(node)`; no size update exists (0x0071B240).
- Two add-ref'd CDefPointer temporaries from an inline by-value `GetPDef() const` (one per argument) reproduce
  retail's "first release has no null test" shape in the lane too; under `/Os` the copy constructor, destructor and
  GetPDef must be `__forceinline` (else out-of-line calls and an ebp frame) (0x00658ACB / 0x00658C5B).
- A list walk `for (n = head->next; n != head; n = n->next)` entered with `jmp` to the bottom test is `/Os`; the speed
  TU duplicates the entry test (0x00658A02, 0x00659C32); the list `size()` of the size-less retail list is the inline
  distance walk, and a byte store scheduled between the walk and its test means the count is a named local (0x00659C32).
- A walk that may erase the current node with retail's `mov ebx,esi` before the entry test and `mov esi,ebx` at the
  bottom is `next = it; while (it != end) { ++next; f(*it); it = next; }`; the `cur = it++` form keeps `this` in ebx
  instead of ebp (0x0065CE56).
- A run of thiscall calls whose argument addresses all go through eax (`lea eax,[esi+N] ; push eax`) is `/Os`; the
  speed TU rotates eax/ecx/edx (0x00659EA9).
- `CVectorMap::Sort()` inline = `if (NeedsSort) { sort(first, last, Compare); NeedsSort = false; }` with the /Gr sort
  taking first/last in ecx/edx and the 1-byte comparator by value (`xor eax,eax ; mov al,[map+0Ch] ; push eax`); the
  dirty flag at map+0x0D (0x009CFB70, 0x00AE34D0).
- An unoptimised list `begin()` (`this` at [ebp-8], node at [ebp-4], hidden result at [ebp+8]) is
  `optimize("", off)` + `Node* first = m_pHead->m_pNext; return iterator(first);` with an out-of-line iterator
  constructor (0x009F7910).
- An explicit HasTC test `&&` the inline GetInterface template loads the bitset word once and tests it twice with the
  mask in a register (`mov ecx,10000h ; test ecx,edx` x2) (0x006DDCD0).
- `return u64 != 0;` after an early `return true` gives `mov eax,lo ; or eax,hi ; je` and full-register `mov eax,1 /
  xor eax,eax` arms while the early return stays `mov al,1` (0x007EE240).
- A vtable's slot-6 entry in CTC classes is OnInitialActivate, slot 4 OnCreate, slot 1 OnSerialise(CPersistContext&, bool)
  (confirmed again on CTCTurncoat, CTCVillage, CTCTrophy, CTCVillageMember).

Review at landing:
- Rows that needed more than 4 grades: none (three rows parked at 4: 0x006DEE80, 0x00659295, 0x007EE370).
- Local models next to an existing engine header: CTCTrapBase.h, CTCTurncoat.h, CTCTurncoatSpell.h and CTCVillage.h are
  PDB layouts that disagree with retail (12-byte vectors, 4-byte lists, 12-byte set): every CTCTrapBase/CTCTurncoat*/
  CTCVillage source uses a local model with the retail offsets stated in its header comment. CBankFile.h (PDB) was not
  used for 0x009CFB70 for the same reason (SymbolCRCIndices +0xEC -> retail +0xD0, ReadOnly +0x9D -> +0x8D).
- Already-landed sources with wrong names: rebuild/src/compiled/00/4d/global_CreateCTCVolumeContainmentTracker_004d4548.cpp
  is the CTCHighlightItem factory (it calls 0x0077C920, whose vtable 0x0126AF14 is CTCHighlightItem; 0x28 bytes =
  sizeof CTCHighlightItem); rebuild/src/compiled/00/84/CCreatureActionOrderSecuritySweep_PerformAction_00849250.cpp
  declares 0x00659C32 as `RecalculateSecuritySweep(long)` -- it is `StartSecuritySweep(bool)`.
- Renamed rows (line-1 names differ from the catalog; each source carries `// Leaf:`): 0059b039 back, 0059b576
  _M_push_back_aux_v, 005c2e68 CTCInventoryWeapons::RemoveAllConfiscatedItems, 0065f01d CTCDummyVillager::Evict,
  00658c5b GetTavernTimes, 00659c32 StartSecuritySweep, 0065ce56 RemoveApologiseableCrimes, 0065ce88/007c7840
  OnInitialActivate, 006ddbf0 OnSerialise, 009cfb70 CBankFile::PostInitEntries (name inferred, verify), 00ae34d0
  descriptive class, 009f7910/009f7940 descriptive list class, 0077c920 ??0CTCHighlightItem. The module column in
  payload.json follows line 1, not the catalog.
- 007898a3 keeps the catalog class CTCTrapBase although it is the obstruction component's method (ImmuneThings +0x2C).
- Catalog row 004a1c8e is not a function entry (catch-continuation tail of the EH function around 0x004A1C41); drop it
  from the manifest/catalog.
