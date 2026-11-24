# 0x00839940 CThingGameObject::OnHit
## Lessons that apply (B24B5, read first)
- MATCHING_PLAYBOOK / sibling rows: a CCharString temp passed to a `const CCharString&` parameter is written as the implicit conversion
  (`MakeNoise("SIMPLE_NOISE")`, landed 0x007C3FB0 etc.); retail then re-takes the address (`lea eax,[esp+N]; push eax`) and builds the temp
  BEFORE pushing the other arguments (GetDef("MATERIAL_..", &p)). Explicit `CCharString("..", -1)` uses the ctor's eax and evaluates later.
- INLINE_DEPTH_REVISIT / 2026-11-03 FINDINGS: plain-inline sites past the caller-size cap (c2 counter g >= 9000) get a penalty; the cap is
  per function and counts EVERY inlined body (forced or plain). Retail's cap falls 2 sites earlier than ours (hero-experience site).
- fwsize.py lists helpers whose /Od size differs from FableWin: use it to find helper bodies that are not the original text.
- C2_ALLOCATOR_PROBE: constants are allocation candidates (const 0x2000 here, refs 3); a constant coloured before `this` can steal ebp.
## B24B3 (older) 0x00839940 CThingGameObject::OnHit -- B24B3 (triage only, not worked)
- Baseline regrade of the Codex pass21 draft (differ/00839940.cpp = base.cpp): DIFFER(5174v5195), 229 fg hunks, 547 raw lines (rawcmp on q_base.log).
- First divergence: `this` in ebp (retail `mov ebp, ecx` 0x0083994D) v ebx (ours); stack slots shifted by 4 (retail lea [esp+0x44] v ours 0x48,
  byte locals 0x58..0x5b v 0x5c..0x5f); frame 0xa0 equal. Register-role / slot-ranking residue over a 5 KB body (REGISTER_SWAP_SUMMARY class).
- Not worked: B24B3 spent its context on rows 1, 2 and 6; read pass21 PROBES.md (A01..A06) before any new idea.
## B24B4 (2026-11-24): regrade only -- DIFFER(5174v5195) 229 hunks (r0.cpp = differ/ unchanged). First hunks: retail `mov ebp,ecx` + `this`
spilled to [esp+0x24] (ours ebx, [esp+0x1c]); ebx/ebp role swap for the CGameScriptInterface pointer ([esi] load at 0x00839991); CRGBColour
byte locals 0x58..0x5b v ours 0x5c..0x5f, a float at 0x44 v 0x48. Not worked further: the coordinator raised item 5 (one-TU test) above it.
- First structural hunks (r0full.log): ours loads `this` back from its slot and keeps the constant 0x2000 in ebp (`mov ebp,0x2000` at
  0x00839BC3, no source literal 0x2000: a c2-materialised constant / flag mask) while retail keeps `this` in ebp throughout; at the
  MakeNoise(CCharString("SIMPLE_NOISE")) site retail builds the temp at [esp+0x14] and passes `lea eax,[esp+0x14]` (ours one slot lower and
  no separate lea). Next: find which source expression yields 0x2000 (a flag test such as `& 0x2000` inside an inlined accessor of
  CThing / CTCBase, or a bitfield) -- removing that repeated constant frees ebp for `this`; then recheck the 21-byte gap.
- Correction/lead (B24B4): the ebp=0x2000 constant comes from the TC-presence bitset tests (HasTC(TCI_DOOR) at [this+0x20] bit 13, and the
  [this+0x2c] bit 13 test at 0x00839DF2): ours `test dword ptr [reg+0x20], ebp` (constant hoisted into ebp, two uses), retail
  `mov eax,[ebp+0x20]; test ah,0x20` at both sites (word loaded to a register, byte test). So retail's HasTC / bitset::test body at those
  two sites yields a LOADED word (e.g. a by-value/`const bool&` result or `_M_w[n] & mask` read through a local), not a memory-operand test.
  Fixing that spelling should free ebp for `this` (the root of the ebx/ebp swap). CThing.h has three HasTC spellings (lines 366 / 378 / 380,
  one with `const bool& found = UsingComponents.test(type)`): try each per site class first.
