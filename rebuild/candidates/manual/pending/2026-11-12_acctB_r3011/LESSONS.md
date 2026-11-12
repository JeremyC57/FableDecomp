# Chunk 3011 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00915B60) `if (!p.Get()) return false;` before a separate `if (p->GetTC(&x))` gives retail's head merge (the next test's
  `lea ecx,[p]` hoisted above the `je` that jumps past the final block's own `lea`, 0x00915BFC); `p.Get() && p->GetTC(&x)` does not.
- (0x00915B60) c2 pads a loop head (`jmp` over `lea esp,[esp]`) only when few bytes are needed; a missing padded head right after a
  6-byte shorter instruction is a size consequence, not a loop spelling (0x00916047).
- (0x00915B60) c2 hoists the load of a by-value vector's `_M_start` (a "param" candidate) to the immediate dominator of a block whose
  destructor read is partially redundant (a `free; return false` bottom reached from the index use's failure AND another path); reads in
  other return blocks are not part of that web (fragment probes probe/f/pre1-3).
- (tool) c2.dll 0x10784B48: the chooser adds `100 * spill(nb)` to COSTS[reg] for every uncoloured neighbour left with one register reg;
  hooking it (tools/nb, nb.sh) names the neighbours behind a `costs: {ebx: 1000, ebp: 1400}` line; zeroing one at 0x10784B4B
  (c2prio_w.py, env C2_ZERO) is a cheap what-if that proves which decision a residue hangs on.

- (0x00915B60) Which failure paths share the final `return false` block decides where c2 hoists a by-value vector's `_M_start` load
  (the immediate dominator of the partially redundant destructor read) and therefore which allocations its live range blocks:
  letting the PerformAction failure free the action point inside the pre-route `if` and fall out of the enclosing block into the
  function's final `return false` (shared with the GetTC failure) moved the hoist from the pre-route test to the GetTC test, gave
  crate_index ebp (0x00916152) and split the vector start into {index use, success free} ebx (0x009161E9 / 0x00916254) + memory.
- (0x00915B60) c2's frame-slot sort (0x107AE0C4) is a K&R quicksort (pivot = middle, swapped left) on density w*1000/size with ties
  returning 0, so equal-density slots land in an order fixed by the presort array; one extra IL reference (an explicit return copy
  that cross-jumps away still counts) breaks the tie: `else if (!goods.Get()) return false; else free;` put the 8-byte goods ahead
  of two 4-byte words of the same density (goods [esp+14h]). Replay: tools/fs/qs.py on a pk.sh trace.
- (tool) A trace's block addresses (ref events) show block creation chunks; when the split pass seeds a later-created block first,
  an earlier reference is left alone and dropped (w2: {succ} seeded before the index use).

Review at landing:
- 0x00915B60 MATCHED (RELOCATION_MATCH) after the predecessor's hand-over; no header changed in this retry (hdr/ = chunk 2029 overlay).
- Manifest label of 0x007FEF40 ('CTCUserActionPointBase::FreeActionPoint') is wrong: PDB name SetActionPointAsFree (vland WARN RELOC_NAME).
