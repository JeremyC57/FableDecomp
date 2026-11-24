# C2T2: lane tools agent (2026-11-22): apply the committed lk tools to 0x00B4D370 and 0x00BD3070

Advisor ruling 2026-11-22 (docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md, the C2T1 entry). Tools: `tools/decomp_pipeline/linux_lane/
lane_tools/c2probe/lk/` (README first: its Scope section says what the readout does and does not predict). C2T1's logs (JSON, objects) are
in `$S/w22/c2t1/logs/` (read only; copy what you need). `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`.
Outputs ONLY under `$S/w22/c2t2/`. Never write into /home/user/FableDecomp; never run git, not even read-only. Wait on jobs by PID (kill -0),
never pgrep -f. No rm -rf on variable paths. Disk is tight (~3.5 GB): keep at most two lk logs at a time, compress or delete old ones.
Run the tools from the repo copy (`source .../lk/env.sh`; runs go to $S/c2lk_runs), not from c2t1.

## Steps, in order
0. Control: rerun lka on C2T1's p8 v p8b logs with the final lka; it must give 0 flips. If not, stop and report.
1. **0x00B4D370, no new run.** For each remaining residue site (AUTH20 ESCALATE.md / AUTH21 NOTES.md list them; p9 =
   `$S/w22/auth20/00b4d370/best_evidence.cpp`), compare p9's final tool order with p9's own EMITTED operand order. Agree = addressable;
   disagree = the tool says nothing there (look up what the wfA6g notes found decided such sites). Table: site, tool order, emitted order,
   retail order, agree?
2. **0x00BD3070, one lk run** on AUTH21's c6 (`$S/w22/auth21/00bd3070/00bd3070.cpp`, overlay `hdr/`, stock flags; copy, do not edit).
   The same check on all 13 residue sites (ESCALATE.md lists them), then fsolve for the flips retail needs (F / T solution sets with gaps).
3. **Terminal standard.** If fsolve gives a shift and the allocator hook can force it, force it on the REAL row and grade (q.py): a
   RELOCATION_MATCH proves a numbering tie (outcome (A) unless PDB evidence supplies the text). An empty solution set = not a uniform shift:
   report the conflicting sites.
You do not author source candidates. A forced run is an oracle, never a landing.
Hand back: REPORT.md (per row: the site table, fsolve result, forced-oracle grade), LOG.md, any tool fixes as patches against the repo copy.
