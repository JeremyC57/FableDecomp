# Probe results (VC7.1 RTM cl 13.10.3077, `cl /nologo /c /O2 /Oy /W3 [extra] /Fo<obj> <src>`, Wine lane)

Tools (all in `$S/inlinesplit/`): `cc.sh <src> [flags]` compiles one TU (120 s timeout) and writes `<src>.asm` (objdump -dr);
`calls.py <asm> [func]` lists each function's call targets; `meter.py <src> <callee> <mark> [T ...]` compiles at each hidden
threshold `/d2inlT<T>` (`-` = default 74) and prints per function the site pattern (`i` = inline expansion, seen as a call to
MARK; `C` = call to CALLEE); `probes/p07_freqmeter/tstar.py <src> <callee> <mark> lo hi step` gives each site's smallest T
that inlines it (T* = floor(score)+1). `/d2inlT` is a diagnostic only (a hidden c2 knob), never a landing flag.

Marker callee used by most probes: `inline void Release(Handle&)` (CCountedPointer-style release on a global handle;
its expansions show as calls to `operator delete`). Calibrated cost c = 49 (T* = 50 at function entry).

| probe | question | result |
|---|---|---|
| p00_basic/p00.cpp | plain-inline releasing dtor of a local at 2-4 straight-line exits | f2 (exits 0.5,0.25,0.25): all inline; f3: first 2 inline, last 2 (0.125) one merged call; f4: first 2 inline, rest call |
| p01_plainfn/p01.cpp | same callee as a plain function: 5 straight-line sites vs 5 exits | s5 (all f=1): 5/5 inline; e5: exits 1-2 inline, 3-5 call |
| p01_plainfn/p02.cpp | one site behind d nested ifs | d0-d2 inline, d3-d5 call; site in a loop behind 5 ifs: call |
| p03_size_vs_freq/p03.cpp | callee size (k+1 stores) x depth d | k=0 all inline; k=2 up to d4; k=4 d3; k=6 d2; k=8 d0 only; k>=10 never. /d2inlT1000 inlines nearly all, /d2inlT20 almost none -> the threshold is inlT (74) |
| p04_freqshapes/p04.cpp | T* by shape | d0 50, d1 57, d2 71, d3 >74 (call), d4 ~150-250; loop bodies 35-40 |
| p05_callersize/p05*.cpp | caller size | padding with N extern calls: no effect up to N=4400; at N>=4600 (~9000 IL) every site is a call (d0 too). 8 sites just under the cap: first sites (source order) inline, later ones call (p05d); a counted loop before the sites lifts the cap (p05e/f, unexplained); while/goto loops do not (p05g) |
| p06_wrapper/p06.cpp | Hypothesis 1: inline wrapper layers W1..W10 | all inline at every depth 1..10, at d0 and d2 -> layering is not a lever |
| p07_freqmeter/p07.cpp | T* per shape (fine) | entry 50; if 57; if-if 71; else 57; `&&` 71; `\|\|` 52; null/==/</!= tests all 57 (no branch heuristics); for(i<N)/for(i<3)/for(i<100)/pointer loop body 36; while/do/for(;;) 35; nested 34; in-if-in-loop 40; after a guarded for loop 36 (!); return inside for(;;) 50; after break 50 |
| p07_freqmeter/p08.cpp | the post-loop "leak" | after for(i<N) or pointer loop: 36 (= loop body); after while(Test): 50; second sequential loop body: 34 (compounded); site after two loops 34 |
| p07_freqmeter/p13.cpp | returns inside for(;;) | top-of-body block 35 whatever the number of returns; 1 return 50 (f=1.0); 2 returns 57 each (0.5); 3 returns 63-64 each (0.33), whatever their nesting depth |
| p07_freqmeter/p14.cpp | 5 returns: nested chain, flat chain, straight line | in a loop: every exit 78 (0.2), nested or flat; straight line: 58, 72, 100, 156, >260 (geometric) |
| p07_freqmeter/p15.cpp | goto / while / do-while / continue loops | equal shares again (4 exits 72, 5 exits 78) |
| p07_freqmeter/p18.cpp | `do { } while (0)` | it is a loop to the estimator: a site inside scores 35-36 (f>1); returns wrapped in do-while(0) inside for(;;) score 36, 40, 46, 54 in program order (declining, NOT equal shares); unwrapped returns of the same loop 57 |
| p09_nestedcost/p09.cpp | is cost measured after the callee's own inlining? | X3 = 3 x Y scores 97 = hand-expanded Z3 98; X1 = Y 34 -> yes, post-expansion |
| p09_nestedcost/p10.cpp | pre- or post-optimisation size? | dead code / redundant stores / named temps / struct temp: all 34 = Y; byte-wise stores 67 -> optimised size counts |
| p09_nestedcost/p17.cpp | callee defined after the caller | same cost (97) -> c2 compiles bottom-up; definition order is no lever |
| p11_scriptthing/p11,p12 | CScriptThing by-value / local dtor | /d2inlT also changes ~CCountedPointer's inlining into ~CScriptThing, so T sweeps on nested callees are confounded; at default: 2 straight returns 'ii'? (s3 'iC', s6 'iC'), loop with 1-2 returns inline, 3+ returns all out of line (equal shares) |
| p16_refcount/p16_*.cpp | do other references in the TU change a site? | 0..16 extra referencing functions: no change (inline and static callees) |

Formula reconstructed from c2.dll (`0x1078d1aa`, non-POGO path) and confirmed by the probes:
`score = (c * 1e8 / f + (k-1) * c) / k` with f the site's static frequency in 1e8 units, k = 7 (`-inld`) when f <= 1.0 and
k = 3 (`-inlld`) when f > 1.0; inline iff `score < 74` (`-inlT`). c = optimised callee size minus per-site argument savings
(`0x1078d03c`), minus 7 for a user-`inline` callee, plus `(size/2)*callerSize/9000` once the caller exceeds 9000.
`__forceinline` skips the test (`0x1078cfc4`: callee flag 0x2000 -> accept).
