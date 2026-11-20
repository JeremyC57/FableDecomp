# hand3c chunk 3 notes (Account B, 2026-11-20)

## Coordinator rule received 2026-11-20 (verify_and_land shared_header_redefinitions)
Strict vland now also rejects local `class X : public B {` models and libpk/ header classes, and reads FABLE_HDR_OVERLAY. Every
snapshot row: no local model of any class that has an engine/ or libpk/ header (drafts in ref/ carry such models: re-port them).

## 0x00B075D0 CPSCSingleSprite::AddToBoundingSphere -- ESCALATED, DIFFER(977v977), one swapped pair (3 grades, ~110 probes)
- Re-port onto shared headers: NEW hdr/engine/CPSCSingleSprite.h + CEnginePrimitiveSpriteTrail.h; decal-row (0x00B1DC70) opt-in set +
  CPARTICLESYSTEM_UPDATE / BOUNDING_TRANSFORM_PASS24 (TransformPosIfLocal forceinline) / CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS.
- Ego_d line table (ref/00b075d0/egod_lines.tsv) fixed: one `return valid`; `C3DVector pos;` at line 262 (after the trail block);
  `if (SpriteBankIndex != 0 && params.GetUpdateParam(pos, PositionParam, interpolate, false, 1))` one 60-byte line; clamps one line each;
  trail = non-const list&, it_trail_pos = non-const iterator (tools/ty.sh on the Ego_d type indexes).
- Sprite X row `fld x ; fmul E11` needs `C3DVector pt; pt = pos;` (struct assignment), not the copy ctor (Ego_d 12-byte line fits both /Od shapes poorly: FITTED).
- Left: loop Y-row term order (0x00B07676). See tmp/ESCALATE_00b075d0.md. probe/b075/ord.py prints both expansions' term orders.
- hdr/engine/CParticleSystem.h and CMatrix3x4.h in hdr/ are PROBE copies (FABLE_PROBE_TPIL / FABLE_PROBE_TP macros): remove before any snapshot.
- g4: `?ToC3DVector@C3DVector@@QBE?AV1@XZ` (Ego_d + FableWin 0x02C9FCA0; body `return *this;`) -> NEW overlay opt-in
  FABLE_RETAIL_C3DVECTOR_TOC3DVECTOR (hdr/engine/C3DVector.h). `C3DVector pt = it_trail_pos->Position.ToC3DVector();` = 19 bytes /Od
  (Ego_d 253), `C3DVector pt = pos.ToC3DVector();` = 12 (Ego_d 265) and fixes the sprite X row without the fitted struct assignment.
  Loop Y row unchanged. Map of the loop order by copy source: direct frame object + copy ctor -> retail; anything through a pointer
  (iterator, this, parameter; plain or ToC3DVector) -> ours. Coordinator: row stays OPEN, interleave; Account A asked for FableWin's line.
- WARNING for shell probes: never assign a shell variable named S or W (tools read them).

## 0x009ACEE0 CDefinitionManager::ParseEnumForSymbols -- RELOCATION_MATCH (b1 READY)
- g1 shared-header port from the Ego_d line table: 1032v1017 (the October residue: comma_or_equals' data pointer reloaded after each
  out-of-line CBasicString operator==). g3 +CBASICSTRING_TEXT_EQ_NOINLINE + COMPARE_BODY (callee bodies visible): 1017v1017, `inc ebx`
  one position early. g4 `for (; ReadNextItem; current_val++)`: RELOCATION_MATCH. Test PASS (4027 scripts), mutants 38/38, vland strict PASS.
- New overlay opt-ins: CDEFINITIONMANAGER_PARSEENUMFORSYMBOLS, CPARSEDITEM_ISSTRING.

## 0x00A4C1F0 / 0x00A52760 / 0x00A53860 keyframe interpolation -- RELOCATION_MATCH x3 (b2 READY)
- Landed 0x00A4D920 helper texts: 1004v1006 / 1004v1004 (x87 term orders). Carved the debug line tables of the whole inline chain
  (ref/x_*/egod_lines.tsv: SetSlerp, Dot, GFFastSin, GFFastArcCos, SetIdentity, SetPosition, InlineGetKeyframeInterpolate) and rewrote
  each body statement for statement with the PDB locals -> all three RELOCATION_MATCH at the first grade (probe/helpers_v2.inc).
