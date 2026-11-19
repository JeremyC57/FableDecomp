# Audit hand3c2 b1..b3 (+ later batches), 2026-11-19, Account B
Repo = today's working tree (read-only; the coordinator was landing L5 = the CCoda b24 set while I worked). Strict = LANDVERIFY_STRICT_HEADERS=1.
Tools: $AW/tools (prior audits' + objfuncs.py: functions an obj defines with COMDAT selection; egodlines.py: Ego_d C13 line-table carver;
orcchk.py; rdis.py). Each fixed_<addr>/ holds src, test, payload.json, oracle.tsv, hdr (if any) + diffs, NAME_FIXES.tsv, mutants.tsv/.log, vland_strict.log.


## Summary (8 rows; batches b1..b7, no batch re-snapshotted after I copied it)
| Batch | Row | Verdict | Grade (fixed set, strict) | Purity | Mutants (killed / total; survivors equivalent) | Header verdict |
|---|---|---|---|---|---|---|
| b1 | 0x00AAD340 CWaveFile::ConvertSamplingRate | PASS (optional Ego_d-order move, 3 test cases added) | RELOCATION_MATCH | retail_asm | 42/46, 4 equiv | no header |
| b2 | 0x0064DD9A CDrawMiniMap::UpdateRegionExitNames | FAIL -> fixed (0x007791F0 is GetRegionName) | RELOCATION_MATCH | genuine | 37/37 | 6 additive, ppcmp 3533/3533, 19 HEADER_POLICY pre-existing |
| b3 | 0x00D6C830 CQ_HangingTreeEvilScript::CheckBanditFollowStatus | PASS (shared distance header) | RELOCATION_MATCH (/d2inlT62; window 62..71 confirmed) | genuine | 28/28 | additive, 2 includers RM on/off |
| b4 | 0x00AABF60 CSoundEmitterLH::PlaySample | FAIL (shared-header policy, gate gap) -> fixed (shared CSoundManagerLH.h / libpk CSoundSystem.h) | RELOCATION_MATCH | genuine | 55/55 | 3 additive, 7 includers RM on/off |
| b5 | 0x00A793D0 / 0x00A79680 CNavQuadTree::Connect*MapEdge (both in fixed_00a793d0) | PASS (unchanged) | RELOCATION_MATCH x2 | genuine | 36/36 each (+1 equiv control) | additive + NEW CNavConnectorNode.h (2 landed rows will hit SHARED_HEADER_AVAILABLE) |
| b6 | 0x00A567A0 CCardinalSpline::GetSplinePosition | FAIL -> fixed (GOTCHAS-455 Fidelity line; Ego_d order) | RELOCATION_MATCH; copy RM at 0x00A56670 | retail_asm | 50/52, 2 equiv | no header |
| b7 | 0x00766660 CTCObjectAugmentations::AttemptToStartParticle | PASS (redundant extern removed) | RELOCATION_MATCH | genuine | 48/48 | no header |

## b1 0x00AAD340 CWaveFile::ConvertSamplingRate -- PASS (fixed set: optional byte-neutral declaration move + 3 test cases)
- Oracle 584 B == Fable.exe (`ret 4`, then int3). No overlay. Strict vland RELOCATION_MATCH PASS (author text, and fixed text: fixed_00aad340/vland_strict.log).
- Purity retail_asm (GFFloatToLongFloor x3, GFFloatToLongNear x1, shared helpers with FABLE_PORTABLE twins). The row's `// Retail-asm:` line
  checked against retail: 0x00AAD381 / 0x00AAD3C8 / 0x00AAD42A `fld [ebp-20h] ; fsub [ebp-24h] ; fistp` and 0x00AAD4E1 `fld [ebp-2Ch] ; fistp [ebp-30h]`. Correct.
