# Pass21 handoff — read this first

This handoff covers exactly the 40 authorized Pass21 functions. It is a local research result; nothing was committed or pushed to any repository.

## Outcome

- 26 of 40 functions match after masking only actual COFF relocation operands: **65.0% by function**.
- Those 26 account for **147,518 of 220,688 verified function-extent bytes (66.84%)** in this pass.
- 14 remain parked with their best selected clean reconstruction, passing caller-behavior fixtures and detailed attempt history. No row is unattempted.
- These counts are for this handoff, not a claim that the repository has already landed the changes. The supplied project baseline was 47,649/49,568 functions (96.13%). Do not add these 26 to it until they pass your own landing process and are confirmed absent.

## Review and landing order

1. Verify `SHA256SUMS` and `VALIDATION.json`. Read `STATUS.tsv`, `ROW_DETAILS.md`, `NAME_FIXES.tsv`, `NEW_BINDINGS.tsv`, `LABELS.tsv` and `BOUNDS_FIX.tsv`.
2. Apply the single `hdr/` overlay over the original Pass21 input `include/`. Read `hdr/README.md`; do not mix in older per-attempt private overlays. Existing default branches were independently checked, and every returned row was compiled against this same header code.
3. Recut each function at the corrected length in STATUS/BOUNDS_FIX using your own original executable. In particular, 00D3E2E0 includes four jump tables and ends before the separate allocator; trimming only trailing alignment is insufficient. The other three spans that include neighbouring code are 00975BB0, 00AFCC30 and 00D0D910.
4. Build each selected `rows/<address>/<address>.cpp` using **MSVC 7.1 RTM, cl 13.10.3077**, base `/O2 /Oy /W3` and only that source's extra `// Flags:` values. `LABELS.tsv` equals the actual compiled Leaf, not a nearby overload or the manifest hint.
5. Mask actual COFF fields only, then validate each target identity, real branch displacement, virtual slot and local jump-table entry. Matched rows include the final `OPERAND_AUDIT.json`. Across the 26 matches, 6,983 operands were decoded and 274 distinct named external targets had no symbol/address conflicts.
6. Build each self-contained behavior test with VC7.1 `/Od /W3 /GR-` **without `/EHsc`**, and 64-bit g++ `-std=c++03`. Expect exactly `<address>_TEST PASS`. `TEST_AUDIT.json` records the matching test hash, the exact production compound-body check and successful compiler/run results. Dependency doubles test the caller; they do not imply full-game execution or verification of every external helper.
7. Land only the RELOCATION_MATCH rows after your own review. Parked rows remain DIFFER even where sizes or much of the instruction stream agree. Keep their returned source as the selected starting point and read PROBES before retrying.

## Naming qualifications to review explicitly

- **00804020:** original helper identifiers, owners, types and native addresses are established, but the original anonymous-namespace source-path hash is unavailable. Only that compiler-generated hash was normalized during identity comparisons. Do not describe the hash itself as recovered.
- **00DB5330 / 00DBB310:** original local-static names, owners, types, values and use sites are proved. The native lexical scope ordinals are not independently recoverable from this evidence. NAME_FIXES proposes the current emitted ordinals and explicitly records their difference from donor ordinals. Review these as proposed local-symbol bindings, not proven original native ordinals.
- **00B0DC20:** a non-ABI iterator local is descriptively respelled `emitterIterator` to meet the explicit forbidden-substring rule. Original local-name evidence is preserved separately. No external symbol changed.
- All source names remain subject to the row's NAMING_EVIDENCE and included original type/name records; manifest labels were not accepted as proof. Wrong inherited spellings are reported, never silently copied merely to satisfy a binding checker.

## Attempt history and parked work

`ATTEMPT_HISTORY.tsv` includes every recorded compiler invocation, including failed dependency compiles, budget diagnostics and repeat verification. It is **not** the count of genuinely different source attempts. Each parked row's `ATTEMPT_AUDIT.json` gives a conservative list of at least ten successfully compiled source reconstructions. PROBES retains failed ideas, measurements, selected/rejected alternatives and the exact remaining obstacle. Where the smallest-size alternative violates the native cleanup choices, the more faithful tested version is selected and the alternative is described explicitly.

The narrowest remaining engine residues are 00B24850 (three commutative operand/destination instructions), 00A1E5C0 (late x87 scaling schedules), and 00975BB0 (normalization scheduling/spills). Script blockers concentrate on cleanup expansion/merging and two speech-loop latches in 00DE8370. These are leads, not promises of a simple next fix.

## Included and excluded

Included: selected sources, all 40 tests, corrected/new shared headers, row status and detailed probes, original naming/layout evidence, test/compile/operand audits, extent/name/label corrections and lessons. Historic audit hashes identify their original checkpoints; final ROW/FINAL_COMPILE_AUDIT/TEST_AUDIT and root SHA256SUMS identify the returned files.

Excluded: executable or donor binaries, PDB binaries, raw extracted code, object files, compiler/Wine installations and Git history. Evidence JSONs omit raw byte payloads while retaining parsed declarations, addresses, counts and conclusions. Workspace paths in historical notes are provenance, not external download links or required runtime dependencies. Use your existing game/PDB/toolchain copies for independent regrading.

Owner clarifications are in OWNER_CLARIFICATIONS.md. Original identifiers containing the letters of the banned storage-class keyword remain allowed; the keyword itself is not used. The sprite's explicit inline-control approval was recorded, but its final match uses the measured flags and source fixes without needing an extra device.

NAME_FIXES contains 106 wrong-spelling corrections; NEW_BINDINGS contains 5 additional/reaffirmed symbols for which no wrong landed spelling was recorded. Both are proposals for your evidence-based merge, not mutations of the supplied repository.
