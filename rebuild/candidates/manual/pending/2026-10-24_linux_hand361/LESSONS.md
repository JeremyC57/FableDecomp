# Chunk 361 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **The heap and remove algorithms in retail are STLport, not Dinkumware.**
  - The catalog's `_Push_heap/_Sort_heap/_Adjust_heap/_Remove_if` names are Ego_r.pdb (Dinkumware) spellings.
  - Write `__push_heap` with `while (hole > top && comp(first[parent], val))`; `top < hole` swaps the cmp operands
    (0x00AE5AF0 and the other 24 matched `_Push_heap` rows).
  - Write `__adjust_heap` with the lone-child step through `secondChild - 1`; `bottom - 1` swaps eax/ecx (0x00777EE0).
  - Write `sort_heap` as `while (last - first > 1) pop_heap(first, last--, comp)` with no count test inside pop_heap.
    A Dinkumware pop_heap's own `1 < last - first` turns `and -4 ; cmp 4` into `sar ; cmp 1` (0x00777EE0).
- An out-of-line STLport `__pop_heap_aux(first, last, _Tp*, comp)` shows at the call site as `push 0` AFTER the
  by-value comparator copy. Dinkumware's `_Pop_heap_0` would push the null pointer first (0x0080B9F0 -> 0x0080B430).
- For 4-byte elements, retail inlines the whole `__pop_heap_aux -> __pop_heap -> __adjust_heap` chain into sort_heap.
  The lane only does this with `__forceinline` on each level; pair elements keep `__adjust_heap` out of line
  (0x00777EE0, 0x00A5A7D0 vs 0x004D15A0).
- **The value copy in sort_heap/push_heap shows the element's copy semantics:**
  - `fld`/`fstp` of a float key into the argument area means a memberwise user copy ctor (0x004D15A0).
  - A dword float copy plus member calls means std::pair's implicit copy (0x004D1600).
  - `rep movsd` means a POD (0x0067F040).
  - Two components through x87 plus dwords means C3DVector members with a user copy ctor (0x008182E0).
- An 8-byte pair<float,CCharString> frame local gives VC7.1's `and esp,-8` aligned frame (0x004D1600).
- `#pragma optimize("s", on)` gives `__push_heap`'s unrotated loop (`lea eax,[esi-1] ; jmp test`) (0x005F7D85, 0x00593965).
- Unsigned `__int64` keys compare the high word first (`ja done ; jb move`, then `jae done`). An 8-aligned
  pair<u64,X> copies its padding word only when the element is trivial (0x00A7D6F0 vs 0x00CB93E0).
- A pair whose second member is polymorphic:
  - The implicit operator= skips the vptr word.
  - The member's compiler-generated destructor stores no vtable and only calls the base dtor on the by-value argument
    (0x00769E20, 0x006A2E20).
- An intrusive CIVCountedPointer release written through a local pointer gives `dec [reg+4]`; through the member it
  gives load/dec/store (0x005E1310).
- When register pressure moves the key in `__push_heap`, two changes give retail's allocation:
  - Read the key into a local once and compare through an (entry, key) overload of the comparator.
  - Store the final value memberwise or through a named destination (0x006FB9C0, 0x00BEE420).
- **remove_if callers inline find_if and call remove_copy_if (or remove_copy) from the element after the hit.** The
  catalog `_Remove_if` rows are these copy halves, and the copy-half name is the correct label
  (0x00748D80, 0x00807B70, 0x00809D60, 0x0080DF90, 0x00737AD0, 0x0074CE40).
- `CFilter_Bind2::operator()` and an empty inline filter called through it both need `__forceinline` in the lane, or
  they are called out of line (0x00809D60).
- **Inline component lookups on CThing (0x00809D60):**
  - Write the inline HasTC as `if (bits[t >> 5] & (1UL << (t & 31))) return true; return false;` to get
    `test dword ptr [eax+28h],100000h`. A `!= 0` bool return gives `mov ; shr 14h ; test cl,1`.
  - In the GetTC ternary, compare against the by-value type (`type < it->first`) for the immediate `cmp [eax],54h`.
    Compare through the address-taken key local and the key is re-read from the frame.
  - Retail CThing layout used here: UsingComponents at +0x20 (Ego_r +0x20) and the Components CVectorMap at +0x44
    (Ego_r +0x48). Flag byte +0x91 bit 0 is Ego_r `Dead` at +0xA1, shifted by 0x10.
- A CTargetPoint copy through the pointer-register pattern (`mov ecx,edi ; mov eax,[ecx] ; mov edx,ebp ; mov [edx],eax`)
  needs C3DVector with a user copy ctor and NO operator=; a user operator= folds it into displaced stores (0x00807B70).
  This confirms the wave-25 chunk-185 note.

Review at landing:
- 00cbaba0 is parked (DIFFER 218v207).
  - Its best source splits the pair assignment: a `__forceinline` memberwise Assign in the loop, and the pair's
    operator= declared out of line (0x00CBA520, the landed SCountedRecord_00cba520) for the final store.
  - Check that split before promoting it; the next idea is a block-local ~CCountedPointer.
- 00809d60 uses a local CTCStockItem model, although `rebuild/include/engine/CTCStockItem.h` exists.
  - The header carries no `GetShopTC` member (0x007F4FC0). The layout agrees: Stealable +0xD.
  - A wrapper deriving from the header struct would satisfy the shared-header rule; it needs one regrade.
- Local CThing models (flag byte +0x91, UsingComponents +0x20, Components +0x44) are used in 00748d80, 00807b70 and
  00809d60. There is no engine CThing header.
- The 25 matched Push_heap rows (plus the parked 00cbaba0) and four Sort_heap rows (00777ee0, 00a5a7d0, 004d15a0, 004d1600) keep the catalog's
  identifiers (`_Push_heap`, `_Sort_heap`, `_Pop_heap_0`, `_Adjust_heap`; Leaf `_Push_heap`/`_Sort_heap`), while the
  comments say STLport. 0067f040, 0080b9f0 and 008182e0 use the STLport names (Leaf `sort_heap`).
  - Renaming changes only symbols. Pick one convention at landing.
- **Errors in already-landed sources:**
  - The eight landed `global_SortHeap*` rows (0x004CD130, 0x00680E00, 0x007423D0, 0x0088CEA0, 0x009B5BB0, 0x009B5C10,
    0x00A6B990, 0x00AE71A0) call their shape "Dinkumware sort_heap". It is STLport (same bytes); only the comment is wrong.
  - Callee labels to fix: 0x00776750 (`__push_heap<long*>`, labelled float), 0x00A586D0 (`__push_heap<float*>`,
    labelled pair<float,CCharString>), 0x0067E8F0 / 0x008175D0 (`__adjust_heap` over CDegradableInfo /
    THitLocationIntersectionElement, labelled CDebugBox), 0x006A7380 (IsInFOV(CTargetPoint) forwarder, labelled
    `_Cons_val<...EHeroMorphType...>`).
- In 00807b70, a thing with flag bit +0x91.0 ("Dead" in the landed CThingDeadCreature/CThingSwitch models) is KEPT by
  remove_if(CTargetPointIsNotEnemyOfThing): the predicate returns false. The bytes are unambiguous, but the meaning of
  the bit name is worth a second look.
- Comment-only edits after the main gall run: the STLport wording and the 0x00777EE0/0x00A5A7D0/0x004D15A0/0x004D1600
  byte counts. The four files were regraded (RELOCATION_MATCH); the others changed only `//` lines before the
  final gall run.
- No row needed more than 4 grades. 00777ee0 and 00809d60 matched on the 4th, and 00cbaba0 was parked after 4.
