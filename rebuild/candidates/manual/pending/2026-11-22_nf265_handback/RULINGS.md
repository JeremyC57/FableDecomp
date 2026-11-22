# Coordinator rulings on NF265 (2026-11-22): the L129 DeleteTexture unit, blocked on the CEngineLandscapeMemoryManager trio family

Nothing landed: the three graded drafts (0x00BDA110, 0x00BE5C30, 0x00BE6090) and the five add-only header opt-ins (`held/`) would make
`?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z` (and its IB / VB siblings) name two different functions: landed
0x00BDA150 (ProcessDisposalList) and 0x00BD9CE0 (FreeDisposalList) strongly define same-TU helpers under the public names with the
destroy-now bodies (measured: the HEAD 0x00BDA150 object + the 0x00BDA110 draft give LNK4006 on DeleteTexture). No relocation, pk.py,
three-object link or table line could see it; found by a census on the new symbol names.
- **D1, family scope:** one unit = the in-place respell of 0x00BDA150's / 0x00BD9CE0's helpers onto the private `Delete<X>BlockWhileBusy`
  names, the self renames of 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110 onto the public `DeleteIndexBuffer` / `DeleteVertexBuffer` / `DeleteTexture`,
  and the binders 0x00BE5C30 / 0x00BE6090. The six other binders of 0x00BDA090 / 0x00BDA0D0 (0x00BFD480, 0x00BEAA00, 0x00BEAAA0, 0x00BEB4D0,
  0x00BFDD40, 0x00BFDE20) may keep their unrelated stand-ins for now: count-reducing at every address, no new split, as long as a census on
  every new symbol NAME (not only address) shows each name defined by exactly one body after the unit.
- **D2, the trio identities:** accepted as the working claim (retail FreeVirtual -> 0x00BD9C00 -> 0x00BD9C20 -> AllocVirtualAligned ->
  FreeVirtualAligned -> 0x00BD9CC0 runs in the order of Ego_r's six functions; both name sets list all six; every catalogued caller of the
  queue trio is a member of another class). The family pass re-proves it from a second angle before landing (the private trio's callers are
  the disposal-list members; the public trio's are other classes) and corrects the landed "proven" Identity lines of 0x00BDA110 / 0x00BD9CE0.
- **D3:** the self rows are ported onto the header (the 0x00BDA110 draft form), not respelled in place: shared headers are the rule.
Next: agent NF267 runs the family pass from `held/`; NF265's locks pass to it.
