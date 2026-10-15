# Chunk 91 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A thiscall caller that does NOT reload ecx after calling a sibling method (retail `call A ; ... push ; call B`
  with no `mov ecx,esi` before B) is not a lane gap: VC7.1 tracks the registers a callee clobbers when the callee's
  body was compiled EARLIER IN THE SAME TU. Define the callee above the caller in the source (it must byte-match too)
  and select the caller with `// Leaf:` (0x0058509F OnItemAdded after GetQuickAccessNextFreeSlot 0x00582027).
- A `__fastcall` function that returns a struct with the hidden result pointer in ecx and its first argument in
  edx needs a NON-POD result type (declare a user copy constructor): a POD return makes VC7.1 push the hidden
  pointer, pass the argument in ecx and copy the result (`movsd` x3 + `movsw`). The 14-byte CEngineGraphic
  temporary then sits at ebp-0Eh (0x00581CFD/0x00581FA4 callee GetEngineGraphic 0x005D96A0; same for
  GetPDef 0x005D97D0 returning CDefPointer in 0x00581DDB).
- `long result = -1; for (...) { if (x == 0) { result = i; break; } if (match) break; } return result;` gives the
  `or eax,-1` before the loop and the shared exits; early `return`s give a different exit layout (0x00582027).
- An /Os TU does not inline a small in-class accessor that holds a branch (GetQuickAccessItem, 0x00581E33) into
  its caller; `__forceinline` it when retail inlines it (0x00581DDB). Same for the thing's GetInterface template
  (bitset test + LowerBound) inlined into ClosePCScoreboard / ClosePCMapMenu (0x00584AF6 / 0x00584FE3).
- An inlined `bool GetInterface(type, T** out)` whose result is ignored and whose out variable is used anyway
  reproduces retail's absent path reading the key's frame slot (`mov eax,[ebp-4]`): VC7.1 gives the never-written
  out variable the same slot as the lookup key (0x00584AF6).
- A 12-byte retail vector<long> `size()` is `(last - first)` as unsigned; `for (unsigned long i = 0; i < v.size(); ++i)`
  re-evaluates it each pass (`sub ; sar 2 ; cmp ; jb`) and VC strength-reduces `v[i] == x` to a walking pointer
  (0x00581D4E, 0x00581FD1).

Review at landing:
- Local base models next to existing engine headers: every CTCInventory source models `this` locally because
  engine/CTCInventory.h is the Ego_r.pdb layout with 16-byte vectors (retail +0x148/+0x154/+0x160/+0x1cc/+0x1d4 vs PDB
  +0x168/+0x178/+0x188/+0x204/+0x20c); CTCHeroStats-labelled rows use other classes entirely (propagated labels).
- 0058509F's source also defines GetQuickAccessNextFreeSlot (the byte-exact body of 0x00582027) -- the lander must pick
  the OnItemAdded symbol (`// Leaf: OnItemAdded`), and 0x00582027 lands from its own source.
- 00581AB3 (catalog "vfunc_59") holds two functions back to back (ProcessButtonBReleased 0x581ab3 and
  ProcessButtonXReleased 0x581ae1); the catalog row should be split before it can land.
- Three parked rows (00581CFD, 00581FA4, 00581D8E) differ only in how the element address is formed (`lea` fold vs
  `shl ecx,2 ; [ecx+eax]` / reloaded compare); one accessor form should land all three.
- The landed 0x00581BDE GetContextSensitivePreferredItemButtonIndex source models the static preferred-item list as
  a free `Entry*` pair (`g_itemButtonTable_first/last`); 0x005829CD shows it is the static vector 0x013B8B40 of 12-byte
  {item type, ?, button index} records with an out-of-line erase 0x0059310D -- worth a shared model.
- No row needed more than 4 grades.