- Object: the leaf (COMDAT NODUP) + 28 COMDAT-ANY STLport/CArray/helper bodies; no strong same-TU copies.
- Opt-in set = landed TU-mate 0x00AAD650 LoadWaveFile's (CARRAY, STLP_VECTOR_N_OUTOFLINE, STLP_VECTOR_C; fable_string.h first).
- Locals/scopes == Ego_d exactly (types: 0x22 ulong, 0x12 long, 0x40 float, 0x411 short*, FRACTION_BITS 0x1809 = const ulong, CArray<short> new_data).
- Ego_d line table (carved, GOTCHAS last line): confirms the wrap order (`pos_int++` line 83 before `pos_frac -=` 84: the author's fix). Line 59
  (7 B) between step_int (58) and step_frac (60) is FRACTION_BITS (the 7-byte lines are 59/63/64; 63/64 are pos_int/pos_frac and line 60 uses
  FRACTION_BITS). Fixed src declares it there: RELOCATION_MATCH in both orders (optional; take either). Lines 55 (328 B) and 69 (348 B) have no
  retail counterpart: disclosed as `// Unreproduced:`.
- Names: ?ConvertSamplingRate@CWaveFile@@QAEXK@Z (Ego_r + FableWin); members = Ego_r layout; obj symbols in FableWin except STLport internals
  (shared-header spellings, pre-existing). NAME_FIXES: none (agree).
- Mutants (mut.py on the ROW SOURCE; the test links the row object): author 33 + audit 13 = 46, 42 KILLED. 4 survivors, all equivalent:
  clamp `>=` (re-clamps SL-1 to SL-1; author's), output `GFFloatToLongNear`->`GFFloatToLongFloor` (argument is an exact integer |v| <= 32768),
  `unsigned long sample1` (the `(short)` keeps bits 16..31 of the sum, identical for logical and arithmetic shifts), a doubled `AudioData = new_data`.
  Two audit mutants first SURVIVED and are now killed by new cases: truncating step_int (case 12, step = 2^24: GFFloatToLongFloor(2^24) = 2^24+1)
  and a float product for the new length (case 14, rate 2^24+1). Case 13 (4097^3) kept as a boundary note.
- REVIEW: cases 12 and 14 allocate ~100 MB each (plus the author's ~25 MB case 10) at every verify; vland PASSed with them. Drop them if the lane
  memory budget objects (then the two mutants above survive as non-equivalent at extreme rates).

## b2 0x0064DD9A NPlayerGui::CDrawMiniMap::UpdateRegionExitNames -- FAIL as delivered (callee name); fixed_0064dd9a PASS
- Oracle 580 B == Fable.exe (unaligned; SetRegion 0x0064DFDE follows directly). Author text: strict RELOCATION_MATCH with WARN RELOC_CONFLICT.
- FAIL (names right first time): the call at 0x0064DE3A goes to 0x007791F0, which is CTCDRegionExit::GetRegionName, not GetNameOfRegionConnectingTo.
  Retail keeps tcd_region_exit_point.obj in Ego_r.pdb's definition order: Construct 0x00779000 (Ego_r idx 2), MoveHeroThroughExit 0x00779080 (3),
  [GetRegionName (4)] 0x007791F0, GetDisplayName 0x00779260 (5), [GetNameOfRegionConnectingTo (6)] 0x007792D0, SetUpPEntranceConnectedToFromUID
  0x00779340 (7). FableWin.pdb has the same run (indices 16..20), and GOTCHAS line 31 records that Ego_r, FableWin and retail keep one TU's order.
  The two bracketed bodies are byte-identical (Ego_r: both 96 B), so only the order separates them; 0x007791F0's only direct (E8/E9) caller is this row.
  This answers the author's RELOC_CONFLICT question: 0x007792D0 keeps GetNameOfRegionConnectingTo (landed 0x00501450 is right), 0x007791F0 is GetRegionName.
  Fix: `ptcd_region_exit->GetRegionName()` + `const CCharString& GetRegionName() const; // 0x007791F0` in the row's MINIMAPLABEL opt-in (with the evidence),
  test and mutant respelled. Strict vland RM PASS; the conflict becomes RELOC_NAME (catalog stand-in label), cleared by the new NAME_FIXES row.
- Also fixed (byte-neutral, measured RM): (a) filter call-operator body = Ego_d's form. The Ego_d line table for
  CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>::operator() is 488 (19 B: call + test) / 490 (4 B) / 492 (2 B), i.e.
  `if (test_thing.GetTCOfLeafType(&ptc_type)) return true; return false;`, not `return test_thing.GetTCOfLeafType(&ptc_type);`.
  (b) CRegion::GetMiniMapRegionExitTextOffset second parameter `text_offset` (Ego_d; was `offset`).
- Locals/scopes == Ego_d exactly (types resolved: const CWorldMap&, const CThing*, const CTCDRegionExit*, const CCharString&, CThing* pentrance,
  C3DVector/C2DVector, list<const CThing*>, long). Line table: every statement in the text's order (world_map 1184 before region_number 1185;
  exit_name 1202 before exit_pos 1204). Lines 1198 (474 B) and 1229 (481 B) wrap the PeekTCDriver / PeekTC calls (retail calls and ignores the
  result at 0x0064DE2C): disclosed as `// Unreproduced:`.
- Object: leaf + ~95 COMDAT-ANY bodies, NO strong copies (GOTCHAS 455 does not apply). The TU is /Os (`#pragma optimize("s", on)`, same as the
  landed TU-mates SetRegion 0x0064DFDE, ToggleCentering, ctor 0x0064E83F); its COMDAT copies of 0x00615520/40, 0x00617CA0, 0x00618E60, 0x00617D30,
  0x00518DC0 are size-optimised and differ from retail's (speed-optimised copies picked from other TUs). Pick-any, so no grade impact; noted.
  The README's "No pragma" means no new pragma; the TU pragma is the TU-mates'.
- Overlay minimal: removing any of the 6 opt-ins fails (REGIONEXITS / MINIMAPLABEL / MINIMAPREGIONEXITTEXTOFFSET / STATCLASSTYPENAME SRC_FAIL;
  PEEKTCDRIVER_PLAIN_INLINE DIFFER(596v580); HASTCOFLEAFTYPE_BODY DIFFER(580v580)). None of the six macros exists anywhere else in rebuild/ or tools/.
- Headers: all 6 additive vs today's tree (only the 3 pinned `FABLE_STATIC_ASSERT_AT` lines change; pin_asserts semantics). hdrcheck2 is blind to the
  nested opt-ins (CDrawMiniMap REGIONEXITS nested in _CLASS, CThing PEEKTCDRIVER, CRegion), so ppcmp over every landed transitive includer of the six
  headers: 3533/3533 raw-identical. Strict regrade with overlay, 85 rows (all 63 includers of the five small headers + 12 further rows among the 25 that define
  CTHINGSEARCHTOOLS_ALLTHINGSBODY / CTHING_GETTCOFLEAFTYPE + 10 seeded CThing.h includers): 56 RM + 10 MATCH PASS; 19 HEADER_POLICY
  (0049e5e0 004a2f10 004a5a40 0050a650 0051d0a0 0057de57 005ebfa0 00606f80 0069adc0 006a7640 006b1f30 00729ca0 007d46d0 0082d3b0 009073a0 0090f110
  00929650 0093d180 0094fbb0) identical with the overlay OFF and RM PASS non-strict with the overlay ON: pre-existing. No new header.
- Mutants (mut.py on the row source, test links the row object; `hdr:` mutants mutate the overlay): author 29 (respelled) + audit 8 (filter body
  x2, IsActive, PeekTCDriver store, Y offset source, list element, display-name callee, Z) = 37/37 KILLED.
- Purity genuine. Names: all row-level symbols in Ego_r/FableWin (PeekTC two-argument overload and STLport internals are pre-existing shared spellings).
- NAME_FIXES (fixed_0064dd9a/NAME_FIXES.tsv): author's three rows agreed (0x00617D30: only caller this row; 0x00615520: only callers this row;
  0x00615540: other callers are a `jmp` thunk at 0x00616900 with no direct callers and a stack list at 0x008FB15F -- a folded pointer-list dtor;
  the const CThing* spelling is the evidenced instantiation) + NEW 0x007791F0 -> CTCDRegionExit::GetRegionName.
- Review: landed SetRegion 0x0064DFDE is a local NPlayerGui::CDrawMiniMap model (same mangled callee name, no conflict); with this opt-in it could move
  to the shared header later. The landed 0x00518DC0 operator() is a local model spelled thing/tc + `return ...` -- Ego_d says test_thing/ptc_type and
  the if/return form (Account A item; not re-ported here).

## b3 0x00D6C830 NScript::CQ_HangingTreeEvilScript::CheckBanditFollowStatus -- PASS (fixed set: shared distance header + notes)
- Oracle 607 B == Fable.exe. Strict vland RM PASS (fixed_00d6c830/vland_strict.log).
- Window re-measured on today's tree: T61 DIFFER(607v607), T62 RM, T71 RM, T72 DIFFER(587v607), default DIFFER(587v607). Edges confirmed;
  smallest N = 62 as the Inline-budget line says. The line follows the 2026-11-08 rule (preference 2, source untouched, smallest N, diagnostic sites).
- TU budget: WatchForBooty 0x00D8A240 is in the same register_scripts.obj (Ego_d/Ego_r) with /d2inlT57 (its window 57..63, re-measured T62/T63 RM,
  T64 DIFFER). The windows intersect at 62..63, so one TU value (T62) fits both. The rule is per-row so this is not a defect (other register_scripts
  rows carry 53..76 too); noted for the coordinator.
- ~CScriptThing: the row object's COMDAT ??1CScriptThing@@UAE@XZ is byte-identical (relocations masked) to retail 0x004AA840. No strong copies.
  `inline CScriptThing::~CScriptThing() { }` is the WatchForBooty convention.
- Fixed: the local `bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(...)` re-declaration replaced by the shared engine/fable_script_distance.h
  (which declares exactly that symbol; 76 landed rows include it, 165 still re-declare) -- byte-neutral (RM).
- Locals == Ego_d (_Fail_Val TRetVoid, CorrectExit CScriptThing; no others). Ego_d line table (16 lines, 880 B) matches the text's structure: loop line +
  terminate test (219), wait/distance lines each with their trailing terminate test, `ShouldFollowHero = true/false` 7 B each (225/227), inner close
  233, outer terminate test 234, close 235. 9 terminate return sites in the text = the line structure. Line 236 (40 B, after the endless loop,
  unreachable) disclosed as `// Unreproduced:` (same as WatchForBooty's line 96).
- Header: CQ_HangingTreeEvilScript.h additive (declaration block + 2 pinned asserts). Includers 00d6a560 / 00d6d670: ppcmp raw-identical; strict RM PASS
  with the overlay on and off. The macro exists nowhere else. No new header.
- Mutants: author 22 + audit 6 = 28/28 KILLED (one further audit mutant, CorrectExit = GetHero(), is LINK_FAIL: not counted).
- Purity genuine; every object symbol in Ego_r/FableWin. NAME_FIXES none (agree).

## b4 0x00AABF60 CSoundEmitterLH::PlaySample -- PASS on bytes / FAIL on the shared-header rule (as delivered); fixed_00aabf60 PASS
- Oracle 620 B == Fable.exe (`ret 14h`). Author text: strict RELOCATION_MATCH (it passes only because the gate's regex
  `\b(?:struct|class)\s+X\s*\{` does not see `class X : public Base {`).
- Shared-header defect: the row defines local CSoundManagerLH (`m_pSoundSystem`, `m_vector`), CSoundSystem and CSoundEventManager, while
  engine/CSoundManagerLH.h (CSoundManagerLH : CASoundManager, `SoundSystem` +0x40) and libpk/CSoundSystem.h (CSoundSystem / CSoundEventManager,
  GetEventManager slot 6) are shared and used by the landed 0x00AAC5E0. It copies the landed sibling PlaySoundEvent 0x00AABD50 (also local), and
  ~20 landed CSoundEmitterLH rows keep local CSoundManagerLH models: a family-wide, pre-existing deviation, so I flag it rather than blame the row.
- fixed_00aabf60: the row on the shared headers + two nested opt-ins, defaults untouched:
  engine/CSoundManagerLH.h FABLE_RETAIL_CSOUNDMANAGERLH_HEADPOSITION (`UnmodelledMembers44[0x30]`, `C3DVector HeadPosition` +0x74, read at
  0x00AAC072; descriptive) and libpk/CSoundSystem.h FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_PLAYSLOTS (v02..v13, PlayBankSample slot 14 =
  0x00AAC14F `call [edx+38h]`, v15, PlayEvent slot 16 = 0x00AAC112 `call [ebx+40h]`, with forward declarations). The CSoundManagerLH.h assert
  is pinned (FABLE_STATIC_ASSERT_AT(21, ...)). CSampleQueryLH / CEngineBankLH / CSoundBankLH / CSoundEmitterLH stay row-local (no header exists).
  GFGetTime now from the shared engine/GFGetTime.h (the row re-declared it). RELOCATION_MATCH; strict vland PASS (same 4 RELOC_NAME stand-in
  warnings as the author). Author's LHA_CEventPlayOptions.h PARAMSETTERS opt-in kept. Verdict: FAIL (shared-header policy, gate gap) + fix built;
  the author's text is byte-correct (RM) and is the fallback if the coordinator keeps the family local.
- Headers: all three additive. Includers 00aabd50 00c0a790 00c0a8a0 00c0a8c0 (LHA) and 00a3e520 00a3e830 00aac5e0 (CSoundManagerLH / CSoundSystem):
  ppcmp raw-identical (after the pin); strict regrade all MATCH / RM PASS with the overlay on and off. None of the three macros exists elsewhere.
  New headers: none.
- Coordinator item: libpk/LHAudioEventPosition.h carries a SECOND, conflicting CSoundEventManager model (0 landed includers). Unify before
  anything includes both.
- Inferred line (override ternary polarity, `!= -1`, measured `== -1` forms DIFFER) is labelled correctly: a fitted condition, not a bracket.
- Names: descriptive LHA setters (SetLoopCount / SetInterruptable from Ego_r CResourceOptions member names LoopCount +0x48 / Interruptable +0x54;
  no LHA method name in any PDB or the FSE/EgoCore sets I searched); NAME_FIXES (4 rows) agreed. No Ego_d record (the LH emitter is not in Ego_d).
- Purity genuine. Object: leaf + COMDAT-ANY only. Mutants (row source, test links the row object): author 47 (respelled) + audit 8 = 55/55 KILLED.

## b5 0x00A793D0 CNavQuadTree::ConnectHorizantalMapEdge + 0x00A79680 ConnectVerticalMapEdge -- PASS (no change)
- Oracles 678 B each == Fable.exe (`ret 0Ch`). Strict vland both RELOCATION_MATCH PASS (fixed_00a793d0/vland_strict.log; RELOC_NAME DeleteData =
  NAME_FIXES row).
- Locals/scopes == Ego_d for both (params this_map_y/adjacent_map_y and this_map_x/adjacent_map_x, quad_tree_area, nav_layer, current_x/current_y,
  neighbour_map_node, neighbour_size, my_map_node, neighbour_map_index in the nested scopes). Ego_d line table (27 lines) puts `current_x +=
  my_map_node->GetSize()` (2281, 14 B: a call) before `current_x += neighbour_size` (2287, 9 B), so the source order of the one-side branches
  supports the FITTED choice. The `// Inferred:` label stays (which flag the else-if tests is inferred from retail). Lines 2263 / 2279 / 2285
  (319 / 361 / 393 B) have no retail code (not disclosed in the row: optional).
- Names: every row-level symbol in Ego_r / FableWin ("Horizantal" is the PDB spelling); the opt-in sits under `private:` (AAE, matches the PDB).
  ?g_QuadTreeLevelSizes@@3QBMB is a pre-existing shared spelling. NAME_FIXES (4 rows) agreed.
- Headers: CNavQuadTree.h additive (no asserts to pin); CNavConnectorNode.h NEW (Ego_r layout Neighbour1/2, Ego_d ctor parameter names
  neighbour1/neighbour2, ??0 / ??1 in Ego_r). 14 includers of CNavQuadTree.h: ppcmp raw-identical; strict regrade 8 RM + 1 MATCH PASS,
  5 HEADER_POLICY (00a14e50 00a1c010 00a85560 00a85b90 00a85cc0) identical with the overlay off and RM PASS non-strict: pre-existing.
- NEW-header SHARED_HEADER_AVAILABLE: once engine/CNavConnectorNode.h lands, landed rows 0x00A780B0 and 0x00A78A90 (the ctor itself) define a
  local `class CNavConnectorNode {` and will go strict HEADER_POLICY (same regex as verify_and_land.py:525).
- Purity genuine; objects: leaf + COMDAT-ANY only. Mutants: author 32 + audit 4 = 36/36 KILLED per row (+1 deliberately equivalent control, `+= 0.0f`).

## b6 0x00A567A0 CCardinalSpline::GetSplinePosition -- FAIL as delivered (GOTCHAS 455 Fidelity line); fixed_00a567a0 PASS
- Oracle 766 B == Fable.exe (`ret 8`). Author text: strict RELOCATION_MATCH.
- FAIL (GOTCHAS 455): the row carries a STRONG same-TU definition of GFGetHermiteSplinePosition (objfuncs: COMDAT-NODUP, i.e. not inline) whose
  body is the landed owner 0x00A56670's text VERBATIM (checked programmatically), but it is introduced by `// Same-TU body (the landed row
  0x00A56670's text): ...` instead of the exact `// Fidelity: same-TU strong definition of 0x00A56670 (retail TU-mate)`. Fixed (line swapped).
  Measured: the copy is byte-required (declaration only: DIFFER(766v766)) and grades at its own address in this TU (0 non-relocation differences
  vs retail 0x00A56670, 295 B).
- Also fixed (byte-neutral, measured RM): Ego_d's statement order. The Ego_d line table puts from_index_s (153, 31 B), to_index_s (154, 31 B), this_s
  (155, 17 B) BEFORE the tangents (157 / 158, 43 B each) and the call (160); the row had the tangents between from_index_s and to_index_s.
  num_control_pts_f is a plain float in Ego_d (type 0x40); the row declared it const. Both adopted. Lines 64 / 70 / 108 / 109 (300+ B each, no
  retail code) disclosed as `// Unreproduced:`.
- The two FITTED `// Inferred:` choices are SUPPORTED by the Ego_d line table and stay labelled: num_points_sub_one is line 80 (after the size<2
  block, before the EndPointsConnected test 83); the open spline's last point (96) is 37 B = `num_points - 1` (34 B through num_points_sub_one);
  both neighbour blocks list the wrap / repeat arm first (115/117/119/121/123/126/128 and 132..145).
- Locals == Ego_d (types: num_points / num_points_sub_one const long, from_index const unsigned long, to_index unsigned long, num_control_pts
  long, C3DVector x6, floats). Names: all symbols in Ego_r/FableWin (`_fmodf`/`__CIfmod` CRT). Retail-asm line checked (0x00A568B1 constants,
  0x00A568CB fsub, 0x00A568CE fistp [ebp-8]); purity retail_asm (shared GFFloatToLongFloor). No header change (existing opt-ins).
- Mutants (row source; the test links the row object incl. the same-TU copy): author 45 + audit 7 (Hermite copy x2, closed wrap x2, to_index
  wrap, both end repeats) = 52: 50 KILLED, 2 survivors equivalent (author's: the `EndPointsConnected &&` drop and `>` -> `>=`; I checked that an
  open spline's from_index <= n-2 even with GFFloatToLongFloor's +1 case, since (n-1)*s rounds to the integer n-1 only when n-1 is a power of two
  and then the product is exact and below n-1).
- REVIEW / Account A: Ego_d's GFGetHermiteSplinePosition locals are dest, from_pos, from_tan, to_pos, to_tan, s; wpos_to, s2, wpos_from, s3,
  wtan_from, wtan_to (six floats) -- the landed owner 0x00A56670 spells result/v1/t1/v2/t2 and eight floats (two_s3, three_s2, h1..h4). Re-port the
  owner in Ego_d spelling, then re-sync this copy verbatim (the copy may not diverge from the owner on its own).

## b7 0x00766660 CTCObjectAugmentations::AttemptToStartParticle -- PASS (fixed set: one redundant declaration removed + notes)
- Oracle 696 B == Fable.exe (`ret 14h`). Strict vland RELOCATION_MATCH PASS (author text and fixed text).
- Fixed (byte-neutral, RM): the row re-declared `extern const float DELTA_NOTIONAL_ZERO;` "as engine/C2DVector.h", but engine/C3DVector.h, which the
  row includes with FABLE_RETAIL_C3DVECTOR defined, already declares it (line 39). Local extern removed.
- Locals/scopes == Ego_d (pfx CIntelligentPointer<CThing>&, effectid / effectidtoblendto long, startpoint / endpoint const CCharString&, end_pos,
  start_pos, orientation CRightHandedSet; {blend_to CThing* {ptc_emitter1/2, pem1/2, the vector, the floats, MAGIC_FACTOR const float}}).
  Ego_d line table: statements in the text's order (two C3DVector ctors on 580 = start_pos / end_pos, orientation 581, MAGIC_FACTOR 629 between
  interp 628 and InterpolateTo 630). Lines 612 / 613 / 617 (488 / 490 / 219 B) have no retail-sized counterpart: disclosed as `// Unreproduced:`.
- No header change; no new header. Object: leaf + COMDAT-ANY only. Purity genuine. Names: all in Ego_r/FableWin; NAME_FIXES (DELTA_NOTIONAL_ZERO
  0x0129BA3C, Ego_d carved names + assert strings) agreed.
- Mutants (row source, test links the row object): author 42 + audit 6 (MAGIC_FACTOR, max->min, attach point, Kill receiver, Sleep receiver, blend id)
  = 48/48 KILLED.

## Names for Account A (one place)
- NEW 0x007791F0: CTCDRegionExit::GetNameOfRegionConnectingTo -> ?GetRegionName@CTCDRegionExit@@QBEABVCCharString@@XZ (landed file
  CTCDRegionExit_GetNameOfRegionConnectingTo_007791f0.cpp: Leaf + catalog + manifest; CTCDRegionExit.h lines 22-23 comment). 0x007792D0 keeps
  GetNameOfRegionConnectingTo. Evidence: Ego_r tcd_region_exit_point.obj definition order == retail address order (b2 section).
- b2 author rows (agreed): 0x00617D30 -> CThing::PeekTCDriver<const CTCDRegionExit>; 0x00615520 / 0x00615540 -> std::_List_base<const CThing*,...>
  ctor / dtor (0x00615540 is also reached by a jmp thunk 0x00616900 and 0x008FB15F: folded pointer-list dtor).
- b4 (agreed, descriptive): 0x00C0A860 LHA::CEventPlayOptions::SetLoopCount, 0x00C0A880 SetInterruptable, 0x00C08750 / 0x00C08780 CSampleQueryLH ctor/dtor.
- b5 (agreed): 0x00A82FA0 CCountedPointer<CNavConnectorNode>::DeleteData; the landed 0x00A7A650 LinkToNeighbouringMaps calls 0x00A793D0 as
  ConnectVerticalMapEdge and declares both public -> ConnectHorizantalMapEdge / private (AAE); 0x00A793D0 module CMap -> CNavQuadTree.
- b7 (agreed): 0x0129BA3C ?g_NormaliseEpsilon@@3MB -> ?DELTA_NOTIONAL_ZERO@@3MB.
- Re-port candidates (Ego_d spelling differs from landed text): 0x00A56670 GFGetHermiteSplinePosition (Ego_d: dest, from_pos, from_tan, to_pos,
  to_tan, s; wpos_to, s2, wpos_from, s3, wtan_from, wtan_to) -- then re-sync b6's copy; 0x00518DC0 CThingFilter_HasTCOfLeafType<...>::operator()
  (Ego_d: test_thing, ptc_type, `if (...) return true; return false;`; landed is a local model spelled thing / tc).
- Minor (shared header, pre-existing): CThing::PeekTCDriver's parameter is `ptc` in Ego_d (header: `result`).

## Review items for the coordinator
1. Gate gap: verify_and_land's SHARED_HEADER_AVAILABLE regex `\b(?:struct|class)\s+X\s*\{` misses `class X : public Base {`. b4 (and ~20 landed
   CSoundEmitterLH rows, plus 00a68370 from the earlier audit) escape it. libpk/LHAudioEventPosition.h defines a second, conflicting CSoundEventManager.
