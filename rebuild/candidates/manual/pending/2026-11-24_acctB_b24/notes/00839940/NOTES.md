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
## B24B6 (2026-11-24)
- d21h (DIAG, c21 + hero site only out of line): 5195v5195, structural hunks unchanged -> slot swap NOT from the cap.
- Frame packer traced (pk/: pk.py + phook.py, qs3.py simulates c2's K&R slot sort 0x107AE0C4, comparator flag then w*1000/size,
  tie 0): `this`, pweapon, colour/damage slots all w=16; pweapon w=15 reproduces retail's order. The extra pweapon weight was the
  spill reload `mov edi,[esp+0x28]` at the join after the explosion block (pweapon piece edx->edi split).
- **e1: no separate `if (IS_THING_ALIVE(pweapon)) {` around the weapon block: `if (IS_THING_ALIVE(pweapon) && IS_THING_ALIVE(phitter)
  && pweapon->GetTC(&ptc_weapon))` then the flammable `if (IS_THING_ALIVE(pweapon) && ...)` at the same level -> 92 -> 7 hunks.**
  Slot swap, edx/edi copy, 0x6d scheduling, lea order all gone. Only the hero-site inline cap remains.
- Cap window (playbook: pen only when a+g > 9000; IS_THING_ALIVE a=9): need g >= 8992 at #567 (e1 8927 -> +65); retail still expands
  bitset::test at the summonable GetTC (#547, a=15), so the extra IL sits within +65..+408 before the hero site.
- Measured on e1 (g at hero / code): e2 C3DVECTOR_ADD_XY_GROUP +4 (26 hunks), e3 INITIALISE_ADD +44 (44 hunks), e4 ADD_NAMED_RESULT
  (44 hunks: retail debug block uses the return-by-ctor form), e5 RELEASE_COUNT -7 (7 hunks), hdrd comma-body __stl_throw_out_of_range 0,
  DIAG e6 (all 24 TC pointer locals before the hero site `= 0`) +24 exactly (+1 IL each, code unchanged) -- oracle-forced, not landable.
- FableWin's STL is Dinkumware (_Xran/_Tidy, no STLport): its bitset/STL sizes are no evidence. HasTC<CTCActionUseWindow> /Od variants
  (hdre FABLE_DIAG_HASTCV 1..8) never reach FW 60i/192b; one forced site, dropped.
- NAME: CHitParameters::GetPWeapon IS proven: ghidra_out/fablewin_pdb_names.tsv + fablewin_obj_functions.tsv (debug\thing_ai_creature.obj,
  0x01D75520, ?GetPWeapon@CHitParameters@@QBEPAVCThing@@XZ) -- closes B24B5's doubt.
- hdrd / hdre are DIAGNOSTIC overlays only (never for hand-back).
- DIAG d1h (e1 + hero site IS_THING_ALIVE/bitset::test forced out of line): RELOCATION_MATCH -> the cap is the ONLY residue.
- B24B6 landing form: overlay hdrf (README there; additive opt-ins only, no DIAG): FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT_HASTC_CLASSTYPE
  replaces B24B5's guard edit; e1f.cpp = 7 hunks on hdrf. e1p.cpp = e1f + FABLE_RETAIL_CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26
  (pick-any: GetSubDef<CCreatureDef> copy EQ only with it). Pick-any details + the cap analysis: ESCALATE.md.
- Local names stat / increase / hit_position / hit_direction: no Fable PDB on the lane to read local records -> descriptive (made up).
## Exact state (B24B6 end of row-1 work)
- BEST: e1p.cpp (hand-back candidate source; differ/ holds e1f = same bytes), overlay hdrf, DIFFER(5187v5195) 7 hunks = only the
  hero-site inline cap (ESCALATE.md). Next idea: find +65 pre-opt IL before #567 with evidence (none found); do not pad with `= 0`.
## B24B7 (2026-11-24): RELOCATION_MATCH (Inferred device -> ESCALATE.md section 0)
- Lessons applied: 0x00975BB0 precedent (dead release ASSERTs, fable_assert.h FABLE_RETAIL_ASSERT_DEAD_ARM, evidenced by FableWin /Od size).
- fwsize (fw_e1.txt, FW_EXTRA=/EHsc): FableWin.pdb OnHit 2761i / 12015b v this text 1544i / 5801b (+1217i, "assert?") -> ~29 debug ASSERTs.
- c2 trace (inl/gh.py, overlay hdrf): e1p g 8927 at the hero IS_THING_ALIVE (#566); each dead pointer ASSERT +18 IL (f1 8945), each
  ASSERT(phit_location->GetArmourDef().Get()) +28. Window (asrt/w*.cpp, N copies of ASSERT(phit_location)): N=3 g 8981 DIFFER(5195v5195),
  N=22 RELOCATION_MATCH (pure N=4 NOT graded; f2/f3/f4/f5/f6 cover +74..+310), N=23 DIFFER(5214v5195) 149 hunks (summonable bitset::test crosses the cap too).
- FINAL = final.cpp = differ/00839940.cpp = payload.json (= asrt/f6.cpp + header), overlay hdrf (NOT hdr): five ASSERTs, one on each
  pointer result dereferenced without a null test before the hero block: phit_location->GetArmourDef() at the DamageMaterial and the
  BlockMaterial reads, ptcd_explosion after GetTCDriver, GetPBaseDef() and phitter->GetPBaseDef() before their GetSubDef calls.
  g: hero site 9237 (cap at 8992, +245 margin), summonable bitset::test 8892 (must stay <= ~8985, 93 headroom).
  Graded IL window: +54 (w3) DIFFER; +74 (f4), +92 (f2), +114 (f3), +192 (f5), +310 (f6), +396 (w22) RELOCATION_MATCH; +414 (w23) DIFFER.
  f2 (with ASSERT(phit_location)) dropped: that pointer IS null-tested on every path above, so it is not an unchecked result.
- Test 00839940_test.cpp (model of the row's decisions): vt.py PASS; mutated copy (absorb-hits `<= 0` -> `< 0`) FAIL.
- Strict vland (hdrf, vland.log): RELOCATION_MATCH PASS. Warnings, none from this row's text:
  * RELOC_NAME x6, labels of byte-identical (ICF-folded) or placeholder bodies: 00814be0 GetPDefaultHitLocation (label
    GetMember_dword0x18 placeholder, body `mov eax,[ecx+18h]; ret`), 007331e0 IsBlocking (placeholder GetMember_byte0x63; reloc_bindings
    already binds ?IsBlocking@CTCCombat there), 00815270 IsToPlayGetHitResponse (folded 7-byte getter also bound as
    CTCShotDefinitionBase::CanBeHitByMeleeStrike), 0129ba3c DELTA_NOTIONAL_ZERO (reloc_bindings binds this symbol there; label is a
    folded-constant alias), 006a92b0 CJoystick::CInitRumble ctor (placeholder CRecord_006a92b0), 0083af60 GetPHitMaterialDef (folded
    with CThing::GetPBaseDef: same `CDefPointer` copy of the member at +0x4C). Sources (grep): GetPDefaultHitLocation, IsBlocking,
    IsToPlayGetHitResponse, ??0CInitRumble: egor_pdb_names + fablewin_pdb_names (1 hit each); DELTA_NOTIONAL_ZERO: egod_carved_names PUB32 +
    reloc_bindings (0x0040ED10) + labels_nf237; GetPHitMaterialDef: fablewin_pdb_names + fablewin_obj_functions (debug\thing_game_object.obj
    0x027EBBE0) + engine/CHitParameters.h. NAME_FIXES.tsv: 0x006A92B0 placeholder, 0x00814BE0 placeholder (folded getter), DeregisterCombatant.
  * RELOC_CONFLICT DeregisterCombatant: retail OnHit calls 0x006EA430 (listing). Two LANDED rows already claim this symbol:
    006ea430 (agentNF22: IsInMeleeAsAttacker -> PerformSequenceReset 0x006EA2C0) and 006ed9c0 (NF122: -> DeregisterAttackingCombatant
    0x006ED510, used by landed 0x007355F0). Ego_r has ONE DeregisterCombatant (0x007173E0, 32 b, release\combat_manager.obj), so one of
    the two landed names is wrong. DECIDED (B24B7, NAME_FIXES.tsv): 0x006EA430 is CCombatManager::SetAttackerAsBeenStruck (Ego_r
    combat_manager.obj record 81, 32 b, right after PerformSequenceReset as in retail); 0x006ED9C0 is DeregisterCombatant (retail order =
    Ego_r object order). OnHit keeps the landed spelling (brief) and is named as a caller to respell; header line 24 says so.
- Pick-any on the f6 final (pick_final.txt, stock bindings): 11 EQ + 2 NEQ; with rb_fix.tsv (GetSubDef<CHeroExperienceDef> under its PDB template
  symbol, NAME_FIXES.tsv) 12 EQ + same 2 NEQ. NEQ 0x0066C340 GetDef<CMaterialDef> (LEAF-REFERENCED) is lane-wide HEAD debt of the shared
  GETDEF_GLOBAL_BODY text (landed 0x008F7950 emits the same copy), reported to Account A, not fixed here; NEQ 0x004292C0
  _Rb_global<bool>::_M_increment not leaf-referenced (shared STLport header copy). CREATURE_OWNER_SIZE_PASS26 kept defined.
- What made it match: retail's source carried IL before the hero site that ours lacked; five dead release ASSERTs (+310 IL) supply it.
- STATE: ready for the lander, but the ASSERT device is Inferred (author may not land it): ESCALATE.md section 0.
- B24B7 text fixes after advisor review: header says "five of" the unchecked results (CMainGameComponent::Get() x2 in the debug block not
  asserted); ESCALATE notes the dead-arm CDefPointer temporaries and f4 as the no-temporary alternative (margin 9). ISDEAD opt-in name:
  hdrf's FABLE_RETAIL_CTHING_ISDEAD_BITFIELD and 0x00975BB0's FABLE_RETAIL_CTHING_ISDEAD_DIRECT are the same `return Dead;` text -- the
  lander should merge them into one opt-in.
- Pick-any gap check (brief 2026-11-24, B24C9): pk2/leafcopies.py compares EVERY COMDAT copy the OnHit code calls (targets mapped from
  retail's own call displacements, relocations masked): 12 EQ (GetSubDef x2, GetTCDriver<CTCDExplosion> 0x007512C0, IS_THING_ALIVE,
  bitset::test, LowerBound x2, map find, iterator ++ x2, CEnginePrimitiveLine ctor, GetPHitMaterialDef) + 1 NEQ = the known HEAD-debt
  GetDef<CMaterialDef> 0x0066C340 (pk2/leafcopies.txt). STATE: ready for the lander (Inferred device, ESCALATE section 0).
## B24B8 (2026-11-24): Account A's ELEVEN FableWin asserts (FableWin.exe OnHit 0x027EC280) replace B24B7's five fitted ones -- g11/
- Base: payload_g.json source (g11/g_base.cpp) + overlay hdrg: RELOCATION_MATCH regraded.
- IL window (c2 trace, inl/gh.py + summonable-site g): retail needs the summonable bitset<274>::test EXPANDED (g <= ~8990 there) and the
  hero-experience IS_THING_ALIVE CALLED (g >= ~9001, the site is +350 IL later), i.e. the summon-site g in about [8650, 8990].
  g counts the function's whole IL (g0 rises with asserts placed AFTER the hero site too), so every assert anywhere counts.
- (A) All eleven in the landed success-form spelling (0x00627BE0: `FABLE_RETAIL_ASSERT(call); call;`): g11/a11.cpp summon 9254 / hero 9499
  -> DIFFER(5214v5195) 149 hunks (frame 0xA4, summon bitset::test called). Bisect: asserts 1..6 fit (s6 8876/9226 RELOCATION_MATCH);
  #7 (dead `phitter->GetTC(&ptc_guard)`, +142 IL from the inlined GetTC chain) alone breaks it (o7 9123); 8..11 each fit alone but
  together break it (n7 9013). FABLE_RETAIL_ASSERT_ARM for all eleven (arm11): same DIFFER. => the eleven do NOT fit as dead arms + call.
- (B) VERIFY-style release form: the three condition asserts (1, 2, 4) as dead arms; the eight success forms (3, 5..11) are the kept call
  itself (a success assert that keeps its call in release evaluates its expression; no extra arm). g11/v11.cpp: summon 8675 / hero 9025
  -> RELOCATION_MATCH (margin ~25 IL on the low side). Test g11/00839940_test.cpp vt PASS, mutation (00839940_test_mut.cpp) FAIL.
  addmatch -> g11/payload.json + g11/oracle.tsv (overlay hdrg). Strict vland (g11/vland.log): RELOCATION_MATCH PASS; warnings = the same
  6 RELOC_NAME + 1 RELOC_CONFLICT B24B7 explained above (none from the new text). Pick-any: pickany_vs_retail 11 EQ + NEQ 0x0066C340
  GetDef<CMaterialDef> (leaf-referenced, HEAD debt, Account A NF341) + NEQ 0x004292C0 _Rb_global (not leaf-referenced, STLport header);
  leafcopies.py 12 EQ + the same 0x0066C340. Inner hit-locations local renamed pTCHitLocations (FableWin assert 11 text).
- Open for the lander: (B) is the release spelling inferred for the success forms (no FableWin evidence of the release macro); the
  landed 0x00627BE0 spelling of the same kind does not fit here. payload.json (B24B7, fitted five) left in place; the new one is g11/.
- B24B8 advisor follow-ups: success forms as `if (!(call)) { dead arm }` (g11/vf.cpp, row-local probe macro): slot / register / block-order shifts (IL not traced):
  DIFFER(5198v5195) 51 hunks -> not a candidate. v11's header now says "one spelling that fits" (not proven unique), hero margin ~24 IL
  (~one arm): regrade after NF341 / the ISDEAD opt-in merge. Assert 7's position is UNVERIFIED (here = the guard-test condition; ask Account A
  whether FableWin tests ptc_guard's GetTC once or twice). g11/payload.json regenerated, strict vland PASS (7 explained WARN).
- Positions of asserts 1, 2, 4 are placed from the coordinator's ORDER only (next to the value they test), unverified like 7.
