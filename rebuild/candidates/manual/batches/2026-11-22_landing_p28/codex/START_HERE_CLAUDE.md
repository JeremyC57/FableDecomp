# Fable Pass 28 handoff

21 of 25 reserved functions match across their complete retail extents. They account for 42,651 of 53,875 assigned bytes (79.17%). 4 functions remain parked. All 25 selected production files compiled and their companion tests printed the expected PASS line on VC7.1 RTM 13.10.3077. Parked rows remain candidates for further reconstruction.

## Final match

00DFD600, NScript::CQ_TraderConflictGoodScript::AttackPeople, matches all 2,088 bytes. The final audit verifies 77 primary relocations, all 16 termination destinations, 11 mapped copies, and the additional deleting destructor at 004AB170. Its C++03 test passes on the frozen headers. The label in the C++ text is a reconstructed way to obtain the observed cleanup sequence. Its original spelling is unknown.

The remaining four rows are 00850A80 (context-sensitive item), 00879900 (projectile camera), 00A2F740 (mesh initialization), and 00D07490 (Bandit Camp guard). STATUS.tsv and the latest entries in each row’s notes describe their selected sources and residues. None is counted toward the 21 matches.

## Verified additions

00560D00 now matches all 1,860 bytes and all 136 relocated operands, including its switch destinations. All 21 mapped copies and ten additional owners pass. The native test checks both arguments to encryption and decryption.

006C6040 now matches. All 65 leaf operands, 26 emitted owners, and the native behavior fixture passed. The recovered feature switch and getter correction are documented.

00B19C60 now matches. The 4,608-case fixture passes. Both particle scale and light-constructor bodies were needed for the original frame, and the separately emitted colour helpers were checked.

00647AB4 now matches. All 57 operands were verified and the RTM fixture exercises all 2,048 scenarios. BINDING_EVIDENCE.json documents the pre-existing folded destructor alias under the owner ruling.

008101F0 now matches. The trail update passes all 25 fixture scenarios, including the final eligible frame, and its original pair and list calls were independently checked.

008664F0 now matches. All 99 operands, 15 mapped copies and the additional 125-byte animation lookup passed. The original projectile-reset switch is identified in NAME_FIXES.tsv.

008E8060 now matches. All 92 operands of the activation and all 25 mapped copies pass. The marker-search helper also matches at 008E8AC0. The 96-case native test covers both marker choices and both effect attachments.

00BE6880 now matches. All 1,776 bytes, 26 operands, and the additional 150-byte vertex-normal helper are verified. The recovered engine-definition global is recorded in NAME_FIXES.tsv.

00491AD2 now matches across all 2,223 bytes. Eight mapped copies and five further original vector helpers were audited, as were all 150 primary operands. The fixture executes the tag-processing body.

00AB7C20 now matches across all 1,910 bytes. Its 22 operands, inline copy and six-vertex render fixture are verified.

00EC32F0 now matches. Its switch table, full operand audit, inline copy audit and native behavior fixture are in its row directory.

## Previous additions

00E03F70, the Trader Escort magic barrier, and 00DC08C0, the Oakvale raid gate timer, now match. Their final native tests passed, and the two leaf audits cover all 175 relocated operands. Their nine mapped inline copies match at their bound retail addresses. The Bandit Camp guard remains parked. The Trader Conflict attack was subsequently verified as the final match. All selected functions were regraded on the merged tree including the two newly recovered font vertex types.

## Reproduce the grades

1. Start from the original pass 28 package includes. Apply hdr/PATCHES.json in array order using hdr/REPLAY.md. The export changes 76 header paths in 137 replacements. Check hdr/EQUIVALENCE.md for the landed-source coverage and the missing sources to check locally.
2. Read STATUS.tsv for the final grade, native extent, inline-copy counts, strong extra functions and residue. LABELS.tsv gives the full object symbol confirmed with objdump. Compile each selected production TU with /O2 /Oy /W3 plus its recorded extra flags. Compare the complete extent in the retail executable of SHA-1 39fd416ba23a91a89713ec80c789f57ddb22e2c8 with relocated operands masked.
3. Review each operand and mapped inline copy using the scoped evidence in its row directory, then rerun the native behavior test with /Od /W3 /GR- and the package include tree. TEST.json records the expected output and test hash. RESULT.json records the native compile and final include-snapshot digest. README assumptions about complete code reachability should be reviewed against the test fixtures and their deterministic collaborators.
4. Review NAME_FIXES.tsv before importing bindings. For 007ebea0, compare the supplied-table PICKANY.txt with PICKANY_CORRECTED.txt and the three binding entries in BINDING_EVIDENCE.json. The corrected table yields thirteen equal copies. For 006c6040, the corrected check covers all 26 copies. Its getter calls the verified 71-byte tree member template at 006C7830.
5. Continue the remaining DIFFER rows from their submitted source, NOTES.md, PROBES.md and operand audit. PROBE_HISTORY.tsv records native grades including repeat verifications and failed builds. Token-shape counts are source fingerprints and can include changes that compile identically. The aligned instruction count is a diagnostic and can desynchronize over embedded data. Use the masked byte comparison and relocation audit to decide a match.

## Coverage

All 25 final tests passed on the frozen header tree. The exact historical-source coverage, unavailable sources, and preprocessing results are recorded in hdr/EQUIVALENCE.md. Additional matching compiler-generated bodies lie outside the 25 primary grades and contribute no bytes to the stated percentage.

Validate the package by running sha256sum -c SHA256SUMS from its extracted root.
