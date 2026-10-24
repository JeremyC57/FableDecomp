# Chunk 325 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A function that returns a sub-list built only for non-empty input should be written as
  `CList* subList = 0; if (!items.empty()) { ...; } return subList;`, not with an early `return 0`. VC keeps the null
  in edi, where it doubles as the `false` argument until the sub-list replaces it, and the block-scoped prompts'
  destructors are skipped on the empty path. 005b70b8 matched on the first grade this way.
- A struct returned by value from a `__fastcall` helper needs a user-declared constructor if retail passes the
  hidden-return pointer in ecx (`lea ecx,[tmp] ; mov edx,key ; call`). For a POD the pointer goes on the stack and
  VC adds a 16-byte copy temp. Example: CEngineGraphic from GetEngineGraphic 0x5D96A0, the Ego_r 14-byte struct under
  `#pragma pack(2)` (005f077f).
- `map[key] = value` where retail loads the value into a temp before the operator[] call and stores it after
  (`mov eax,[val] ; mov [tmp],eax ; push &key ; call ; mov ecx,[tmp] ; mov [eax],ecx`) is an inline
  `Set(const long& key, T* value) { (*this)[key] = value; }` helper. A direct `map[key] = value` loads the value
  after the call (005f077f).
- The opposite residue still has no fix in this lane. When retail loads the parameter / vtable for the next call
  BEFORE the store through operator[]'s reference, ours stores first, because the key's address escaped
  (`ItemSellLists[parent] = list; parent->AddChild(...)`). Tried: /Oa, /Ow, const param, key type, by-value Set, rvalue
  key, a copied `target` local. Rows: 005a3e47, 005a45f6, 005a731f, 005a79e6, 0058a0c7. Park these after one try.
- `switch` with `case 4:` and `default:` as separate, identical arms leaves retail's dead `dec eax` before the merged
  jump. Putting both labels on one arm drops it (00590235).
- A biased-frame (`lea ebp,[esp-N]`) builder uses `#pragma optimize("s", on)` alone. Frame-pointer builders
  (`push ebp ; mov ebp,esp`) use `optimize("y", off)` + `("s", on)`. Twins in the same class can differ.
  005b70b8 (biased) and 005b6a66 (plain) are twins.
- A clamp written as `long max = tab->GetMaxBuyable(i); long q = count; if (max <= count) q = max;` gives retail's
  `cmp eax,ecx ; mov [q],ecx ; jg`. STLport `min(a, b)` by const reference gives a `lea`-select
  (`lea eax,[ebp+x] ; jl ; lea eax,[ebp+y]`) instead (005a3e47).
- In a function-scope null-initialised pointer pair (`T* a = 0; ... GetInterface(&a); U* b = 0;`), put the declaration
  of the first one before the hero lookup. Retail schedules that zero store between the PlayerManager load and the
  GetMainPlayer call (005a3e47).
- The manifest's `CTCInventoryItem` static helpers around 0x5D95C0..0x5D9B50 carry propagated labels; most read
  "IsAugmentation" or "GetInventoryCategory". Identify them by their GetSubDef target and its def-class string:
  0x5DA240 is CExpressionSubDef (IsExpression 0x5D9830), 0x5DA3C0 is CCarriedReadableDef (IsBook 0x5D9970). The run
  0x5D95C0 / 0x5D9630 / 0x5D96A0 / 0x5D96F0 follows FableWin's GetDescription / GetDetails / GetEngineGraphic /
  GetUIInventoryCategory order.

Review at landing:
- 005f077f was parked at the 4-grade cap with the statement shapes already matching; the remaining residue is
  register/slot permutation. Worth a fifth look: swap the `==` operands and drop the named GetInterface key.
- All matched sources use local class models next to existing engine headers (CTCInventory.h, CTCInventoryClothing.h,
  CTCInventoryTrade.h, CTCInventoryAbilities.h). Those headers carry the flattened Ego_r.pdb layout, which disagrees
  with retail offsets (for example CTCInventoryTrade ItemBuyLists +0x200 retail vs +0x248 PDB, and CTCInventory
  ItemsMenuEntries +0x1FC). Each source says so in its Layout comment.
- 0x005D96F0 is bound under three names in already-landed code: GetInventoryCategory (global_GetInventoryCategory_005d96f0),
  GetInventoryTabState (several callers) and here GetUIInventoryCategory. Choose one at landing (RELOC_CONFLICT risk).
- 0x005D9630 is bound here as GetItemDefDescription (consistent with this chunk's clothing rows); its FableWin name is
  CTCInventoryItem::GetDetails.
- 17 rows were not graded (time box). Their STATUS rows read `DIFFER -v<len>` with "not graded" and they have no
  differ/ source.
