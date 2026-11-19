# hand4a NOTES (2026-11-19)

## 0x006B5890 std::__lower_bound<pair<long,float>*, pair<long,float>, less<pair<long,float> >, int> -- MATCH, test PASS (vland strict) -- INFERRED
- Bounded re-check (9 grades) of the sourced readings, none matches:
  1. STLport 4.6.2 3-arg lower_bound builds less<value_type of the iterator>; __lower_bound takes const _Tp& val. The retail caller
     (CEnvironment::SetWeightedThemes 0x006B3F1B / 0x006B3F3C) passes the SAME key &[esp+10h] to lower_bound and to
     vector<pair<long,float> >::insert (const value_type&), so val's type is the element type: no converting pair temporary exists.
  2. Implicit instantiation through the 3-arg forwarder in the same TU (s2), POD comparator without binary_function base (s3),
     /Ob1 /Oy- /GX-: all the stock DIFFER(76v76) (&val in ebx, `fld [middle+4]; fcomp [ebx+4]; test ah,5`).
  3. Donors: FableWin std::operator< <long,float> (0x0245A510, ABU..0 const& both) and Ego_r _Lower_bound 0x006F1B30 (VC10 /O2 also keeps
     &val in ebx); FableWin's pair ctors from non-const / rvalue refs are VC10 Dinkumware artefacts.
- Handed back hand3a's fitted text unchanged (src/006b5890.cpp) with an `// Inferred:` note; overlay opt-in
  FABLE_RETAIL_STLP_PAIR_LESS_RIGHT_BYVALUE in hdr/stlport/fable_containers.h (copy of today's repo header + the nested opt-in; no
  FABLE_STATIC_ASSERT after the insertion point). Coordinator decides whether the Inferred landing is accepted.
- vland WARN STANDIN_LEAF: the catalog label is the Dinkumware `_Lower_bound<...>` spelling; NAME_FIXES.tsv carries the STLport name.

## 0x0047ED76 CTCInGameMenu::SetToSave -- RELOCATION_MATCH, test PASS (vland strict)
- Draft (shared headers, Ego_d locals pos / ptext / ptext_real, size TU pragmas of the landed sibling CloseInGameMenuPC) was
  DIFFER(366v366): `push ecx ; fsubp st(1)` at 0x0047EEBB/0x0047EEBD (retail `fsubp st(1) ; push ecx`), 100+ earlier grades of spellings.
- No change from: TU flags /O1 /Oy- instead of the pragmas; the GetBackBuffer* inline bodies visible (_INLINE / _ORIGINAL_DIMENSIONS opt-ins).
- What closed it (MATCHING_PLAYBOOK scheduling-region rule): a no-code PAREN tuple. 16-variant measurement of grouping parentheses:
  only `(GetBackBufferWidth() * 0.5f) - scale.X * 204.0f` (left half grouped) matches, whatever Y has; the source carries the
  symmetric natural spelling (both halves grouped). Local GFGetTime declaration replaced by engine/GFGetTime.h. No flags, no opt-ins.

## 0x00853670 NCameraModes::CScriptModuleOpenChest::OnInitLeaf / 0x008539C0 CScriptModuleUseLamp::OnInitLeaf -- RELOCATION_MATCH, tests PASS (vland strict) -- INFERRED
- Ported from local models onto shared headers (C3DVector, CThing, CIntelligentPointer + COMPARE_NULL_INT, ThingAlive IS_THING_ALIVE,
  GFRandom, NCameraModes_CBase MEMBERS GetPWorldSeed, CTCGraphicAppearance FOCAL GetHelperPoint) with FableWin's debug bodies
  (0x0283B7D0 / 0x0283BEC0, cm_scripting_controls.obj) and Ego_d's block locals (theta_end, height_end, mag, rad_end).
- NEW overlay headers (no repo header existed): engine/NCameraModes_CScriptModuleBase.h (CScriptModuleBase + CScriptModuleSelfTerminateBase,
  Ego_d LF_CLASS 0x3C015 / 0x3C07C, inline GetOwner / GetDuration per FableWin.pdb), engine/NCameraModes_CScriptModuleOpenChest.h,
  engine/NCameraModes_CScriptModuleUseLamp.h (Ego_d 0x3C140 / 0x3C150, size asserts 0x4c / 0x54 / 0xa0).
- Port alone: chest DIFFER(445v445) (Rad stored before Theta, owner load after Height: 0x008537A4..AD); lamp DIFFER(475v475) (AxisY copy
  reload 0x00853A90, `test eax,eax` 0x00853B4B).
- Mechanism (c2 list-scheduler trace, hand892 sched tools copied to tools/sched, sched.sh / sch.sh): the region after the alive tests
  is capped at 81 IL tuples; chest needs the cut exactly 2 tuples earlier (between fadd and the Rad fstp), lamp 2 or 3.
- Measured placements: one tuple anywhere (Rad paren, CrossWithUp `(-X)`, operator/= paren, GetSquaredMagnitude nesting) no; two in the
  subtraction (SUBTRACT_XY_GROUP) breaks the sub copy; CrossWithUp + /= no; three grouped squares matches lamp only;
  GetAccurateMagnitude = GFRoot(GetSquaredMagnitude()) + /= paren matches both but contradicts FableWin's GetAccurateMagnitude body
  (0x01D99FB0 inline squares, call GFRoot) and turns landed 0x00875690 DIFFER(762v762) -> rejected.
  Chosen (INFERRED, coordinator decides): `float mag = (AxisY.GetAccurateMagnitude());` in the row + new overlay opt-in
  FABLE_RETAIL_C3DVECTOR_DIVASSIGN_RECIPROCAL_PAREN (`float reciprocal = (1.0f / v);`). Cross-check: that opt-in body as the DEFAULT
  left every one of ~45 landed C3DVector-using rows (GetAccurateMagnitude / AccurateNormalise / `/=` users) matching.
- Names: Leafs checked with objdump; TARGETING_CURSOR_POSITION_NAME (0x013BAC50) is the PDB global (labels_landing_2026-11-12dj).
- Overlay caution found on the way: the repo C3DVector.h changed during the session (CROSS_MEMBER_XY_GROUP added for 0x007BBB3B);
  hdr/engine/C3DVector.h was re-made from today's repo copy + only the new opt-in.
