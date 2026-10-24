# Parked-row diagnosis by residue class (2026-10-24, w22/diag)

Index: `INDEX.tsv` -- 2,755 distinct parked addresses from 2,847 differ files in 305 batches; 978 are already landed
(skipped), 28 are the landed-but-wrong de-bake / bounds_landed_differ rows, 1,749 are genuinely parked. 1,777 rows
carry a class and a diagnosis (100 %); 53 have prior retry attempts (retry11/12, de-bake A/B/B2), 25 appear in more
than one batch. Bounds: 1,739 OK, 2 OVERLONG (007ae3d0, 00c72200), 36 not in the audit (the de-bake rows and chunk-337
rows graded from the exe).

Classes are assigned from the parked header's `// Grade:` text (`mkindex.py classify`, priority order below); the
class-level diagnosis sits in `INDEX.tsv` column `diagnosis` for every row, and per-row notes for the rows worked here.

| class | rows | root cause | fix found | fixed here | not fixed, why |
|---|---|---|---|---|---|
| register-tie | 646 (279 equal length) | callee-saved / scratch register choice is a use-weight tie; VC7.1 prefers esi > edi > ebx > ebp by use count, scratch regs rotate from the first load | only indirect levers (GOTCHAS: move one use, named locals, de-bake B2 stand-ins, real STLport iterator) | -- | no source-level control of the allocator; 00675e20 (register rotation) is inert even with the callee bodies visible |
| stack-slot/frame | 279 | frame-slot assignment and dead-argument-slot homing depend on what VC knows about the callee that takes the address (IPA on same-TU bodies) | **GOTCHAS 24l real-body recipe**: give the out-of-TU callee that takes an empty tag / traits object its REAL body in the TU as `__declspec(noinline)` (no `auto_inline(off)` -- the pragma also pushes in-class traits ctors out of line) | **007ae3d0 (1104 B), 0046c3b9 (119 B)**; improved 00830fe0, 00756d00, 007be05c | the recipe only moves the home when the object is unread AND the callee's IPA summary is clean; a live-slot home inside an inlined dtor chain (00a98760/7b0) and the "which live slot" choice (00756d00: value.second vs the CMapValue temp) are still open; 00768a70/00769340/008068b0 (16-byte swap temp kept in memory) resist every copy-ctor form |
| x87-order | 265 | VC7.1 canonicalises commutative x87 operands and reassociates sums by operand memory class; stack-vs-memory residency of locals | known levers only (double locals, per-use `(float)g`, `x /= l` pairs, `__forceinline` float helpers, chunk 293 notes) | -- | the parked rows already tried those; the remaining residues (dup older/newer product, cross-product term order) have no source form |
| block/arm-layout | 219 | basic-block placement: case order, nested positive ifs vs early returns, merged `case N:`/`default:`, shared vs duplicated fail tails | GOTCHAS recipes (case order = retail arm order, named bool flips, merged labels) | -- | each row is individual; the 40v43 twins 00681790/006817c0 (def tested through eax then re-read) and 006817f0 (`sub eax,2` vs `add eax,-2`) are open |
| ltcg/crt/asm-impossible | 210 | 109 bare dword `fistp` / `fnstcw` / `rdtsc` / `cpuid` rounding helpers, 20 SSE blocks, 13 LTCG custom conventions (libogg/libvorbis), CRT hand-written asm (`__SEH_prolog`, `__security_check_cookie`) | the fistp/rdtsc/fnstcw rows are `retail_asm` candidates under the owner's 2026-10-24 exception (minimal `__asm` + `// Retail-asm:` evidence); SSE blocks pending the owner's decision | -- (no `__asm` allowed in this task) | LTCG and CRT rows are permanently asm |
| incomplete-model | 119 | not a residue: first-pass stubs / out-of-budget drafts (e.g. 22v1081, 37v1330) | fresh reconstruction | -- | needs RE work, not a codegen recipe |
| wrong-flags/pragma | 14 | /GS, /Os, /O1 TU shape not yet confirmed or confirmed with remaining frame residue | `// Flags:` line; `/O1` for CRT rows; `optimize("s",on)`+`("y",off)` for size TUs | -- | after the flags the residues are register/frame |
| eh/unwind | 7 | EH frame / state-store shape | -- | -- | 00c14b50/00c14bc0: retail keeps an EH frame with ONE `-1` state store and no state-0 store although no call can throw in the lock scope; inline accessor, same-TU non-inline helper (back end inlines it, frame dropped) and the QFE compiler all give 66v109 -- a dead-state-store elimination this lane never reaches |
| bounds | 6 | catalog row extent (OVERLONG / mid-start / jump table) | re-cut with `rebound_landed.py` / `$S/bounds/oracle_corrected.tsv`, grade on the exe span | 007ae3d0 graded on its corrected 1104-byte extent and matched | 00c72200 (737v741 on the corrected span) is a real residue |
| inline-vs-call | 6 | inlining decisions | `__forceinline`, `auto_inline(off)` + forceinline copy; **explicit inline dtor + `__declspec(novtable)` makes a `??_G` inline the whole destructor chain** | improved 00a98760 / 00a987b0 (30v80 -> 62v80) | the iterator tag's live-slot home remains |
| load/schedule-order | 5 | load vs push scheduling | named locals / inline accessors (GOTCHAS 00a3e0d0) | -- | the AddTC family (0072a970 +7 twins: `this` loaded into ecx before the constant pushes) is inert to implicit conversion, smart-pointer `operator->`, a forceinline template helper, /G3 /G5 /G6 /G7 /Ob1 /Ox and the QFE compiler |
| wrong-model/layout | 1 | struct model | re-derive offsets | -- | -- |

