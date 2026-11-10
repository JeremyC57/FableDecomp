# Pass19 handoff — read this first

33/40 VERIFIED_MATCH (82.5% by function count);7 PARKED. All40 rows were worked. There are no untouched, skipped, blocked or wrong-extent rows in this pass. Both actual MSVC7.1 RTM cl13.10.3077 and64-bit g++03 independently passed all40 final behavior tests. No commits or pushes were made.

## Verify and land

1. Check SHA256SUMS. Read STATUS.tsv and the row's NAMING_EVIDENCE.md, PROBES.md and MAIN_REVIEW.md where present.
2. Apply only root hdr/ over the incoming pass19 include/ base. Do not apply historical header snapshots. Review hdr/README.md, PACKING_APPROVAL.md and verification/default checks.
3. Rebuild each selected rows/<addr>/<addr>.cpp under real VC7.1 RTM /O2 /Oy /W3 plus its // Flags: extras. Check exact // Leaf: and full extent, compare masked bytes, then validate every relocation target/name/literal independently. No raw retail bytes or tools/game/compiler files are bundled; use your own originals.
4. Rebuild/run each selected standalone *_test.cpp under actual VC7.1 /Od /W3 /GR- and g++ -std=c++03. Require the exact address_TEST PASS line. Main's results and source hashes are in verification/BEHAVIOR_RESULTS.json.
5. Land only the33 VERIFIED_MATCH rows after your own checks. Apply NAME_FIXES.tsv with its evidence; per-row naming notes also record field/semantic corrections that are not standalone mangled bindings. LABELS.tsv gives readable identities.

## Important limitations

CFloatQuad::Interpolate (00c462e0) and InvertMatrix3x4 (00c1d1c0) are explicitly descriptive identities after unsuccessful searches, not recovered original names. The current owner rules allow accurate descriptive naming after a real search. Preserve their uncertainty.

00680050 remains parked and retains a documented byte+8 validation-world access: the derived validation-init type is forward-only in both PDBs. No complete derived layout or field name is asserted. Its final plain GetSubDef helper needs no new suppression overlay.

The seven parked rows retain their best reviewed selections and passing behavior tests. Equal lengths and high instruction agreement do not make them matches. Each has at least10 distinct strategies recorded; source-shape variants, failed builds and final verification runs are distinguished in PROBES.md. The inherited pre-pass attempts remain in your incoming package; this archive preserves all current-pass candidate sources/header snapshots and compiler/test logs without binaries or raw retail dumps.

## Parked rows

- 0047dd9d CTCInGameMenu::PCSetToLoad: two instruction schedule swaps: final coordinate push/fsubp and third observer global/vtable load.
- 0047ed76 CTCInGameMenu::SetToSave: push ecx at float argument reservation precedes fsubp; retail reverses.
- 00680050 CThingObjectDef::Validate: One adjacent load swap at00680196: pappearance_def reload versus conditional cleanup flags.
- 00774c60 CTCShop::StockDisplay: this esi vs ebx; counts ebx/ebp vs esi/edi; index edi vs ebp; two loop argument loads exchanged.
- 0086be40 CRandomAppearanceMorph::GetRandomBodyParts: empty branch precedes three zero stores; original-lifetime form292 bytes materialises element address before folded loads.
- 00933db0 ?Update@CAIStateGroup_MinionKidAnnoy@@UAEXXZ: three radius fld/fadd instructions and fstp versus one push.
- 00986ed0 ??0CThingFilter_IsAICreatureMatchingTypeAndFactionNearby@@QAE@ABVCThingAICreature@@M_N@Z: three commutative cross-product multiply operand pairs; correct all field offsets and copy schedule.

## Archive layout

- rows/: selected sources/tests, identity evidence, probes, final object symbol/relocation reports, per-row HISTORY and TEST_HISTORY. Only the top-level address.cpp/test.cpp are selected; historical drafts may fail current rules or compilation and are never landing candidates.
- hdr/: merged selected overlay, plus README.
- STATUS.tsv, LABELS.tsv, NAME_FIXES.tsv: final reviewed state and corrected identities.
- HISTORY_INDEX.tsv: every saved current-pass trial and its immutable compiler outcome. Pending-audit reports are raw compiler results; STATUS.tsv reflects the later independent audit and tests.
- LESSONS.md / FINDINGS.md: measured winning ideas and remaining limits.
- verification/: final hashes/results, binding conflict checks and default-header review.
- SHA256SUMS: all archive files except itself.

No executable, object, PDB, retail-byte dump, game content, compiler, Wine installation, git history or old handoff ZIP is included.
