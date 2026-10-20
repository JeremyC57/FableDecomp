# Retry 10, first landing (2026-10-20)

Fable 5.1 retry pass over every parked row (1,112 at the pause) under the RTM compiler, three strategies: callee bodies in
the TU, engine headers instead of local models, sibling functions as one TU; five attempts per strategy per row.
Paused by the owner at 82 rows worked; the 34 matched rows (tests written, STATUS.tsv) landed here with
`verify_and_land.py --land`. Winning idioms (LESSONS.md): inline-once destructor on the loop return + out-of-line at the
end (00d17090, 00e32200); hand-split `== ""` with goto-joined true exits (008bb130 family, 00892cf0, 00811100, 006820e0);
STLport `_Vector_base` with an `_STLP_alloc_proxy` end-of-storage model for def constructors (00458c19 family, 7 rows);
`ImpulseVelocity.Set(0,0,0)` as an inline member storing Z,Y,X (00724f80, 00724fc0, 007268d0); CAnimInit temporary
argument of SetMainAnim (008d6780 family, 5 rows); `DecRefToZero(long&)` forceinline release helper (00591e8c; closes
most of the long-release gap on 16 more rows, 2–8 bytes left: the pointer is loaded into eax before the dec);
four CInputProcess rows matched unchanged under RTM + /GS. The pass resumes from STATUS.tsv in `$S/w22/retry10`.
