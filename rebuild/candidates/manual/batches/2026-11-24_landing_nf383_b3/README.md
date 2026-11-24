# Landing nf383 b3: three std instances take their unsigned element types

- 0x0050E500 `fill<short*,short>` -> `??$fill@PAGG@std@@YIXPAG0ABG@Z` (unsigned short).
- 0x00A326D0 `pair<long,C3DMeshVolume>::~pair` -> `??1?$pair@KVC3DMeshVolume@@@std@@QAE@XZ` and 0x00A335E0 the matching `fill` (unsigned long). Sources: retail placement between two C3DMeshStats functions inside lib_3d_mesh_stats.obj, whose FableWin copy defines 144 pair<unsigned long,...> functions and 0 pair<long,...>; the landed callers' table spelling.
- Retail has no ICF, so the long / unsigned long twins elsewhere are separate functions. Tests respelled the same way; vland PASS HEAD and new, objsec ALL_EQ, pick-any and link gates clean. Catalog labels queued (NAME_FIXES_QUEUE nf383). T3 nf340 (0x005149F0) names 0x0050E500 in its reason text only (not a reservation).
