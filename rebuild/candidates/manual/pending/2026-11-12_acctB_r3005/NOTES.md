# Hand chunk 3005 (C3DAnimationManager .. NUISystem::CTable): 0 of 2 byte-exact (near-miss retry pass)

Both rows start at real function starts and end at their own `ret` (checked by chunks 2001 / 2006 / 3001 / 3002; the oracle lengths are
right, no SPLITS.tsv). Every compile used `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3005/hdr` (union of the 3001 and 3002 overlays, no
change of mine: hdr/README.md). Both prior differ sources re-graded exactly as recorded (grade 1 of each row). Both rows stay parked; this
chunk's result is the measured mechanism of the 0x00A9EB90 tie and the proof that the 0x00550DC0 tie is not the r3004 symbol-count key.

Grades: 0x00A9EB90 1 (baseline), 0x00550DC0 1 (baseline). Passes 2 and 3 were not used for either row: no source, natural or not
counting diagnostics, reached retail's state in a free trace except dead-code diagnostics, so there was nothing to grade. Free work on
top: ~700 trace compiles (tools/my/k.py: our own object only, the deciding instruction read from it, a diff against OUR baseline object,
never against retail), ~25 c2 hook runs (tools/c2h/), ~30 packer / trace compiles on 0x00550DC0.

## Parked for Fable 5.1 (2)
- 00a9eb90 C3DAnimationManager::BlendAnimations DIFFER(1337v1337): 0x00A9EC5A retail `fld dword ptr [eax+edx]` (d9 04 10), ours `fld dword ptr [edx+eax]` (d9 04 02) | tried: outer dead-name sweep k=1..60, dead-statement sweeps by kind, 2-D grids (below), inner dead temps / names in GFFastSin, CalculateSlerpWeights, SetSlerp, natural spellings (named GFFastSin / GFFastArcCos locals, `scale` / `inv` reciprocal locals, `weights.Y *= sign`, Dot written out, SetSlerp through CalculateSlerpWeights, pointer-walking / named-reference scalar loops, sqrt spellings, an extra TU function, `start = GFGetCycleCount()` / `end = ...` in all four combinations, each combined with outer-name and dead-op sweeps) | next: a natural construct that holds about one chunk (32) more symbol records alive during the scalar-loop inlining (SetSlerp and its callees) up to loop strength reduction -- most likely the original nesting of SetSlerp / GFFastSin / GFFastArcCos -- see "Mechanism" below.
- 00550dc0 NUISystem::CTable::Draw DIFFER(1399v1399): retail 0x00550EFE `fstp [esp+74h]` / 0x00550F53 `fstp [esp+3Ch]`, ours 3Ch / 74h; retail 0x00550F59 `fstp [esp+38h]` / 0x00550F63 `fstp [esp+34h]`, ours 34h / 38h (20 masked lines with the reads) | tried: outer dead-name sweep k=0..16 (moves only a member-product operand order, period 8, never these stores), PDB member operator* (FableWin ??DCRGBFloatColour@@QBE?AV0@ABV0@@Z), converting constructor through operator=(const CRGBColour&) (??4CRGBFloatColour@@QAEAAV0@ABVCRGBColour@@@Z), both together (all byte- and slot-identical), parent colour as the named `colour` then `colour = CRGBFloatColour(Colour) * colour` / `colour *= ...` (235 / 383 lines), named table colour (123 lines) | next: chunk 3002's lead (a codeless IL reference that makes the parent temporary reach packer weight 2 before the table temporary, c2 0x10788642 / 0x10789A99), plus the alpha / blue qsort tie.

## 0x00A9EB90: mechanism measured (c2 hooks, free)
Tools: tools/c2h/sib.py (dumps the base+index memory nodes, type 0x253, at the allocator's block pass), cands.py (every integer
candidate with its symbol record id), recs.py (every symbol record with id and name), watch.py (hardware watchpoint: who writes a
field). tools/my/k.py (free compile, deciding instruction, diff vs our own baseline object), sw.sh / grid.sh / g2.sh / pos.sh / kin.sh /
ids.sh (sweeps; all diagnostics, nothing is delivered from them).
1. Symbol records (temps kind 3, locals kind 4, params 5, globals 7) are allocated by c2 in chunks of 32 records of 0x60 bytes; the chunk
   allocator (c2.dll 0x107603DD..0x10760419) numbers each record from a global counter (0x108A79D0) at chunk creation; records are reused
   through a free list, so a record's id is its pool slot, not its creation order.
