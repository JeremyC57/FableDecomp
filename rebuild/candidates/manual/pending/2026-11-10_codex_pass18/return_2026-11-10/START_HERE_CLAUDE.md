# Pass18 handoff — read this first

38/40 functions verified (95%):2 raw byte matches and36 matches after COFF relocation masking with operand identities checked. One parked row and one invalid assigned extent remain. This is pass-local progress; do not add it to the global catalog until your own regrade/landing. No commits or pushes were made.

## Verify and land

1. Check SHA256SUMS from this directory. Use your original pass18 repository, retail exe and unmodified MSVC7.1 RTM13.10.3077. No game files, compiler binaries, object files or retail byte cuts are included.
2. Review HEADER_DEFAULT_AUDIT.md and overlay hdr/ onto include/. Apply NAME_FIXES.tsv as reviewed spelling corrections; NAME_RECOVERY_NOTES.json retains non-address type/member observations. LABELS.tsv contains the evidence-backed row identities.
3. For each VERIFIED_MATCH/RELOCATION_MATCH in STATUS.tsv, build rows/<addr>/<addr>.cpp with /c /O2 /Oy /W3 plus its single Flags line, compare the assigned original retail span with relocated words masked, then audit every operand against FINAL_OPERAND_AUDIT.json and the original PDB/name evidence. FINAL_GRADE.json records the actual emitted symbol and source hash.
4. Compile each standalone <addr>_test.cpp with RTM /Od /W3 /GR- and with 64-bit g++ -std=c++03; require exactly <addr>_TEST PASS. All39 authored tests were rerun successfully. Test fixtures exercise logic with stubs; they are not execution of the game or whole-program equivalence proofs.
5. Land only the38 verified rows after your independent verification. No integration/online repository write has been performed here.

## Unfinished rows

- 00903f30 FollowPreCalculatedRoute:254/254 bytes,89/89 instructions,10 strict diff hunks caused by an EBX/EBP role exchange. Best original-helper-chain source retained. At least15 genuine main strategy families plus further main/Sol diagnostics; see PROBES.md and INDEPENDENT_RETRY.md. The strongest rejected250-byte candidate gets the desired roles only by moving conversion across a virtual call, changing native order. All288 selected behavior cases pass both compilers. This is PARKED, not a match.
- 00be2120 ClearChildPrimitive:assigned97-byte span ends inside the call at00be2180; complete body is106 bytes, ending00be218a. README explicitly requires reporting the true extent instead of authoring a fragment. No source/test was fabricated. STATUS assigned_name is deliberately empty because no selected emitted leaf exists; LABELS/NOTES retain the evidenced identity.
- No untouched assigned rows. The invalid extent is inspected/reported, not an authored/graded row.

## History and interpretation

STATUS attempts counts all 332 recorded grade invocations across the pass, including dependency failures, flags, and final revalidation; it does NOT mean that many genuinely different source strategies. PROBES distinguishes source families from syntax checks and diagnostics. Matched rows need not reach ten attempts. The sole parked row exceeds the minimum independently of repeat validation.

rows/<addr>/history/<tag>/ has the graded candidate and original RESULT receipt. HEADER_SNAPSHOT.json points to a deduplicated history_headers/<hash>/ overlay against the original pass18 baseline. These are rejected/historical experiments, not files to land. Some receipts retain absolute workspace paths for provenance; no remote links or game data are included. Selected sources are only rows/<addr>/<addr>.cpp plus the root hdr/ overlay.

Anonymous-namespace comparator rows00978790/0097abf0 carry actual reference-build symbols in their Leaf comments. MSVC derives the ?A0x... TU hash from the build path; it changes when relocated. Preserve each anonymous TU and all remaining mangling exactly. FINAL_GRADE records each final emitted symbol; only this path hash was normalized for cross-build identity checks. Byte twins are not one externally linked function.

The original qualifier exception for009d5df0 is documented in its REVIEW/NAMING_EVIDENCE and the supplied CBankFileAsyncData.h; plain test fields do not use that qualifier. Row0099ba70 uses /Zc:wchar_t for its proven original wchar_t STL instantiation—read its naming evidence before substituting unsigned-short mangling. Pure inline-budget flags remain on their rows with supporting probe history.

Matched spans total8481 of8832 assigned bytes (the denominator includes the invalid97-byte span). Use38/40 functions as the headline progress.
