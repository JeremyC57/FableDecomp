# Chunk 71 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Reach a list's head sentinel through an inline accessor at EVERY use (`GetHead()->PrevResource`, then `GetHead()`);
  a cached `CResource* head` local makes the lane form the head address first and hold the link in esi.
- Retail `inc`/`add` on memory needs `#pragma optimize("s", on)`; under "s" an inlined list method must be
  `__forceinline` or it stays out of line.
- Copy constructors keep the base vtable store (the source reads after it may alias `this`), written-out and implicit
  alike; a default constructor drops it.
- VC7.1 emits the implicit copy constructor out of line, so a placement-new helper gets the symbol into the TU; the
  lane refuses to inline an implicit destructor into the deleting destructor where retail did (0x009FBD00, parked).
- `member = C2DVector(0, 0)` in the constructor body = an 8-byte frame temporary; `m(C2DVector(0, 0))` folds to zero stores.
- A record cleared with an intrinsic `memset` gets its own `xor` and a copied address register per expansion;
  init-list zeroing shares one zero register across records.
- x87 operand order: VC7.1 does not keep the source order of `*`; GetRight (0x0049CE00) matched only through an inline
  `CrossProduct(a, b)` helper taking references.
- A "constructor" whose eax is not `this` at return is not a constructor: 0x004BBE10 is `CRightHandedSet::Set`.

Review at landing: 0x009FBFE0 (CResourceList::AddResource) matched on a FIFTH grade (over the 4-attempt rule) after
the agent saw the lane emit an identical out-of-line body under `optimize("s")` while grading its sibling 0x009FC380;
the lander's re-grade is the check. Parked 0x009FC4A0 (SetAsMRU): try the `GetHead()` accessor form that fixed
0x009FBFE0 first. Label note: 0x009FC450 is SetAsLRU and 0x009FC4A0 the real SetAsMRU. Label corrections in NOTES.md.
