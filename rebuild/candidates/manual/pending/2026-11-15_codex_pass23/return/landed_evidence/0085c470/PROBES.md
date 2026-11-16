# 0085c470 probes

Historical local-model attempts preserved in package prior/: 2026-10-22 hand291, 2026-10-27 hand620, 2026-10-30 retry2, 2026-11-11 Q7. Their template and ground accessor stand-ins were discarded before new work.

| Attempt | Lever | Result |
|---|---|---|
| sol_baseline_localmodel | Diagnostic only, exact emitted historical local leaf | DIFFER857v857,4strict hunks,loose1.0; wrong public/struct-local identities |
| sol_port01 | Shared CBase/CMoveOnCylinder/CThing/C3DVector/CTCHitLocations; original profile/def/physics getters; genuine PeekTC | DIFFER857v857,6strict/4loose hunks, firstdifference764 |
| sol_port02_ground_accumulate | Ground call in its own statement, then += def height offset | MASKED_MATCH857v857,241instructions,0hunks |
| sol_port03_original_localnames | Original both-PDB local names; lightweight shared WorldMap query opt-in, pinned existing assertions | MASKED_MATCH857v857,241instructions,0hunks |
| sol_final01 | Final private headers including original ctor declarations for normal-lifetime fixture | MASKED_MATCH857v857,241instructions,0hunks; full23-relocation auditPASS |

Test sol_native01: VC7.1RTM /Od /W3 without EHsc, compile/link/runPASS. Final sol_native_final01 repeats against immutable final graded include tree. GCC64 C++03 same real-header fixture PASS. Tests exercise96 combinations of old/new profile, both inversion axes, sensitivity clamping, height clamping/ground floor; null/dead targets return with no work. Test doubles retain real shared classes and normal object lifetimes.