- m1.cpp (overlay hdrm: FABLE_RETAIL_CTHING_HASTC_MATERIALISED + the ONHIT template copied into that branch; diagnostic): `this` moves to
  EBP as in retail, but every HasTC site materialises -> frame 0xa4, 5409v5195, 287 hunks. So the ebx/ebp swap IS the HasTC spelling;
  retail mixes forms (in-place `test byte ptr [ebp+0x24],0x40` for the template's inner test, `mov eax,[ebp+0x20]; test ah,0x20` for
  HasTC(TCI_DOOR) and the [ebp+0x2c] site, out-of-line bitset::test 0x00410DE0 late). Next: plain-inline (unforced) HasTC(type) with the
  forced template, and the STLport bitset::test body visible but noinline (Q32 lesson), so c2 decides per site as retail did.
- p1.cpp (overlay hdrp, plain-inline HasTC(type), template forced): 5170v5195, 230 hunks, this still ebx -- no.

## B24B5 (2026-11-24)
- c4: MakeNoise("SIMPLE_NOISE") implicit conversion (sibling form) -> lea/push matches retail. c5: GetDef("MATERIAL_*", &p) implicit
  conversion -> the three material arms now build the temp before pushing &phitting_material like retail: 5198v5195, 208 hunks.
- c7 = c5 minus both `nonse_params` const-ref shadow devices (Codex dead names): byte-identical, 208 hunks. BEST (differ/).
- HasTC<T>: FABLE_RETAIL_CTHING_HASTC_CLASSTYPE body (proven by retail's out-of-line copy 0x006AE680, `mov eax,[ecx+20h]; test ah,20h`)
  replaces the ONHIT template (c1 219 v 229 hunks). Forced / plain / CLASSTYPE HasTC all leave const 0x2000 enregistered: exhausted.
- Inline cap (c2 trace kit copied to inl/: run.sh + gh.py prints g at the hero site): ours inlines IS_THING_ALIVE + bitset::test(0x68)
  (+test(0x42)) at the hero-experience / entity-events GetTC where retail calls 0x0040DCE0 / 0x00410DE0. g at that site: c5 8666,
  c8 (default Find GetTC) 7738, c9 (GETTC/PEEKTC_ORIGINAL_CHAIN + CVECTORMAP_ORIGINAL_FIND, FableWin bodies) 8734, c10 (+CLASSTYPE)
  8808, c11 (+CDEFPOINTER_RELEASE_SELFDELETING_POINTEE + CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22) 8976, c13 (+IsDead
  `return Dead;`, FableWin 10i SAME) 8928. Need >= 9000. c10..c13 grade 5239..5245 bytes (unexplained +41 step, see below).
- DIAG d14 (not landable): forcing those two sites out of line aligns every call but `this` stays ebx: the inline cap and the register
  swap are independent residues.
- Allocator (c2prio, prio_c13_trace.txt): const 0x2000 (refs 3, the HasTC<CTCActionUseWindow> test and the two CTCFlammable GetTC
  tests) prio -99 coloured into ebp before `this` (-138, split); the early `this` piece then takes ebx. In retail `this` stays in ebp
  through the flammable block (interferes with const 0x6d in ebx), so its first piece must be coloured before const 0x2000.
- **c18/c19 (the big one): include `engine/CDefClassBase_GetSubDef.h` (the shared GetSubDef<T> template body, visible, plain inline;
  NOINLINE opt-in gives the same bytes) -> 220 -> 92 hunks, `this` in ebp as retail.** Retail keeps pcreature_def's pointee in edi
  across the loops (`mov edi,[esp+10h]` right after the 0x00485815 call): with the body visible c2 knows GetSubDef only writes
  through the out pointer (chunk 1021 lesson). Needs StatGetTypeName for CCreatureDef / CHeroExperienceDef: overlay opt-ins
  FABLE_RETAIL_CCREATUREDEF_STATGETTYPENAME, FABLE_RETAIL_CHEROEXPERIENCEDEF_STATGETTYPENAME (hdrc).
- c16 (kept): inner hero loop reads `stat = it2->first` / `increase = it2->second` (descriptive names) before the CHitThing ctor,
  as retail 0x0083A725. c21: `const C3DVector& hit_position = hit_params.GetHitPosition();` after IsLocalised and
  `hit_direction` inside `if (IsDirectional())` in the wound/degradable block (retail caches +2 / +0xE in ebx / edi) -> 5187v5195,
  92 hunks, 15 structural. BEST = c21 (differ/, overlay hdrc).
- Remaining (c21): (1) CRGBColour temp (frame 0x28 retail) and pweapon spill slot (0x2c retail) swapped in ours; (2) pweapon piece
  edx->edi copy at 0x00839D51; (3) `lea ecx` v `lea eax` order at 0x0083A204; (4) inline cap: g 8930 at the hero site (need 9000).
- Tried after c21 (no gain): c22 C3DVector ADD_NAMED_RESULT (FableWin 34i SAME, g +35 to 8965) breaks the debug-line block
  (126 hunks) -> keep the plain operator+; DIAG bsA/bsC bitset::test without _Unchecked_test (FableWin has no
  _Unchecked_test / __stl_throw_out_of_range copy) -> g drops to 8040, 174 hunks; c23 GETTC_EXPANDED on the c21 base = same 92
  hunks (original GetTC/PeekTC/Find chain kept, FableWin evidence); c24 `hit_params.PWeapon.Get()` through a friend opt-in = same
  92 hunks. NAME DOUBT: CHitParameters::GetPWeapon (header line 124) is in neither PDB (FableWin / Ego_r list only SetWeapon);
  retail OnHit probably reads PWeapon directly (friend) -- not changed, recorded here.
- g counter (inl/gh.py) is the function's running IL size, not a sum of decision sizes (plain v forced HasTC leave it unchanged);
  deeper inline nesting raises it. c21: 8930 at the hero site, needs >= 9000 and < ~9900.
## Exact state for the next agent (B24B5 end)
- BEST: differ/00839940.cpp = c21.cpp, DIFFER(5187v5195), 92 fg hunks, 15 structural (sd_c21.txt). Grade with
  FABLE_HDR_OVERLAY=$W/hdrc (README there). Residues: (1) CRGBColour temp slot (retail frame 0x28) and pweapon spill slot (retail
  0x2c) swapped; every other early hunk is that 4-byte shift; (2) pweapon piece edx->edi copy at 0x00839D51 / the 0x6d and
  `mov edi,[eax+4]` scheduling at 0x00839E05..E27; (3) `lea ecx`/`lea eax` order at 0x0083A204; (4) inline cap: retail calls
  IS_THING_ALIVE 0x0040DCE0 and bitset::test(0x68) 0x00410DE0 at the hero-experience site (0x0083A825..), ours expands them.
- Next ideas: (a) the slot swap: find which source fact orders the colour temp before pweapon (CRGBColour temp v a named colour
  local; pweapon's declaration point / its first address-taken use IS_THING_ALIVE(pweapon) at 0x0083A679); (b) inline cap +70 IL:
  HasTC<CTCActionUseWindow> body (FableWin 60i, ours 56i), CIVCountedPointeeBase::Release (FableWin 24i/58b, ours 23i/56b),
  CDefPointer::Release (FableWin 14i, ours 15i); use fwsize.py on c21 for the full list (fw_c11.txt is close).
- Corrections: the g = 8930 figure is c19's trace (c21 not traced). "Same" results for c7/c5, c19/c18, c23/c21, c24/c21 mean
  same grade and hunk count, not a byte compare.
- HAND-BACK WARNING: the visible plain GetSubDef<T> body makes the row's object emit pick-any copies of GetSubDef<CCreatureDef>
  (0x00485815) and GetSubDef<CHeroExperienceDef> (0x006DAA90). CDefClassBase_GetSubDef.h has FABLE_RETAIL_CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26
  because the 0x00485815 owner is size-optimised, so the plain copy probably differs: run pickany_vs_retail.py with and without it.
- NAME_FIXES.tsv: 0x006DAA90 is bound as a non-template QAE GetSubDef; both PDBs give the const template instance.
- Re-tested on the c21 base: c25 (no stat/increase loop locals) 5166v5195, 139 hunks -> the locals are needed (kept);
  c26 (plain-inline HasTC(type)) 5183v5195, 94 hunks -> forced HasTC (ONHIT opt-in) stays. Not run: d14-style diagnostic on c21
  (hero site only out of line) to test whether the colour/pweapon slot swap is downstream of the inline cap -- do it first next.
