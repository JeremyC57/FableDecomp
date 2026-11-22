# AUTH11: Account A authoring agent (2026-11-22): two bounded measurement steps, then STOP and report

Owner orders: do not park; stalls go to the advisor through the coordinator. This brief is deliberately narrow (two agents in a row ran
out of context on six-row briefs). Locks read AUTH11. `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`.
Outputs ONLY under `$S/w22/auth11/<addr>/`. Never write into /home/user/FableDecomp. Residue counts and differing hunks only.

## Read first (only these)
- `$S/w22/auth1/BRIEF.md` "Paths", "Tools", "Hard rules"; CLAUDE.md; docs/BUILDING.md "VC10 SP1"; odall10.py docstring.
- The advisor rulings of 2026-11-22: last entry of `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md`. They are binding.
- PDBs: `$S/dl_debug/files/Ego_d.pdb`; `$S/dl_debug/files2/Ego_r.pdb`, `FableWin.pdb` (repack4k.py from
  `rebuild/candidates/manual/pending/2026-11-22_codex_pass27/tools/` before llvm-pdbutil).

## Step A: 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (do this first)
History: `rebuild/candidates/manual/pending/2026-11-21_auth7/00a4efc0/ESCALATE.md` and AUTH7's files in `$S/w22/auth7/00a4efc0/`
(best b3_00a4efc0.cpp, overlay hdr/, v10/ model, the order readout kit). AUTH2's users of AllocateType: 0x00A50510 / 0x00AA0FA0 / 0x00BD2D90.
1. The check (no grading): in Ego_d, is AllocateType's block local `overflow_check` used by anything besides the 261-B debug assert? What
   do Ego_r.pdb and FableWin.pdb show for AllocateType<T> (S_GPROC32 / locals / inline sites, the parameter name)? Write CHECK.md.
2. If the local is assert-only, or absent from Ego_r / FableWin: STOP. The row goes to the owner; write OWNER_PACKET.md (b3 is the
   Ego_d-exact text, residue 16 is a proven c2 order tie, no calibrated freedom left).
3. Otherwise, pre-register in LOG.md BEFORE grading: b3 + Ego_d's exact AllocateType shape (parameter `elements`, block local
   `overflow_check`) as an add-only header opt-in. Grade the row once and read the order once. Grade the three users once each with the
   SAME header shape, no re-fit. Acceptance (fixed now): the row flips to MATCH / RELOCATION_MATCH AND all three users still match. If it
   is accepted, write a land-ready hand-back; if not, write OWNER_PACKET.md. Either way, STOP this row.

## Step B: 0x008E18D0 UpdateLightningBeams, measurement only (no grading hunt)
Files: `rebuild/candidates/manual/pending/2026-11-22_auth10/008e18d0/` (ESCALATE.md, e4.cpp, hdr.diff) and AUTH10's tools in
`$S/w22/auth10/tools/` (read tools/README.md first) plus `$S/w22/auth10/008e18d0/`.
1. Measure e4 with odall10.py (the row's Ego_d library preset) against Ego_d line by line: every line, plus the wrappers on the path to
   the four forced sites. Calibrate first on Ego_d's out-of-line GetTC<CTCDParticleEmitter> (74 B). Report the lines that differ,
   starting with the unexplained 27 B at line 3297, and the sky line 3358 (49 B) / 3283 (44 B) / 3285 (26 B).
2. Check the for-lines against the VC6 for-scope idiom (`if (false) {} else for (...)`, GOTCHAS 442): does Ego_d's for-line size match
   it, and does it move inl.py's f at the four sites (inl_force.py readout)?
3. Every correction needs its own Ego_d evidence. Apply all of them as ONE set, grade once (stock VC7.1, no forcing), and report.
   Do NOT start the c2 decision-routine hunt; the coordinator decides that after your report.
Write MEASURE.md with a table (line, Ego_d bytes, e4 bytes, corrected bytes) and the one grade. Then STOP and report.
