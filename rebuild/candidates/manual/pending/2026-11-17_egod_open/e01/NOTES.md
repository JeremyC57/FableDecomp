# e01 notes (egod lane, 2026-11-17)

## 0x00408820 CUserProfileManager::ResetAssignedInputsWASD -- RELOCATION_MATCH (2 grades), test PASS, vland RELOCATION_MATCH
- What matched: port onto the landed twin ResetAssignedInputs (0x004085F0) -- same opt-in set, same-TU GetDef<T> body
  (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY), inline CMainGameComponent::Get / CStartupDefinitionManager::IsAllocated --
  plus Ego_d's scope: one block holding `CDefPointer<const CControlsDef> pdef` then `CCharString controlDefName(flag ? A : B)`,
  ApplyValues after the block, and the aggressive arm `ResetAssignedInputs(); return;`. The old draft's local models,
  forced helper and raw-pointer release were the cause of its register rotation.
- Header: overlay opt-in FABLE_RETAIL_CUSERPROFILEMANAGER_RESETASSIGNEDINPUTSWASD (declaration only).
- Names: row Ego_r + Ego_d (proven). 0x013B864F: `g_KeyRedefinerFlag` is DESCRIPTIVE (spelling of the landed ProcessEvent
  0x00557EB0; fable_gfmain.h also declares `g_FableDisableCapsLock_013B864F` for the same address -- two spellings, no PDB data
  name in Ego_r / FableWin / Ego_d name sets; not in reloc_bindings.tsv). Coordinator: pick one spelling for 0x013B864F.

## 0x00903F30 CScriptGameResourceObjectScriptedThing::FollowPreCalculatedRoute -- OPEN, DIFFER(254v254), register-tie (pass 1: ~16 grades)
- Best: differ/00903f30.cpp = the earlier draft on today's headers (shared headers already). Only residue: proute in ebp and
  ai_move_type in ebx where retail has ebx / ebp (0x00903F75..0x00904001; every instruction else identical).
- Measured (c2prio, probe/tools copy of hand892 c2prio_op.py): proute prio -3 key 26, ai_move_type prio 0 key 40. Relative sum
  over the blocks that differ: proute +12 (block of GetPThing/HasTC, K=3, cost 4) -7 (live through the switch blocks) v
  ai_move_type +8 (the three switch arms). Retail needs proute > ai_move_type: +4 for proute (one more candidate referenced in
  the GetPThing/HasTC block, K 3 -> 4) would do it.
- Ego_d: proute (const CThing*) in the if-alive block; ai_move_type + ptc_script_control in a nested block (the HasTC if). NO
  named `is_route`: the draft's `const bool& is_route` (which gives retail's stored byte `and al,1 ; mov [esp+14h],al`,
  0x00903F83) is not in Ego_d. Without it (plain HasTC) the store is lost (`test byte ptr [ebp+34h],40h`); with
  FABLE_RETAIL_CTHING_HASTC_MATERIALISED the GetTC<CTCScriptedControl> presence test is materialised too (retail tests it in place,
  `test ecx,ecx ; jns` 0x00903FB9). Debug length 809 v MoveToThing 631 with fewer arguments: Ego_d's body has ~180 more debug bytes
  (an assert / report on the route test?) -- unexplained.
- Tried (no change to the tie): GFConvertScriptMoveTypeToAIMoveType as if/else-if (253 bytes), static / extern / static cdecl;
  nested ifs for the alive tests; `is_route==true`; declaration orders of ai_move_type / ptc_script_control.
- Next: find the block-4 candidate (the retail HasTC text that stores the byte); check the HasTC / GetTC texts against FableWin
  sizes (fwsize) for this TU.

## 0x00A55F90 CMatrix3x4::SetFromVectorAndAngle -- OPEN, DIFFER(259v259), x87-order (pass 1: ~33 grades)
- Best: differ/00a55f90.cpp. Ego_d names (`float cos = GFCos(angle); float sin = GFSin(angle);` then vx2..stvz) plus grouping
  parentheses on E11 and E22 only (`(vx2 + cos) - vx2 * cos`; E33 without) closed the tail scheduling residue (xor / zero stores
  around the E33 fsubp, 0x00A5607B..0x00A56088): from 7 hunks to 2.
- Left: vxvy and vxvz products load X first (`fld [eax] ; fmul [eax+4]`, `fld [eax] ; fmul [eax+8]`) where retail loads Y / Z
  first (0x00A55FC9, 0x00A55FD2); vyvz already matches (Z first). Read as candidate ids: ours X > Z > Y, retail Z > Y > X.
