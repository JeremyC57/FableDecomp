# Compiler probes

- package baseline — COMPILE_FAIL.
- tail deliverable — DIFFER, 2268 v 2266 bytes, 97 instruction hunks, first unequal byte at 0x007542B2.
- tail emitted leaf diagnostic — DIFFER, 2268 v 2266 bytes, 97 instruction hunks, first unequal byte at 0x007542B2.
- tail pdb rekey — DIFFER, 2268 v 2266 bytes, 97 instruction hunks, first unequal byte at 0x007542B2.
- tail structure guard 1 named — DIFFER, 2268 v 2266 bytes, 96 instruction hunks, first unequal byte at 0x007542B2.
- tail structure guard 2 named — DIFFER, 2192 v 2266 bytes, 94 instruction hunks, first unequal byte at 0x007542B2.
- tail structure guard 3 named — DIFFER, 2268 v 2266 bytes, 96 instruction hunks, first unequal byte at 0x007542B2.
- tail structure guard 3 nested and — DIFFER, 2268 v 2266 bytes, 97 instruction hunks, first unequal byte at 0x007542B2.
- tail structure guard 4 named — DIFFER, 2268 v 2266 bytes, 96 instruction hunks, first unequal byte at 0x007542B2.
- tail target named eye primitives — DIFFER, 2268 v 2266 bytes, 97 instruction hunks, first unequal byte at 0x007542B2.
- tail target named eye test — DIFFER, 2268 v 2266 bytes, 96 instruction hunks, first unequal byte at 0x007542B2.
- tail target named morph — DIFFER, 2268 v 2266 bytes, 96 instruction hunks, first unequal byte at 0x007542B2.
- tail target separate creature checks — DIFFER, 2268 v 2266 bytes, 96 instruction hunks, first unequal byte at 0x007542B2.
- tail target test bank index — DIFFER, 2268 v 2266 bytes, 97 instruction hunks, first unequal byte at 0x007542B2.

The copied body verifies the drawable creature guard, both-eye setup, eye-axis rotation, left-only morph path, and clearing both missing dummies. The selected draft initialises the mesh pointer inside the left-dummy block, so a right-only dummy can reach a null mesh pointer. That branch remains open.

Native control flow confirms the right-only case is a retail limitation. 0x00754327 clears esi. Only the left-dummy arm at 0x007543B0 or 0x007543B9 assigns the eye-mesh address to esi. With the left dummy absent, control reaches the right lookup at 0x007547C9 directly, and the right-dummy arm writes the orientation through `[esi+0x24]` at 0x00754AE2–0x00754B03. The candidate preserves that behavior. The fixture covers no eyes, no live creature, both dummies, left-only and neither dummy.

The previous local-model grade was 2268/2266 under `tail_pdb_rekey`. The selected shared-header grade `tail_eye_shared_handles` emits 2220/2266 with 75 strict instruction hunks. The shared owner, render mesh and attachment classes retain the native receiver offsets and frame extent. Retained handles, index-slot evaluation and the two eye transform schedules remain different. The standalone fixture executes the selected method body with controlled service stubs and passes both compilers.

The selected result is PARKED (2220 object bytes, 2266 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
