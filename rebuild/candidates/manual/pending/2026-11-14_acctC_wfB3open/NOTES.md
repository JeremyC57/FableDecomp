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
