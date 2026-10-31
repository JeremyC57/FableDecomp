# Chunk 647 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Three float projections that retail keeps in contiguous frame slots are one C3DVector local** (0x007E8A40).
  - The source was `C3DVector local(forward.Dot(d), side.Dot(d), up.Dot(d)); ... d = forward * local.X + side * local.Y + up * local.Z;`.
  - It gave retail's 0x3C frame (forward 0..0xB, projections 0xC..0x17 with Y kept on the x87 stack, side 0x18.., sums above).
  - Three named floats gave 0x34, with the first projection homed in the dead argument slot.
  - The x87 operand order of the dots then changes, and no Dot/operator spelling moves it back.
- **x87 dot-product order changes with the result's home, not with the spelling** (0x007E8A40).
  - The same `forward.Dot(delta)` sums Y,Z,X when its result goes to a float local in the argument slot, and Z,Y,X when it goes to a C3DVector member.
  - `delta.Dot(v)`, `f * X` and member-wise stores were identical.
- **A store to a local struct member is placed by the scheduler, not by the source** (0x005A887D).
  - `deletion.Method = 3` landed in the same slot whether it was written before push_back, after a named end(), or as a comma-this on a direct insert() (where `this` is evaluated last).
  - Park such single-store residues after two probes.
- **Twin functions can show a scheduler tie-break mirrored** (0x0047D296 / 0x0047DD9D).
  - Both sources are identical apart from one member store. Retail 0x0047D296 loads the vtable before the observer global in its third AddModalObserver block, and retail 0x0047DD9D does not.
  - Our build does the opposite in each.
  - Neither the extern order of the globals nor a same-TU preceding function flipped it. Record both swaps when parking this family.
- **The positive nested-if form moves an address-taken out-local into a different spill slot** (0x0062BEF0).
  - A body of early returns shared the camera out-local with the `this` spill. Writing the body as nested positive ifs gave the camera retail's slot (the ranged-pointer spill).
  - This is worth trying whenever a parked row's only frame residue is an address-taken local in the wrong dead slot.
- PDB respelling alone (parameter kinds, class keys, access, real callee names) changed no bytes in 5 of 5 rows here (0x00B396E0, 0x00687540, 0x007E8A40, 0x0062BEF0, 0x00A90260). Do it for the names; do not count it as a codegen lever.

Review at landing:
- No row matched. Grades this wave (one lever each; parallel probes counted, plus one reproduction grade per row):
  - over the 4-attempt rule: 007e8a40 7, 0047d296 4 + 1 access/leaf regrade
  - 005a887d 4 + 1, 0062bef0 4 + 1, 00687540 3, 006d68d0 2, 006d6cb0 2, 0047dd9d 2, 00a90260 2 + 1, 00b396e0 2 + 1
- 0x007E8A40 (18 masked lines, frame fixed) and 0x0062BEF0 (probe/p6_nested.cpp) are the closest. Retry them first.
- The landed 0x006D6770 GetTrainableStatMaxLevel TU declares `GetFrame` on a `CWorld_Calls` struct, so its binding mangles as `?GetFrame@CWorld_Calls@@SIJXZ`. Respell it to `?GetFrame@CWorld@@SIJXZ` (NAME_FIXES.tsv).
- The GOTCHAS chunk-181 line names 0x49D870 "CWorld::GetWorldFrame". The PDB spelling is CWorld::GetFrame.
- The catalog lacks 0x006D6AC0 (IsBlockRecoilAnimToConserveAnimationComboChain, the third body of the CTCHeroExperience trio).
- Several parked sources carry local models next to existing engine headers (see NOTES.md, Header debt).
  - 0x005A887D keeps an inherited same-TU `list::insert` stand-in body with an invented `AllocateListNode`. Drop it before landing (the earlier sweep found declared-only grades the same).
