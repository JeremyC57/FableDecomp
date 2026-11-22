# Name-fix agent NF225 (2026-11-22, round 10): the nf224 follow-ups

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. An authoring agent (AUTH12) is
running on 0x00CF4EC0; its locks read AUTH12. Codex pass 27 rows are off limits.

## Your brief is NF224's with overrides
Read `$S/w22/agentNF224/BRIEF.md` and the briefs it chains to (NF223, NF222, NF221, NF215 "Lane", `$S/w22/agentNF225/NF214_BRIEF.md`).
Paths -> `$S/w22/agentNF225`, names -> `NF225`. On a conflict: this file > NF224 > earlier. nf224 LANDED (`batches/2026-11-22_landing_nf224/`);
NF224's hand-back with its forms and probes is `pending/2026-11-22_nf224_handback/` (work/e1 GetFont forms, work/e2 the ~CManager probes).
Start from the repo tree as it is now.

## Entries (ENTRIES.tsv), in order
1. **0x00494840 / 0x0054ED90 (GetFont binders).** The coordinator authorises a probe of add-only, member-specific SIZE_BODY opt-ins in
   engine/CCountedPointer.h: the nf213 `_SIZE_BODY` pattern in stlport/fable_containers.h (size-pragma scaffolding rule, EXTRA_RULES
   2026-11-20 / 2026-11-21 condition 2). Goal: the rows' pick-any copies (converting `??$?4`, ReleaseData, ShareData, `??4`) become the
   size-optimised bodies their retail owners have (0x0042FE34 etc.), with every row still MATCH / RELOCATION_MATCH. Every pick-any copy
   with a retail owner must equal it (pk.py); GetFont then ends on 1 spelling. Run the header proof (pin_asserts, assert_names_check;
   name the includers that would change). Budget: 6 forms per row; then hand back what you measured.
2. **0x0042DB20 (~CManager).** First the evidence check: do Ego_r.pdb / FableWin.pdb show CManager::Clear inlined anywhere (S_INLINESITE,
   or an inline copy in another compiland)? If NOT: stop the entry (the row stays as at HEAD) and record the result. If yes: the 3-row unit
   NF224 described (header opt-in with an inline Clear and the protected static GPUniqueInstance; port 0x0042FB40 so both Clear copies are
   COMDATs; respell 0x0041E5F2), with 0x013B8710 and 0x0041E641 each ending on one spelling. No friend scaffold (nf95b).
3. **0x013B8710** `?GPUniqueInstance@CManager@NUISystem@@1PAV12@A` (all three PDBs): respell its binders (Instance in 0x0042FB40,
   g_pUIManagerInstance in 0x0041E5F2) with entry 2's unit, or on its own if entry 2 stops (only if no header member must change).
For every entry: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv. Never write into
/home/user/FableDecomp; never run git. Report briefly when done or when context gets heavy.
