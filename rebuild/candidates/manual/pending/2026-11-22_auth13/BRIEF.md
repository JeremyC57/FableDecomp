# AUTH13 (2026-11-22): 0x00CF4EC0 CFanMarker::Main, the four checks of the advisor ruling, then (a) pre-registered, then STOP

Owner orders: do not park; stalls go to the advisor through the coordinator. Locks read AUTH13. Outputs ONLY under `$S/w22/auth13/00cf4ec0/`.
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Never write into /home/user/FableDecomp.

## Read first (only these)
- The ruling: last entry of `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` ("AUTH12's 0x00CF4EC0 escalation"). Binding.
- AUTH12's hand-back: `rebuild/candidates/manual/pending/2026-11-22_auth12/00cf4ec0/` (ESCALATE.md, LOG.md, 00cf4ec0.cpp = p6d, hdr.diff)
  and its working files `$S/w22/auth12/00cf4ec0/` (inl/, od/, tools/). AUTH10's inline tools `$S/w22/auth10/tools/` (READOUT only).
- `$S/w22/auth1/BRIEF.md` "Paths", "Tools", "Hard rules"; docs/BUILDING.md "VC10 SP1"; NF222's pick-any checker
  `rebuild/candidates/manual/pending/2026-11-22_nf222_handback/tools/pk.py`; AUTH9's `pending/2026-11-22_auth9/tools/leafrel.py`.
- PDBs: `$S/dl_debug/files/Ego_d.pdb`; `$S/dl_debug/files2/{Ego_r,FableWin}.pdb` (symbols dumps: `$S/codex27/pdbx/*.symbols.txt`).

## Steps (write CHECKS.md as you go; no grading hunt before step 5)
1. The 5-B Ego_d misfit in each fan hit block (Ego_d 541 / 561 v ours 536 / 556): find the one instruction (a 32-bit-operand call, jump,
   immediate or global) and whether those blocks hold the seven movie-dtor sites. Correct the text only with Ego_d evidence.
2. Is the movie class's destructor compiler-generated? Read the PDB method lists (Ego_d / Ego_r / FableWin). Measure that dtor, GROBase's
   and CCountedPointer<MovieBase>'s with odall10 (calibrate as AUTH11 did for GetTC).
3. Does the row's movie-dtor COMDAT equal retail at 0x006E7B80 (leafrel / pk.py)? Does retail's body call ReleaseData or carry it?
4. Falsify "cost <= 13": xref 0x006E7B80 in Fable.exe; a call from straight-line code (not an unwind funclet) in a landed row graded with
   stock inline flags kills (a): then write OWNER_PACKET.md and STOP. Confirm the one-TU premise from Ego_d's compiland record. List the
   landed rows that carry /d2inlS15 and use this class.
5. Only if (a) survives: write the capped candidate list (at most 4 shapes) in LOG.md BEFORE the first readout; pick ONE by readout on this
   row only, preferring a shape steps 2 / 3 support. Then grade once each: this row at stock S9; 0x00CF2B70 and 0x00CF3D20 with /d2inlS15
   removed (no re-fit); the other header users (equiv_check over the changed header, then recheck the differing rows); the dtor copy v
   0x006E7B80. All pass: land-ready hand-back (also grade the local EScriptAIPriority enum under LANDVERIFY_STRICT_HEADERS=1, and check
   the CTimer forcing is byte-identical with a row-local body). Any fails: OWNER_PACKET.md with the forced oracle. STOP and report.

## Owner override (2026-11-22)
Owner: "have advisor decide". Where step 4 or step 5 would write OWNER_PACKET.md, write ESCALATE_ADVISOR.md instead (same content, options laid out); the coordinator takes it to the advisor, whose ruling is final for this row.
