# AUTH12: Account A authoring agent (2026-11-22): 0x00CF4EC0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main, ONE row

Owner orders: do not park; stalls go to the advisor through the coordinator. Narrow brief by design. Locks read AUTH12.
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Outputs ONLY under `$S/w22/auth12/00cf4ec0/`.
Never write into /home/user/FableDecomp. Residue counts and differing hunks only.

## Read first (only these)
- `$S/w22/auth1/BRIEF.md` "Paths", "Tools", "Hard rules", "Hand-back"; CLAUDE.md; EXTRA_RULES "shared headers"; docs/BUILDING.md "VC10 SP1".
- Advisor ruling 2026-11-22 (last entry of `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md`): port onto the shared headers BEFORE
  any more inline forcing (inline scores depend on helper body sizes, so readings on row-local models do not carry over).
- History: `rebuild/candidates/manual/pending/2026-11-22_auth10/00cf4ec0/` (HANDOVER.md, LOG.md, c0.cpp = AUTH1's draft, DIFFER 6244v6164
  on row-local models) and `pending/2026-11-21_auth1/` for this row. Landed sibling quest-script rows (other NScript::CQ_* Main rows in
  rebuild/src/compiled) show which headers and macros the family uses: copy their include set and opt-ins.
- PDBs for names / locals: `$S/dl_debug/files/Ego_d.pdb`, `$S/dl_debug/files2/{Ego_r,FableWin}.pdb` (repack4k.py first).

## Steps
1. Port c0.cpp onto the shared headers (no local model of a class that has a header; the three-condition rule otherwise). Grade
   (stock VC7.1, rg1.sh / q.py). Report the residue.
2. Then measure the remaining hunks (AUTH10's two: the top SetPredicate(CIsThingAlive(Me)) temporary's release, the loop-1 movie exit
   pads) against Ego_d line sizes with odall10.py, correct only with Ego_d evidence, grade.
3. If it matches: land-ready hand-back (source, test, NOTES, MATCHES.tsv). If not: HANDOVER.md or ESCALATE.md with the inline readout
   (AUTH10's tools in `$S/w22/auth10/tools/` are allowed for READOUTS only, on the ported text). Then STOP and report.
