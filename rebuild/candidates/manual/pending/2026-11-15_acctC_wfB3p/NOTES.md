# wfB3 NOTES (continues prev/NOTES.md = agentC20 and prev/NOTES_C19.md)
## 0x00BA2350 SetupRenderModeShadersAndConstants
- Start state regraded on today's headers + hdr/: prev/differ/00ba2350.cpp = w/d1.cpp DIFFER(9376v9632), 346 hunks; forced mesh(id 19)=70: 9412, frame-masked diff 72 hunks / 214 lines (tools/fd.py <obj>).
- BREAK (w/x1.cpp): the last two passes are TWO arms with the whole body duplicated (`else if (pass == RENDER_PASS_OUTLINE_STENCIL) { SetupOutlineStencilStates(..); SetupBlankTextures; ApplyVertexShader(?:);
  ApplyPixelShader; *render = true; } else { SetupOutlineFillStates(..); <same four statements> }`), not `if/else` around the Setup call followed by one shared body.
  Evidence (retail bytes): 0x00BA45CF..E2 the argument pushes hoisted above the jne (head merge of two identical arm heads = the expression-temp rotation pointer is in the same state at both arm starts,
  which a sequential if/else cannot give: c2prio --rotation on the forced build showed then-arm Scale eax, else-arm Scale edx); 0x00BA4663 `call 0xBF0B20; jmp 0xBA45E7` sits INSIDE the then body (between the
  ?: arms), i.e. the else arm had its own body copy that cross-jumping folded; the tail 0x00BA464F (temps eax, ecx, edx) is one rotation step off the smooth-stencil tail 0x00BA42D0 (edx, eax, ecx).
  Result UNFORCED: DIFFER(9632v9632) = retail size, retail prologue registers (mesh edi, sub ebx, this ebp+[esp+14]): the two extra mesh_primitive reference blocks are the missing priority points.
- w/x2.cpp: `const C2DAlphaMaskInfo* mask = GetCurrent2DAlphaMaskInfo();` is the FIRST statement of cases 16/17/18, before the texture_stages stores (retail 0x00BA3214 / 0x00BA3555 / 0x00BA399A load
  GPPrimitiveTypeInfo->manager before `mov [eax],0`). Instruction count = retail (2401).
- w/x3.cpp: + `#define FABLE_RETAIL_CARRAY` (existing repo opt-in: CArray::operator[] through begin()): closes the SetPixelShaderConstant block 0x00BA2489 (`mov ecx,eax` inline-this copy, T in eax,
  copies through edx). The web-join theory of agentC20 is not the cause (k2 diagnostic: no change); a named element/table pointer in the helper gives the same bytes but contradicts FableWin's 28i size.
- w/x4.cpp (= f1): case 5 passes a NAMED LOCAL to the two filter setters (`TEXTURE_FILTER_MODE filter_mode = TEXTURE_FILTER_POINT; SetTextureFilterModeMin(1, filter_mode); ...Mag(1, filter_mode);`):
  retail 0x00BA27BD..0x00BA2840 has no global-candidate / `mov edi,esi` copy form at these two sites (it has it at the UV / transform sites 0x00BA28AD, 0x00BA291C, 0x00BA29E0): value esi / desired edi at
  site 1, value edi / desired esi at site 2 = single 3-reference locals (priorities 38 > 24 > 18 reproduce the esi/edi alternation). Name `filter_mode` is made up (FITTED name, EVIDENCED shape).
  State: DIFFER(9632v9632), 2401 instructions, every address aligned; frame-masked residue = bookmark test/or operand + load order at ~13 SetRenderState expansions only; plus frame slots (exact diff 114 hunks).
- Frame slots (tools/fm.sh = /FAs symbol map v retail; tools/pk.sh = c2 frame-packer probe, order + slot joins): w/w2.cpp -> case 5 holds the camera window BY VALUE
  (`C2DBoxF window = GPCamera->PeekWindow();` with the existing opt-ins FABLE_RETAIL_C2DBOXF + FABLE_RETAIL_C2DBOXF_COPY = member-wise float copy constructor). Retail 0x00BA2752 / 0x00BA275E keeps BRX at
  [esp+24h] = offset +8 of a 16-byte slot shared with the 16-byte vector temporaries (+1Ch..+2Bh); a named `float right` is a 4-byte node that joined render_mode's slot instead. With it every frame slot
  is in retail's order except ONE membership (below). x87 operand order then needs the four named floats kept (w/y2.cpp = w/x5.cpp) or the sums spelled TLX+BRX / TLY+BRY (w/y1.cpp).
