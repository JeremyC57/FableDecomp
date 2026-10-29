# Chunk 563 lessons

## GOTCHAS one-liners
- Inline CThing::GetTC: take `TCMap* map = &Components;` before LowerBound and compare `map->Data.end()`. Retail does `lea esi,[thing+44h]` and reads end at [esi+4]. Reading end off the thing pointer keeps the thing in a callee-saved register and hoists the key store (0x006C1150).
- A jump table that starts at 0 with entry 0 = the shared `return 0` block needs an explicit `case 0: return 0;`. A bare `case 0: break;` is dropped and the table is rebased with `dec eax` (0x0071BA60).
- `return b ? 1.0f : 0.0f;` keeps its 0.0f arm as a separate block. `if (b) return 1.0f; return 0.0f;` tail-merges it into the switch's shared `return 0.0f` (0x0071BA60).
- A named local versus a temporary as GFLimitReturn's value argument does not change VC7.1's stack slots. A named local only moves the 0.0f/1.0f temporary stores after the value's call (0x0071BA60).
- "~Class" catalog rows with no vptr store are often a different class's implicit dtor. Find the caller's vtable (`xref.py`, then `rtti.py`): 0x0071A430 is CActionTalkBase and 0x0071B120 is CTCEmoteIcon.
- Twin bodies that differ only in string literals (UI_ vs PC_MENU_ENTRY_*) are the X / XPC pair in Ego_r donor order. The catalog gives both the first name.
- CPersistTraits<T>::TransferBinaryIn exists in two FableWin.pdb forms (void with T&, or returning T). Pick the one matching the pushed argument count.
- engine/CList.h is a global `struct CList`, not NUISystem::CList. Do not include it for NUISystem code.

## Review at landing
- 0x0071BA60 is parked in differ/ (frame 0x14 v 0x10, only the stack-slot packing of case 11's temporaries differs).
- NAME_FIXES.tsv has 23 rows. The key ones are the four label corrections (0x005FAC20, 0x00685CC0, 0x0071A430, 0x0071B120), the reference/const/private access fixes (0x004C0000, 0x00661ED0, 0x00661E80, 0x0045ED25, 0x006C0FD0, 0x006BFB90, 0x006C0F00, 0x0071BA60), and ~CTCBase 0x00686830 / ~CThingCreatureBase 0x006661B0.
- Possible ICF-shared addresses with several bound spellings: 0x0070B8A0, 0x0070BA00, 0x00527910, 0x004378C0 (see NOTES.md).
- The implicit-dtor rows rely on the `// Leaf:` line to pick the dtor over the `EmitImplicitDestructor` helper.
