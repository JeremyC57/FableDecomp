# T3 queue stale-line sweep (agent NF342, Account A, 2026-11-24)

Agent NF342 read the whole of T3_PORT_QUEUE.tsv (3,742 lines, plus 99 that NF340 appended while it ran) for lines a later landing had already
fixed. Landings that went in through apply_rows did not use `land_scripted.py --release-t3`, so those lines never closed. The agent's own
summary is `AGENT_SUMMARY.md`; its tools are in `naming_batch/agent_tools/nf342/`.

## Result
- **263 lines stale** (on 189 addresses), each proven by a HEAD compile.
  - 249 self lines: the row defines and grades the PDB symbol, and no named stand-in remains.
  - 14 binder lines: every named binder binds only the PDB symbol, and the target row defines it.
  - The coordinator closed all 263 by content hash; all 263 matched (`T3_CLOSED_STALE.tsv`).
- **15 partial** (`PARTIAL.tsv`), **79 compiled and still open**, **2,712 open by pre-filter** (not compiled), **762 not decidable by
  script** (clusters, research, debt, held, model ports), **10 with no exact PDB target**.
- **Calibration:** all 62 lines at NF339's 30 stale addresses came out stale. All 28 negative controls came out open.

## Rulings (coordinator)
- **D1:** close the 263 without a landing. Accepted, as for NF339's 138.
- **D2:** the five nf324 CIEngine held lines were already "done" when NF324 wrote them. They are among the closed lines.
- **D3:** partial lines stay whole. Each keeps its remaining half as the open reason; `PARTIAL.tsv` records what is done.
- **D4:** lines about strong same-TU copies stay open while their binders still define P as strong copies.
- **D5:** the two stale lines whose three files pass only `cl /Zs` (clang errors on MSVC-only syntax) are accepted under NF305 Q2, the same
  precedent NF339 used.

## Follow-up
The T3 queue drops from about 3,840 lines to 3,578. To stop new stale lines piling up, every landing whose rows also appear in T3 lines should
run through `land_scripted.py --release-t3 <tags>` (multi-line release since NF339 D6), or close its lines by hand.