2. The SSE loop's `fld [eax+edx]` node (0x253) holds the two operand candidates in a fixed order already at the allocator's block pass;
   the first operand slot (+0x38) gets the record with the SMALLER id MOD 1024 and is emitted as the SIB index. Base: cand 78 (to-pointer
   IV) id 1021, cand 79 (from - to offset, spilled) id 1064 -> 40, so the offset is first -> index -> ours `[edx+eax]`. In every variant that
   has retail's SIB, all three SSE IVs (cands 76 / 77 / 78) sit past 1024 (one more chunk: 1043 / 1047 / 1053, offset 1096 -> 72).
3. The scalar-loop x87 orders (SetSlerp's dot product, its four products and one `fxch`, 21 lines) are a second id relation. All data
   points fit: OK iff (id of cand 82, the scalar from - to offset) & 0xFF < (ids of the SSE-loop inline locals i / frac / result) & 0xFF.
   base 262 (6) < 226..233 OK; 9 dead ops 262 < 224..234 OK; 10 dead ops 262 (6) vs 256..266 (0..10) BAD; 24 dead ops 294 (38) vs 256..266
   (0..10) BAD; 24 dead ops + 6 dead names 294 (38) < 230..240 OK; GFGetCycleCount start 268 (12) vs 257..264 (1..8) BAD.
4. So retail's source allocates about one chunk more records than ours AFTER the SSE-loop inline locals are created and BEFORE loop
   strength reduction creates the IV temps (records that are alive at that point). Dead temps inside SetSlerp (8 `(long)t + N`) or inside
   each GFFastSin expansion (6 `i + N`, x4 expansions) do exactly that and give retail's bytes alone; outer dead statements (c1 temps, read
   before inlining) push the SSE locals too and need 5..10 outer dead names to pull them back. Dead pure arithmetic on a local
   (`sx + N` in SetSlerp) is removed too early and does nothing.

