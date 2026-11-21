# Name-fix agent NF217 (2026-11-21, round 10): today's landing follow-ups (tags land1121nf214 / land1121auth3)

Owner (2026-11-21): "we can start a new agent when one lands / finishes"; "you can continue the naming queue agent too". You are the ONLY
naming agent; authoring agent AUTH4 runs at the same time on 0x00A4EFC0 (locks `AUTH4`; overlays under `$S/w22/auth4/` read-only).

## Your brief is NF215's / NF214's with overrides
Read `$S/w22/agentNF215/BRIEF.md` (its "Lane" section applies to you unchanged) and `$S/w22/agentNF217/NF214_BRIEF.md` in full with
everything it says to read first. Paths `agentNF214` / `agentNF215` -> `$S/w22/agentNF217`, names -> `NF217`. On a conflict: this file >
NF215 BRIEF > NF214 BRIEF. Lane caveat (FINDINGS 2026-11-21): a bare CompilerFlags catalog value makes recheck.py add /Oy-; grade such rows
with q.py and the row's own `// Flags:` line. Read the batch READMEs behind each tag: `batches/2026-11-21_landing_nf214/README.md`,
`batches/2026-11-21_landing_auth3/README.md` (and `_nf215`, `_nf216` for the current fable_string.h state).

## Entries (ENTRIES.tsv), in this order; rows locked to you: rows.txt
1. **q12368 / q12369 / q12370** (0x00A35B30 / 0x00A35AF0 / 0x00A35B50, the C3DMeshObject wrappers GetAllDummyObjectNames /
   GetAllHelperPointNames / GetDummyObjectNamesStartingWith): respell the self rows onto the PDB spellings in the entries (both PDBs per
   NF214's evidence) and their landed binders, no new split. engine/C3DMeshObject.h exists: use it (add-only opt-ins).
2. **q12372 / q12373** (0x0066E799 / 0x0066E7BD, NHeroInformationScreens::CManager::SelectScreenOfType / GetCurrentScreen): landed 0x006116E0
   binds stand-ins (SetCurrentScreenType(long); a non-const CBase* return) where witness 0x00577216 binds the catalog spellings. Settle each
   spelling from both PDBs, then respell the odd binder(s). 0x006116E0 was just ported (nf214): keep its /d2inlS12 and its pick-any copies.
3. **q12379** (0x00C432F0): the self row's catalog name is the placeholder `CMarkedBufferOwner_00c43810::ReportMark`; three landed binders
   (0x00C43A10, 0x00C5B300, 0x00C73050) call it `?ReportQueueInfo@CAudioObject@@QAEXXZ`. No PDB records this libpk/GameCODA class: the made-up
   name rule (CLAUDE.md, owner 2026-11-10) applies. Pick the one accurate descriptive name from the body (what it reports), use it at the
   self row and every binder, and mark it descriptive in NOTES. If ReportQueueInfo is accurate, only the self row moves.
4. **q12371** (0x0047E196): its strong 23-byte fill_n<NUISystem::CComponent**> definition duplicates owner 0x0048514C's body that the
   vector<CComponent*> rows now emit pick-any (one LNK2005). Make 0x0047E196's copy inline (nf140 / nf86 / nf88 patterns, GOTCHAS) without
   changing its leaf bytes.
5. **q12380** (0x00C576E0 and its inliners): re-port the SourceUnqueueBuffers owner 0x00C576E0 and its landed inliners (0x00C57460
   0x00C57AF0 0x00C57C90 0x00C57D80 0x00C72620 0x00C72800 0x00C72A40) onto 0x00C73050's resolver text (named `void* proc` before the member
   store), so the TU has one spelling. AUTH3 measured that text equal on all of them: `rebuild/candidates/manual/pending/2026-11-21_auth3/00c73050/cons/`.
   Only rows whose source spells the SourceUnqueueBuffers resolver; leave the other resolvers on the ternary. Each row needs an
   `// Inferred:` line citing 0x00C73050 (batches/2026-11-21_landing_auth3).
One entry to a terminal state before the next. Report briefly when done or when your context gets heavy (hand-back files current first).
