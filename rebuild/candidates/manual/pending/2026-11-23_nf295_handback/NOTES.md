# NF295 notes (2026-11-23): NF294 follow-ups + T1 "other" class / namespace entries, hand-back 1 (ENTRIES lines 2-65)

Base: the repo working tree as of today. I never wrote to it and ran no git.

Numbering:
- My "entry n" is the n-th data line of my ENTRIES.tsv, which is file line n+1 because line 1 is the header.
- Entries 1-4 are NF294's follow-ups.
- Entries 5-64 are NF294's entries 61-120 (NF294 entry = mine + 56). All 64 are "class/ns" kind.

## Summary (counts from STATUS.tsv / APPLIED.tsv)
- **64 entries reached:** the 4 follow-ups plus NF294's entries 61-120. Entries 65-599 (NF294's 121-655) are still `open`.
- **9 rows changed** (rows.txt) and **one header add-only opt-in**.
  - Every changed row is RELOCATION_MATCH at HEAD and after the change.
  - The vland `--recheck-landed` run on the new sources gives 9/9 RELOCATION_MATCH, test PASS (recheck_final.log).
  - 7 rows are objsec ALL_EQ: `.drectve` and vtables are equal, and every function COMDAT is paired under the renames.
  - 0x00752640's COMDAT set is the same (16 = 16), and every COMDAT is equal under the renames except one deliberate change: its
    unreferenced pick-any ReleaseData copy now takes the owner's retail order (see entry 4, evidence/proof_u3_00752640.txt).
  - 0x00634470 is equal in its function `.text`, its relocations and its `.drectve`, but its object emits 5 extra COMDATs, so it
    does not pass "COMDAT sets SAME". It is landing-ready only if Q2 is accepted.
- **Status counts:**
  - done 4: entries 4, 11, 12, 13.
  - done-conditional 1: entry 3 (0x00634470, Q2).
  - done-partial 3: entries 2, 30 (the stale CMemoryDataInputStream table line stays) and 54.
  - done-already 3: entries 62-64, table only, stale witness.
  - skipped-port 33, skipped-held 20. No rejects.
- No vtable appeared or changed.

## Follow-ups (entries 1-4)
- **Entry 1, 0x008FD010 IsStopping: not doable as briefed (decision Q1).**
  - 0x0093C4E0's `CAIStateGroup_StealthSeekEnemy_Adapter` derives the flat generated `engine/CAIStateGroup_StealthSeekEnemy.h`. That
    struct is a byte view (`__vftable`, `_pad_0x04`.. `PEnemy`) with **no CAIStateGroupBase base**. NF294's note "the Adapter derives the
    FLAT CAIStateGroupBase" was wrong. So an add-only declaration in CAIStateGroupBase.h's flat branch is unreachable from that row.
  - Adding a base, or a local CAIStateGroupBase model, is a port.
  - 0x0094FEB0 is left: line 110 still holds NF292's Terminate at HEAD, so NF292's landing has not changed it.
- **Entry 2, 0x0097EC00 AIAction_MoveToPos: three of the four stand-in binders are done.**
  - **0x0094A6D0** (flat CAIStateGroupBase base): uses the new opt-in `FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_MOVETOPOS` (HDR_README.md).
    Its Methods declaration is dropped, and `&reached` becomes `reached`.
  - **0x0093CB20:** it includes the header under `#define CAIStateGroupBase CAIStateGroupBaseLayout`, so an opt-in member would mangle on
    the Layout name. The member is declared instead on the row's own real-name `class CAIStateGroupBase : public CAIStateGroupBaseLayout`.
  - **0x0091E4B0:** same fix, on its real-name local `class CAIStateGroupBase`. That class is an empty first base at +0, so `this` is
    unchanged.
  - **Widenings (R1 kinds), byte-identical:**
    - `long` -> `EAIActionRepeat` / `EMovementType` / `EAIActionReturn`. The enumerators come from the shared
      `engine/fable_ai_action_enums.h` (Ego_r enum records): 2 = `ACTION_REPEAT_CALLBACK`, 1 = `ACTION_REPEAT_UNTIL_FINISHED`,
      3 = `AI_MOVEMENT_WALKING`.
    - `bool*` -> `bool&`.
    - `CThing*` -> `const CThing*`.
  - **Left:** 0x00968230. `engine/CAIStateGroup_TavernKeeper.h`'s flat struct has no CAIStateGroupBase base either (the retail branch
    does): port.
