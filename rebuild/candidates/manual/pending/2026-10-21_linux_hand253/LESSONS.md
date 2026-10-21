# Chunk 253 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Ternary GetInterface: write `*ppOut = (T*)(it == map->last || type < it->type ? map->last : it)->component;`. VC7.1 then loads
  the component straight into ecx and stops renaming this/ebp. The if-reassign form puts the component through eax. Fixed
  008039a0, 00800540 and 0072cba0; 005ec0f0 uses both forms, one per lookup.
- Inline `C3DVector::Zero() { X = Y = Z = 0.0f; }` (stores Z,Y,X) plus a member `operator+=` keeps the accumulated vector's
  address in edi across the function (00759d30 ComputeForces).
- STLport `clear()` (= erase(begin, end)) codegen depends on the element type:
  - POD element: the `CopyTrivial` ternary with memmove, which shows as `cmp eax,eax`;
  - C3DVector element: the element-copy loop `for (n = last - first; n > 0; --n)` (0075ad50).
- A local with an inline ctor declared BETWEEN two float statements splits `fst` into `fstp ; fld`. In 008e5480 that local is the
  CSurfaceHit record with Fraction = 1.
- A member sub-object with its own inline ctor (a map inside the allocated manager) orders the vtable store BEFORE the member
  init. Folding the init into the owner's ctor orders it after (00786700). Tell-tale: a 355v355 DIFFER where udiff shows only
  relocation lines. Compare the raw bytes: a `push 0x1c` had swapped with the vtable store.
- Counted-pointer count block: write it as a `new SCountBlock(1, DeleteData, object)` expression with a `__fastcall` deleter.
  The CCountedPointer dtor nulls both words (00786700).
- Wrapping a body in `if (!flag) { ... } return false;` puts the `xor al,al` arm last. The guard-return form puts it first (0072cba0).
- Argument temporaries vs named locals (00800540, 0072cba0):
  - a class temporary passed directly to a call is built in place, and the ctor's eax is pushed;
  - a named local in a block is pushed with `lea`.
- A named sum computed before a call (`C3DVector p = v + pos; f(i, p, -v);`) moves the negation after the sum. Inline sums in
  the argument list evaluate the third argument first (0079c290).
- A test stand-in that returns a class by value needs a counting copy ctor. VC7.1 /Od does not elide `return local;`, so the
  local's dtor runs twice (test 00772250 passed under g++ and failed under Wine until fixed).
- The `const T& x = e` load-order idiom did not help on these pure register-allocation rows: 00773950, 007f5150, 00744b50 (it
  moves the call order, not the register choice), and 007ff190.

Review at landing:
- 006cf040 is CTCWife::CTCWife (vtable 0x0125ED5C), not the catalog's CTCOpinionSource::CTCOpinionSource. Line 1 and the
  leaf `??0CTCWife` use CTCWife. It uses a local model, because retail STLport trees are 12 bytes against the PDB's 16.
- 007d0850 and 007d6f70 have swapped catalog names: ShoveHaPenny is 0x7D0850 and CardPairs is 0x7D6F70.
  - 007d2a00 is GetBestScore (catalog: vfunc_43).
  - 008039a0 is IsTargetedThingOfTypeInRange (catalog: vfunc_38).
  - The leaves still grade under the catalog selector.
- Local models next to existing engine headers, each with a // Layout: comment:
  - offsets are PDB minus 4 per 12-byte release STLport vector or tree;
  - affected: CTCOracleMinigame (+0xe8 PDef), CTCTargetingPlayer (positions -8), CTCShotDefinitionBase (PDB-4),
    CTCPhysicsRigidBody, CTCPhysicsStandard, CTCPerceiveThing;
  - CTCSpecialAbilities::Creature sits at +0x18.
- Rows that needed more than 4 grades: 008039a0 (5 probes), 0072cba0 (6), 00786700 (4), 008f20f0 (8).
- Catalog rows holding several functions: 005e3ea0 (two), 007f4e10 (four). 005eadd6 is a fragment of landed 0x005EADC0.
- 00971ed0's catalog name (CTCOwnedEntity::OnForceRecreateUID) looks wrong for a spawn-point picker. Check the vtable owner.
