# research861 (2026-11-08): Fable research agent on VC7.1 13.10.3077 register / scheduling / x87 / frame-slot ties

The user asked for research into the residues that park our near misses. The report is `docs/pipeline/VC71_CODEGEN_RESEARCH.md`. This
directory holds its reusable tools (`tools/gr.py` grader, `tools/tu_flag_check.py` per-TU flag admissibility check, symbol / path / flag
sweeps), the micro-TU probes behind each finding (`probe/`; `probe/mt.py` compiles one and prints its disassembly), c2.dll's switch table and
assertion-site addresses (for a future debugger probe of the allocator), `STATUS.tsv` (29 parked rows regraded at their baselines; none
moved) and `LESSONS.md` (16 one-liners).