- **Entry 3, 0x00A0D2D0 RemoveFromInterface: 0x00634470 re-ported onto the landed siblings' shape** (0x00633A60 / 0x00633880).
  - The re-port uses `FABLE_RETAIL_PLAYER_INPUTPROCESSES` + `_SCOPED_PTR` (+`_RESET`) + `_INPUTPROCESS` + `_CAINPUTPROCESS_REMOVEFROMINTERFACE`
    + `_CINTELLIGENTPOINTER` + `_DEF_CLASS_BASE`, the five per-process headers, and the row's `FABLE_RETAIL_PLAYER_REMOVEMODE`.
  - The function is byte-equal and relocation-equal under the respell, and `.drectve` is equal (evidence/proof_u1b_00634470.txt).
  - The object now also emits five unreferenced 3-byte `boost::scoped_ptr<T>::operator->` pick-any COMDATs. The HEAD sibling 0x00633A60
    emits 15 of the same (work/obj/H_00633a60.obj). Decision Q2.
  - Owner 0x00A0D2D0 defines only the stand-in `CAInputProcess_Methods`. The PDB name is unresolved both before and after (link_B), and is
    recorded in NAME_FIXES.
- **Entry 4, 0x0073A9B0 CAnimEntryLite ctor: done, with the self row.**
  - Both binders moved onto `engine/NAnimationEntryBodge_CAnimEntryLite.h`:
    - 0x008E9190 needed only the include and the qualified name;
    - 0x00752640 also dereferences the `const CAnimationEntry*` at its four `new` sites (same push).
  - HEAD 0x008E9190 bound `??0CAnimEntryLite@@QAE@ABVCAnimationEntry@@@Z`, which the self row defined. Respelling the binder alone would
    leave unresolved a reference HEAD resolved.
  - So the self row 0x0073A9B0 is ported onto the same header too. It now defines the PDB ctor (objsec ALL_EQ), and its Leaf line is
    added (RENAMES section 1). With that, link_C resolves the ctor.
  - This also closes entries 11 / 12 / 13 (0x0073D180 / 0x00754F10 / 0x00863090): the two binders now bind the PDB CCountedPointer spellings.
  - **Pick-any check** (tools/pickany.py, evidence/pickany_00752640.txt). 0x00752640 now defines these COMDATs under PDB spellings that
    have owners, and each is byte-equal to retail at its owner address:
    - `??4` = 0x00754F10, 72 bytes;
    - the `(T*)` ctor = 0x0073D180, 78 bytes;
    - DeleteData = 0x0073CC90, 8 bytes.
  - Every relocation of those copies resolves to the retail target. The only exception is DeleteData, whose table line is the
    `_unattributed` stand-in `Destroy_0073cc90`.
  - **ReleaseData (one deliberate COMDAT body change).** Once respelled, 0x00752640's unreferenced pick-any
    `?ReleaseData@?$CCountedPointer@VCAnimEntryLite@NAnimationEntryBodge@@@@QAEXXZ` would share its name with the copy the owner of `??4`,
    0x00754F10, emits.
    - The bodies differed: the default branch zeroes Info then Data, while 0x00754F10 defines `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA`
      and zeroes Data then Info.
    - Only the Data-first order occurs in retail: a relocation-masked byte search of Fable.exe finds 732 sites for it and 0 for the
      other.
    - So 0x00752640 now defines `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` (one define line above the include). Its function and every
      other COMDAT stay equal to HEAD; only ReleaseData changes, to the owner's body (cmpcomdat_00752640_v_00754f10.txt: all 3 shared
      COMDATs equal).
    - vland was rerun on this final source (recheck_final.log tail).
  - Its other COMDATs (`??1`, ReleaseData, ShareData) have no table line and are not called by the row's function.
  - **Made-up name moved into the real class (needs a look):** 0x008E9190's local CCountedPointer model has an inline `Clear()` that is in
    no PDB. It is emitted unreferenced as a pick-any COMDAT, and the port moves it from `?Clear@?$CCountedPointer@UCAnimEntryLite@@@@QAEXXZ`
    to `?Clear@?$CCountedPointer@VCAnimEntryLite@NAnimationEntryBodge@@@@QAEXXZ`. The census finds no other definition of the new name.
    Its body is the inline release + zero that retail expands at four sites. It is defined by no other landed row (grep `void Clear(`
    over every CAnimEntryLite user).
