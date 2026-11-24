# AUTH16 (2026-11-22): 0x00CF4EC0 CFanMarker::Main, option (C) bounded, then the terminal state; STOP

Owner: "have advisor decide" for this row; the advisor's ruling is final. Locks read AUTH16. Outputs ONLY under `$S/w22/auth16/00cf4ec0/`.
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Never write into /home/user/FableDecomp.

## Read first (only these)
- The ruling: last entry of `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` ("AUTH13's 0x00CF4EC0 checks"). Binding, step for step.
- `rebuild/candidates/manual/pending/2026-11-22_auth13/00cf4ec0/` (ESCALATE_ADVISOR.md, CHECKS.md, xref_rows.tsv, clean_rows.tsv) and
  AUTH13's working files `$S/w22/auth13/00cf4ec0/` (oracle/, obj/, tools/); AUTH12's `pending/2026-11-22_auth12/00cf4ec0/` and
  `$S/w22/auth12/00cf4ec0/` (best p6d = 00cf4ec0.cpp, hdr/, inl/); AUTH10's inline tools `$S/w22/auth10/tools/` (READOUT only).
- `$S/w22/auth1/BRIEF.md` "Paths", "Tools", "Hard rules"; docs/BUILDING.md "VC10 SP1". PDBs: `$S/dl_debug/files/Ego_d.pdb`,
  symbols dumps `$S/codex27/pdbx/{Ego_r,FableWin}.symbols.txt`, modules `$S/codex27/pdbx/*.modules.txt`.

## Steps (CHECKS2.md as you go)
1. The pruning check: at one hit-block return, readout f for the expanded movie dtor and for the called ~CTimer. Same f or not? Ego_d's type
   of the object destroyed there v the class the 55 rows use; what the costs 19 / 22 are; are 0x00EB40C0's expanded and called dtors one class?
2. The 5-B instruments, in order: (1) PROC diff of Ego_d's register_scripts.obj against our VC10 object; (2) PDB parameter counts v our
   declarations for every function the macro calls (an extra imm32 / address argument is 5 B under -Gr). Stop at the first that names it.
3. If named: pre-register at most three candidates (each fitting Ego_d's line sizes at all 14 macro lines, this row and 0x00CF2B70; never
   chosen by readout) in LOG.md, then grade once each set: this row at stock; 0x00CF2B70 and 0x00CF3D20 without /d2inlS15; other users of
   any changed header (equiv_check + recheck). Bonus: regrade 3 of the 16 /d2inlS rows at stock, no re-fit. Pick-any per the ruling
   (existing same-bytes copies in landed rows = debt; a copy new to this row holds it).
4. If not named, or nothing passes: write TERMINAL.md = (A): the forced oracle as evidence, the row unlanded, no owner packet, and the one
   FableWin question for the next Codex package (instrument 3). STOP and report.
