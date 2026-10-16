# Retry batch 7 (Fable 5.1, 2026-10-16): 8 near-miss parked rows from chunks 142-154
- 0 of 8 matched. 00a17620 (3 tries) now stores the reach into the `radius` parameter slot like retail; only the x87 operand order (member first) remains.
- 00438ff0, 0041d03c, 009e42e0: one try each, no gain (register rotation or no change).
- 008d0190, 00c1c3f0, 00987bf0, 00720610: not retried -- register swap, LTCG custom convention, FP chain reassociation, pre-frame load.
