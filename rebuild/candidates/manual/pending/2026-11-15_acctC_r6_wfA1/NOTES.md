# wfA1 NOTES (acctC lane)



## SUCCESSOR 8 (2026-11-15; sections below are older)
### 0x006D0830 (successor 8) -- unchanged DIFFER(2581v2584), ~12 grades
- Dead FABLE_RETAIL_ASSERT arms (fable_assert.h, condition with a CDefPointer temp: `ParentThing && PHeroMarriageDef.Get()`), k = 1..4
  at three positions in the wedding block (before the sex test, inside the movie arm, before InitWifeNonpersistentState): all
  byte-identical (exact 60 / masked 54 lines, as the base). The assert header was removed from hdr/ again (no kept source uses it).
- Retail ebx is untouched from the prologue push to 0x006D0928, so no ebx candidate can separate the movie block from block 9 in
  retail; with successor 3's piece rule this leaves only "the movie zeros were not const-0 references in retail's IL", for which no
  source form is known (typed nulls, defaults, wrappers, bool / float zeros all join). Parked.
- The row is a const-0 piece question like 0x00682810's register residue; neither closes with devices a-d.
### 0x00682810 (successor 8) -- unchanged DIFFER(3439v3442), ~12 full grades + ~30 probe compiles
- New measurement: the unfolded AddRef test (`mov eax,[eax] ; cmp eax,ebx ; je ; mov esi,eax ; cmp esi,ebx ; je ; inc [esi+4]`,
  0x00682A36) is UNIQUE in all of Fable.exe (tools/bsearch.py byte patterns, any registers); the same "assign a CDefPointer temp to an
  empty local" shape appears folded at 4 other retail sites (0x0065817D, 0x00688CCF, 0x009AE1DE, 0x009AE280). So retail's spelling here
  differs from the ordinary one at one site only.