- 0x00A52760 = SequenceData::InlineGetKeyframeInterpolate out-of-line copy; 0x00A53860 = C3DAnimationSequence::InlineGetKeyframeInterpolate
  (`mov ecx,[ecx]`), both uncalled COMDATs of the animation-file TU. rows.tsv names were wrong (NAME_FIXES).
- tools/gx.py regrades any address from Fable.exe bytes: the new texts keep the landed 0x00A4D920 at RELOCATION_MATCH.
- LESSON: x87 term-order residues in inline helper chains: carve EVERY helper's debug line table and write its body in that statement
  shape (stepwise statements, operand roles, which object calls Dot) before anything else.
- mut.py: an empty replacement field breaks the tsv row (use `/**/`); a mutant's first occurrence may be in the header comment (use index 2).

## 0x00B075D0 CLOSED (g5, 2026-11-20) -- the mechanism behind "x87 term order follows IR numbering history"
- c2.dll 13.10.3077 tree optimiser (per statement, 0x1075CFD0 -> 0x1075CF0C): node canonicaliser 0x1075CC68 gives every operand a
  key (0x1075D388: [depth:8][size:8][hash:16]; leaf hash 0x1075D54E = symbol number << 5 for frame symbols, << 6 for temps; node hash =
  sum(key_i.low16 << i) + opcode - 0x24A) and merge-sorts the operand list of add / mul (0x1076A2F3, comparator 0x1076A394) by key,
  descending. So `E*v`: higher symbol number first; a sum of products: by (E + 2v) of each product (E > v).
- Symbol numbers: 0x1075A0C5 allocates per kind from pools (kind 3 temps; kind 4/5 locals, inlined callee locals/params, field
  symbols) with free lists; a pool takes numbers 32 at a time (0x107603DB, counter 0x108A79D0). Field symbols of frame aggregates
  (pt.Y, mat.E32 ...) are created by the tree optimiser in IL order (0x1073B0D9 <- 0x1076C552), each with one extra transient.
- Consequence: the order flips where a row's field symbols straddle a 32-number block. Retail needs E22 - E32 > 2 * (ptZ - ptY), i.e.
  E32 = last number of a block. Our count was 5 short; the three Ego_d / STLport helper shapes add 3 + 1 + 1.
- Tools (W/tools/il): cdis.py (c2 disassembly + function index), dump2.py + pp2.py (pre-lowering IL with symbols), fm.py / sym.sh / sv.sh
  (symbol numbers of the 18 TransformPoint products for a variant, no grade), symlog.py + lpool.py (every symbol allocation with name),
  wp1.py (hardware watchpoints inside c2). Run through tools/sched/host.py.

## 0x0078A690 CTCHeroRangedAttacks::FrameUpdate -- RELOCATION_MATCH at g4
- g2 (declaration-only GetRootsNumerically): frame 0x1C v 0x14, roots._M_start reloaded. g3: the landed 0x0078B7C0 template text visible in the TU
  (out of line, FABLE_FASTCALL) -> frame 0x14, `_M_start` in ebp; one instruction left (fidiv after `test eax,eax`).
- Residue = c2 list scheduler (tools/sched t20, region of 11 nodes): fild->fidiv edge has latency 3, so `sub esp,8` and `test` are issued first.
  `((float)AimingTimer) / GetConstantFPS()` puts a PAREN (0x269) tuple on fild's edge; fidiv is ready at cycle 1 -> retail order. 9 other spellings measured
  (double local, (float) divisor, quotient bracket, cast-of-quotient, call-site brackets, visible GetDamageMultiplier body): no change.
- GetShotBuildupTime is NOT a header inline in retail: Ego_d lines 751..753 out of line (42 bytes, only `this`), retail 0x00789A60 (10 bytes). Overlay
  CTCHeroRangedAttacks.h gets FABLE_RETAIL_CTCHERORANGEDATTACKS_GETSHOTBUILDUPTIME_OUT_OF_LINE (declaration only); the row defines it after FrameUpdate
  (`// Fidelity:` same-TU strong definition; emitted bytes db 41 18 da 35 <reloc> c3 = retail).