- OPEN frame residue (all displacements +4 between +10h and +3Ch): retail slot +10h = {tinted, the G tint-byte temp, all eight used_blank}, and the first __ftol2 byte temporary of GetRGBColour (ours $T12964)
  has its own slot at +40h; ours: used_blank x8 join the slot of that byte temporary (packer rule: join the latest slot without conflict; the three byte temps are w2 and come before the w1 used_blanks,
  tie order = time of last IL reference), so {tinted, G temp} sit at +18h. Tried: one function-scope / switch-scope used_blank (conflicts with tinted: own slot, worse), GETRGBCOLOUR_NAMED_RESULT (code changes),
  CTOR4_INITIALISE off (neutral), SetTextureFactor with named rgb_colour / factor / C4DVector constant (all byte-identical).
- OPEN bookmark residue: 13 of 34 SetRenderState expansions (tools/sites.py matrix) have test/or operand order or mask/flags load order swapped. Ruler sweep of 0..100 dead FABLE_RETAIL_ASSERTs
  (position-independent: top / case / end give the same result; only the TOTAL matters): best 3 bad at N=23 (but SetFogEnable tails then cross-jump differently), 6 bad at N=16,17,19,20; no N gives 0.
  Site 12 (0x00BA365A, case 17 first DisableTextureFiltering setter) is wrong for every N in 0..100 -> the missing record(s) are not a uniform count; lead: FableWin sizes say SetTextureFactor (35i v 29i),
  GetRGBColour (54i v 30i), CRGBFloatColour(const CRGBColour&) (46i v 38i) have other texts (not found; candidates measured in ATTEMPTS / notes), and the row is 1207i v FableWin 1488i at /Od /EHsc (~3 asserts).

## State at hand-back (wfB3)
- differ/00ba2350.cpp (= w/x5.cpp + header): DIFFER(9632v9632), 2401/2401 instructions, exact diff 108 hunks / 144 lines, frame-masked 24 hunks / 28 lines (tools/gs.sh, tools/fdq.py).
  NOT matched. No forced priorities, no stand-in bodies. Opt-ins added to the row (all EXISTING repo opt-ins, no header edits): FABLE_RETAIL_CARRAY, FABLE_RETAIL_C2DBOXF, FABLE_RETAIL_C2DBOXF_COPY.
- differ/tu_00ba2350_00ba5260.cpp (= prev/w_C20/tu2.cpp with the new body + the three opt-ins): in the TU 0x00BA5260 RenderMeshPrimitive RELOCATION_MATCH, 0x00BA50A0 / 0x00B9D670 / 0x00B9E5B0 /
  0x00B9E750 RELOCATION_MATCH, 0x00BA2350 DIFFER(9632v9632) with the same residue as stand-alone. Row 2 still lands only with a matching row 1 (it needs the real body in the TU). No test written for either row.
- Regrade set on this overlay (hdr/ unchanged by me): c19a 0x00B9E5B0, 0x00BB30A0, 0x00BB3E80 all RELOCATION_MATCH.
- Spellings: EVIDENCED = duplicated stencil/fill arms, mask-first in cases 16/17/18, by-value window, filter_mode local (shape), FABLE_RETAIL_CARRAY. FITTED = the name `filter_mode`; the four named floats
  kept beside the by-value window (w/y1.cpp without them grades one exact hunk worse, same frame).
- Measured dead ends (do not retry): SetTextureFactor text variants (named manager pointer / reference / bool / rgb_colour / factor / C4DVector constant): byte-identical; k2 "web local" diagnostic: no
  effect on the PS-constant block; Min/Mag order permutations in case 5; GETRGBCOLOUR_NAMED_RESULT and INITIALISE_CTOR flips (code changes); one shared used_blank; a dead assert reading used_blank
  (packer weight unchanged: dead-arm references do not count); empty-inline ruler calls (no effect at all); ADDSTATE_DIRTY (acts like a uniform count, +8 asserts equivalent, 9 bad);
  FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED alone makes cl spin on this row (header nesting; not pursued).
