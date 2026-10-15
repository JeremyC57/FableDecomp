# Chunk 69 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A key loaded once after the empty-list test and kept across `Compare` calls reproduces only as a local initialised
  inside an `if (node != head)` guard around a do/while (a local declared before the loop is scheduled above the test;
  reading inside the loop reloads it every iteration; 004af670, 004b3900).
- A `C3DVector(0,0,0)` float temporary passed by const reference puts its zero stores after the argument pushes; when
  another argument is a call, compute it into a named local first or the zero stores are hoisted above it (004af3f0).
- An inline empty base destructor re-stores the base vtable before the member-vector tail jump; when retail has no such
  store, model the base as `__declspec(novtable)` (0064a515).
- With a cached `last` plus a container pointer, declare `last` first for retail's edi/ebx assignment; put the found path
  in the `if` body so the `return false` / `return 0` tail lands last (004b0160, 004b27f0, 009ccdc0).
- The wave-24i inline `Set(r, g, b, a)` helper reproduced a CRGBColour byte copy first try (006caba0).
- New lane gap: the lane eliminates a function's tail call to itself (turns it into a loop); retail keeps the `jmp` (00bde080, parked).

Review at landing: parked 004b3810 is the wave 24l/24n tag-over-live-slot gap; 004afbb0 is register colouring only.
Label corrections (FableWin.pdb names from `ghidra_out/_fablewin_api.tsv` were the main evidence) are in NOTES.md:
00b48e10 is `CDynamicObjectHeap<56,16>::Release` (siblings 0x00B48C70 / 0x00B48D40 carry the same wrong label);
00c256a0 / 00c257e0 / 00c88990 are `C3DVector::DotProduct`, `C3DVector operator+`, an SSE four-float add.