2. b4: land the shared-header port (fixed_00aabf60) or, if the family stays local for now, the author's text (also RM) with a recorded deviation.
3. b5: when engine/CNavConnectorNode.h lands, 0x00A780B0 and 0x00A78A90 go strict HEADER_POLICY (local `class CNavConnectorNode {`).
4. b3: register_scripts.obj rows carry different /d2inlT values (WatchForBooty T57, this row T62; their windows share 62..63). The rule is per row, so
   not a defect; a TU-wide value would be T62.
5. b1: the test now allocates ~100 MB twice (cases 12 / 14) on top of the author's large case 10; drop them if the lane objects (two mutants then
   survive as non-equivalent at extreme rates).
6. b2: the TU's /Os COMDAT copies of 0x00615520/40, 0x00617CA0, 0x00618E60, 0x00617D30, 0x00518DC0 differ from retail's chosen (speed) copies:
   pick-any, no grade impact (not measured beyond the per-function byte compare).
7. The 300-500 B Ego_d lines with no retail code (b1 55/69, b2 1198/1229, b3 236, b5 2263/2279/2285, b6 64/70/108/109, b7 612/613/617) are most
   likely debug-only checks; I disclosed them as `// Unreproduced:` in the fixed sources (b5 left unchanged).
8. Disclosure: inside one pipeline I ran `git -C /dev/null log` (not a repository; it failed and touched nothing). No other git command and no
   repo writes.

### Landing (coordinator; not run). b2 ORDER: respell the landed 0x007791F0 row (and the CTCDRegionExit.h comment) to GetRegionName BEFORE
### landing 0x0064DD9A, otherwise two names bind one address. b6: after keypayload, re-check the GFGetHermiteSplinePosition copy is still verbatim.
    cp -r $AW/fixed_<addr>/hdr/. rebuild/include/          # b2, b3, b4, b5 only
    python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/fixmove.py $AW/fixed_<addr>/payload.json
    python3 tools/decomp_pipeline/linux_lane/lane_tools/naming/keypayload.py $AW/fixed_<addr>/payload.json
    LANDVERIFY_STRICT_HEADERS=1 python3 tools/decomp_pipeline/linux_lane/lane_tools/vland.py $AW/fixed_<addr>/payload.json $AW/fixed_<addr>/oracle.tsv --land
    python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/bindings_multi_check.py   # must print 0
    # + NAME_FIXES.tsv of b2 / b4 / b5 / b7 via batch_rerow.py.
