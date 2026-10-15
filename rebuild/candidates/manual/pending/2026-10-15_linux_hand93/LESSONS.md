# Chunk 93 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `sqrtf(x*x + y*y)` computes `y*y` first: VC7.1 evaluates the right operand of a float `+` first, so the retail order of the two
  squares names the source order directly (0x005BCEA4 `y*y` first = `x*x + y*y`; its Z twin 0x005BCEF8 = `y*y + x*x`).
- A chained `Find(a, b, Key(x))->Method(x)` pre-pushes Method's arguments before evaluating Find's argument calls; retail's
  sibling `jmp` (or pushes that sit between the key call and the find call) need the find result / the key in a NAMED local
  (0x005BE8D3, 0x005BF3C6).
- A by-value-temporary assignment `out = Categories[i].GetName()` loads `Categories.first` before the hidden-pointer push only when
  `first` is a named pointer local; the accessor form adds it after the push (0x005BDB68).
- A vector `erase(first, last)` whose bounds are pushed through the vector's own register (`push [esi+4] ; mov ecx,esi ; push [esi]`)
  is an inline member (`clear() { erase(begin(), end()); }`); at the call site the lane addresses `[edi+0B0h]` from `this` (0x005BF91A).
- Two stores to the same slot in a row (`mov [eax],0 ; mov [eax],esi`) survive DSE when the slot is a union written through one
  reference (header pointer then free-list link) -- CHandleBasedMemoryPool free handle (0x00C28610).
- A persist `Transfer(name, value, default)` whose value address is the caller's own argument slot (`lea eax,[ebp+0Ch]`) transfers
  the PARAMETER by reference (`OnSerialise(ctx, bool inGameSave)`); the default temporary takes the high byte of the dead context
  slot (0x005B369A).
- SGI `__pop_heap` for an intelligent-pointer element: the speed TU (`mov eax,[esp+1Ch] ; push eax` for the comparator) with
  `__forceinline` copy constructor (base ctor, vtable store after the `lea` of the source, SetPItem(GetPItem())) and operator=; under
  "s" both are out-lined and the comparator becomes a pass-through push (0x008081B0, 0x00927450).
- A `return g_global;` of a class with an out-of-line copy constructor through a thiscall hidden result re-reads `[esp+4]` for the
  return value only under optimize("s"); the speed form parks the hidden pointer in esi (0x004E7D4D).

Review at landing:
- 005bd5db parked after 4 grades (register allocation; see NOTES).
- Every CTCInventoryBase row uses a local `CTCInventoryBase_Retail` view: no engine/CTCInventoryBase.h exists; the PDB layout shifts
  by 4 bytes per preceding std::vector (offsets listed in NOTES). The CTCInventoryClothing rows use a local view next to the existing
  engine/CTCInventoryClothing.h because the header's PDB offsets (ClothingMenuEntries +0x14C .. LoadedFromSaveGame +0x181) are
  0x18..0x24 above the retail ones (+0x134 .. +0x15D); 00c28610 uses a local view next to engine/CHandleBasedMemoryPool.h (retail
  reads the handle table at +0x440 and the free head at +0x44C; the header has Handles +0x438 (16 bytes) and FirstUnusedHandle +0x448).
  005bfc90 uses a local CItem copy of engine/CItem.h with the counted pointer's pointee typed (CPerItemData has no header).
- Landed sources with wrong/opaque identities spotted while reading relatives: rebuild/src/compiled/00/5b/
  CTCInventoryBase_GetItemDescription_005bf30d.cpp and _005bead5.cpp model 0x005D97B0 (GetInventoryCategory(long itemType),
  `__fastcall` on the item type) as a method of the CWideString out argument and 0x005C08C0 (SGI find_if over Categories) as
  opaque; CTCInventoryBase_RemoveItemFromInventory_005bc499.cpp is a `__declspec(naked)` asm transcription (asm_bake);
  CTCInventoryBase_GetSelectedCategory_007b8570.cpp and CTCInventoryBase_GetCategoryWithIndex_00af1280.cpp carry propagated labels
  (offsets +0x34 / +0x1E0 are not CTCInventoryBase members).
- Rows renamed from their catalog labels (line 1 + `// Leaf:`): 005bcef8, 005bdcdc, 005bdd8c, 005be8d3, 005bf3c6, 005bfa44, 0061a040,
  008081b0, 00927450, 009e2560, 00c21e20, 00c28610, 00c2c7b0, 004c6140, 004e7d4d, 005c2e2a (see NOTES "Label corrections").
