# Name-fix agent NF221 (2026-11-22, round 10): the CCountedPointer<T>::ReleaseData family, one dedicated pass

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. A builder agent (CODEX27) is
writing `pending/2026-11-22_codex_pass27/` and reserving rows in UNMATCHED_ROWS.tsv: leave those alone.

## Your brief is NF215's / NF214's with overrides
Read `$S/w22/agentNF215/BRIEF.md` (its "Lane" section applies unchanged) and `$S/w22/agentNF221/NF214_BRIEF.md` in full with everything it
says to read first. Paths `agentNF214` / `agentNF215` -> `$S/w22/agentNF221`, names -> `NF221`. On a conflict: this file > NF215 BRIEF >
NF214 BRIEF. Lane caveat: a bare CompilerFlags catalog value makes recheck.py add /Oy-; grade such rows with rg1.sh and the row's own
`// Flags:` line. NEW since NF220: Codex pass 26 landed 2026-11-22 (327 header patches; `batches/2026-11-22_landing_p26/README.md`),
including CCountedPointer.h changes: re-read the headers you touch from the repo, never from an older overlay.

## The family (one unit; ENTRIES.tsv: q12226-q12229 and q12402 / q12403)
Start from NF220's census and five-step plan: `rebuild/candidates/manual/pending/2026-11-21_nf220_handback/NOTES.md` (q12226-q12229 and
"entry 4") and its NAME_FIXES.tsv; NF220's probes are in `$S/w22/agentNF220/work/` (00403750_g1.cpp, 0042f50b_g1.cpp, the
RELEASEDATA_DECL_ONLY swap rows, 00419134_g1.cpp). About 40 rows in one component:
- the four self rows 0x004190B6 / 0x00419108 / 0x0042F974 / 0x0042F9A0 (ReleaseData for CLUA, CGraphicDataBank, CIEngine, CASoundBank;
  FableWin.pdb names) and the destructor thunks (e.g. 0x004194B5);
- the multi-type binder rows (0x0042F50B pulls in 0x0042918C CMeshDataBank, 0x0042F948 const CFontBank, 0x00403750
  NGameText::CDataBank, 0x004057A0 CFontBank: 7 spellings in 10 caller rows, plus NF220's CFont-v-CFontBank pointee question);
- the stand-in ShareData / operator= / Reset rows whose own leaves change, and their callers;
- q12403: the 4 rows that bind `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` at 0x004291DE (0x0042D45D, 0x005C8459, 0x005C9A95,
  0x0058F016; the one-line RELEASEDATA_DECL_ONLY change), then move the reloc_bindings line `??1 -> 0x004291DE` (stale witness
  0x00570332) to the thunk 0x00429C53 (Codex pass 26 adjudicated the same: its PICKANY_CORRECTED files show the ??1 copy equal at
  0x00429C53). Overlaps the family at 0x0041E641.
Lock every family row up front (claim with mkdir / echo NF221; locks reading `released` are free). Prove the recipe on 2-3 rows, script
it, grade the rest in payloads of 20-30. Hand back at a row boundary with the census written down if the family is larger than your
context allows. Report briefly when done or when your context gets heavy (hand-back files current first).

## Evidence: all three PDBs are on the lane now (owner, 2026-11-22: "you can also check other pdbs for evidence")
`$S/dl_debug/files/Ego_d.pdb`, `$S/dl_debug/files2/Ego_r.pdb`, `$S/dl_debug/files2/FableWin.pdb` (from the owner's shared "debug tlc"
folder; never commit them). Use them directly (the pass-26 package tools `pending/2026-11-21_codex_pass26/tools/egod_*.py` read a PDB
path; the ghidra_out/*_pdb_names.tsv dumps are only summaries) for names, signatures, access levels and class layouts: two sources
per name, as always.
