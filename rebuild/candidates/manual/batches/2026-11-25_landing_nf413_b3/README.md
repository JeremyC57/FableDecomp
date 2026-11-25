# Landing nf413 b3: fable_boot.h no longer declares g_FableEmptyText_0122D70E

This is the follow-up to nf413_b2: no row binds or names the symbol any more, so its one-line extern declaration is removed from rebuild/include/fable_boot.h.

The coordinator regraded the header's three includers from the repo (logs/includer_regrade.log):
- 004022B0: RELOCATION_MATCH
- 0049B460: MATCH
- 009E9FD0: RELOCATION_MATCH

All three tests pass.

NF413's matching deletion in rebuild/integration/stage2_engine_boundary.cpp line 59 is NOT applied, because the lane cannot build that file at HEAD. That definition now has internal linkage and no user; its removal is queued in T3 (nf413 integration).