- /Od size hints still open (FableWin later debug build): SetTextureFactor 35i/105b (ours 29i/85b; `C4DVector constant; constant.InitialiseFromColour(colour);` gives 32i/96b, + `unsigned long factor`
  34i/102b), GetRGBColour 54i/165b (ours 30i; eight GFFloatToLongTrunc / named-local spellings give 34..46i), row 1488i v ours 1207i at /Od /EHsc (about three asserts).
- NEXT (ordered): (1) frame: find why retail's used_blank x8 do not join the byte temporary's slot -- in the packer model they must CONFLICT with the three GetRGBColour byte temporaries or be processed
  before them; the `first` events of tools/pk/pkg.py (first-fit path 0x107349C0) are not interpreted by pkj.py and may be the real join rule for 1-byte items: decode them first (10 s per probe,
  tools/pk.sh). (2) bookmark sites: only the TOTAL record count moves them and no total in 0..100 fits, so retail's optimizer-temp sequence differs between sites: look for helper texts whose expansion
  creates a different number of temporaries (SetTextureFactor / GetRGBColour / CRGBFloatColour(const CRGBColour&) are the three with FableWin size gaps and all sit before site 0). (3) then the
  count of release asserts (tools/mka.py + tools/sites.py give the matrix in one line per variant).
- Tools written here: tools/gs.sh (one-line grades), tools/gq.sh, tools/fdq.py (frame-masked / exact / register-masked hunks from q.py -v), tools/sites.py (bookmark-site matrix), tools/slots.py,
  tools/fm.sh (frame symbol map), tools/pk.sh (packer probe), tools/gp.sh (probe overlay probe/hdrp, never hand in), tools/mka.py / mkr.py (ruler variants, diagnostic only), tools/fd.py (obj v retail).
- Grades used (wfB3): 174 logged grades (ATTEMPTS.log; ~100 of them the assert-ruler sweep) + 4 c2prio traces (1 forced, 1 rotation) + 9 packer probes + 8 fwsize runs.
- Process note: one read-only `git status` was run by mistake at the end (no write, no output); nothing else touched the repo.

## Successor (wfB3 second agent, 2026-11-14) -- 0x00BA2350: both open residues are now MEASURED mechanisms (not closed)
Start state regraded: differ/00ba2350.cpp = w2/b0.cpp DIFFER(9632v9632), exact 108 hunks, frame-masked 24 hunks (same as hand-over). No better source: differ/ is unchanged.
New tools (all diagnostic): tools/ab.py (per bookmark site two bits: A = load order of mask/flags, B = test/or operand order; line 1 = retail), tools/gab.sh (grade variants + A/B matrix),
tools/shp.py (role-normalised site shapes), tools/tp.py (c2prio candidate pairs per site), tools/lka.sh + tools/lk/lka*.py (wfB6's comparator log 0x1076A394 without the float filter: EVERY operand-sort
comparison with keys and c2 record ids; 6 min per run, tmp/lk_b0.json), tools/mkb.py (ruler variants of w2/b0.cpp: asserts at top / inside helpers stf sbt dtf m2d aps avs g2d), tools/cd.sh (c2.dll disassembly).
### Bookmark residue = two independent bits per site (34 sites; retail A 0000001111110000000000000000001111, B 00000011000?0111111111111111100000; ours A ...1111111111 at sites 6-16 and 0 at 30-33, B wrong at 0, 6, 7, 29)
- c2 CANDIDATE ids do not move with ruler asserts (c2prio of a00 v a20: identical ids and keys) -- the deciding numbers are c2 SYMBOL RECORD ids (lesson r3005 / wfB6), read from the comparator log.
- Bit A (which of mask / flags is loaded first) is decided at the FIRST sort, on the un-CSE'd trees `state.BookmarkMask & CurrentBookmarkMask`: A = 1 iff key16(flags tree) > key16(mask tree), no swap.
  Measured keys (ours, site order: 1 TextureFactor | case 5 x5 | case 16 x6 | case 17 x6 | case 18 x6 | case 15 x6 | Z_WRITE x1 | tail x4): flags tree 797e | 8cde 8d3e 8d9e 8dfe 8ebe | d69e d6fe d75e d73e d7de f47e |
  f6be f71e f77e f75e f7fe 211f | 231f 237f 23df 23bf 405f 40ff | 42df 433f 439f 437f 441f 6cbf | 6edf | 8c1f a49f a51f a59f; mask tree = flags tree + ~0x4f38 (structural). So A = 1 iff the flags key is in
  [0xB0C8, 0xFFFF]. The key moves with the record ids of the expansion's inline parameter records (+0x60 per setter inside one helper), i.e. with the number of records allocated BEFORE that expansion.
  Retail therefore needs, as cumulative key shifts s over ours: case 16 (incl. its fog site) s <= 0x0B81; case 17 s >= 0x0942 (wrap); case 15 / Z_WRITE s < 0x41E9; tail (four UV setters) 0x24A9 <= s <= 0x5A60.
  => retail allocates about (0x24A9 - 0x0B81) / 64 = ~100 MORE symbol records than ours between the case-16 expansions and the tail setters (cases 17 / 18 / 15, the code after the switch, the Z_WRITE and outline
  passes), and ~40 more before case 16/17 (an outer ruler of 2..7 asserts gives sites 6-16 retail's A). More duplicated arms in the outline passes (like the stencil / fill pair) or bigger helper texts expanded
  there (IsDP3Bump x15, ApplyPixelShader, ApplyVertexShader x5, SetupBlankTextures x6, MeshEffect.Get x5) are the natural candidates.
- Bit B (test/or operand order) is decided at a LATER sort on the two CSE temporaries (leaf v leaf, key = record id * 32 mod 65536, descending): one temp from a per-expansion series (ours 4068, 4238.., +36/37 per
  setter), one from a series created after all inlining (ours 7651, 7770.., +17..20 per setter). So B depends on the records before the site AND on the function's TOTAL.
  Ruler measurement: B is right at ALL sites (also 0x00BA365A, which no outer count fixes) when the total is 16..18 outer-assert units with at least ~5 units inside the SetTextureFactor chain:
  stf=8; stf=7 top=2..4; stf=9; stf=6 top=4; stf=5/6 + one assert in SetupBlankTextures (grid in ATTEMPTS.log "diag grid"; unit: 1 assert in SetTextureFactor = 2, in SetupBlankTextures = 6, at top = 1;
  one outer assert = 7 record ids). The SetTextureFactor chain is exactly where FableWin's sizes say our texts are too small (SetTextureFactor 35i v 29i, GetRGBColour 54i v 30i = four more calls at /Od,
  CRGBFloatColour(const CRGBColour&) 46i v 38i): retail's GetRGBColour most likely converts each channel through an inline helper (4 expansions = the missing records). Not found yet.
  With B right, A is still wrong at site 11 (case-16 fog) and tail sites 30-32: A's tail needs more records than B's total window allows inside this ruler family, i.e. the real texts differ in WHERE records are made.