- Overlays: CTCWeapon.h (+AUTOLOADING, pending Account A's OK), CTCSoundPlayer.h (+PLAYATTACHEDSOUNDEVENT), CTCHeroRangedAttacks.h (+..._OUT_OF_LINE).
- Coordinator 2026-11-20: Account A released engine/CTCWeapon.h; FABLE_RETAIL_CTCWEAPON_AUTOLOADING is ours to add (retail branch beside LOADING, and the
  generated-layout branch beside ISPROJECTILEWEAPONLOADED, both declaration only). Owner: a new shared header that flags landed local-model rows no longer
  holds a row; list the flagged rows in the hand-back.
- 0x0078A690 DONE: g5 (landed 0x00789CE0 body in the TU: an anonymous-namespace callee must be defined in its TU, else the test cannot link).
  Test PASS (34,793 cases), mutants 138/139 (1 equivalent), strict vland PASS, snapshot batches/b4. tools/gother.py grades TU mates.
- Test tip: SetUnhandledExceptionFilter -> ExitProcess in the test makes crashing mutants die at once (Wine otherwise starts winedbg, ~50 s each).
- Coordinator 2026-11-20: b3 0x00B075D0 LANDED (06248da5c); overlay copies of C3DVector.h / CPSCBase.h / CEnginePrimitiveSpriteTrail.h / CPSCSingleSprite.h retired to hdr_landed/b3 (repo has them; the audit replaced the row's noinline ScaleParticleDimensions copy by a shared-header inline under a nested opt-in). b4 with the rolling auditor.

## 0x0077A430 CTCDExperienceOrb::FrameUpdate -- DONE (b5 READY), 2 grades
- Port from the local-model draft onto shared headers using the Ego_d line table for every statement. Residues: `<=` (test ah,41h) and the IsNonZero
  square order = the header's DEFAULT IsNonZero is a fitted 3-statement running sum; one-expression variants (any term order) match.
- Test builds a real thing map for the in-TU search instance (layout: map +4 level-info vector (20-byte records), +0x14 vector of cell vectors,
  node {PMapwho, PNext,..}, CTCMapwho SearchID +0x4C, driver vtable slot 28). Reusable for other rough-area rows.
- tools/il target is now IL_TARGET env (default AddToBoundingSphere); tools/undef.py lists an obj's undefined externals; tools/rdf.py reads retail floats.

## 0x0079D360 CTCBulletTime ctor -- DONE (b6 READY), 3 grades
- Matched at the first compile of the port: landed sibling UnPause (same class, shared headers) gave the define set and the whole second half.
- vland NOT_GENUINE without a reason = purity gate: a row including a shared retail_asm helper needs the `// Retail-asm:` comment line.
- Test technique for constructors of abstract classes: derived shell class in the test; log letters for smart-pointer ctor/SetPItem/dtor with a
  "which member" suffix give the full construction order. Fake def vtable slot 14 = GetPSubDefInfo; the map find stub plants the tree header.
- Coordinator 2026-11-20: b4 0x0078A690 LANDED (a1b89caa5); overlay copies CTCWeapon.h / CTCSoundPlayer.h / CTCHeroRangedAttacks.h retired to hdr_landed/b4.

## 0x0057C4F1 CTCHeroStats::ApplyBonusItemEffects -- DONE (b7 READY), 3 grades
- /Os TU: `#pragma optimize("y", off)` + `optimize("s", on)` in the row; FABLE_RETAIL_STLP_BITSET_TEST_DECL + IS_THING_ALIVE_NOINLINE + CTHING_GETTCDRIVER give
  the out-of-line calls. Only residue of the first compile: operand order of a sum whose left term is a member read and right term has a call
  (member read first -> frame copy). Product-first matched.
- Test: event list with (code, a, b, float) + tolerance compare; explicit specialization `template<> bool std::bitset<274>::test(size_t) const` as stub.

## 0x00A781A0 CNavQuadTree::UpdateLines -- DONE (b8 READY), 3 grades; mutants 189/195 (6 equivalent); strict vland PASS
- Port on shared headers was 1243v1244 / 233 off; three levers, each measured with pp.py: (1) `index` declared after the overflow return (Ego_d
  lines 1911/1912/1915/1918) 233 -> 134; (2) FABLE_RETAIL_STLP_VECTOR_SIZECTOR_VISIBLE_NOINLINE instead of the declaration-only _N_OUTOFLINE
  134 -> 20 (c2 must see the vector(n) body to home the allocator temp); (3) bracket `(y * 8.0f) + MapPos.Y` fixed the last one-slot
  scheduling residue (`fld st(1)` vs `sub eax, esi`). Index spelling variants are all canonicalised (no effect).
- The visible vector<C2DBoxF>(n) COMDAT was 87 bytes against retail 0x00A80360's 122: fixed with FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE +
  a NEW overlay opt-in FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_FORCEINLINE (stlport/fable_containers.h, the dispatcher) + FABLE_RETAIL_C2DBOXF_COPY;
  gother.py grades it RELOCATION_MATCH. NAME_FIX: 0x00A80360 is vector<C2DBoxF>(size_type), landed as vector<C2DLineF>.
- Test: private member reached with `#pragma comment(linker, "/alternatename:@Call@20=<mangled>")` + `extern "C" __fastcall`. Wild scenarios make
  the dead-looking guards live: TLX < 0 skips the unsigned first loop; W = 2^k, H = 2^(32-k)+a gives size a*W (index == size cell); W = 2^16,
  H = 2^16+1 gives the > 0x7fffffff return; TLX = -2^31, BRX = 8, H = 2 gives the negative return. IntersectsWith stub answers true there
  (boxes are uninitialised). No <windows.h> on this toolchain setup: cl spins for minutes on the missing include under Wine; declare the two
  kernel32 imports by hand.
- Coordinator 2026-11-20: b5 0x0077A430 LANDED (90f32fe4d); overlay copies CPlayer.h / CTCDExperienceOrb.h retired to hdr_landed/b5.

## 0x0041AFA0 NUISystem::CSprite::Draw -- DONE (b9 READY), 3 grades; mutants 206/207; strict vland PASS
- The parked draft (1014v1455, local models, "lane gap") was wrong about everything hard: the "texture refresh" is the inline
  CGraphicInfoFrame::GetFrame chain the landed same-TU SetGraphicIndex 0x0041B550 already spells on shared headers, the class is
  NUISystem::CSprite (engine/NUISystem_CSprite.h, Draw already declared), and the Ego_d local `range` is a C2DExtentsF
  (C2DBoxF(position, range) = the existing FABLE_RETAIL_C2DBOXF_EXTENTS ctor). First compile of the port: 1404v1455, only residue the
  Handle.Clear() call -> FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR = RELOCATION_MATCH (no /d2inlT window: T80/T88 expand
  the CCountedPointer dtor that retail calls at 0x004194B5).
- Ego_d /Od line sizes (VC10 debug) decide statement forms: 84-byte lines = `size.X = (Width == 0) ? Size.X : (float)Width;` without the
  zoom multiply; 21 = member copy through a copy ctor call; 16 = two-reference ctor; 12 = one-reference member call (Move); 27 = the
  GFFloatToLongNear snap; 25 = `x = VirtualCall();`.
- New overlay opt-ins: C2DBoxF.h FABLE_RETAIL_C2DBOXF_MOVE (inline body of the landed 0x0041A810); CEnginePrimitive2DSprite.h
  FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_BOX_CTOR (declaration + inline body, grades MATCH against 0x0041BF60 from the row) and
  the guard FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER (EEngine2DLayer is defined by NUISystem_CManager.h too).
- Test without any class: /alternatename both ways (row entry and every undefined callee -> extern "C" __fastcall stubs), global operator
  new / delete defined in the test, two identically seeded object sets (row / model) compared with self-pointers as offsets. Do NOT
  relocate a copied object set by scanning for pointer look-alikes: image addresses are 0x0040xxxx and collide with small ints / colours.
- Coordinator 2026-11-20: b6 0x0079D360 LANDED (ec9be76e1); overlay copy CTCBulletTime.h retired to hdr_landed/b6. Audit patterns to follow:
  overlays strictly ADD lines (never widen an existing #ifdef line), prefer existing shared helpers (engine/CDefClassBase_GetSubDef.h,
  GFDebugPDynamicCast) to row-local copies.

## 0x0077D5B0 CTCHeroCentre::FrameUpdate -- IN PROGRESS (probe/0077d5b0_v5.cpp = 1491v1491, 84 diffins; 0 g.py grades yet)
- Port on shared headers; overlay additions so far (all add-only): CTCHeroCentre.h FABLE_RETAIL_CTCHEROCENTRE_CLASS (typed CTCBase-derived
  branch, flat layout stays default), CTCQuestCard.h FABLE_RETAIL_CTCQUESTCARD_FEATACCESSORS (Get/SetFeatIndex, SetQuestName,
  SetScriptQuestName), CQuestManager.h FABLE_RETAIL_CQUESTMANAGER_QUESTCARDTYPES (GetAvailableQuestCardTypes, GetQuestCardsNeedUpdating),
  CGameEvent.h FABLE_RETAIL_CGAMEEVENT_INLINE_ADDULONG. Flags: /GS.
- Levers found (each measured): existing_quest_cards declared AFTER reserve (Ego_d line 129); FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY +
  CTHINGFILTER_HASTC_OUTOFLINE + CTHINGOBJECT_STATGETTYPE (visible GetAllLeafThings body -> size cached in the second c1 loop);
  IMPLICIT const char* -> CCharString conversions (`Create(.., "")`, `GetDefGlobalIndexFromName("OBJECT_FEAT_CARD_DUMMY")`: an explicit
  CCharString("..") pushes the ctor's eax and loads QuestCardPositions' start before the ctor); FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
  (push_back's placement copy = base ctor + SetPItem(GetPItem)); CINTELLIGENTPOINTER_COMPARE_NULL_INT for IS_THING_ALIVE(pquest_card).
- Remaining residue = register colouring only: (A) 0x0077D8E2 ScriptName compare: retail loads it->ScriptName data into eax first, ptc's
  into ecx (FeatIndex temp in eax before it; ours edx / eax / ecx); (B) 0x0077DA2C push_back: retail finish in esi, &QuestCards in edi
  (ours swapped). Not fixed by: operand swaps (e1..e4), other-first operator== body, implicit push_back conversion.
- Ego_d line 192 is 23 bytes against 26 for 191 / 193: SetQuestName(def_name) (the local reference), not it->DefName. TODO apply.
- GetAllLeafThings instance from the row TU grades DIFFER(126v136) against 0x0077EC20 so far (check LEAFBODY_CASTARG).
- 0x0077D5B0 MATCHED (probe t3 -> src/0077d5b0.cpp, 3 grades): the last two colouring residues went with the FableWin-text helper shapes
  FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN (fixed the ScriptName compare registers) + FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
  (fixed push_back esi/edi). Test (test/0077d5b0.cpp, alternatename stubs, 60,000 cases) PASS; mutants (140) running -> then vland + b10.
  A linker /alternatename with a ~200-char name fails silently (LNK2019): define such a callee as an explicit template specialisation.
- 0x0077D5B0 b10 READY (2026-11-20): test strengthened (event player byte logged, def indices 1 / 2), full mutant rerun 139/141
  (2 equivalent), strict vland PASS (2 RELOC_NAME warns: 0x0077EC20, 0x004AC940 -> NAME_FIXES). 13 of 20 matched.

## 0x00B16410 COrbitState::Update -- STARTED (references read, no compile yet)
- Ego_d line table (54..143) maps to: NextAngle += (RotateSpeed + RotateSpeedRandomValue) * GetOneOverTickRate(); FromPosition = ToPosition;
  ToPosition = NextPosition; C2DVector next_pos_2d; if (settings.Enabled) { squeeze branch (temp_x, temp_y, cos_a, sin_a) / plain;
  radius; expand+cycle; next_pos_2d.X *= radius .. } else next_pos_2d.Clear(); long orient = settings.Type & 3; switch ->
  NextPosition.Initialise(..); if (settings.Type & 4) parent_transform.TransformPoint(NextPosition); else NextPosition +=
  parent_transform.GetTranslation(); switch -> PreZRotate / PreYRotate / PreXRotate(-NextAngle); SetTranslateOnly(NextPosition);
  FromTangent = ToTangent; ToTangent = (NextPosition - FromPosition) * 0.5f.
- COrbitSettings::Type is a plain long in Ego_d TPI (no bitfield): mask 3 = orientation, bit 4 = local space.
- CMatrix3x4::PreXRotate/PreYRotate/PreZRotate(const float angle): Ego_d lines per row = tmp = E*sinA (12) ; E *= cosA (15) ;
  E -= E'*sinA (21) ; E' *= cosA (15) ; E' += tmp (15); `const float sinA / cosA` through GFSin / GFCos (15-byte lines), `float tmp`.
  Not in engine/CMatrix3x4.h yet -> add-only opt-in. C2DVector::Clear not in C2DVector.h -> add-only opt-in.
- 0x00B16410 MATCHED (3 grades: 842v1671 plain / 1669v1671 / MATCH). Levers: (a) all helpers plain inline + `/d2inlT230` (window:
  T229 and below differ, T230..T9999 same object); (b) PreRotate with `const float cosA` declared BEFORE sinA (sinA first: cos
  spilled to memory, 1726 bytes); (c) TransformPoint ZYX_SUMS text: the default text leaves the same TransformPoint code but flips
  two commutative sums in the PreYRotate expansion (symbol-number class residue; operand order / assignment form in PreYRotate
  itself is inert: 8 variants tried). Conditions are GFIsNonZero(..) (Ego_d 26-byte lines; FableWin 0x02F50800 sits next to
  PreXRotate 0x02F50A10 = same TU COMDAT cluster). Test: model in double; NOTE VC7.1 does not round `(float)` casts kept in a
  register -> model uses Single() through memory (881 false failures before that, all with |CycleTime| one float above 1e-4).
- 0x00B16410 b11 READY (2026-11-20): mutants 307/309 (2 equivalent), strict vland PASS. 14 of 20 matched. Next: 0x0078D250 CTCBerserk::FrameUpdate (references carved: ref/0078d250/).
- 2026-11-20 coordinator round: b7/b8 landed -> retired CTCHeroStats.h, CThingPhysical.h, CNavQuadTree.h, stlport/fable_containers.h (hdr_landed/b7b8); b9..b11 re-probed RELOCATION_MATCH on the tree headers. b10 rev 2: the GetAllLeafThings COMDAT now matches 0x0077EC20 (two existing opt-ins). GFFloatAbs.h comment says which body (mask, Ego_d lines).

## 0x0078D250 CTCBerserk::FrameUpdate -- MATCHED (3 grades: 1668v1729 first port; v6 1724v1729 94 off; match)
- Shared-header port compiled first time. Levers in order: FABLE_RETAIL_CTHING_ORIGINAL_VTABLE (OnHit slot 65); the landed
  0x0078DB20 define set so the rough-area body is visible (list head kept in ebp; COMDAT == 0x0078DB20, list dtor == 0x006FE660);
  HIT_CTOR_BODY; `AnimTimer < Released`; CRGBColour::Interpolate needs a new FORCEINLINE opt-in (no /d2inlT window);
  LAST: berserk_fraction is the SCALE local (Ego_d 33 + 21-byte lines), not the tint fraction.
- Not needed (dropped): NORMALISE_UNPAREN, GETTC_ORIGINAL_CHAIN, CINTELLIGENTPOINTER_COPY / _DEREFERENCE.
- The TU's CHitParameters ctor COMDAT is 242 bytes vs retail 0x00789238's 178 (unaligned address = a size-optimised TU's copy).
- Coordinator FYI (2026-11-20): b4 row 0x0078A690 re-ported in hand3c2 b18 (bc7fd6c06); GetRootsNumerically<..> template text is now the landed 0x0078B7C0's -- take it from there if a later row copies it (none of my remaining rows does).
- 0x0078D250 b12 READY (2026-11-20): test PASS, mutants 267/268, strict vland PASS. 15 of 20 matched. Next: 0x006E5A00 (draft probe/006e5a00_v1.cpp + overlay CScriptConversation.h CLASS branch written, not compiled yet).

## WIND-DOWN (owner order via coordinator, 2026-11-20) -- hand3c3 final state
- 14 of 20 rows matched (my earlier messages said 15: miscount, corrected here). b1..b8 LANDED (10 rows). b9..b12 READY, awaiting
  audit / landing: 0x0041AFA0 (b9), 0x0077D5B0 (b10 rev 2), 0x00B16410 (b11), 0x0078D250 (b12). All four re-probed RELOCATION_MATCH on
  the live overlay at wind-down (tools/pp.py).
- 0x006E5A00 PARKED, not close: parked/006e5a00/ (README with the block structure, callee table, the per-site inline split, the
  compile errors left; draft_v2.cpp; four draft overlay headers moved out of hdr/). 0 grades of the new text.
- Not started beyond the inherited baseline (no draft from this chunk): 0x00A475C0 (1857; `and esp,-8` frame), 0x00B079B0 (2032;
  `xor cl,[mem]` bitfield write-back), 0x0087CFE0 (1447; local vectors + sort/unique), 0x008D5AA0 (1126), 0x00988E80 (917; hand-written
  SSE block, the inherited HANDOFF marks it "do not grade").
- Live overlay copies: hdr/README.md, last section. Retire b9..b12 copies when they land.