- Insensitive (no byte change): spelling the products Y*X / Z*X / Z*Y, `stvx = normal.X * sin`, const floats, one declaration
  line for all locals, `(float)GFCos(..)` casts, dead-local rulers k = 1..7, 12, 20 BEFORE and 3 AFTER the products (diagnostic
  only), chained / reversed zero stores; GFSin/GFCos texts with a named double / (double)(float) / float constant (fwsize: FableWin
  GFSin / GFCos 10i/30b v the header's 7i/21b; a named `double a` text is 10i/29b) -- none moved the two products.
- fwsize: row at /Od 120i/357b v FableWin 125i/357b (Ego_d debug length also 357): five more instructions at equal bytes in
  the original text; not identified.
- Next: find the IL that changes candidate ids for normal.X (accessor reads? the angle scaling written in the row?), c2 trace of
  the operand choice.

## 0x007DA400 / 0x007DA520 / 0x007DA640 CTCTavernGameCardBlackjack::GetCurrentScore / GetRequiredScore / GetBet -- RELOCATION_MATCH x3, tests PASS, vland RELOCATION_MATCH
- Ported from the local-model drafts onto the shared headers (engine/CTCTavernGameCardBlackjack.h + overlay SCORETEXT opt-in,
  CTavernGameDef.h, CDefPointer<const CTavernGameDef> with FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF, the landed GetWinPhrase /
  CardsMainGame opt-in set with GETDEF_OUTOFLINE). Ego_d locals pdef (and ret in GetBet).
- GetCurrentScore / GetRequiredScore: the old frame residue (0x14 v 0x10; the long -> double conversion word of the hand value not
  sharing the GetText result word) closed by giving the SAME-TU callee GetHandValue (0x007DA230, landed) its body in the TU, defined
  AFTER the row as in Ego_d's object order (2 grades). The body is written with Ego_d's names (hand, include_face_down_cards, total,
  num_aces, iterator `card`) and itself grades MATCH against retail 0x007DA230 (q.py with its leaf). GetBet matched at its first
  grade with Ego_d's `CWideString ret = GetText(...); if (...) ret += GFIntToWideString(Bets[i]); else ret += GFIntToWideString(BaseBet);
  return ret;` (the shared GFIntToWideString.h inline: called in one arm, expanded in the other, as retail).
- Landing note: rows 0x007DA400 / 0x007DA520 objects also define ?GetHandValue@CTCTavernGameCardBlackjack@@QAEJPAV?$vector@PAVCCard@@...@std@@_N@Z
  (same-TU body; the landed 0x007DA230 row is a local model with a different mangled parameter type).
- Names: rows Ego_r/FableWin/Ego_d (proven; vtable slots 44/45/47). 0x009A08B0 = GFFloatToCharString(double n, long precision)
  (Ego_r + FableWin; landed spellings CCharString::FormatDouble and FormatRealText_009a08b0 are wrong -> NAME_FIXES.tsv).

## 0x00AB7B00 CStaticFontBank::GetStringExtents -- OPEN, DIFFER(287v287), x87-order (pass 1: ~20 grades)
- Best: differ/00ab7b00.cpp (overlay FABLE_RETAIL_CSTATICFONTBANK_FINDGLYPH). Shape from the landed twin CStreamingFontBank::
  GetStringExtents 0x009A14E0 + Ego_d names: `C2DExtentsF extents(0,0); float sx = 0; float sy = (float)(unsigned long)(FontHeight+1);`
  `while (*string) { letter = *string++; if newline { if (first_line_only) return extents; sx = 0; sy += ...; } if out of
  [MinChar, MaxChar] continue; CGlyph* pglyph = FindGlyph(letter); if (pglyph) sx = sx + Offset + Advance; maxima } return extents;`
  The early `return extents` (not `break`) was the lever that put extents.Y on the x87 stack and sx in the frame (15 -> 5 hunks;
  the remaining zero-store "hunks" are identical text, a difflib artefact).
- Left: the add chain Offset + sx + Advance: ours `fild Advance ; fadd sx ; fiadd Offset`, retail `fild Offset ; fadd sx ; fiadd
  Advance` (0x00AB7BB2..0x00AB7BCA). Insensitive to: every source order of the three terms, explicit (float) casts, const pglyph,
  dead-local rulers k = 1..6 (diagnostic), 1..3 dead FABLE_RETAIL_ASSERT arms inside FindGlyph (probe).
- fwsize: row /Od 96i/305b v FableWin 106i/319b (Ego_d debug length 319): the original text has ~10 more /Od instructions
  (+14 bytes) -- not identified; FindGlyph 35i v FableWin 116i/409b (asserts in the debug build).
- Next: find the +10 /Od instructions (a different loop / range-test form?) -- the missing IL is the likely key for the add-chain order.

## 0x0086BE40 CRandomAppearanceMorph::GetRandomBodyParts -- RELOCATION_MATCH (8 grades), test PASS, vland RELOCATION_MATCH
- Ego_d has no first / last locals: `if (!BodyParts[cbody_part_type].empty())` after `CMeshTextureSet mesh_texture_set;` left only the
  parked residue (retail loads the element's begin / end before the set's three zero stores, 0x0086BE87..0x0086BE9A).
- That residue closed with BOTH (a) the same-TU body of GetRandomTextureMorphs (0x0086B810, landed; written with Ego_d's names and
  scopes, defined after the row as in Ego_d's object order) and (b) the existing FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (push_back
  body visible, kept out of line; retail calls push_back<CMeshTextureSet> 0x0086E7C0) in place of the declaration-only
  PUSHBACK_OUTOFLINE. Either alone leaves the stores first; a dummy helper body lets VC7.1 prove the set stays empty (diagnostic only).
- The helper body grades DIFFER(191v230) against 0x0086B810 itself: under the TU-wide PUSHBACK_NOINLINE it calls
  push_back<CMorphSetTextureMorph> where retail expands it (plain inline push_back expands BOTH push_backs: 488 bytes). Not graded here.
- Overlay: FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TEXTUREMORPH_CTOR (helper only); existing FABLE_RETAIL_CVECTORMAP_UPPERBOUND.
- Landing note: the row object also defines ?GetRandomTextureMorphs@CRandomAppearanceMorph@@IBE... (same-TU body) and calls the COMDAT
  push_back<CMorphSetTextureMorph> from it.

## 0x004B01B0 CQuestManager::IsRegionValidInRegionLockingMode -- RELOCATION_MATCH (6 grades), test PASS
- Ego_d's structure (two `long i` loops, pactive_quest, any_regions_specified, `const vector<const CRegion*>& regions = GetRegions()`),
  not the old draft's pointer iterator. The first loop's strength-reduced element pointer (ebx, start read once) and i in a frame word
  came only with GetActiveQuestFromScriptName's body visible in the TU (Ego_d: defined before the row; a noinline stub proved it,
  then the landed 0x004AF5B0 text over the shared headers, which itself grades RELOCATION_MATCH against 0x004AF5B0).
- Layout: the landed 0x004AF5B0 opt-in set (LOADGAMESTATE + ACTIVEQUEST + SAVEGAMESTATE + GETACTIVEQUESTFROMSCRIPTNAME) + overlay
  FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_REGIONLOCK (the REGIONLOCK inline helpers on the full record).
- Ego_d also records a nested `long r` loop with `const CRegion* pregion` over `regions`: it compiles to nothing (probe: with or without it,
  and with a bare `GetRegions();`, all RELOCATION_MATCH). Omitted; `regions` kept with a `// Fidelity:` line. Coordinator: if a named
  but unread reference is not wanted, `pactive_quest->GetRegions();` is byte-identical.
- Landing note: the row object also defines ?GetActiveQuestFromScriptName@CQuestManager@@QBEPAVCActiveQuest@1@ABVCCharString@@@Z and the
  noinline std::__find instance (as the old draft did).

## 0x00747C70 CTCStealth::UpdateDangerInfo -- RELOCATION_MATCH (9 grades + 1 diagnostic), test PASS, vland RELOCATION_MATCH (one WARN)
- Ported from the local-model draft onto engine/CTCStealth.h (DangerThings vector<pair<CIntelligentPointer<const CThing>,
  NStealth::CDangerStatus> >, PDef CDefPointer<const CStealthDef>), engine/CTCTargetingAI.h (FABLE_RETAIL_CTCTARGETINGAI_ATTACK),
  CThing GetTC<CTCTargetingAI> (GETTC_EXPANDED), STLport erase / sort (ERASE_RANGE_OUTOFLINE, SORT_FASTCALL); the inline STLport
  remove_if over the out-of-line __find_if 0x00748E40 / remove_copy_if 0x00748D80 is declared in the row (STLport 4.6 _algo.h text).
- The old register residue (list start reloaded, ebx/ebp swap) closed with the SAME-TU body of GetNumberOfAttackersOverMaximum
  (0x00747890, Ego_d order: defined just before the row; its Ego_d names preal_danger_things, rdt_prop, num_real_threat,
  max_number_of_attackers, it, dan_stat) PLUS its reserve visible (FABLE_RETAIL_STLP_VECTOR_RESERVE). The helper body grades
  RELOCATION_MATCH against 0x00747890 itself. GetDangerFeedbackHangtimeFrames (Ego_d: defined earlier in the TU) is defined before
  the row too (expanded: `PDef ? ConstantFPS * DangerFeedbackHangtimeSeconds : 1.0f`).
- Scope note: Ego_d gives the iterator `it` its own block; the source uses a do-while with `it` in the if block (an explicit bare block
  is also RELOCATION_MATCH; `for (it...; num > 0; ...)` and `for (;;) {... break;}` are not).
- vland: WARN RELOC_NAME for the erase call 0x00749280, catalog label '_global::CTargetPoint_Copy_WithDestruction' (propagated, wrong);
  the landed row 0x00749280 is the vector<pair<...> >::erase(first, last) -> NAME_FIXES.tsv.
- Landing note: the row object also defines GetNumberOfAttackersOverMaximum and GetDangerFeedbackHangtimeFrames (same-TU bodies).

## Pass 2 (after rows 8-10 showed the "same-TU callee body" lever)
- 0x00903F30: a visible noinline CActionMoveTo(const CThing&, EMovementType, bool, bool) stub (Ego_d lists the ctor right before the row)
  changed nothing (diagnostic, 1 grade). The tie is not an escape question; still the c2 priority gap (proute -3 v ai_move_type 0).
- 0x00AB7B00: compiling the landed CStaticFontBank ctor body (0x00AB8E10, same TU, earlier) ahead of the row changed nothing
  (diagnostic for "record ids carried over from earlier functions", 1 grade).
- 0x00903F30 more (pass 2, 7 grades): fwsize: row /Od 81i/268b v FableWin 230i/809b (+149i: debug asserts, ~2), GetTC +76i (assert),
  GFConvertScriptMoveTypeToAIMoveType 19i/52b v FableWin 20i/56b (switch-with-default; explicit SNEAK case 23i/65b, case+default 19i/52b,
  a result local 19i/59b -- none SAME; Ego_d has no result local). Dead FABLE_RETAIL_ASSERT(proute->HasTC(..)) x1 / x2 / x4 (and the
  _ARM form, and inside the if, and on ptc_script_control) with the plain `if (proute->HasTC(..))`: no change -- the HasTC byte is not
  stored (`test byte ptr [ebp+34h],40h`) and the proute / ai_move_type swap stays. The stored byte (0x00903F85) still needs the draft's
  `const bool& is_route` (not in Ego_d) or HASTC_MATERIALISED (which also materialises GetTC's test, unlike retail).
- 0x004B01B0: the old draft's `#pragma auto_inline(off)` / `__declspec(noinline)` around the std::__find body were dropped (VC7.1 keeps
  the body out of line by itself; regraded RELOCATION_MATCH), so the row carries no inline control.

## Coordinator decisions / review items
1. Same-TU callee bodies in row objects (WF_RULES: "text equal to the landed body"): the landed rows 0x007DA230 GetHandValue, 0x00747890
   GetNumberOfAttackersOverMaximum and 0x0086B810 GetRandomTextureMorphs are LOCAL MODELS, so equal text is impossible on the shared
   headers. My shared-header texts (Ego_d names) grade MATCH at 0x007DA230 and RELOCATION_MATCH at 0x00747890 (they could replace the
   landed local-model rows); 0x004AF5B0's text is the landed one verbatim (RELOCATION_MATCH there). The 0x0086B810 text grades
   DIFFER(191v230) at its own address because the TU-wide FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE keeps its push_back<CMorphSetTextureMorph>
   out of line (retail expands it) -- decide whether that is acceptable for row 0x0086BE40.
2. Row 0x00747C70 declares the STLport 4.6 remove_if / find_if (inline) and the out-of-line __find_if / remove_copy_if in the row
   (std namespace, STLport text), as the old 0x004B01B0 draft did for __find; move to a stlport opt-in if wanted.
3. Names: 0x013B864F has two landed spellings (g_KeyRedefinerFlag in 0x00557EB0 / this row; g_FableDisableCapsLock_013B864F in
   fable_gfmain.h), no PDB name; 0x009A08B0 is bound as three names (NAME_FIXES: GFFloatToCharString, Ego_r + FableWin);
   0x01375728 g_maxNumberOfAttackers (bound, descriptive).
4. Row 0x004B01B0 keeps the named-but-unread `regions` reference (Fidelity line); `pactive_quest->GetRegions();` is byte-identical.
5. Row 0x00747C70 uses a do-while where Ego_d gives `it` its own block (a bare block around it is also RELOCATION_MATCH).
6. Overlay FABLE_RETAIL_CSTATICFONTBANK_FINDGLYPH belongs to the OPEN row 0x00AB7B00 (no matched row needs it).
7. Next steps for the open rows: fit the row TEXT to FableWin /Od sizes with fwsize first (0x00A55F90 +5i/+0b, 0x00AB7B00 +10i/+14b,
   0x00903F30 +149i (asserts)), then regrade; the residues are c2 record-id / priority orders that no local spelling moved.
