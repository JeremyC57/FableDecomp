# AUTH15 (2026-11-22): make 0x008E18D0 UpdateLightningBeams land-ready (it already matches), then STOP

Locks read AUTH15. Outputs ONLY under `$S/w22/auth15/008e18d0/`. Never write into /home/user/FableDecomp.
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`.

## Read first (only these)
- `rebuild/candidates/manual/pending/2026-11-22_auth14/008e18d0/README.md` (the state: e5n.cpp RELOCATION_MATCH at `/Gr`, default inline
  threshold, overlay `hdr.diff` = the add-only IMPLICIT_DTOR opt-in in engine/NParticleEngine_CParticleUpdateParameters.h), its LOG.md,
  DECISIONS.md, and `pending/2026-11-22_auth11/008e18d0/MEASURE.md` (the Ego_d line comparison).
- `$S/w22/auth1/BRIEF.md` "Paths", "Tools", "Hard rules", "Hand-back"; CLAUDE.md; EXTRA_RULES; docs/BUILDING.md (header proof:
  pin_asserts / assert_names_check / equiv_check in tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/).
- Working files: `$S/w22/auth14/008e18d0/` (e5n.cpp, hdr/, grade_t74.sh).

## Do, in order
1. **Flags.** Set e5n's `// Flags:` line to `/Gr` (it says none). Cite Ego_d's compile line for creature_modes.obj (-Gr in the fablelib
   preset). Check the flags landed TU-mate 0x008DD7E0 carries (build_candidates.ps1 / its `// Flags:` line). If they differ from `/Gr`,
   regrade it once at `/Gr` and report; do not change it.
2. **The opt-in's evidence.** Read the PDB method list for CParticleUpdateParameters<ulong,ulong>'s destructor in Ego_d / Ego_r / FableWin:
   compiler-generated, "marked inline", or user-declared out of line? Name and comment the opt-in to match the evidence (add-only;
   no change to the default path). Then regrade once, with the opt-in defined and no re-fit, every landed row that binds 0x006E2500 and
   the sibling 0x007A1380. A class's destructor is implicit everywhere or nowhere: list any that break (do not change them).
3. **Behaviour test** `008e18d0_test.cpp`: self-contained (C++03, /Od /W3, no /EHsc, include/ only), a case per condition in the row,
   printing `008e18d0_TEST PASS`; no local model of a class that has a header.
4. **NOTES.md**: identity (leaf, both PDBs), every correction with its Ego_d evidence (C1-C4 from MEASURE.md), and the remaining Ego_d
   differences with their reason: layout +3 at lines 3279 / 3305 (member offset), the four IsDebuggerPresent assert lines, and the
   unexplained +10-B prolog (frame is about 552 B, so not __chkstk). Label Inferred spellings.
5. **Checks:** grade with q.py at `/Gr` and the overlay, then a vland dry run with LANDVERIFY_STRICT_HEADERS=1 (payload with the source,
   test, leaf, flags `/Gr`, pass pattern). Run the header proof on the opt-in (pin_asserts, assert_names_check, equiv_check old v new
   include, then recheck the rows it lists).
6. Hand back: source, test, NOTES.md, MATCHES.tsv, hdr/ + hdr.diff, the proof logs, STATUS.tsv. STOP and report.
