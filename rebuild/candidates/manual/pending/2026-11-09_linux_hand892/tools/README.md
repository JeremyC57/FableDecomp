# Chunk 892 tools (scheduling / test-or family)

Recovered from the owner's 2026-11-09 tools bundle (`fable_matching_tools/sched_family_chunk892/`); they were not in the original
hand-back. Paths inside point at the old `5221bbec-...` lane scratch: set `S` to the current lane and edit the path lines first.
- `sched/sim.py`, `sched/t11.py` / `t13.py` / `t14.py`, `sched.sh`, `showdag.py`: replay c2's list scheduler (two issues per cycle,
  c2.dll 0x1078734C; 81-tuple regions; no-code PAREN tuples count) and print each cycle's choices and the dependency DAG.
- `c2prio_op.py`, `tor.py`: commutative operand order by candidate id (`test` / `or` / `add`), the 0x008A8010 / 0x00B6FA40 lever.
- `c2.sh`, `c2dbg.py`, `c2dis.py`, `tup.sh`: c2.dll breakpoint harness, disassembly and IL tuple counts.
- `gq.py`, `gr.sh`, `batch.py`, `var.py`, `cmpvar.py`, `cmpfn.py`, `mc.py`, `rdis.py`, `xref.py`: grading, variant batches,
  function / variant comparison, retail disassembly and xrefs.
Method and measured rules: docs/pipeline/MATCHING_PLAYBOOK.md ("Scheduling regions and capture").
