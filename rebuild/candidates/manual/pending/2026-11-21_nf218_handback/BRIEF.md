# Name-fix agent NF218 (2026-11-21, round 10): nf217 follow-ups and the UnassignString spellings

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent; authoring agent AUTH4 runs at the
same time on 0x00A4EFC0 (locks `AUTH4`; its overlays under `$S/w22/auth4/` read-only).

## Your brief is NF215's / NF214's with overrides
Read `$S/w22/agentNF215/BRIEF.md` (its "Lane" section applies unchanged) and `$S/w22/agentNF218/NF214_BRIEF.md` in full with everything it
says to read first. Paths `agentNF214` / `agentNF215` -> `$S/w22/agentNF218`, names -> `NF218`. On a conflict: this file > NF215 BRIEF >
NF214 BRIEF. Also read NF217's hand-back (`rebuild/candidates/manual/pending/2026-11-21_nf217_handback/`, especially NOTES.md and
NAME_FIXES.tsv) and `batches/2026-11-21_landing_nf217/README.md`: your first three entries come from it. Lane caveat (FINDINGS
2026-11-21): a bare CompilerFlags catalog value makes recheck.py add /Oy-; grade such rows with q.py and the row's own `// Flags:` line.

## Entries (ENTRIES.tsv), in this order; rows locked to you: rows.txt (claim others with plain mkdir)
1. **q12385** (0x00C442F0 CircularQueue::GetInfo, descriptive): move the self row 0x00C442F0 (stand-in
   `?GetMark@CMarkedBuffer_00c43810@@...`) and 0x00C43810 onto `?GetInfo@CircularQueue@@QAEAAUSQueueInfo@@AAU2@@Z`, the spelling
   0x00C437C0 / 0x00C432F0 already bind. libpk has no PDB: the made-up-name rule applies (one accurate name per address; mark descriptive).
   Respell the catalog name too if it is an address-suffixed placeholder (the coordinator renames files at landing; list it in RENAMES section 1).
2. **q12384** (0x0041D165): it is bound as `?ResetMenus@CManager@NUISystem@@SIXXZ` (0x0047D78B), `?OnCloseMenus@CManager@NUISystem@@SIXXZ`
   (0x0056F30B) and the free `?CloseMenus@@YIXXZ` (0x00621720). Settle the real name (PDBs first; engine/CManager.h may already declare it),
   then respell the self row (if landed) and every binder (census with callers.py + compile; REFS.tsv).
3. **q12386** (0x00C576E0 CAudioContext::SourceUnqueueBuffers): the owner defines it strongly while 0x00C57C90 / 0x00C73050 emit pick-any
   copies (1 LNK2005). Apply the nf86 / nf88 one-definition scaffolding (GOTCHAS lines "landing nf80", "nf86", "nf88"); never change the leaf's
   bytes; keep 0x00C73050's `// Inferred:` resolver text exactly.
4. **q12377** (0x0099E9B0 CCharString::UnassignString, protected, both PDBs): landed rows spell it `?ReleaseData@CCharString@@QAEXXZ`,
   `?UnassignString@CCharString@@AAEXXZ` / `QAEXXZ` and others (NF215 NAME_FIXES). Census every binder and definer; move them onto
   `?UnassignString@CCharString@@IAEXXZ` (fable_string.h declares it) as a family pass: lock every family row up front, prove the rewrite on
   2-3 rows, script it, grade the rest in payloads of 20-30. Hand back at a row boundary with the census written down if the family is larger
   than your context allows.
One entry to a terminal state before the next. Report briefly when done or when your context gets heavy (hand-back files current first).