## Recipes confirmed or refined in this pass
1. **24l real-body stand-in for empty traits/tag objects** (fixed 007ae3d0, 0046c3b9; improved 00830fe0, 00756d00): the
   declared-only callee that receives the empty object's `this`/reference gets its real shape in the TU as
   `__declspec(noinline)`. A forwarding or side-effect-only body is NOT enough (0095d4c0: the empty-body call is elided;
   00a98760: `g = last` leaves the tag in its own byte). Do not add `#pragma auto_inline(off)`: it pushed the in-class
   `CPersistTraits<long>() {}` constructor out of line (0046c3b9 v1: `call ??0?$CPersistTraits@J@@QAE@XZ`). With the body
   visible VC7.1's IPA also deletes dead stores through the traits (00632830 lost its `DefaultVal = K()` store, 194v196),
   so the recipe is only right where retail shows the same dead-store elimination.
2. **De-bake B2 stand-in (same-TU noinline address-taking callee)** moves a named local into the dead argument slot
   (007be05c: `effect` -> [ebp+8] as retail) but cannot stop a later temporary sharing that slot.
3. **`??_G` destructor chains**: a class with no declared destructor leaves its inline base destructor as a call; declare
   `~T() {}` inline and mark the class `__declspec(novtable)` to inline the chain without a vtable store (00a98760/7b0).
4. **Flags/compiler sweeps are inert for scheduling residues**: /G3 /G5 /G6 /G7 /Ob1 /Ox and the QFE 4035 compiler left
   0072a970 (this-load order) and 00c14b50 (EH frame) byte-identical to the RTM /O2 build.

## Per-row work log (`STATUS.tsv`)
MATCH: 007ae3d0 (hand321, OVERLONG row re-graded on 1104), 0046c3b9 (hand331).
IMPROVED (differ/): 00830fe0 (residue 1 of 2 gone), 00756d00 (1120 -> 1116, one slot), 007be05c (154 -> 153),
00a98760 / 00a987b0 (30 -> 62 of 80).
DIFFER after fresh forms: 0072a970 (+7 twins), 00c14b50 / 00c14bc0, 00768a70 (+2 twins), 0095d4c0, 007dda30, 00675e20,
006d8f10.