- **Census of the respelled targets (evidence/xsites_nf295.tsv, every E8/E9 site in Fable.exe).** It checks whether any other landed
  caller still binds one of the old spellings.
  - 0x0073A9B0 is called only from 0x00752640, 0x00861E60, 0x008621A0 and 0x008E9190. 0x00861E60 and 0x008621A0 are on the shared
    header, so no other landed caller binds the global ctor.
  - 0x00863090: 0x008621A0 and 0x008E9190. 0x00754F10: 0x00752640 and 0x008621A0. 0x0073D180: 0x00752640 and 0x00861E60.
  - 0x0084AF90: 0x00845900, which keeps its own line, and 0x00846660.
  - 0x0097EC00 and 0x00A0D2D0 have many callers, but each deleted stand-in spelling is a class named in exactly one landed source.

## Entries 5-64 (NF294's 61-120)
- **Classification.**
  - tools/cls.py and tri4.py read every stand-in table line and witness source. Both now read only the row's own catalog block, the same
    bug NF294 fixed in flags.sh.
  - The HEAD objects of the 37 compilable non-off-limits witnesses were then compiled (gr_b2a/b, all RELOCATION_MATCH) and read with
    objbind.py.
  - Not compiled: 0x00698F50 and 0x009FE8E0, whose tests fail clang only on `_assert`, and 0x00CFD6C0, whose source fails clang.
- **Entry 30, 0x00993C10** (binder 0x00B6D940). The protected base member moved onto the row's real-name local `class CDataInputStream`,
  which already has `friend class CMemoryDataInputStream`.
  - The call uses the row's own existing `((CDataInputStream*)this)->` pattern, the same one it uses for ReadWithSrcChunkOverflow.
  - Owner 0x00993C10 defines only the stand-in `CDataInputStream_Methods`, so the name is unresolved before and after (link_D; NAME_FIXES).
  - The stale `CMemoryDataInputStream` table line is **not** deleted. Other rows (0x00995B10, 0x00BE91E0, 0x009BAE70, ..) declare the
    setter on their own local models and may bind it, so the split gate's exe scan should rewitness or delete it.
- **Entry 54, 0x0084AF90** (binder 0x00846660). The global local `struct CEventTalkedTo`, whose only symbol is the ctor, is nested in the
  row's existing `namespace NEntityEvents` block as a `class`.
  - It takes `CThing&` (Ego_r + FableWin), and the call site dereferences, giving the same push.
  - Link resolves against owner 0x0084AF90, which defines the PDB name.
  - Left: 0x00845900 (NF292/NF293 witness).
- **Entries 62-64, 0x00E8BE10 / 0x00E8BD30 / 0x00E8CA60: stale witness.**
  - 0x00E8B9C0's HEAD object binds only the NScript PDB spellings, which already have their own table lines with the same witness. The
    three non-NScript lines are deleted.
  - The Allocs are only address-taken in that Main (source census).
- **Entry 9, 0x00704580: stale witness, not acted on.**
  - 0x00CF8970 binds the PDB `??4?$CCountedPointer@VCScriptThing@@@@`, but about ten other NScript rows still model
    `CCountedPointer<CScriptThingImp>` and may bind the stand-in. The line is left for the gate's exe scan to rewitness.
- **The rest are ports (33) or held (20).** The held ones have a binder that is a witness in NF292's or NF293's ENTRIES (work/sibwit.txt),
  or is off-limits. STATUS gives the reason per entry.
- **Recurring port shapes:**
  - a stand-in local `CCountedPointer` over a global element where the PDB nests the element;
  - a member bound on a header-less local class where the PDB member sits on a base (CCombatActionBase -> CCreatureActionBase);
  - free `extern "C"` / `@helper` stand-ins for members;
  - the binder's own self leaf on a stand-in (entries 32, 45, 52: NF282 D2, T3);
  - entry 46, whose shared `NThingManager_CEntityRuntimePersistence.h` pulls CArray / fable_map beside a local CCharString.

## Proof files
- evidence/proof_u1.txt: 0x0093CB20 / 0x0094A6D0 / 0x0091E4B0 / 0x008E9190 / 0x00752640 objsec. 0x00634470's line there is the plain
  index-paired DIFF; use proof_u1b_00634470.txt for it.
- evidence/proof_u3_00752640.txt: final 0x00752640 matched by symbol (proof_u1.txt has its first, pre-RELEASEDATA, form).
- evidence/proof_own2.txt: 0x0073A9B0.
- evidence/proof_u2.txt: 0x00B6D940 and 0x00846660.
- evidence/link_A..D.txt.
- evidence/eq_aisg* (header equiv plus control).
- evidence/pickany_00752640.txt.
- The pre_*.log files record which check each compiled source and test passed. All passed clang, except the not-compiled ones above.