### Frame residue: packer rules read from c2.dll (13.10.3077)
- Item list (0x108A78A4): insertion 0x10788660 / 0x10789A99: size ascending, then weight descending; a new item goes to the END of its size group, each later reference adds the weight and bubbles the item up
  only past STRICTLY lighter items (ties keep the order in which the weight was reached = predecessor's "time of last reference").
- Packing runs TWICE per function (tmp/pk_w2.json): round 1 creates one slot per stack parameter (0x10790B2F..) and first-fits locals ASCENDING into dead parameter homes (0x10790CFD..0x107349C0, "first" events);
  round 2 starts again without parameter slots and scans from the LATEST slot down (0x10791426 -> 0x10791A6A): join iff item size <= 2 * slot size, the item is not in the slot's conflict set and the item's
  interference set does not meet the slot's members; else a new slot. Final order = density (weight / size) descending (presort / postsort dumps).
- Retail slot +10h holds tinted (setne 0x00BA23C0 + 3 reads), the byte temporary of 0x00BA2389 / 0x00BA2394 and ALL nine used_blank address-takes (9 x `lea r,[esp+18h]` after two pushes); the three __ftol2 bytes
  are separate at +3Ch / +40h / +44h (density 2 each, so each is still a 1-byte weight-2 item).
- Ours (eight block-scope used_blank): list = tinted w4, tmp w2, G, B, A w2, used_blank(case 17) w2, seven used_blank w1 -> every used_blank joins the latest slot (A's). Under the rules above eight symbols can
  never all reach tinted's slot (weight-1 items always come after the weight-2 bytes), so retail has ONE used_blank (weight 9, processed first, tinted and the temporary then join it).
- Tried: ONE `bool used_blank;` at the top of the COMBINED block / before the switch / first in the block (w2/u1 u2 u3: byte-identical objects, exact 107 hunks): used_blank gets slot 0 but CONFLICTS with tinted
  (iset 0 = {tinted, this, texture_stages}; no conflict with the __ftol2 bytes), because tinted is read in cases 9 / 11 / 12 between the first and last address-take. Retail needs the opposite: no conflict
  with tinted. Open question for the next agent: what makes tinted's and used_blank's ranges disjoint in retail (the conflict looks like a first-reference..last-reference hull in block order: test by moving
  cases, or by a per-case copy of tinted), or whether round 1's joins feed round 2's conflict sets (0x107AFAC0 merges sets at a join).
### State
0x00BA2350 stays DIFFER(9632v9632) (near miss); 0x00BA5260 unchanged (RELOCATION_MATCH only inside the TU; no test). Nothing handed back with rulers: every assert / mkb variant is diagnostic (w2/).
Grades used by this agent: ~95 (ATTEMPTS.log) + 3 c2prio traces + 1 comparator log + 1 packer probe.
- Conflict model that fits every packer measurement so far (8 block-scope copies: no conflict with tinted; one variable at function / block / switch scope: conflict with tinted, none with the __ftol2 bytes):
  a conflict is recorded where one symbol is born / defined while the other is live; a block-scope out-flag is born in its own case (tinted dead there), a single one is live at tinted's `setne` or born while
  tinted is live. Under this model neither spelling gives retail's slot, so one premise is still wrong: next measurements = (1) dump the interference sets again AFTER round 1 (does 0x107AFAC0 write into the
  item sets?), (2) the weight increment ebx at 0x10789A99 per reference (is it always 1 in switch cases?), (3) a `tinted` that is not one weight-4 symbol (e.g. read through an inline helper parameter).
- NEXT (ordered): (a) fwsize-fit GetRGBColour (54i: four inline conversion calls) / CRGBFloatColour(const CRGBColour&) (46i) / SetTextureFactor (35i), regrade and read tools/gab.sh: bit B should come right
  without any ruler; (b) then re-read bit A's key windows with tools/lka.sh and look for ~100 records between case 16 and the tail (duplicated arms / helper texts of the outline passes); (c) the frame.

## Third agent (wfB3c, 2026-11-15) -- 0x00BA2350
- FRAME RESIDUE CLOSED (w3/t1.cpp): the out-flag of every GetReflectionTexture / Get2DAlphaMaskTexture call is the SAME variable as the tint flag (`tinted` passed as the `bool&` argument, no `used_blank`
  locals). Retail +10h holds the setne result 0x00BA23C0 and all nine `lea r,[esp+18h]` address-takes; a second variable can never share that slot (packer rules above). Behaviour is unchanged: the flag is
  read only in cases 9 / 11 / 12, which pass it to nobody, and nothing after the switch reads it. Grade: DIFFER(9632v9632), exact hunks 108 -> 31, exact == frame-masked (every esp offset is retail's).
  The variable's real name is unknown (one bool reused; `tinted` kept, FITTED name).
- w3/t4.cpp = t1 + existing opt-ins FABLE_RETAIL_C4DVECTOR_INITIALISEFROMCOLOUR + FABLE_RETAIL_CRGBFLOATCOLOUR_CTOR4_INITIALISE (FableWin sizes 12i/28b and 22i/52b): byte-identical to t1, A/B unchanged.
- Remaining residue = ONLY the two bookmark bits (tools/ga.sh: one line per variant with the A/B matrix, tools/ab.py). Unused `long unused_N;` locals (k = 1..8): byte-identical (no record).
- EXACT B RULE (tools/lkb2.py on the comparator log, verified on b0 and on a variant): B = [kX >= kY], kX = 2*X mod 2048, kY = 2*Y mod 2048 (X, Y = c2 record ids of the two CSE temporaries; X advances
  36/37 per setter, Y 19/20). tools/bsolve.py: with ours' spacing retail's B needs X shifted by +190..208 record ids (mod 1024) = 17.5..19 dead-assert units (one dead-arm assert, outer or per expansion
  inside a helper, = 12 record ids of TOTAL, about 7..8.5 of them before/at the site in the first pass). So B is right for N = total assert expansions in {18, 103-104, 189-190} (period 85.6):
  measured right at N = 104 (2 asserts at the top of SetRenderState + 36 outer) and at 189/190 = exactly FableWin's assert counts in every expanded helper (SetRenderState 2, AddStateToUpdateList 1,
  AttachD3DTextureToStage 3, CArray::operator[] 2, GFDebugPDynamicCast 1; fwsize "assert?" gaps +168i / +85i / +285i / +177i / +90i; row +283i = 3). Site 12 (0x00BA365A) needs d = kX - kY < 0 there and
  >= 0 at site 13 (ours b0: -36 / 0; with helper asserts +16 / +52 -> wrong): the X..Y distance is a third quantity.
- A (load order) is NOT linear in the count (first-pass ids come from a free list): 2 dead-arm asserts at the top of SetRenderState alone (probe overlay probe/hdrq, macro PROBE_SRS_TOP) give retail's A at
  ALL 34 sites (w3/q7.cpp, robust to 0..8 outer asserts), but B then needs 36 outer asserts, which destroy A. Helper asserts also change the fog tail merges (size 9612 / 9580) until both bits are right.
- Probe machinery (diagnostic, never hand in): probe/hdrq = hdr + copies of CRenderStateManager.h / CRenderManagerCore.h / CArray.h / GFDebugPDynamicCast.h with PROBE_* insertion points; tools/mkp.py
  (variants), tools/mkc.py (row-only rulers), tools/ga.sh, tools/lkab.py / lkb2.py / bsolve.py (log readers; a log = tools/lka.sh, 6 min).
- CORRECTED A WINDOWS (first-pass id = key >> 5, A = 1 iff id mod 2048 in [1414, 2047]; ours t4: fog16 1955, case-17 first 1973, tail four ~3360..3372, ZW 2934): retail needs shift(fog16) <= 92,
  shift(case 17) >= 75, shift(tail) >= ~102 (NOT 294: the second agent's 0x8C1F is not a tail site), i.e. only ~10 more first-pass ids between the case-16 fog site and the tail than before it.
- BOTH BITS RIGHT AT ALL 34 SITES (diagnostic, w3/s4/d_001_18.cpp, graded with HDR=probe/hdrq): t4 + ONE dead-arm assert in GFDebugPDynamicCast (= FableWin's count for that helper; 4 expansions, 3 of
  them after the case-16 fog site) + 18 dead-arm asserts in the row (any position: top / case 5 / Z_WRITE / end give the same object). DIFFER(9632v9632), 6 hunks = the prologue (below) + the pad.
  Neighbours: 17 row asserts -> tail A wrong (4 sites); 15 / 16 -> + B site 8; other N = 16..22 mixes (CArray::operator[] x2, AttachD3DTextureToStage x1, + row asserts) have B right and A wrong at
  the fog16 / tail pair (tmp/s4.txt, tmp/s3.txt). FableWin's row gap (+283i) says about 3 row asserts, so 18 is a FITTED ruler, not the real text: NOT handed back.
- OPEN, the last residue with the single flag variable (5 hunks, 0x00BA2379..0x00BA23C7): retail `cmp eax,0FFFFFFh ; mov eax,[esp+0C4h] ; setne byte ptr [esp+10h]`, ours `setne al ; mov [esp+10h],al`
  and two loads (`mov ecx,[eax+3D4h] ; mov al,[esi+5Fh]`) scheduled three instructions early. It is there in EVERY variant whose flag is address-taken (t1, t4, all rulers: not a count tie) and absent in
  every variant with a separate non-aliased `tinted` (b0, x1). So retail's setne target is a NON-aliased bool whose home is the shared slot: retail has two kinds of symbol in slot +10h after all
  (tinted + the out-flag(s)), and the single variable only imitates the packing. Tried for the setne form (all unchanged): `bool tinted; tinted = ...`, `?: true : false`, negated ==, a `bool&`
  alias for the calls (y4), an inline `IsTinted(colour)` helper (v1), `const bool is_tinted` copied into the aliased flag (v3); `if (...) tinted = true` changes the code (9720).
  One separate function-scope `bool used_blank` (w3/x1.cpp) has the prologue right and the frame wrong again (exact 107 hunks, +4 on +10h..+3Ch), as the second agent measured.
  Lead not tested: retail's out-flags may skip the three __ftol2 byte slots because those temporaries' records are REUSED by the Z_WRITE SetTextureFactor expansion (free-list record reuse: their
  hull then spans the whole switch and conflicts with every block-scope used_blank, which leaves tinted's slot as the only one without a conflict). GetRGBColour through FableWin's
  `long __fastcall GFFloatToLongTrunc(float)` (6i / 15b) in the argument list is byte-neutral and does not move the frame (w3/z2.cpp); named-local GetRGBColour / CRGBFloatColour(const CRGBColour&)
  texts that reach FableWin's 54i / 46i change the code (w3/h1, h3, z3).
- fwsize fits found (hints only): GetRGBColour 54i = four named floats, four named bytes through GFFloatToLongTrunc, a named CRGBColour, return (tools/fwm.sh m3: 54i/161b v 54i/165b, the +4b is
  FableWin's movzx byte-load form as in CRGBColour(EEEE)); CRGBFloatColour(const CRGBColour&) 46i = four floats read first, then Initialise(r * k, ...) (m6: 45i); GetPixelShader2DDiffuse* 19i v 16i.
- 0x00BA5260: differ/tu_00ba2350_00ba5260.cpp now carries the frame-fixed row-1 body (w3/tu3.cpp): RenderMeshPrimitive still RELOCATION_MATCH inside the TU (tools/gl.sh 00ba5260 r1), SetupStaticBlocks
  RELOCATION_MATCH. Still not addmatched (needs a matching row 1), no test written.
- State: differ/00ba2350.cpp = w3/t4.cpp, DIFFER(9632v9632), exact 31 hunks (was 108): 5 prologue hunks + the bookmark bits. No header changes (hdr/ untouched; probe/hdrq is diagnostic only).
- NEXT: (1) the packer question with a NON-aliased tinted: make the block-scope out-flags conflict with the three __ftol2 byte temporaries or precede them (tools/pk.sh; test the record-reuse lead by
  logging the temporaries' record ids at both SetTextureFactor expansions); (2) with the real variable structure re-run tools/ga.sh grids (tools/mkp.py; B windows N = 18 / 103-104 / 189-190 units,
  A thresholds above) -- the count will move with the eight extra symbols; (3) then test + addmatch both rows from the TU.
- Eight block-scope used_blank + non-aliased tinted (w3/b4.cpp = b0 + the two FableWin opt-ins) under the same rulers (tmp/s5.txt): the frame stays wrong for every count (92..104 exact hunks), so the slot
  membership is not a record-count effect; B is right there at 1 GFDebugPDynamicCast assert + 12..14 row asserts (the seven extra symbols are worth ~5 assert units), A still wrong at fog16 + tail.
- Grades used by the third agent: ~313 logged (ATTEMPTS.log lines 270..596; four background grids of 60 / 139 / 40 / 14 variants), 1 comparator log (tmp/lk_q7.json), 10 fwsize runs. No vland, no addmatch,
  no git command, hdr/ untouched.

## Fourth agent (wfB3d, 2026-11-15) -- 0x00BA2350 prologue (setne form) + 0x00BA5260 under the asserted row-1 body
- Regraded: differ/00ba2350.cpp (= w3/t4) 31 hunks; w3/s4/d_001_18.cpp (HDR=probe/hdrq) 6 hunks = 5 prologue hunks + the 2-byte `mov edi,edi` pad at 0x00BA48A6
  (the pad hunk is only the 2 bytes our setne-al form costs: retail's code ends 2 bytes earlier).
- VC7.1 setcc rule (toy probes probe/sc/t1..t3.cpp): a store of a comparison to ANY memory destination that is not a spilled non-aliased
  symbol (aliased local, bool[1], struct member, *ptr, ref, member via this) is always `setcc reg ; mov [m],reg`. `setcc byte ptr [m]` only
  appears for a non-aliased symbol whose home is memory. So retail's `setne byte ptr [esp+10h]` (0x00BA23C0) defines a NON-aliased bool.
- w4/g1.cpp (t4 + non-aliased `is_tinted` computed at the top and copied into the aliased flag just before the switch): the prologue is
  retail's EXACTLY (setne [mem] after `mov eax,[esp+0C4h]`, both loads where retail has them), but the copy costs 12 bytes (9644) and the
  packer puts is_tinted elsewhere (+43h): at the copy is_tinted is live, so it is in the aliased flag's interference set.
- /Oa (an accepted landed flag elsewhere): wrong for this TU (t4 9200 bytes, b4 9196). Never tried before; ruled out.
- `const bool tinted` + const_cast<bool&> at the nine calls (w4/c1.cpp): byte-identical to t4.
- Packer interference sets (tools/isets.py, tools/iscmp.py on tools/pk.sh json; 10 s per probe): an address-taken bool whose scope encloses
  the switch has `tinted` in its set wherever it is declared (function top before tinted k1 / after tinted x1 / combined block u1 /
  before the switch f1a / inside the switch body e2), even with ONE reference (case 5 f1, case 15 f2) and even with cases 9/11/12 moved
  after every address-take (w4/xd1.cpp). It also conflicts with every block-scope copy in other cases, but NEVER with the block-local
  temporaries (the three __ftol2 bytes, the 0x00BA2389 byte, tint_colour). Model that fits all probes: an address-taken local is live over
  its whole scope for cross-block interference; block-local temporaries interfere only locally. The sets are asymmetric (tinted's own set
  lacks the flag), but the join test checks both directions. Consequence: a non-aliased tinted (needed for setne [mem]) and ONE flag
  variable can never share a slot; per-case flags never conflict with tinted but are processed after the w2 __ftol2 bytes and join the
  latest slot (A's). Every natural structure therefore fails one of the two facts; the missing premise is still unknown.
- 0x00BA5260: w4/tu_d18.cpp = TU with the asserted row-1 body (d_001_18 + PROBE_DYN, HDR=probe/hdrq): RenderMeshPrimitive still
  RELOCATION_MATCH, and row 1 inside that TU = the same 6 hunks with both bookmark bits right at all 34 sites.
- Grades used by the fourth agent: ~12 full grades + 14 packer probes (ATTEMPTS.log "wfB3d").
### wfB3d BREAKTHROUGH: both rows RELOCATION_MATCH (src/00ba2350.cpp, src/00ba5260.cpp = one TU)
- Closer of the prologue: the BODIES of CEnginePrimitiveManagerMeshBase::GetReflectionTexture 0x00BA9830 and Get2DAlphaMaskTexture 0x00BA9A70
  defined in the TU (above or below the row: identical objects, c2 has whole-TU knowledge). Neither callee reads or writes its bool& parameter, so
  c2 knows the address is not captured: `tinted` stops being aliased, its definition is `setne byte ptr [esp+10h]` with retail's schedule, and the
  single-variable frame stays. Diagnostic chain: non-capturing stand-ins (w4/v1..v4); opaque-calling stand-ins + d18 rulers = RELOCATION_MATCH (v4);
  then the real texts (tu style on shared headers, like GetDiffuseTexture / GetBumpTexture already in the TU): both RELOCATION_MATCH THEIR OWN
  retail rows in this TU (q.py via tools/gl.sh with their leaves) -- GetReflectionTexture 0x00BA9830 was parked DIFFER(570v570) elsewhere.
- Bookmark bits: 18 dead row asserts + 1 in GFDebugPDynamicCast (new overlay opt-in FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT); sweep in the TU
  (tools/swa.py): only (dyn=1, N=18) matches among dyn 0/1 x N 0..26; the FableWin per-helper assert counts (tools/swp.py, probe/hdrq) do not.
- New overlay opt-ins: FABLE_RETAIL_CENGINERESOURCEMANAGER_SATRGBATEXTURE (FableWin ?GetSatRGBATexture, asserts pinned), FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT.
- Source defines added to the TU: FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES (existing opt-in), the SATRGBATEXTURE one, the dyn assert one.
- Hand-back (wfB3d): src/00ba2350.cpp + src/00ba5260.cpp (identical code, row-specific headers), test/00ba2350.cpp + test/00ba5260.cpp (host models,
  vt.py PASS), payload.json + oracle.tsv (addmatch), read-only vland: both RELOCATION_MATCH PASS, 0 RELOC_IDENTITY (RELOC_NAME warnings only: stale
  labels g_pEngineCamera / g_pEngineLightingManager / g_pEngineShadowRenderer / g_textureUVModeToD3D / g_NormaliseEpsilon / Nop_ret* v the PDB names used).
  Row 1 carries row 2's `// Retail-asm:` evidence line (the TU calls the shared GFFloatFloor helper inside RenderMeshPrimitive; without it purity says naked_asm).
- Bonus in the same TU: GetReflectionTexture 0x00BA9830 (parked DIFFER(570v570) in 2026-10-27_linux_hand683) and Get2DAlphaMaskTexture 0x00BA9A70
  (landed with local models) both RELOCATION_MATCH from this file (tools/gl.sh with their leaves); not addmatched (outside the batch, no test).
- differ/00ba2350.cpp is superseded (kept as the pre-breakthrough near miss).
