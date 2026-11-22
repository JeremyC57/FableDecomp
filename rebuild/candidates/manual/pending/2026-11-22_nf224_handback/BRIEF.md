# Name-fix agent NF224 (2026-11-22, round 10): the nf223 follow-ups (two held GetFont rows, 0x0042DB20 onto its header, a catalog name)

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. An authoring agent (AUTH11) is
running; its locks read AUTH11: leave them alone. Codex pass 27 rows (UNMATCHED_ROWS "reserved (Codex pass 27 ...)") are off limits.

## Your brief is NF223's with overrides
Read `$S/w22/agentNF223/BRIEF.md` and the briefs it chains to (NF222, NF221, NF215 "Lane", `$S/w22/agentNF224/NF214_BRIEF.md`) in full.
Paths -> `$S/w22/agentNF224`, names -> `NF224`. On a conflict: this file > NF223 > NF222 > NF221 > NF215 > NF214.
nf223 is applied to the working tree as of now (landing in progress: `batches/2026-11-22_landing_nf223/README.md`; hand-back
`pending/2026-11-22_nf223_handback/`, incl. its drafts `work/e2/` and the rejected `rows_held/0042db20_rejected.cpp`). Start from the repo
working tree as it is now. NF222's pick-any checker: `pending/2026-11-22_nf222_handback/tools/pk.py`.

## Entries (ENTRIES.tsv), in order
1. **0x00494840 / 0x0054ED90 (GetFont binders, held at nf223).** Respelling them onto `?GetFont@CFontManager@@...` gives CCountedPointer
   pick-any copies with retail owners that differ (0x00494840: the 72-B converting `??$?4<const CFontBank>` v owner 0x0042FE34;
   0x0054ED90: ReleaseData / ShareData / `??4<CFontBank>`). Clear them the nf222 way: the existing declaration-only opt-ins in
   engine/CCountedPointer.h (CONVERTING_ASSIGN_DECL_ONLY, RELEASEDATA_DECL_ONLY / _NOINLINE, SHAREDATA / ASSIGN_DECL_ONLY). Each row must
   stay MATCH / RELOCATION_MATCH, and every pick-any copy with a retail owner must equal it (pk.py). GetFont should end on 1 spelling.
2. **0x0042DB20 (q12556, ~CManager).** Port the row onto engine/NUISystem_CManager.h (no stand-in class renamed into the namespace, and no
   friend scaffold: nf95b). Its own leaf is `??_GCManager@NUISystem@@IAEPAXI@Z` (both PDBs), and it should call
   `??1CManager@NUISystem@@IAE@XZ` (0x0041E641). If the header lacks what the row needs, an add-only opt-in is fine; record it in hdr/.
3. **0x00415B80 catalog name**: SNavigationLayerDefaults -> CGraphicDataBankInit::CGraphicDataBankInit (RENAMES section 1; the coordinator
   renames files). Check both PDBs.
For every entry: recheck every changed row, compare every new pick-any copy with its owner's retail bytes, census spellings, link-check,
and list TABLE_CHANGES.tsv. Never write into /home/user/FableDecomp; never run git. Report briefly when done or when context gets heavy.
