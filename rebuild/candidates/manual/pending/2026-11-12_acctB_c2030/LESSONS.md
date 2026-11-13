# Chunk 2030 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00BEA110) A shader-constant CMatrix3x4 filled from 12 values is `M.Set(12 floats)`: a `M = CMatrix3x4(...)` temporary copies with
  `rep movsd`; Set stores straight into the member and keeps a constant 0 in a register for the zero entries.
- (0x00BEA110) A lerp written `(h[0] - h[1]) * t + h[1]` on members reads `h[1]` twice from memory (`fld ; fsub ; fmul ; fadd [mem]`);
  GFGetInterpolatedValue(old, new, t) keeps old on the x87 stack (`fld old ; fld new ; fsub st(1) ; ... ; fadd st(1)`).
- (0x00BEA110) Values used by two later Set calls after member stores: named float locals (the second Set reuses them on the x87
  stack); a `const C4DVector&` is reloaded after the stores.
- (0x00BEA110, 0x00BE9920) The scheduling-region cut (81 IL tuples) is moved by no-code PAREN tuples: one extra `((a - b) * t) + b`
  grouping fixed 0x00BEA661..0x00BEA68B; a `(float)(double expr)` return cast adds one per value (12 in the ctor). Measure with the
  t13 region dump + sim.py replay (pending/2026-11-09_linux_hand892/tools/sched; copied to this chunk's tools/sched/, t15.py dumps every
  region's op list): replay a DAG with one tuple moved across the cut before spending a grade.
- (0x00BE9920) Stores through an inlined member's `this` (CMatrix3x4::Set on a member matrix) get DAG edges from earlier stores to
  sibling members of the same object, element-wise member stores do not: Set keeps ShoreLookupMaskMatrix2's int copies behind the last
  ShoreLookupMaskMatrix fstp (0x00BE9A4C..), explicit stores are hoisted into the fcos chain.
- (0x00BE9920) A local copied from a member through an inline constructor's `const T&` parameter makes earlier member stores barriers
  for the copy's loads (0x00BE9EAD); reading the member directly does not.
- (0x00BE9920) A copy-back from an address-taken local array into members by twelve assignments chains every store (store -> later load
  edges) and keeps the epilogue's `pop edi` behind them; `M.Set(a[0], a[4], ...)` drops the chain and lets `pop edi` issue first
  (0x00BEA062).
- (0x00BE9920) Constants that get registers in a long store sequence follow live-range overlap in IL order: defaults in declaration
  order made 0.1f / 0.05f / 3.0f / 2.0f take ecx / edx / edi / ebx as retail; a 2.0f store placed before a CRGBFloatColour copy keeps
  ecx / edx free for the copy (the copy's pointer and temp are local registers chosen after colouring).
- (0x00BE9920) VC7.1 unrolls a 2-trip `for` (histories: all [0] then all [1]) but not a 3-trip outer loop, and never a 12-trip loop of
  fcos; constant folding of `cos(k * (2pi_f / 12.0))` happens only for literal k. A chained `a[0] = a[1] = 0.0f` stores [1] first.
  `X = C2DVector(0.0f, 0.0f)` keeps the temporary's two dead frame stores (18 pairs in the ctor).
- (0x00BE9920) Folded double constants can be checked without grading: the object's `__real@<hex>` COMDAT names are the bit patterns.
- (0x00BEA110) `long % GetFrameCount()` with the FableWin `unsigned long GetFrameCount()` accessor gives the unsigned `xor edx,edx ;
  div`; the null / zero-frame test is `cmp word ptr [eax+0Ah],reg ; jbe` against the zero register.

Review at landing:
- New overlay header engine/CIDrawWorld.h conflicts with the CIDrawWorld definition inside engine/CWorld.h (which also has slots 1 / 2
  swapped against retail); decide where the one model lives.
- Overlay opt-ins added to headers copied from pending hand-backs (c2008 CEngineComponentRenderParams, c2018 CDisplayManager /
  CGraphicInfo, c2021 CEngineLandscapeRenderer): merge them after those hand-backs land.
- Uncatalogued functions in the CEngineWaterSettings TU (SPLITS.tsv): 0x00BE9490, 0x00BE94A0, 0x00BE94B0, 0x00BE94E0, 0x00BEA080,
  0x00BEA0B0 -- cut rows for them; 0x00BE94B0 is already reproduced byte-identically by both sources here.
- NAME_FIXES.tsv: the landed 0x00BE9530 row carries the sky renderer's spelling (and a non-const public declaration); 0x00B61AA0 shares
  it. Respell both.
- The descriptive helper `ShoreLookupMask` (static inline, no symbol) in 0x00BE9920.