## Decisions / questions
1. **Entry 1 / 0x0093C4E0.** The flat `CAIStateGroup_StealthSeekEnemy.h` has no CAIStateGroupBase base, so no CAIStateGroupBase.h opt-in
   reaches the Adapter. Options:
   - (a) T3 port: give the generated header a `FABLE_RETAIL_` opt-in that derives the flat CAIStateGroupBase. The layout is the same,
     since the first 0x14 bytes are the base.
   - (b) leave it.
   The same holds for 0x00968230 and CAIStateGroup_TavernKeeper.h (entry 2).
2. **0x00634470's extra pick-any COMDATs.** The sibling shape makes VC7.1 emit unreferenced 3-byte `scoped_ptr<T>::operator->` COMDATs
   (5 new: QuickAccessItems, ControlCreatureRightStick, CameraLookAround, CycleSpecialCameraModes, DebugControls). The function bytes,
   relocations and `.drectve` are unchanged, and the HEAD sibling 0x00633A60 already emits 15 such COMDATs. Is this acceptable as
   "stays a respell", or should the row wait?
3. **0x0073A9B0 self row ported in the same unit.** It was needed so 0x008E9190 keeps a resolved ctor reference. If an owner self port
   is out of scope for this batch (NF282 D2), land 0x00752640 alone, which has no regression (its old pointer spelling was unresolved at
   HEAD), and hold 0x008E9190 and 0x0073A9B0 together.
4. **`Clear` in 0x008E9190.** The local model's made-up inline name now sits under the real `CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>`
   class name. It is unreferenced and in no PDB. Accept it as debt, or should the binder spell the clear as the PDB `reset(0)` /
   ReleaseData in a later port?
5. **0x00752640's `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` define.** Without it, the respell would create a new same-name pick-any
   ReleaseData copy whose body differs from 0x00754F10's, and that body appears nowhere in retail. With it, the copy equals the owner's,
   and the function and every other COMDAT are unchanged. The define is row-local, not a header edit. The cost is that one
   unreferenced COMDAT body changes against HEAD. Accept it, or hold 0x00752640?

## Rule notes
- **Locks.** Every compiled row was locked before compiling: the 9 changed rows; the owners 0x0097EC00, 0x0073A9B0, 0x00863090,
  0x00754F10, 0x0073D180, 0x00A0D2D0, 0x00993C10 and 0x0084AF90; the sibling 0x00633A60; and the 37 witnesses in work/gr_b2.txt (two of which, 0x00B6D940 and 0x00846660, became changed rows).
  - Every landed test was prechecked before its lock (pre_tests1/2, pre_towner/2), and every source after its lock and before its grade
    (pre_srcH/H2/N/N2/N3/Owner/Owner2).
  - 0x00CFD6C0 failed its source precheck. It was released, never compiled.
- **Background runs.** Every grade and link ran in the background with `timeout 900` on its own launch line (gr.sh, lk.sh, rf.sh).
  equiv_check ran under eqrun.sh's `timeout 3000`; it is not a grade.
- **No forbidden operations:** no sleep, no foreground waits, no `rm` (clean.py only), no git, nothing written under the repo, no `__asm` /
  `naked` / `_emit`. No retail or donor bytes in hand-back files: pickany prints only equal/unequal and relocation targets, and the
  oracle stays in work/pay1.
- **orig/** holds the HEAD copy of every changed row plus orig/hdr/engine/CAIStateGroupBase.h. All still equal the repo at hand-back (cmp).
- **Tests:** unchanged, all standalone models (D2). No landed source or test includes a changed row.
- **Name census:** each new callee spelling is defined only by its own self row or by the header's existing decl-only lines. Exception
  (existing debt): 0x00A0D3D0 carries a same-TU `CAInputProcess::RemoveFromInterface` body under the PDB name (NAME_FIXES note).
- **Locks held at hand-back:** the 9 changed rows. The rest are released (see the final lock list in this hand-back).
- **Reading list.**
  - Read in full: the prefix, the standing brief, all of GOTCHAS.md, EXTRA_RULES.md, and NF294's NOTES, LESSONS, HDR_README,
    NAME_FIXES, RENAMES and TABLE_CHANGES.
  - NF294's STATUS.tsv: read cut to 400 characters per line. The four follow-up lines were read in full.
  - Not read: NF294's APPLIED.tsv, ASM_RULES, COORDINATION and the nf28x landing READMEs.
- **tools/:** NF294's set; cls.py and tri4.py bounded to the row's own block; new pickany.py (pick-any COMDAT v retail at the owner
  address), cmpcomdat.py (function COMDATs shared by two objects, by symbol) and mkstatus295.py.
