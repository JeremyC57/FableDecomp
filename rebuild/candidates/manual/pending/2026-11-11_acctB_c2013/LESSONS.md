# Chunk 2013 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00597DF7) `mov ecx,[x] ; lea ebx,[eax+ecx]` after a call where VC7.1 gives `mov ebx,eax ; add ebx,[x]` is a 2-reference split
  piece of `x`: c2 drops a 1-reference piece (c2prio `drop .. refs 1 spill -2`). Retail wrote the sum at every use
  (`names.resize(max_slots + first_slot); times.resize(max_slots + first_slot); for (i = first_slot; i < max_slots + first_slot; ...)`)
  and c2 CSEs it; a named `count = max + first_slot` folds the add. A diagnostic second read of `x` (a global store) proved the lever.
- (0x00597DF7) A loop index offset written `f(i - first)` is strength-reduced to its own frame word zeroed AFTER the loop entry test;
  a separate `slot = 0` counter is zeroed before it.
- (0x00597DF7) `long x = 0; if (b) x = 1;` is retail's `and [x],0 ; cmp [b],0 ; je ; mov [x],1`; `x = b ? 1 : 0` gives setne and a
  different frame.
- (0x00595CC1) In a size TU the CIVCountedPointer destructor with Release()'s null store is called as a COMDAT; retail expands it and
  keeps the null store where the slot is live (0x00595F7C): one-level FABLE_FORCEINLINE body with the store inside the null test
  (overlay FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT_CLEAR).
- (0x00595CC1) `static_cast<CViewport*>(menu->FindChild(..))` tested afterwards gives `xor edi,edi ; cmp eax,edi`; test the CComponent*
  and cast at the call (retail `test eax,eax ; je`, 0x00596041).
- (0x00595CC1) A value used across an out-of-line call that retail keeps in a callee-saved register (`&FileRegionMiniMapNames[index]`
  in ebx, `&MiniMapGraphics` in edi around _M_find) is a named reference in the source; re-indexing reloads the vector start.
- (0x00595CC1) `if (valid && name != GetEmptySlotName())` adds a conditional-destruction flag (`mov [flag],2 ; test byte [flag],2`);
  nested ifs give retail's straight `mov bl,al ; ~CWideString ; test bl,bl`.
- (0x00595CC1) CArray::operator[] through begin() (FABLE_RETAIL_CARRAY) changed one register only: `mov edi,[edi] ; push [edi]`
  (retail) v `mov eax,[edi] ; push [eax]` with `_M_start[i]`.
- (0x00595CC1) fable_string.h can be overlaid as a whole file when every root-level includer in the TU is overlaid too (here
  fable_definition_table.h via engine/CDefString.h); check with `g++ -E` that only the overlay copy is included (else C2011).
- (0x00626110 / 0x00626750) CTCQuickAccessMenu speed TU: the four quick-access icon arms are one factor 2 too hot under a plain `for`
  (c2 inline trace: ctor c1 70 at f 2.5/1.25/0.625/0.3125, dtor c1 47); retail's split (ctor expanded only LEFT, dtor called only
  DOWN) comes with `/d2inlT59` (T 59..61) or with a folded `&& true` in the loop condition (body 2.5x). `while` (9x) and a guarded
  do-while (5x) do not change it.
- (0x00626110 / 0x00626750) The CAction constructor (0x0042BE50) is expanded in speed TUs after `new(10h)`: give the shared header its
  body (`CAction() : ActionType(UI_ACTION_TYPE_NONE), Valid(true) { }`, overlay FABLE_RETAIL_CACTION_INLINE_CTOR); the two list members
  default-construct through STLport's inline _List_base ctor (`mov [esi+4],0 ; malloc(0Ch) ; self-link`).
- (0x00626110) The QuickAccessMenu bodies are the landed CTCInventory::ConstructItemEntry 0x0058A0C7 statements inlined into the item
  loop: reuse that row's spellings (Reset of a default entry, `inventory->ItemsMenuEntries[def] = entry.Get()`, the else-if icon
  chain, `load_name->AddParam(entry.Get(), load_name)`).

Review at landing:
- All four rows need the hdr/ overlay (hdr/README.md); CFrontEndManager.h and CUserProfileManager.h build on chunk 2003's pending
  overlay copies, so merge chunk 2003's opt-ins first. fable_string.h gains the `FABLE_RETAIL_CWIDESTRING_NE` opt-in (whole-file
  overlay here, with an unchanged copy of fable_definition_table.h).
- 0x00626110 / 0x00626750 carry `// Flags: /d2inlT59` (Inline-budget device); the `&& true` loop form (probe/v_andtrue.cpp,
  probe/00626750.cpp) matches without a flag if the coordinator prefers the source device.
- The landed same-TU rows 0x00626E40 / 0x006276F0 were not regraded with the new CAction opt-in (they do not define it).
- reloc_bindings.tsv: 0x004291DE is bound as ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ by landed 0x00570332 although that
  row's comment and the header say ReleaseData (??1 is the jmp COMDAT 0x00429C53): respell the binding (NAME_FIXES.tsv).
- Landed 0x004091C0 is named LoadFileList (descriptive); the PDB name is GetSortedSaveNamesAndTimes (NAME_FIXES.tsv). Landed
  0x0047EEE4 binds 0x005CCC91 / 0x00A0D980 with wrong class / signature spellings (NAME_FIXES.tsv); 0x009CC410 has seven invented
  binder spellings.
