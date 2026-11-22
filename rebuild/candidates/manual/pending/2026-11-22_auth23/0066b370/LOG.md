# 0x0066B370 LOG (AUTH23): three bounded checks on AUTH22's RELOCATION_MATCH; no re-authoring
Not grades: pk.py control on AUTH22's final.obj (GetSubDef NEQ 182 v 162-owner; GetDef x2, push_back, _M_insert_overflow, operator CCharString,
LowerBound, GetString, operator new EQ; bitset::test / HasTC "NEQ:relocs" only on the unbound "bitset" string literal symbol).
1. c0 = AUTH22's final source, my copy of its overlay (control): RELOCATION_MATCH.
## 0a pre-registration (ONE grade, oracle only)
Candidate a1: overlay hdr_a = hdr with CAnimSet::GetPercentageComplete / GetLastPercentageComplete returning `const float&` and the
`friend class CTCSoundPlayer;` line deleted; source line 158 calls `panim_set->GetLastPercentageComplete(), panim_set->GetPercentageComplete()`
as Ego_d line 208 does (line 155 already uses the accessors). Outcomes: RELOCATION_MATCH = the residue of the Ego_d-exact text (r12, 1844v1808)
is entirely the by-value return of the two accessors (same finding as 0x0093D270); it is an ORACLE, not a landable text, because FableWin.pdb's
`?GetLastPercentageComplete@CAnimSet@@QBEMXZ` / `?GetPercentageComplete@CAnimSet@@QBEMXZ` return float by value. DIFFER = the reference form does not
explain retail either; report where (0x0066B6CB compare or 0x0066B6E0 pushes). No second grade.
2. a1 (0a oracle: const float& accessors, no friend, Ego_d line-208 calls): RELOCATION_MATCH. Object a1.obj.
## 0b (not a grade): see NOTES.md. All three PDBs: CurrentPercentageComplete / LastPercentageComplete are `private`; the two accessors are
public by-value float; 0 LF_FRIEND* records in any of the three TPI streams (cl never emits them), so the friend line is NOT attestable.
## 0c pre-registration (ONE grade)
Candidate c1 = c0 (AUTH22's final, hdr overlay) + after the function's closing brace `#pragma optimize("y", off)` / `#pragma optimize("s", on)`
(the owner row 0x0065F604's own two pragmas, rebuild/src/compiled/00/65/CDefClassBase_GetSubDefCEntitySoundDef_0065f604.cpp lines 19-20).
Acceptance (fixed before grading): leaf still RELOCATION_MATCH; pk.py: GetSubDef<CEntitySoundDef> EQ against 0x0065F604; the copies that are EQ
in the control (GetDef<CMaterialDef> 0x0066C340, GetDef<CEngineThemeDef> 0x0063F3A0, push_back 0x0066C4D0, _M_insert_overflow 0x0066C3F0,
operator CCharString 0x00415DD0, GetString 0x00415D70, LowerBound 0x0040F020, operator new 0x00405C10) still EQ. Any failure is reported, no re-fit.
3. c1 (0c: c0 + the two row-tail pragmas): leaf RELOCATION_MATCH. pk.py (c1.pk.full.txt against ctl.pk.full.txt): GetSubDef<CEntitySoundDef>
   162 B, bytes EQ to 0x0065F604 (pk's "NEQ:relocs" is only the unbound literal `??_C@_0BA@OKBMKDFK@CEntitySoundDef` at 0x01235DD0, which holds
   "CEntitySoundDef\0" in retail). ACCEPTANCE FAILS on the third clause: the tail pragma re-modes EVERY template instance of the object:
   GetDef<CMaterialDef> 98v108, GetDef<CEngineThemeDef> 98v108, push_back 49v50, _M_insert_overflow 140v216, LowerBound 64v60 bytes differ,
   bitset::test 34v63 (all EQ in the control). Non-template inlines (GetString, operator CCharString, HasTC) are untouched.
Not grades: mini/t1.cpp (a pragma pair around a member-template DEFINITION does not reach its implicit instance; it does reach a plain inline),
mini/t2.cpp (a pair around an explicit INSTANTIATION after the use: no effect), mini/t3.cpp (a pair around an `inline` explicit SPECIALIZATION:
that one instance gets the frame-pointer size mode, a sibling instance is untouched).
Grades used by AUTH23 on this row: 3 (c0 control, a1, c1).
## Follow-up (coordinator, after the owner's "land as Inferred" decision for the const float& form)
Not a grade: mini/t4.cpp. `extern template` on one instance of a noinline member template: VC7.1 3077 prints C4231 and then dies with
fatal error C1001 (INTERNAL COMPILER ERROR) at the call. Out. (Control: the visible body gives the caller the no-escape knowledge, the
declaration-only form does not.)
Pre-registration (ONE grade): f1 = a1 + the renamed opt-in FABLE_RETAIL_CANIMSET_PERCENTAGE_REF_INFERRED (same tokens as a1's accessors) +
FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY (an `inline` explicit specialization inside optimize("y", off) / ("s", on) ..
optimize("", on), body = the template's tokens with T = CEntitySoundDef, nested beside the template in engine/CDefinitionManager.h), overlay
hdr_f. Pass = leaf RELOCATION_MATCH, GetSubDef<CEntitySoundDef> EQ 0x0065F604, every other defined symbol's size / class unchanged from
a1.obj, no new symbol. If the leaf moves the device is out; no variants.
4. f1 (final): RELOCATION_MATCH. pk.py on every defined symbol (f1.pk.full.txt v a1.pk.full.txt, 100 symbols each): the only change is
   GetSubDef<CEntitySoundDef> 182 -> 162 B, now EQ 0x0065F604 (its class-name literal bound to 0x01235DD0, which holds "CEntitySoundDef");
   no new symbol, no other size or class change, 0 NEQ lines. vt.py PASS; strict vland dry-run (LANDVERIFY_STRICT_HEADERS=1
   LANDVERIFY_STRICT_RELOCS=1, overlay hdr_f): RELOCATION_MATCH PASS, WINS 1 (land/vland.log).
Grades used by AUTH23 on this row: 4 (c0 control, a1, c1, f1).