Sweep tables (state vs OUR baseline object: `d0` = our bytes, `A2` = our bytes with only the SIB line flipped = retail's bytes, `A25` /
`d23` = SIB + / - 21-23 scalar x87 lines; `/1335` = length):
- k dead outer `long dN = (long)count * N;` (name + statement each): k=0..11 d0, 12 d6, 13 d23/1335, 14..17 A25/1335.
- one name, k dead statements `dead = (long)count * N`: 0..10 d0, 11..22 d23/1335, 23..30 A25; `+ N`: 10 / 22; two ops per statement 5 / 11;
  three ops 4 / 8; identical statements (`+ 1` every time) 22 (so CSE'd duplicates count); constant or plain copies: no effect up to 200;
  monotonic up to 200 (no return); all statements on one line: same as on separate lines; blank lines: nothing.
- grid dead ops (rows) x dead outer names `long sN = 0;` (cols): ops 0..9: d0 for names 0..10, d6 11, d23 12, A25 from 13/14; ops 10..21:
  d23 for names 0..4, d0 for 5..10; ops >= 22: A25 for 0..4, A2 for 5..10 (ops 22: 5..10; ops 24..40: 6..10), A8 at 11, A25 from 12.
  Names alone (ops 0): A2 again at 39..43, A8 44 (period ~34, not 8). Forwarded names (`= count`, `= to`, `= t`, `= 0.0f`) count exactly
  like `= 0`; unused uninitialised names do not count (chunk 3001).
- names inside SetSlerp (ops 0): d0 to 10, d6 12, A25 from 14, never A2 (inner names count for the SIB but not for the scalar relation);
  dead temps inside GFFastSin (rows, per expansion) x outer names: 0..5 d0, 6..16 A2 for outer names 0..8 (A8 at 10, A25 from 11); named
  forwarded locals inside GFFastSin: A2 at 4..5 per body (outer 0), at 2 (outer 6), at 3..4 (outer 3).
- GFGetCycleCount start (`__int64 start = GFGetCycleCount();`, end macro): A43/1335; both: A33/1335 (SSE IVs 1017 / 1021 / 1027: only the to-IV
  wraps, so the SSE dot order flips too); end only: d23. With outer dead ops 0..28 x outer names 0..12 and inner SetSlerp /
  CalculateSlerpWeights / GFFastSin dead temps 0..24 the best is A23/1335 (the scalar relation), never A2.
- Natural spellings measured (k0 state; then the dead-op threshold / window that would still be needed): named GFFastSin locals `a` /
  `index` and `one_minus_frac` (d0, threshold 22 / 26, window narrower); named table entries (A12: SIB right, 10 other lines);
  `const float* entry` (1317 bytes); `scale = 1.0f / sinom` (1321); `inv` in SetSlerp (1329); `weights.Y *= sign` (d0, threshold
  unchanged); Dot written out in CalculateSlerpWeights (d0, threshold 23); SetSlerp through CalculateSlerpWeights (frame 0x50);
  GFFastArcCos with a named scaled value (1335); `C3DKeyframe& keyframe` / `const CQuaternion& target` in the scalar loop (d0, neutral);
  pointer-walking or index-free scalar loops (64-72 lines); `sqrtf` / `(double)` sqrt (13-31 lines); an extra function in the TU (neutral).
- Evidence for the original helpers: FableWin.pdb lists ?GFGetCycleCount@@YI_JXZ (so the original may well call it rather than use the
  project's macro), ?GFFastSin@@YIMM@Z next to ?GFFastCos@@YIMM@Z, and after FableWin's own ?BlendAnimations@... (0x033858F0, 0x1D0 bytes in
  that VC10-era debug build) the run ?CalculateSlerpWeights@CQuaternion@@SIXAAV1@ABV1@1M@Z, ??$AllocateType@VC3DKeyframe@@@... -- a
  different build (VC10 Dinkumware iterators), so its sizes do not calibrate against VC7.1.

## 0x00550DC0: what this chunk measured
- k dead outer names at the top of the bastard-child block (`long dN = layer + N;`, k = 1..16): the four frame stores never move; the
  only effect is a period-8 operand order of `fld [esi+34h] ; fadd [eax]` / `fld [eax] ; fmul [esi+5Ch]` (dref 0 / 4 / 8 against our base).
  So this tie is not the r3004 per-function key.
- Packer trace (tools/pk/run.py, copied from chunk 3002): with the member operator* the two 16-byte temporaries swap which c1 temporary
  (t140 / t156) is node 57 / 58, but the list order and slots are unchanged -- the walk order of the final references (P def, T def,
  T use, P use) alone decides it, as chunk 3002 measured.

## Skipped
None.

## Names
- Rows: as proven by chunks 2001 / 2006 (LABELS.tsv). No new callee or global names. Descriptive names unchanged from the prior sources
  (0x00A9EB90: g_BlendAnimationsCycles 0x013D2D88 / g_BlendAnimationsKeyframes 0x013D2D90; 0x00550DC0: local names `colour`,
  `child_colour`, `render_colour`, `position`, `relative_position`, `render_position`, `zoom`, `relative_zoom`, `sprite`).
- New evidence for the coordinator: CRGBFloatColour's product is a const MEMBER operator* in FableWin.pdb (??DCRGBFloatColour@@QBE?AV0@ABV0@@Z
  0x02292A60, also ??DCRGBFloatColour@@QBE?AV0@M@Z); no free operator* appears in either name set, while rebuild/include/engine/CRGBFloatColour.h
  models it as a free inline operator* (byte-neutral in 0x00550DC0; worth checking at the next header merge).

## Header debt
None new (hdr/README.md: union of the 3001 / 3002 overlays, which still have to land first).
