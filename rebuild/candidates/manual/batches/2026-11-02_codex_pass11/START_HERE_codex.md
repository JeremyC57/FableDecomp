# Start here — Codex pass 11 return

This is the return for the **100 addresses reserved in the supplied pass 11 package**. Read this file, `STATUS.tsv`, `ROW_DETAILS.md`, `NAMING_SUMMARY.md`, `RULES_AND_PRECEDENCE.md`, and `LESSONS.md` before selecting rows to land. `SCOPE_ROWS.tsv` is the original assignment; its labels remain hints. `AUTHORIZED_SCOPE.json` is the address allowlist.

## Final outcome

**54 of 100 functions (54%) passed independent main verification for their assigned retail extents.** They cover 68,754 of 133,292 authorized bytes (51.58%). These are locally verified candidates for your lander; no repository commit or push was made.

| Disposition | Rows | Action |
|---|---:|---|
| Verified assigned extent: 49 relocation matches, 5 unmasked matches | 54 | Independently regrade, inspect bindings, rerun tests, then land |
| Authored, ordinary code differences | 38 | Retry from the selected source and documented alternatives |
| Authored, compliance hold | 1 | Keep parked; historical exact diagnostics are not accepted |
| Exact bytes, unresolved callee identity | 2 | Resolve the real symbol before landing |
| Exact only after extent correction | 1 | Confirm and authorize the corrected extent before landing |
| Researched original-name/interface/layout blockers; no source authored | 4 | Resolve the documented blocker first |
| Unresearched rows | 0 | None |

All **96 authored current source/test pairs** have real RTM compiler receipts, a unique emitted Leaf, and passing GNU64 C++03/noRTTI and VC7.1 `/GR-` behavior-test receipts. A behavior PASS does not establish byte parity. The 39 ordinary parked/compliance rows have at least ten substantive source-shape strategies documented; verification reruns, flag-only probes, failed builds, and unsupported diagnostics are not additional strategies. The four pre-authoring blockers have zero compiler attempts, explicitly reported rather than fabricated.

`STATUS.tsv` uses the requested four-state `status` column. `disposition` preserves more detailed holds. `landing_eligible=yes` identifies only the 54 verified rows. The `attempts` column counts all immutable compiler receipts, including unsuccessful probes and later verification; it is **not** a count of distinct strategies. `selected_attempt`, source/test hashes, and `current_review` identify the current result.

## Files and precedence

1. `STATUS.tsv` and `ROW_DETAILS.md` describe the final selection and remaining work for every address.
2. `rows/<address>/<address>.cpp` is the current selected source; the matching `_test.cpp` is its self-contained behavior fixture. The four researched blockers intentionally have no invented source or test.
3. `rows/<address>/verification/MAIN_REVIEW.json` is the independent current review where available. Compiler and test receipts plus actual build/PASS logs are in the same verification directory. Each authored row also has `PROBES.md`, `NAMING_EVIDENCE.md`, and its selected compiler receipt.
4. `rows/<address>/probes/attempt_NN/` preserves every compiler candidate, receipt, and available compiler log. `HISTORY_CLASSIFICATION.json` and `HISTORY_CONTEXTS.json` distinguish these historical diagnostics from current selections. Older exact grades do not override the final selection.
5. `SELECTED_HEADER_CONTEXTS.json` and each row's `selected_context/hdr/` preserve the precise project include closure used for the reviewed selection. Root `hdr/` contains the combined corrected/new development headers and documentation.
6. Historical include contents are stored once in `hdr/history_objects/`, indexed by SHA-256. `HISTORY_CONTEXTS.json` maps every logical header path to its exact version for each attempt. Use `tools/restore_context.py` to recreate an isolated context.
7. `NAME_FIXES.tsv` records proven incorrect landed/bound spellings and their corrections. `NAME_FIXES_MERGE_REVIEW.md` documents reconciliation. `NAMING_SUMMARY.md` distinguishes genuine recovered names from explicitly descriptive vendor names.
8. `evidence/` retains supporting research and older reviews. These are evidence and chronology, not alternate landing decisions. Workspace paths appearing inside old receipts are provenance references, not portable include instructions.
9. `SHA256SUMS` is the authoritative manifest for all returned files except itself. Run `python tools/verify_manifest.py` after extraction.