- Probe TU (probe/fold/: hdr.inc = the row's include block, run.sh <name> compiles <name>.body as a free function Probe and counts
  kept AddRef tests; c2prio --blocks on p0 shows the test is gone BEFORE register allocation: block 5 references effects 3x = def +
  the inc's read/write, no test). Every one of these keeps the fold: effects declared before the deed lookup / inside `if (deed)`;
  a named local or a const& bound to the Peek result then `effects = p`; explicit `CDefPointer(...)` temp; `effects.operator=(...)`;
  nested range ifs; early `if (!deed) return`; `= CDefPointer()` init; `else effects = NULL_DEF`; SetDynamicCast (REREAD and Assign);
  guarded `if (!e.IsNull()) effects = e` (named, const&, Get()); a real join (fake `if (x == K) handle = 0;`) between the ctor and the
  assignment (full row, j1/j2: still folded, longer); dead FABLE_RETAIL_ASSERT arms at 4 positions round effects (full row:
  byte-identical) and asserts naming effects (probe e1..e4); `if (0) f(&effects)` (effects goes to memory for good: no).
  __forceinline -> inline makes operator= an out-of-line call (no). QFE 4035 toolset: identical counts.
- Also measured: a reference / pointer alias of effects as the assignment target (probe r1/r2: folded); 12 / 20 / 32 / 48 dead
  asserts at the function top (full row: 12 / 20 identical, 32 / 48 change one reload in the wives region, fold kept) -- not an IL
  size cap. The landed CDefPointer copy-init of region_def (0x006828A5) shows the same function folding ctor test + IsNull normally.
- So the fold is an IL fact that no local spelling of this block reaches; the residue (const-0 piece / register allocation in the
  guard, flirt, wives and violence blocks) stays as described by successors 2-7. Parked.

## SUCCESSOR 7 (2026-11-15; sections below are older)
### 0x006B2CA0 CEnvironment::UpdateSunAndMoonPositions -- RELOCATION_MATCH 910v910, src/006b2ca0.cpp, test PASS (mutant without the clamp FAILs), addmatch + read-only vland PASS (~35 grades)
- Mechanism (from the real c2 trace picks, tmp/s4/sc_s6base.json region 2, NOT the sim.py model, which mis-prices no-code nodes): the PAREN
  tuple 0x269 i6 after the sun fcos has early c75 but priority 2048 (its value is unused: only an edge to the region end), so at c75 the
  xx-copy load wins unit 0 and at c76 the store (p120832) wins too. A PAREN whose value FEEDS something gets a real height (the rotation's
  `E11 = (GFCos)` PAREN i14 has p146432). Retail needs a unit-0 node with priority > 120832 eligible at c76: a used PAREN at i6.
- Closer 1 (FITTED): X argument `((float)GFCos(GameTickState.Time))` in both vector constructors (outer parens round the narrowing cast;
  `(GFCos(t))`, `(float)GFCos`, `(float)(GFCos)`, `((double)GFCos)` do NOT do it) -> both xx-copy slots right; +1 tuple per site moved the
  moon lea 3 slots early (2 lines).
- Closer 2 (FITTED): re-fitted the rotation PAREN count with a probe overlay (probe/p7, FABLE_PROBE_SYZ_Y/_Z macro bodies, 16 variants):
  SetYRotate + SetZRotate tuples 3+2 and 2+3 match, everything else (incl. the old 3+3) does not. New overlay opt-in
  FABLE_RETAIL_CMATRIX3X4_SETYZROTATE_GF_PAREN5 (Y: all three entry assignments parenthesised; Z: cosine and sine entries only).
- No flags. Spellings marked EVIDENCED / FITTED in the source header. Device: a (fitted ordinary spellings) only.
- The same `fstp ; mov r,[tmp] ; fsin|fcos ; mov [dst],r` residue of the parked 0x00803540 CTCTargetingPlayer::UpdateTargetedPositionRandomWalk
  (hand777 / codex_released) is very likely the same fact: try `C2DVector(((float)sin(..)), ...)`-style used PARENs on the first trig value there.

### 0x006D0830 (successor 7) -- unchanged DIFFER(2581v2584), ~14 grades
- Tried, all byte-identical or worse: landed InitWifeNonpersistentState body (rebuild/src/compiled/00/6d/..._006d03e0.cpp) defined above
  the row (same-TU callee visibility, LESSONS_wfB5); movie block as a named CCharString (worse), ActivateQuest through GetWorld() (worse),
  `bool female` local, unused movie return value, inverted sex test, fully typed null casts for the movie zeros; k = 0..4 unused
  initialised named locals before `villages` and 1..3 before InitWifeNonpersistentState (byte-identical: such locals make no c2 record).
- Not tried yet: the InitWifeAtEndOfMarriageFrescoStartFadeIn body above the row (needs a CTCRegionFollower::RemoveFollower decl opt-in;
  low expectation: both callees are opaque `this` calls).
### 0x00682810 (successor 7) -- unchanged DIFFER(3439v3442), ~12 grades
- The folded redundant AddRef test (retail keeps `mov esi,eax ; cmp esi,ebx ; je` at 0x00682A3C) is an IL fact, not a peephole (retail has
  the same register pattern unfolded): it survives effects declared before `deed`, `effects(NULL_DEF)` / `= NULL_DEF` / `= CDefPointer()`,
  operator= written `other.Object != Object` (probe/p8), and all three CDefPointer Release opt-ins (SELFDELETING 3239 / COUNTREF 3479 /
  POINTEE 3471, fold present in each). k = 1..7 unused named locals before the wives list: byte-identical.
- Same-TU callee visibility: the landed COpinionTransientOffset(const&, const COpinionPersonalityTraits&) body (0x00681680, rebuild/src/
  compiled/00/68/) defined above the row: same counts (tmp/s8/ct1.cpp). The other near calls (0x00685BC0 / 0x006860C0) are STL template
  bodies, already visible.
- Hypothesis left for a successor: retail's two loads of temp.Object (the compare and the copy) were not CSE'd when the fold ran, i.e. a
  store that may alias the (address-exposed) temporary sat between them at that time; find which source shape keeps such a store alive
  until after the fold (e.g. a Release whose dead-ness is proven later), then re-read the register picture.

## SUCCESSOR 6 (2026-11-15; sections below are older; ~35 grades + 3 sched2 traces; no new match)
### 0x006B2CA0 -- unchanged DIFFER(910v910), 4 lines (xx-copy store before fsin, 0x006B2CCD / 0x006B2DDC)
- New evidence (byte search of Fable.exe, `fstp [esp+a] ; mov r,[esp+a] ; fsin|fcos ; mov [..],r`): retail has this shape at exactly
  three places: both sites of this row and 0x00803804 in CTCTargetingPlayer::UpdateTargetedPositionRandomWalk 0x00803540, which is
  itself PARKED with the SAME residue (rebuild/candidates/manual/pending/2026-11-05_linux_hand777 + 2026-11-13_codex_released differ:
  "retail stores the sine copy after fcos, ours before it"). The reverse shape (`fstp ; mov r ; mov [..],r ; fsin`) appears in retail
  once (0x00A0C6A0, landed CCamera row) in a different context. So the residue is one systematic fact shared by two rows/TUs; solving
  one solves both. Not a /G flag (/G7 re-checked: 5 hunks, same slot wrong).
- sched2 DAG reading (tmp/s4/sc_s6base.json): x87 ops are a chain in IL order (0-latency edges to every later x87 node), only the
  integer load/store (unit 0) and no-code nodes float. Load ready c75 with no other unit-0 candidate of higher priority (the
  ctor-binding no-code 0x269 i6 has p2048), so it goes at c75, the store at c76, fsin (e77) after. Retail needs exactly one of: a
  unit-0 node of priority > 155648 ready at c75, or the load ready at c76. The 0x269 after fcos is the C3DVector ctor's x binding
  (gone in the member-store form, which is 912 bytes anyway).
- Tried this pass (all byte-identical 4 lines unless noted): parens round the GFCos ctor arg (sun / both / double), `(0.0f)` and `0`
  for z, rotation declared after sun_vector / at function top, member-store form with parens on X or Y (912, 10 hunks), /G7.
### 0x006D0830 -- unchanged DIFFER(2581v2584)
- Tried (byte-identical, md5 of our listing): village loop as `while` / `for` without increment (c2 static-frequency lever), an
  `if (!ParentThing) __assume(0);` diagnostic in the movie arm (frequency), Init calls duplicated into both sex arms (2597 / 2588:
  VC7.1 does not cross-jump them), Init calls in their own braces.
### 0x00682810 -- unchanged DIFFER(3439v3442)
- Structural (insert/delete) residue list with tools/idd.py (mnemonic diff): the only non-register instruction differences are the
  folded redundant test `cmp esi,ebx ; je` after `mov esi,eax` in the guard effects assignment (retail 0x00682A3E, the copy-assignment
  `if (Object) AddRef` after `Object != other.Object` with Object known 0) and the Release copy `mov edx,ebp` (0x00682A56); the rest
  are register / spill moves. CDefPointer Release opt-ins COUNTREF (3479) / POINTEE (3471) / SELFDELETING (3239) and a non-const
  `CDefPointer<COpinionDeedEffectsDef> effects` (template operator= -> Assign, 3455) are all worse. Whether the fold is an IL fact or
  a post-allocation peephole is not measured yet (next step: c2 trace of the compare before / after allocation).

## SUCCESSOR 5 (2026-11-15; sections below are older)
### 0x006B2CA0 CEnvironment::UpdateSunAndMoonPositions -- DIFFER(910v910), 4 differing lines (was 14), differ/006b2ca0.cpp (old: differ/006b2ca0.prev910_14.cpp)
- Closer 1 (EVIDENCED, twin): no `scalers` local; every theme scaler read in place as `GameTickState.BlendedTheme[0].Scalers[n]` (landed
  CEnvironment::Update 0x006B3FF0 line 67 says it is the one spelling of this TU). 14 -> 10 (fixes the Scalers[5] load slot 0x006B2EE3).
- Closer 2 (FITTED): region-cut arithmetic with tools/rgi.sh (region sizes + lea/call indices from the sched2 trace). Measured with k nested
  parens as a diagnostic: the sun AccurateNormalise lea needs +6..+8 tuples before it (block offset 77 -> region 3 index 1-2) and the
  sun+moon blocks need exactly +12 before the moon Z sum (so the moon lea opens region 4 ahead of the Z sum, as retail 0x006B2EBA).
  6 + 6 = three PAREN tuples per SetY/ZRotate call (4 calls). New overlay opt-in FABLE_RETAIL_CMATRIX3X4_SETYZROTATE_GF_PAREN (the
  _GF_NEGENTRY bodies, each entry assignment parenthesised; the two cosines CSE to one tuple) -> 4 lines. Any 3-per-call form gives the
  same bytes (also `E31 = (((-E13)))`, `E13 = ((GFSin))`); 2 or 4 per call do not. Parens in the row (Z sum, SetYRotate arg) do the same
  job but are less plausible; row-level named locals add NO tuples (rgi: height / cos-sin locals 0 / -1).
- Re-measured and rejected: TransformPoint DEFAULT (paren groups) 32 lines (association differs), whole-row TransformPoint parens 15,
  PARTICLE_POINT / ZYX_SUMS 10 (= base), member-store vector forms, `(float)` casts, `0` for 0.0f, /G5 /G6 /GB equal, /G7 6, /Op /Oy- worse.
- RESIDUE (a) unchanged: xx copy `mov eax,[X] ; mov [xx],eax` with `fsin` between in retail (0x006B2CCD sun, 0x006B2DDC moon). sched2
  trace (tmp/s4/sc_b4.json region 2): fcos c5, fstp X c73, load early 75 picked c75 (only unit-0 candidate besides the no-code PAREN i6
  p2048), store c76, fsin early 77. Retail needs the load at c76 (then store and fsin tie at c77 and fsin wins on priority) or fsin at
  c76: a unit-0 node of priority > the load at c75 or one more cycle on the fstp -> load latency. No spelling found.
- fwsize note: SetYRotate FableWin 32i/88b v NEGENTRY 29i/79b, float-s 30i/84b, 4 casts 31i/87b (probe/fw t*/u*); parens do not change /Od
  size, so /Od cannot choose the paren form.

### 0x006D0830 / 0x00682810 (successor 5): regraded unchanged (2581v2584 / 3439v3442); no new closer
- 0x006D0830 observation (not yet a closer): retail's `xor ebx,ebx` sits at 0x006D0928, AFTER InitWifeNonpersistentState /
  InitWifeAtEndOfMarriageFrescoStartFadeIn / GetGameTimeManager, i.e. retail's coloured const-0 piece STARTS at the FastForwardTimeTo
  0.0f push and the movie zeros are in a separate, uncoloured piece (immediate `push 0`). Ours joins them through the join block and the
  three calls. Whatever split retail's root must conflict across those three calls (not only the village loop where `i` lives).
  CMainGameComponent::Get() re-read after the movie is right (retail reloads [0x013B86A0] at 0x006D08EC), so `component` is not it.
- Device sweeps (k unused locals, dead asserts) were already byte-identical (successor 3); not repeated.

## SUCCESSOR 4 (2026-11-15, Opus 5.5 high; sections below are older)
### 0x006B2CA0 CEnvironment::UpdateSunAndMoonPositions -- DIFFER(910v910), 14 differing lines (was 72), differ/006b2ca0.cpp (old: differ/006b2ca0.prev910_72.cpp)
- Closers (each graded, in order): (1) unary `-GameTickState.SunVector` (FABLE_RETAIL_C3DVECTOR; drop _C3DVECTOR_ACCURATENORMALISE, the
  C3DVECTOR block declares it) fixed the diffuse -Sun reads through eax (72 -> 50); (2) new overlay opt-in
  FABLE_RETAIL_CMATRIX3X4_SETYZROTATE_GF_NEGENTRY (`E13 = GFSin(angle); E31 = -E13;` / Z: `E21 = GFSin; E12 = -E21`) fixed the
  `fxch ; fmul xx` E13*xx order (50 -> 42); (3) `C3DVector sun_vector(GFCos(t), GFSin(t), 0.0f)` / same for moon (constructor instead of
  member stores) fixed ALL E11*xx factor orders and the moon Z turn (42 -> 18); (4) `X*X + Y*Y` in the clamp (18 -> 14).
- Measured mechanism of (2)/(3) with the c2 operand-sort log (tools/lk, wfB6): leaf keys = record id*32, descending, emitted
  `fld second ; fmul first`. Before (3): rotation+0 = 228 < xx = 230, retail needs the reverse; the constructor changes the leaves.
- RESIDUE (3 facts, all scheduling): (a) `mov [esp+8],eax` (xx copy) before `fsin` (0x006B2CCD, moon 0x006B2DDC): scheduler trace
  (tools/sched2 t13 + tools/scy.py) -- load ready c75, store c76, fsin early c77 (FP unit busy with fcos), so ours stores first;
  retail needs the store >= c77; NOT moved by any PAREN / helper text / vector spelling / /G5 /G6 /G7 tried; (b) `lea ecx,[vector]`
  hoisted before AccurateNormalise (0x006B2DB9 / 0x006B2EBA) and (c) `fld Scalers[5]` after the SunVector copy (0x006B2EE3): the
  81-tuple region cut (tools/rg.sh): ours region 2 ends 5 tuples after the sun lea; whole-row PARENs in TransformPoint
  (`point.X=(...)`, 3 per expansion, probe only) fix both leas -> PAREN tuples DO move it, but break the Y-turn loads; no combination
  closes (a). Rulers k = 0..7 (top / before clamp): byte-identical.
- 0x269 in the scheduler traces = the no-code PAREN tuple. Row-level parentheses DO make PAREN tuples here (`(GFSin(t))` in the
  constructor call, `SetYRotate((-scalers[8]))`), as do parentheses round the GFCos / GFSin calls inside SetY/ZRotate (probe overlay
  probe/yp). Best PAREN combination (probe/yp overlay + `SetYRotate((-scalers[8]))` / `((-scalers[11]))`, tmp/s4/k_pa.cpp, FITTED,
  NOT handed back): 12 lines -- sun lea one slot off (`fstp st(0) ; lea` v retail `lea ; fstp st(0)`), moon lea 3 slots late, (a) and
  (c) unchanged. Adding more PARENs after that changes nothing (some parens make no tuple). Whole-row TransformPoint parens fix both
  leas but break the Y-turn loads.
- (a) in scheduler terms (tools/scy.py on tmp/s4/sc_c2.json, region 2): c73 fstp X, c75 load (prio 308224) beats the PAREN i6 (prio
  10240) on unit 0, c76 store, c77 fsin (FP unit busy with fcos until then). Retail's `load ; fsin ; store` needs the load one cycle
  later (a higher-priority unit-0 node ready at c75, or a later-ready load); no spelling found that changes it: ctor body / paren
  init texts, (float) casts, `= C3DVector(...)`, float / double locals for the cosine, a time local (worse), TransformPoint with
  separate / reordered / copied xx locals, SetY/ZRotate without SetIdentity, separate matrices for sun and moon (same bytes).
- Next for a successor: find what makes retail's xx-copy load late (DAG edge or priority) -- compare with another retail row that
  has `fstp [v.X] ; mov eax,[v.X] ; fsin ; mov [copy],eax` (byte search) and read its landed source.

## SUCCESSOR 3 (2026-11-15, ~150 grades + 2 c2 traces + 9 what-ifs; no new match; sections below are older)
State: 0x00C271F0 / 0x00DFCA90 matched (untouched, regraded RELOCATION_MATCH after the overlay changes); 0x006B2CA0 moved from a
908v910 draft with a WRONG CONSTANT to 910v910 with the right helpers; 0x006D0830 and 0x00682810 unchanged (mechanism below).
Overlay repair: hdr/engine/CThingCreatureBase.h had LOST the FABLE_RETAIL_CTHINGCREATUREBASE_AUTOPLACENEAR block (the fix-up team's
GETSEX re-copy dropped it; differ/006d0830.cpp graded SRC_FAIL). Restored after the GETSEX block (asserts are pinned, none moved).
Last-resort devices measured on all three open rows (WF_RULES acceptance test): k = 0..8 initialised unused locals at the function
top and 1..8 dead FABLE_RETAIL_ASSERTs (fable_assert.h, not kept in hdr/) leave every row byte-identical; 19 TU flags on 0x006D0830
(/G5 /G6 /G7 /Ox /Oa /Ow /Op /Os /GF /Gy /GA /QIfist /arch:SSE /GR- /Gs /Oi- /EHsc /EHa /Ob1) are equal or worse. None of the three
residues is a record-count tie or an inline decision, so devices b-d do not apply; no FITTED spelling closes a row.
New tools (tools/): sw.py (one line per variant: `@NAME@` axis or k-ruler), gv.sh (grade files, one line each), cnt.py (exact /
masked differing hunks + lines of a q.py log; `cnt.py log 30` lists hunks), pieces.py (which blocks each split piece of a c2 candidate
covers per priority pass, from `c2prio.py --trace --blocks --json`), wfq.sh (what-if priority forcing graded against retail,
DIAGNOSTIC ONLY), perm.py (statement-order sweep of the rotation helper body). Variants in tmp/n and tmp/sw.

### 0x006B2CA0 CEnvironment::UpdateSunAndMoonPositions -- DIFFER(910v910), 72 differing instruction lines (was 220), differ/006b2ca0.cpp
- LEVEL CHANGE that the earlier drafts missed: the angle helpers are GFCos / GFSin (FableWin ?GFCos@@YINM@Z / ?GFSin@@YINM@Z, shared
  engine/GFCos.h + GFSin.h). PROOF: the qword at 0x0124F2B8 that every angle is multiplied by is 6.2831854820251465 = (double)6.2831855f
  (read from the exe), the GFCos / GFSin constant; the old draft spelled `cos(x * 6.2831853071795862)` -- a different VALUE that q.py's
  relocation mask hid (differ/006b2ca0.prev908.cpp is that draft; do not go back to it).
- Source now: `sun_vector.X = GFCos(Time); .Y = GFSin(Time); .Z = 0; rotation.SetYRotate(-scalers[8]); TransformPoint; SetZRotate(
  scalers[9]); TransformPoint; Z += scalers[10]; AccurateNormalise` (moon 11..13), `sin_clamp = GFSin(DiffuseClampAngle)`. The X store
  survives and Y is forwarded exactly as retail (the double -> float store of GFCos is real).
- New overlay opt-in FABLE_RETAIL_CMATRIX3X4_SETYZROTATE_GF: `SetIdentity(); E11 = GFCos(angle); E33 = GFCos(angle); float s =
  GFSin(angle); E13 = s; E31 = -s;` (Z turn: E11, E22, E21 = s, E12 = -s). EVIDENCED: two entry stores from one x87 value
  (`fst [esp+50h] ; fstp [esp+70h]`, 0x006B2CE5) and sin / -sin kept on the stack (`fsin ; fld st(0) ; fchs`). FITTED: `float s`.
  With FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR (landed text) the length is equal. Measured alternatives (bytes / differing lines):
  float c + float s 900 / 102; chained E11 = E33 = GFCos 902 / 81; all four from calls 926 / 132 (24 orders, tools/perm.py);
  double locals 934..954; E13 = GFSin; E31 = -E13 with default TransformPoint 922 / 79, with _SUMS 906 / 64, with PARTICLE_POINT 906 / 67.
- RESIDUE of the handed-back source (retail addresses): (1) x87 commutative factor order in all four TransformPoint expansions:
  retail `fld xx ; fmul E11` (0x006B2D01, 0x006B2D77, 0x006B2E12) and `fxch ; fmul xx` with E13 on the stack (0x006B2D3D, 0x006B2E50),
  ours the entry first; a point-first text (`xx*E11 + ...`) compiles to the SAME bytes, so the order is c2 record numbering, not text;
  (2) one integer move slot (0x006B2CCD / 0x006B2DDC: retail `fsin` before `mov [esp+8],eax`); (3) `lea ecx,[vector]` before
  AccurateNormalise (0x006B2DB9, 0x006B2EB8: retail after the last store, ours earlier) with `fstp st(1)` v `fstp st(2) ; fstp st(0)`;
  (4) moon Z turn 0x006B2E86..0x006B2EAE (retail keeps E11 on the stack: `fld yy ; fmul st(1)`); (5) the diffuse block 0x006B2EE3:
  retail loads Scalers[5] (`fld [esi+19Ch] ; fld st(0)`) BEFORE the SunVector / MoonVector copies and addresses the copies through
  ecx / eax swapped. Next: measure the factor order with the operand-sort log (LESSONS_wfB5 tools/lk: leaf record ids), name locals
  in TransformPoint / the helper to move records (lever list in LESSONS_wfB5), and re-derive the diffuse statement (a named
  `float moon_weight = scalers[5]` before the two vector stores is the first thing to try).

### 0x006D0830 CTCWife::GlobalUpdate -- still DIFFER(2581v2584); the two residues are ONE fact (new)
- Measured with tools/pieces.py on the c2 trace: c2 splits `const 0` (one root candidate for every zero of any type; ebx-only
  because of byte references) the moment the first candidate takes ebx (here the village loop index i, prio 33). The pieces are
  the connected live regions outside the conflict, each from its first real (non-compare, non-byte) reference to its last. The
  piece that holds the wedding zeros (#28) ALSO holds the divorce / dowry arms (blocks 1-9, 49-77, 88-116) and is ebx-only
  (ebp is village_count's in block 9), so every later candidate that overlaps it pays 201 for ebx -> wife_def takes ebp (dowry
  residue). It is only split when temp #99 (line 266, prio 12) takes ebx; the wedding piece then is {movie block, join block}
  (prio 18) and the load goes before the first use on each path = `xor ebx,ebx` before the movie pushes + on the no-movie edge
  (wedding residue).
- COUPLING proven three ways (the earlier note said the residues were independent): diagnostic `bool keep = WaitingForDowry;` held
  across the movie block (tmp/n/gu_k1.cpp) removes the dowry residue; what-if `cutscene_skipped` coloured before i (wfq.sh
  '27:7=50') leaves ONLY the six wedding hunks (dowry right, wife_def ebx); what-if i lowered to 29 / 21 fixes the dowry too.
  So retail's `const 0` was split earlier / differently than ours, and the dowry follows from the wedding.
- The wedding part resists every ordering: what-ifs (i up / down, #68 first, village_count late, root const first, movie temp
  first) all keep the movie pushes in the block-9 piece. A piece takes in every real zero reference that flow-connects to its
  seed while a register is free: one zero left in the movie call is enough (N-sweep), an extra branch or an early return between
  the movie call and the join does not cut it, a dword zero store before the sex test pulls the load up to there. Therefore
  retail's eight `push 0` were NOT joined to the block-9 references at the time that piece was made: either they were not
  `const 0` references of the same root (no source form found: typed nulls, defaults, inline wrapper (forceinline diag), bool /
  float zeros all join) or ebx AND edi were unavailable between them (no such value in retail's code).
- Dead ends this round (all byte-identical unless noted): i declared at 7 earlier positions, `for (i = 0, n = size(); ...)`,
  village declared outside the loop (worse), do-while guard form, do {} while (0) / for(;;) break / switch(0) around the movie
  call, switch on the sex, inverted if, two ifs (longer), bool local (longer), first-zero-constant type (`== 0`, `== false`,
  `(int)`), a dword compare / store as the function's first zero (root stays ebx-only), named helper bodies, QFE toolset.
- A successor should NOT sweep spellings again. Open question to answer first: what makes c2 start the block-9 piece AT block 9.
  tools/pieces.py + wfq.sh answer it in two minutes per hypothesis.

### 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed -- unchanged DIFFER(3439v3442), 40 masked hunks
- Only the device sweep was run (k rulers 0..8, asserts 1..8: byte-identical). The residue is the same class as 0x006D0830 (which
  `const 0` piece exists when `it` is coloured, NOTES below); the 0x006D0830 finding applies: look for the candidate that takes
  ebx FIRST in retail and split the root there (pieces.py on tmp/s/u1 with a fresh --json), before any spelling work.


## 0x006D0830 CTCWife::GlobalUpdate -- PARKED DIFFER(2581v2584), differ/006d0830.cpp (~30 grades)
- Name: Ego_r 0x00705510 / FableWin 0x02483170 ?GlobalUpdate@CTCWife@@UAEX_N@Z, vtable 0x0125ED5C slot 26 (draft). Extent ok (`ret 4`).
- Port of C42's unported draft to today's headers: switched fable_game.h -> engine/CMainGameComponent.h (as landed CTCWife::Marry);
  GetHomeBuilding inline body in the row (as landed 0x00946400 / 0x00958840) -- without it 2571 (out-of-line call);
  dowry wait `CMessageEventManager& messages = GetWorld().GetMessageEventManager(); if (messages.Find...)` (filter store after the
  manager load, EVIDENCED by bytes 0x006D1100..0x006D110A); IsScreenFullyFadedOut = direct member tests (FITTED: IsOpen() on the
  subobject kept `add eax,0BCh` + [eax+0Dh]).
- New overlay opt-ins (hdr/README.md): CDISPLAYENGINE_ENDSCREENFADEOUT, CDISPLAYENGINE_ISSCREENFULLYFADEDOUT, CSUBTITLERENDERER_CUTSCENEFLAGS,
  CTCSOUNDPLAYER_ATTACHEDDIALOGUE, CTHINGCREATUREBASE_AUTOPLACENEAR, CTHINGCREATUREBASE_GETSEX, CTCVILLAGE_ONHEROSLEPT,
  CGAMECAMERAMANAGER_RESETCAMERA, NHEROINFORMATIONSCREENS_CMANAGER_ISOPEN; CWifeDef.h (C42 new header) gained StatGetTypeName + virtual decls.
- RESIDUE: (1) wedding: retail movie call pushes 8 immediate zeros; ebx (= loop index i, copy-propagated zero for the FastForwardTimeTo
  0.0f push and the vector's three zero stores) is first set at 0x006D0928. Ours sets ebx=0 before the movie pushes and on the no-movie
  arm (`jmp ; xor ebx,ebx`), i.e. i's zero is hoisted through the join. Diagnostic: without later zero uses the movie pushes are
  immediates, so the hoist comes from i. (2) dowry: wife_def in ebp (retail ebx) -- probably coupled to (1).
  Tried (no change): dowry decl orders (3), i declared before the for, villages in own scope (worse), ESex local, named movie string,
  movie call in own block, no time-manager ref (worse), loop index long/int, count long.

- SUCCESSOR c2 trace (c2prio.py --trace --blocks on differ/006d0830.cpp; tmp/pt_base.trace): both residues are const-0 splitting.
  The wedding loop index `i` (#23) is coloured ebx (only ebx left), which triggers split passes of `const 0` (#1, prio -105,
  allowed ebx only); piece #29 (prio 18 -> ebx) covers c2 block 8 (the movie call, lines of the movie + quest string; its 8 zero
  pushes are cost-0 references of const 0) AND block 9 (the join: 0.0f push + the vector's 3 zero stores), hence the hoisted xor
  on both arms. Retail's piece starts at block 9. The dowry: wife_def (#38, allowed ebx/ebp) gets ebp because ebx carries a
  201 penalty = a const-0 piece left with only ebx is a live neighbour (100 x weight 2 + 1); in retail that piece is absent in
  the dowry region -> coupled to the piece layout. Lead: whatever makes block 8's zeros NOT const-0 references (or splits
  block 8 from block 9) closes both.

- SUCCESSOR measurements (8 grades + 3 c2 traces/what-ifs): DIAG movie zeros -> 5 (all 8 args) removes the wedding hoist completely
  (retail allocation there) => the wedding residue is exactly "block 8's zero pushes are const-0 references". Defaults on the
  declaration (`GFPlayFullScreenXBoxMovie(name, 0x1b)`, FableWin signature + the landed 0x0041E6D3 default list) give the SAME
  bytes (re-checked). The dowry residue is NOT coupled: it stays with the movie diag. Dowry priorities: dowry #39 = 20,
  wife_def #38 = 19 (keys 659/662). What-if wife_def forced to 21 (tools/wf/wf.py '38:19=21') -> wife_def edi, dowry ebp
  (not retail either): retail = dowry coloured first (edi), then wife_def ebx WITHOUT the 201 ebx penalty, i.e. the const-0
  piece live in the dowry region must still allow another register in retail. Dowry spellings tried: ternary init (2583),
  nested ifs, named `const CWifeDef* def = wife_def.Get()` (no change).

## 0x00DFCA90 NScript::CQ_TraderConflictGoodScript::CBanditExtra::Main -- RELOCATION_MATCH 373v373, test PASS (3 grades), addmatch + vland PASS
- Name: Ego_r ?Main@CBanditExtra@CQ_TraderConflictGoodScript@NScript@@EAEXXZ (0x00E3D170) + landed Alloc 0x00DFBED0 (vtable 0x012DEE14 slot 1). Extent ok.
- Written by hand from the disassembly in the CTraderToRescue::Main 0x00DFE0F0 idioms (shared NScript headers). Callees: Pause (376),
  IsDistanceBetweenThingsOver 0x00CBE3EA (not ...Under), CreateCreature (91, implicit CCharString temps), GetDistanceBetweenThings,
  GFFloatToLongCeil (shared retail-asm helper; row cites the expansion in a `// Retail-asm:` line -> purity retail_asm), rand, GiveThingBestEnemyTarget (451), UpdateLiveEnemies.
- New opt-ins: FABLE_RETAIL_CQ_TRADERCONFLICTGOODSCRIPT_BANDITEXTRA (class decl after CTraderToRescue's; new assert at file end, none moved),
  FABLE_RETAIL_SCRIPT_GETDISTANCEBETWEENTHINGS (fable_script_distance.h). No flags.
- EVIDENCED: every statement (bytes). FITTED: local names (bandit, chance); `* (1.0f / 15.0f)` (same constant as 0.06666667f).

## 0x00C271F0 CPhysicalPlayer::Play (descriptive, reloc-bound name) -- RELOCATION_MATCH 846v846 (successor), src/00c271f0.cpp, test PASS, addmatch + read-only vland PASS (no RELOC_IDENTITY; RELOC_NAME warns only = manifest labels of libpk callees)
- FIX-UP (wfA1 fix-up team, 2026-11-14): the local `class C3DVector` was rejected by LANDVERIFY_STRICT_HEADERS=1 vland (HEADER_POLICY).
  Now `#include "engine/C3DVector.h"` with the new additive opt-in FABLE_RETAIL_C3DVECTOR_SWAPYZ (decl only; hdr/README.md). The shared
  header includes <math.h>, so the old local `__declspec(dllimport) double __cdecl ceil(double)` became C2375 (C++ linkage) and an
  `extern "C"` dllimport placed before the include is overridden by math.h (direct `call _ceil`, DIFFER 845v846). Closer: flag /MD
  (EVIDENCED: retail links MSVCR71 and calls ceil via IAT `call [0x014400F4]` at 0x00C2738B; /MD is the CRT model of the vendor
  libs in docs/pipeline/LIBRARY_LINKING.md) -> `_CRTIMP` = dllimport -> RELOCATION_MATCH 846v846. Flags now `/EHsc /MD` (payload too).
  vt.py PASS, addmatch redone, LANDVERIFY_STRICT_HEADERS=1 read-only vland: RELOCATION_MATCH PASS (RELOC_NAME warns only), 00dfca90 PASS too.
  Coordinator: the landed libpk siblings 0x00C1C280 / 0x00C27550 still carry the same local C3DVector (could respell on the opt-in).
- SUCCESSOR closer (6 grades): the scratch registers of the whole tail (from the 3D block on) were rotated by one (retail
  `lea edx,[esp+20h] ; lea ecx,[eax+4] ; push edx` before SwapYZ; ours pushed the return pointer first in ecx). A diagnostic with
  CODA_SourcePlay(src, 0) showed the rotation is independent of the ebp save. Reading the position through a named
  `const C3DVector& eventPosition = desc->m_position;` then `position = eventPosition.SwapYZ();` (FITTED) fixed the rotation AND
  the ebp save (the late flag then takes ebx, ebp nests mid-body). An inline const-ref accessor or a tail-scope decl did NOT.
- (earlier agent's notes follow; the old residue below is closed)
- Name: the landed caller CSoundEvent::PlayPhysical 0x00C20A20's reloc binding ?Play@CPhysicalPlayer@@QAEXPAUCSoundEventManager@@PAUSPlayDesc@@PAUCSoundEvent@@PAUSEventLevel@@MM@Z
  (no PDB covers libpk). Extent ok (EH frame, `ret 18h`). The player is the class landed 0x00C27550 / 0x00C276B0 call CCodaPlayback
  (vtable 0x012B4034, same offsets) -> coordinator: one class, two descriptive names.
- Written by hand, local libpk models in the landed rows' style (Flags /EHsc, PKCProtectionInstance shared header, extern "C" __stdcall CODA API).
  What closed most: `buffer->m_format.X` spelled per access with a named `CStreamBuffer* buffer` (retail reads the format fields via
  ebp+0x24/26/28 and calls the format methods on a CSE'd lea edi,[ebp+18h]) -> 846v846; HasLoop returns long (`test eax,eax`);
  CODA_SourcePlay's flag param long + an inline `bool IsLooping() const` (retail `setne al ; movzx ecx,al`).
- RESIDUE: retail saves ebp mid-body (push 0x00C27283 after the source is created, pop 0x00C2742D right after m_players.Add) and
  computes the late looping flag in ebx; ours saves ebp at entry and uses ebp for that temp (all later esp offsets +4, 3 disp32 bytes).
  Tried (no change): same-TU bodies of 0x00C271E0 / 0x00C271B0 above the row, early return instead of the `offset < m_length` block,
  locals declared at top, bool/ternary/local spellings of the flag, format pointer local. GOTCHAS l.358 (nested mid-body saves need
  the earlier-pushed registers to have no hoistable restore) is the lead.
- Descriptive callee names (FITTED where no landed name fits): CODA_SetSourceEnvelope 0x00C35B30, CODA_CreateSample 0x00C36080,
  CODA_SetSourceRangeCallback 0x00C33F20, CODA_SetSourcePitch 0x00C35B10, FullRange 0x00C271B0, UpdateManagerTime 0x00C271E0,
  CSoundEventManager::GetTime 0x00C11700, CPlayerList::Add 0x00C21E90, CSoundTicker::Update 0x00C1C460, CStreamBuffer::GetData 0x00C26240;
  the landed rows' symbols kept where they fit (AudioSystem_SetModeFlags / _SetOutputFlags, CODA_SetSourceVolume, CODA_SetSourcePosition,
  AudioObject_SetSource, CODA_SourceSetPlayCursor, CODA_SourcePlay, CAudioFormat::GetBlockAlign / GetLoopEnd / GetBytesPerSample).

## 0x006B2CA0 CEnvironment::UpdateSunAndMoonPositions -- first draft DIFFER(750v910), differ/006b2ca0.cpp (3 grades)
- Name: Ego_r 0x006EE710 / FableWin 0x0244D960 ?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ + reloc binding of the landed caller
  Update 0x006B3FF0 (call 0x006B42A4); header already declares it (private). Extent ok (`ret`, 910). Anniversary PDB locals:
  sun_vector, moon_vector, diffuse_vector (C3DVector). FableWin debug body 0x3F0 bytes (helpers out of line there).
- Opt-in set copied from the landed RenderUpdate 0x006B42F0 (CENVIRONMENT + TICKSTATES + RENDERUPDATE + SCREENEFFECTS ...; TICKSTATES alone
  fails the layout asserts). Reads: GameTickState.Time (+0xC0C), BlendedTheme[0].Scalers[5] (+0x19C), [8..13] (+0x1A8..+0x1BC),
  DiffuseClampAngle (+0x40); writes SunVector +0xC30, MoonVector +0xC3C, DiffuseVector +0xC48; AccurateNormalise 0x00A14440 x4. Constants:
  2*pi as double 0x0124F2B8, 0.0f 0x0122DEDC (ten `fmul` by zero kept: rotation matrices with constant 0/1 entries), 1.0f 0x0122DED8.
- DECODED EXACTLY (0x006B2EDF..0x006B3023): sun/moon stores; `diffuse = (-Sun) * (1 - S5) - Moon * S5` (Moon*S5 evaluated first);
  AccurateNormalise; `s = sin(DiffuseClampAngle*2pi)`; `if (-diffuse.Z < s) { diffuse.Z = -s; k = sqrt((1 - s*s) / (Y*Y + X*X)); X *= k; Y *= k; AccurateNormalise(); }`; store.
- OPEN: the sun (0x006B2CB4..0x006B2DBF) and moon (0x006B2DC4..0x006B2EDA) blocks: v = (cos t, sin t, 0) with t = Time*2pi, turned by
  -S8 (resp. -S11) and S9 (S12) through 3x3 rotations whose 0/1 entries are compile-time constants, then + S10 (S13) on Z.
  tools/x87sym.py <dis> <first> <last> prints the symbolic stores; its trace shows terms like `(-sin a + sin t) * 0` (an add, `fadd st(2)`,
  then `fmul 0.0f`), i.e. the helper is NOT a plain matrix*vector -- find the FableWin helper (CMatrix33 / CRightHandedSet rotation +
  multiply) whose text gives that, fwsize.py it, then rebuild the two blocks.
- (0x00C271F0, later) also tried `default: { __assume(0); break; }` in the format switch (GOTCHAS l.358 dead-end idea): drops retail's
  reachable `dec eax ; jne` default (844) and still saves ebp at entry. Not it.
- (0x006D0830, later) villages vector declared before the time-manager line / before the InitWife calls: no change.

- SUCCESSOR (row 0x006B2CA0, ~10 grades): the rotation text is CMatrix3x4::SetYRotate / SetZRotate + TransformPoint (FableWin COMDAT
  run 0x02453F00 SetYRotate 88b, 0x02453F70 SetIdentity, 0x02454000 SetZRotate 88b, 0x024543E0 TransformPoint(C3DVector&) 168b = our
  default text SAME by fwsize). Decoded from the x87 trace: v = (cos t, sin t, 0); Y-turn by -S8 (E11=E33=c, E13=s, E31=-s), Z-turn
  by S9 (E11=E22=c, E21=s, E12=-s), Z += S10, AccurateNormalise; same for the moon (S11..S13). New overlay opt-in
  FABLE_RETAIL_CMATRIX3X4_SETYZROTATE. 750 -> 914 (ctor init) -> 908v910 with componentwise init (`v.X = cos; v.Y = sin; v.Z = 0`).
  The TransformPoint opt-in texts PARTICLE_POINT / LTR / SUMS / ZYX_SUMS all give 892 (identical: un-parenthesised rows), the default
  parenthesised one 908. Residue: retail spills cos t through [esp+20h] -> [esp+8] and keeps sin t on the stack (ours spills sin);
  retail products are `xx*E11` operand order (fld cT ; fmul cA), and the transformed X'/Y' go through frame temps copied by integer
  moves (in-place TransformPoint locals). Next: SetYRotate entry order / c,s local order (c2 symbol numbering), a `CMatrix3x4` per
  body (two locals) v one reused, the 2pi multiply inside the helper v at the call.

- SUCCESSOR further tries (all 908v910 or worse): helper entry orders (sin-first 920, no locals 914, E31 before E13 908), chained
  `E11 = E33 = c` (908; no-local chain 914, double sin 918), one matrix per body / separate Y and Z matrices / matrix declared after
  the vector (908, scalarised away), Z store first (908), Y before X (876), `float angle = (float)(Time * 2pi)` local (908, 61 hunks v
  63: tools/../probe/env/C1.cpp). Retail clue not yet reproduced: cos A is stored to TWO frame slots (`fst [esp+50h] ; fstp [esp+70h]`)
  = two matrix entries kept in memory, and the vector X store survives (cos t -> [esp+20h] = sun_vector.X, then xx copy [esp+8]) while
  the Y store is dead-store eliminated (sin t goes straight to yy [esp+0Ch]); ours keeps one c and eliminates the X store instead.
  tools/hsw.sh <tag> <src> '<SetYRotate body>' '<SetZRotate body>' sweeps helper texts on an overlay copy.

## Header-overlay regrade (successor, after adding FABLE_RETAIL_CMATRIX3X4_SETYZROTATE): includers of hdr/engine/CMatrix3x4.h among
   matched sources = style_small 00a5cb90, 00bbddd0, 00bd2d90 -> all RELOCATION_MATCH (unchanged); own src/ rows do not include it.

## 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed -- DIFFER(3439v3442), 40 masked hunks (successor 2, ~110 grades + c2/inline traces), differ/00682810.cpp
- SUCCESSOR-2 closers (each EVIDENCED by a measured mechanism; v9 = 90 hunks -> 40):
  1. list<long>::push_back EXPANDED (retail malloc 0x0068337F) is a FREQUENCY fact, not a budget flag: c2 inline score sc=(c/f+6c)/7 < T=74
     (T read from c2 0x108819A4 by tools/inl.sh); push_back c=37 needs f>0.125, ours had f=0.0315. The three early returns (hero==0,
     !GetTC, log==0) each halve the static frequency; retail has ONE: `CTCHeroOpinionDeedLog* log = 0; if (hero) hero->GetTC(TCI_HERO_
     OPINION_DEED_LOG, &log); if (log == 0) return;` (GetTC's own branch is inside the forceinline body and does not count; c2 threads the
     hero==0 / HasTC-false edges straight to the return, bytes as retail). push_back then f=0.126 (expanded), insert stays called (sc 81).
     /d2inlT and /d2inlS sweeps cannot do it (push_back<long> sc 199 > push_back<COpinionTransientOffset> 184/192 which retail calls).
  2. effects loop end cached: `for (it = def.Effects.begin(), end = def.Effects.end(); it != end; ++it)` (retail ebx = [def+4Ch] once,
     0x006833C9; ours re-read [ebp+4Ch] each pass). Length then 3442v3442.
  3. wives loop: `const CThingAICreature* wife_creature = *it;` + `IS_THING_ALIVE(const_cast<CThingAICreature*>(wife_creature))` + the same
     const_cast in PostDeed: retail keeps the wife in edi and passes IS_THING_ALIVE a TEMP copy ([esp+28h] = edi, 0x00682FA7); naming the
     non-const pointer and passing it made it address-taken (memory). This also gave the top block its zero register (ebx=0) and
     village_member its memory home (83 -> 47 hunks). (w4 = `IS_THING_ALIVE(const_cast<const CThingAICreature*>(p))` same bytes.)
  4. bribed: `GetWorld().GetFrame() < village->GetFrameBribeEnds()` (Ego_r ?GetFrameBribeEnds@CTCVillage@@QBEJXZ; new opt-in
     FABLE_RETAIL_CTCVILLAGE_GETFRAMEBRIBEENDS): retail loads +0xA8 between GetWorld and GetFrame (0x00682BE5).
  5. against_me: `crime->PTarget.Get() == &GetParentThing()` -> retail reloads ParentThing after GetPItem for GetTC(TCI_GUARD)
     (0x00682C11); with plain `ParentThing` c2 CSEs it across the call (FITTED-ish: GetParentThing() is the TC's normal accessor).
  6. marker: `PUnknownThreatOpinionSource = CTCDInternalMarker::Create(...)` by IMPLICIT conversion (retail `lea edx,[temp]` for
     operator=, 0x006834EF), not an explicit CIntelligentPointer<CThing>(...) temp.
- Measured dead ends this round: top-of-function decl of village_member (stores 0 at the top; vb/vg/vh), non-const/GetTC/1-arg PeekTC
  spellings of village_member (no change), crimes_count as one bool expr (same bytes), CDefPointer Release opt-ins (POINTEE /
  COUNTREF / SELFDELETING all worse), `if (deed) { rest }` instead of the deed==0 early return (dup vanishes, 3407), spouse local vs
  IS_THING_ALIVE(me->GetSpouseCreature()) (same bytes), FrameBribeEnds operand order (setg), for v while on the pending loop (code-identical).
- REMAINING (u1 = differ): (a) guard !crimes block: retail log in memory + const 0 in ebx (`xor ebx,ebx` 0x00682A1C) and keeps a
  redundant `cmp esi,ebx ; je` after `mov esi,eax` in the effects assignment (0x00682A3E); ours log in ebx, 0 in ebp, test folded.
  (b) flirt: retail hero_thing edi (+[esp+1Ch]), me ebp (+[esp+30h]), targeting map ebx; ours hero_thing memory, me ebx, map ebp;
  also retail evaluates the spouse GetPItem BEFORE `push 1` for IsAwareOfThing (0x00682EDD). (c) wives loop: retail `it` ebx, const 0
  piece ebp (GetWives push, CHeroMarriageInfo stores, PostDeed pushes), stats spilled [esp+2Ch] -> frame 0x58; ours it ebp, stats ebx,
  ctor zeros via eax -> frame 0x54. c2 trace: `it` (prio 2) picks ebp because ebx costs 14300 (byte-only candidates); retail must have
  coloured a const-0 piece in ebp before it. (d) violence/boasting: retail this in ebx (0x006830AC). (e) duplicated epilogue: ours
  at the criminal return (0x00682CE9), retail at deed==0 (0x0068318F); l2.cpp (before closer 3) had it right -- it flips with
  unrelated register changes, probably settles when (a)-(d) do.
- Mechanism of the remaining register residue (c2 traces tmp/s/u1.trace, u1.blk): in the wives loop retail needs a const-0 piece
  covering blocks 159..172 (GetWives push, ctor stores cost 6 = +72, PostDeed pushes; live-through -48 => prio +24) coloured
  ebp BEFORE `it` (prio 2); ours never forms that piece before `it` (const-0 pieces after the split passes at #77 local->ebx and
  crime_handle->ebp have prio <= 0), so `it` takes ebp (ebx costs 14300 from byte-only neighbours), stats takes ebx, the hero map
  temp (#227, lines 229/250) takes ebp and `me` is split late into ebx -- retail: map ebx, me ebp, it ebx, 0 ebp, stats memory.
  The flirt/violence differences follow from the same ordering. Local spellings in that region (stats GetTC, uid local, state local,
  const_iterator, it outside the for, nested ifs, separate IsAware statement, single/double hero_thing, GetParentThing() forms,
  no `me` local (82 hunks), IsDead text, /GS (cookie, +44), /Gr) do not move it. y1.cpp (one hero_thing in the violence block) is
  39 hunks but keeps hero in edi across both blocks where retail reloads [ebp+4] -- rejected.
- Tools: tools/gf.sh <src> <tag> [flags] (grade + masked structural hunk count), tools/sdx.py, tools/c2/c2prio.py (--trace / --blocks),
  tools/inl.sh (inline decisions with f and sc; FUNC=?InformOfDeed@... HDR=$W/hdr). Variants in tmp/s/ (b0 = old differ v9).

### (previous fix-up team notes follow)
#### 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed -- PARKED DIFFER(3409v3442) on SHARED headers (fix-up team, ~28 grades), differ/00682810.cpp
- Re-authored by hand on shared headers (no local class model left; only file-scope decls: IS_THING_ALIVE<T> template decl as the landed rows,
  NCreatureAI::IsAwareOfThing / IsAwareOfThingIndirectly, NGlobalConsole::EnableUnknownThreatOpinionSources, and the TU's anonymous-namespace
  UNKNOWN_THREAT_OPINION_SOURCE definition copied from the landed staticinit row 0x0121DFE0). New opt-ins + 2 new headers: hdr/README.md (wfA1 fix-up block).
- Names: draft's CanCreatureSeeCreature 0x008645E0 is NCreatureAI::IsAwareOfThing (Ego_r name set + reloc binding); draft's Partner = SpouseCreature
  (+0x188, FableWin GetSpouseCreature inline); Flags1c9 bit 3 = CanBeCourted (FableWin IsCourtable inline); CrimesSuspended (+0xB3) = LaggyLimbo via
  the existing GetVillageLimbo() (CTCVILLAGE_LIMBO); LineOfSight +0x2F = IgnoreFOVLineOfSight (IsThingIgnoreFOVLineOfSight, CTCPERCEIVETHING_AWARE_CALLED);
  COpinionDeed Victim/LinkedHandle = PTarget/ExtraData; g_UnknownThreatMarkers 0x01375733 = NGlobalConsole::EnableUnknownThreatOpinionSources
  (ONE source only: Anniversary 360 Test PDB S_GDATA32; neighbours 0x01375753 are NGlobalConsole globals) -- WEAK, coordinator to confirm.
- BEHAVIOUR FIX vs the stale draft (EVIDENCED, 0x00683342 `cmp edx,ebp ; setge bl` then `jne` insert): the new deed is inserted before the first
  pending deed whose ReactionPriority >= ours (draft had <).
- What moved it (hunks 185 -> 90 masked-structural): frame 0x58 needs crowd_group declared uninitialised inside the boasting-crowd block (the
  top `mov [esp+30h],0` is some other slot); `effects` declared before `if (deed)` (retail `xor esi,esi` before the null test); the courting test
  as one bool over a TEMPORARY PeekPDeedEffectsDef(type)->IsFlirting with ParentThing re-read (retail `mov bl,1/xor bl,bl`, `mov eax,[eax]`);
  `since = gap >= GetFrame() ? 0 : GetFrame() - gap` (0 arm first); out-of-line CIntelligentPointer ctor/operator= (CTOR_OUTOFLINE /
  ASSIGN_OUTOFLINE: retail calls 0x004A96F0 / 0x004A9760); ~list out of line (STLP_LIST_DTOR_OUTOFLINE: retail calls ??1?$list 0x00685BC0);
  GetDefGlobalIndexFromName("INTERNAL_MARKER_LIGHT") by IMPLICIT conversion (retail builds the temp, calls GetWorld, then `lea ecx,[temp]`).
- RESIDUE (v9 = differ): (1) register allocation of the first ~0x150 bytes: retail keeps 0 in ebx (cmp eax,ebx; village_member homed in
  memory [esp+20h], sharing the slot with the 0x23 lookup key; `handle` cached in ebx through the shop block), ours keeps village_member in
  ebx and has no zero register -> knock-on register/reload differences through the guard block; (2) retail keeps the TCI_WIFE key 0xD7 as
  an immediate, ours hoists it into edi in the flirt block; (3) retail uses ebp=0 for the GetWivesInMarriageState / CHeroMarriageInfo /
  PostDeed zeros; (4) list<long>::push_back is EXPANDED in retail (malloc(0xC) 0x0068337F) but called out of line in ours (inline budget; insert
  0x0042AB33 is out of line in both) -- candidate for a /d2inl flag once the rest closes; (5) the duplicated epilogue: retail duplicates the
  region_def release + epilogue at the `GetDeedFromHandle(handle)==0` return (0x0068318F..0x006831B0) and the bribed / criminal returns jump to
  0x00683566; ours gets the dup at the deed==0 return only with `GetWorld().DefinitionManager->...(CCharString(...))` (tmp/v7.cpp, 90 hunks),
  at the criminal return with the implicit-conversion spelling (v9); `goto finished` for the bribed/criminal returns moves the dup to the
  bribed return or removes it (v10/v11). The dup choice is a c2 layout heuristic sensitive to unrelated code size; not closed.
- Previous note (stale draft, kept for history):
- SUCCESSOR: still not re-worked (context spent on the three closer rows). Inventory for the port: the stale draft's ~25 local classes
  all have shared headers today (CThingAICreature, CRegion(Def), CWorldMap, COpinionDeed(EffectsDef), CTCVillage(+Member), CTCShopKeeper,
  CTCGuard, CTCWife, CTCPerceiveThing, CHeroMarriageInfo, CTCHeroStats, CTCCreatureGroup, CTCBoastingCrowdControl, CTCDInternalMarker,
  CDefinitionManager, COpinionReactionManager, CWorld) except the COpinionPersonality* / COpinionOfHeroDef models; ~20 landed same-TU
  siblings in rebuild/src/compiled/00/68/ (CTCCreatureOpinionOfHero_*) carry the opt-in set to copy. Descriptive member names in the
  draft (Partner, Flags1c9, FrameBribeEnds, CrimesSuspended, LineOfSight, Victim, g_UnknownThreatMarkers) need their header spellings.
- differ/00682810.cpp is the earlier lane's draft copied as is (DIFFER(3385v3442) on its own stale overlay). It models CThingAICreature,
  CRegion, CRegionDef, CWorldMap, COpinionDeedEffectsDef, COpinionDeed, ... as LOCAL padded structs (forbidden: shared headers exist);
  on today's headers it fails on COpinionTransientOffset(offset, traits) and PUnknownThreatOpinionSource (today's opt-in is
  FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_THREATSOURCE, the stale one was _THREAT). A port onto the shared headers is the next step.

## Header-overlay regrade (WF_RULES): style_small 006a1a00 0074af50 007b57c0 007c0690 007ef200 007fc9d0 00944940 00945060 include
   overlay headers changed here; all eight still RELOCATION_MATCH under hdr/ (q.py, their own Flags lines).