Historical reviews can mention earlier counts, prior blockers, or “matched” raw grades. The final `STATUS.tsv` and hash-bound current review take precedence. Two particularly important rejected historical diagnostics are `00409730` attempts 19–22 and `0052f900` attempt 14: their exact raw grades relied on unsupported stronger annotations. They are preserved for audit only.

## Holds that must not be silently landed

- `00520670` and `00521620`: exact selected bytes and passing tests, but the real one-argument string-transfer callee at `00431143` remains unproven. Both remain naming holds, excluded from 54.
- `0099dcb0`: assignment is 1454 bytes; the complete function is 1455. The corrected span matches and tests pass, but the assigned-span row is `EXTENT_WRONG`. Preserve both extent facts and the boundary evidence.
- `00409730`: selected ordinary implementation is 641/1078. Exact historical diagnostics are not compliant, and are not the selected source.
- `00c0f440`: eight-byte original vendor forwarding member; original owner/slot/signature unresolved despite PDB, vendor-library, vtable and neighbor searches. Assigned extent itself is valid. No source authored.
- `005eb9e0`: `CTCTavernGame::FrameUpdate` identity is known; the genuine reduced-alignment `CTCOwnedEntity` member layout remains unavailable under the no-new-packing rule. No source authored.
- `006286f0`: `GFPlayFullScreenXBoxMovie` identity is known; the required complete original `CASoundManager` interface is unproven. No source authored.
- `00631c60`: `NUISystem::CUIDef::Transfer` identity is known; original packed `PositionOffset` at +0x142 cannot yet be expressed with the supplied faithful header layout. No source authored.

No claim is made that vendor/libpk descriptive names are recovered original names. GameCODA `CSalIIRFilter::Process` and its members are independently supported by named release objects and debug CodeView; other vendor naming limits are recorded individually.

## Reproduce and review

Use your own legally held retail executable and **MSVC 7.1 RTM `cl 13.10.3077`**. No compiler, Wine installation, PDB binary, executable, retail cut, native machine-byte dump, or game download link is supplied. The authorized executable SHA-1 is `39fd416ba23a91a89713ec80c789f57ddb22e2c8`.

For a current source, run `python tools/restore_context.py 0078b7c0 <new-empty-directory>` from the extracted return. This creates the selected TU, test and exact project include tree. To reproduce a historical compiler attempt, add `--attempt N`; this does not copy or assert a current behavior test for that old candidate. Python 3.9 or later is sufficient for the helper.

Compile production with `/nologo /c /O2 /Oy /W3`, the row's EXTRA flags, and the restored include tree before the original VC7.1 SDK includes. Verify that the line-2 Leaf identifies exactly one emitted COFF symbol with `objdump -t`. Anonymous namespace path hashes alone require the documented house normalization; never normalize access, types, namespace, calling convention or actual names. Compare only the assigned extent unless the owner approves the explicit extent correction. Mask relocation operands, then independently check every call, global, string, vtable and jump-table destination against the native target and its correct symbol.

Tests are standalone C++03. The standard lanes used `g++ -std=c++03 -pedantic-errors -fno-rtti` and VC7.1 `/nologo /c /Od /GR- /W3`, followed by the original linker/runtime. Supply the normal SDK headers/libraries. Each successful run must print exactly `<address>_TEST PASS`. Some rows have additional optimized/no-elision or sanitizer evidence; do not generalize those checks to all rows. The test counts in row records are deterministic scenarios, not thousands of independent test executables.

Use the real full headers, preserve opt-in guards and default branches, honor the package's original-name/two-source rules and latest owner clarifications, and retain all attempt counts. Start retries from the final selected version unless the history gives a reason to examine an alternative. A smaller length gap is not automatically a better reconstruction: navigation's 3184-byte alternative and faction's 1435-byte alternative are explicit examples.

The archive includes all requested work and documentation, with a compact historical header store. It deliberately excludes build products and raw retail material. Checksums prove file integrity, not semantic correctness; your independent compiler, symbol and behavior verification remains the landing gate.
