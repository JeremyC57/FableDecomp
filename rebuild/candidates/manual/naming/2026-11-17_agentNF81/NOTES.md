# NF81 notes (round 7: q10951 and the land1117fu nf78 follow-ups q10975..q10988)

Every count below comes from STATUS.tsv, APPLIED.tsv and recheck_final.log.

## Rows (STATUS.tsv: 27)
- **Totals:** 25 ported, 2 not-a-binder, 0 blocked, 0 deferred.
- **Which rows:** the 23 rows of rows.txt, plus four rows I claimed through nflocks:
  - 0x008C1600, for q10988;
  - 0x004E81E4 and 0x004EBB1C, for q10975 (the real wrong binder of 0x004E7720 and its caller);
  - 0x006DB060, for q10986. After the 0x006DC350 port it had to define the PDB spelling that row now binds, or a new link split would
    open.
- **recheck_final.log:** one vland run over all 25 changed rows, rerun after the review fixes: 3 MATCH and 22 RELOCATION_MATCH, every
  test PASS.
- **Not-a-binder (2):** each was graded once unmodified (RM + PASS).
  - 0x004E7720: the q10975 self row already defines the entry spelling.
  - 0x0059899A: the q10983 self row already defines the protected LoadProfile.
- **Grades:** 63 over 27 rows, the sum of STATUS grades_used (q.py grades plus the vland runs each row took part in).
  - grades.log lists 40 single grades and 13 vland runs.
  - The single grades include verification compiles: the old 0x0063AA50 and 0x006DC350 for RENAMES, 0x006DC970 / 0x00869900 for
    NAME_FIXES, and 0x004E81E4 / 0x009EDC50 to compare COMDATs.

## Entries (APPLIED.tsv: 15)
- **Done (14):** q10951, q10975, q10976, q10977, q10978, q10979, q10980, q10982, q10983, q10984, q10985, q10986, q10987, q10988.
- **Blocked (1): q10981**, 0x0059A238's own leaf, as blocked/port. That row's binder respells for q10982 / q10983 are done, so the row
  itself is `ported`.
  - **Grade 1 (draft in work/0059a238_virtual_draft.cpp):** I put the PDB virtual non-const Action into the row-local model. It gave
    DIFFER 1486 v 1486 with every member 4 bytes higher.
  - **Why:** MSVC pads a class's own vfptr to the class alignment, and this class is 8-aligned by its double. Retail's vptr is the
    CObserver base subobject, so retail's data starts at +4.
  - **What it needs:** the port onto engine/CFrontEndManager.h (`FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR`, with the CObserver base
    from engine/NUISystem_CComponent.h).
  - **What must move first:** the row models about 15 classes locally: CCharString, CWideString, the std less / pair / allocator /
    map / list / _List_node, CFileInstaller, CUserProfileManager, CTBaseSingleton, NGameText::CDataBank, CNewFrontendGameComponent,
    CVirtualKeyboard, FRONTEND_SCREEN, CActionParamString, CAction, CComponent, CScreenStack and CFrontEndManager. It also has a
    same-TU map::operator[] stand-in body.
  - **A second fault:** the row includes engine/CCountedPointer.h inside `namespace NUISystem`, so its leaf spells
    `?$CCountedPointer@...@2@`.

## The CMap theme group (q10951, q10976..q10980, q10987)
- **0x0081EE60 and its three siblings were all DIFFER unmodified.**
  - 0x0081EE60 was DIFFER 174 v 207 (NF78).
  - The siblings were DIFFER 144v138, 145v147 and 143v143 (vl_base_cmap.log). The July rows (world_map_theme.h with local
    storage casts) never matched.
- **All four are re-authored on engine/CMap.h** (the CopyPos / LoadFromFile define blocks). Each matched on its first grade, using the
  Ego_d parameter and block-local names: theme_palette_index and gpos, with `PaintType == 0` then `== 1`.
  - SetEngineThemeAt repeats the IncludingExtraCells text; it does not call 0x0081EF30. Ego_d gives it its own theme_palette_index.
  - GetEngineBlendAt calls CGameMapCell::GetAtmosBlend 0x006B74E0 out of line. That callee is new: the July row had no relocation
    there, so there is no Section 2 line for it.
- **The CWorldMap callers moved onto engine/CWorldMap.h:** 0x0063A970, 0x0063A9E0, 0x0063AA50 and 0x0063AAB0.
  - This is the 0x0063A7E0 recipe. With it, their own leaves become the PDB ABVC2DCoordI (the July header's struct gave ABU).
  - 0x0063A970 is q10976. The other three get the same leaf fix as a side effect of their binder port (RENAMES s1, both PDBs).
  - NF78's held 0x0063A970 was regraded first, as it was (RM + PASS, vl_cmap1.log), before the port.
- **0x0063EB20** (DestroyThemeAt) was a binder of q10977 that WORKLIST missed: callers.py found its call at 0x0063EC62.
  - Its local CMap model and its local EThemePaintType were replaced by engine/CMap.h. Its THEME_PAINT_NORMAL became THEME_PAINT_ENGINE
    (value 0).
  - The other local models in that row are unchanged.
- **Tests.** The CMap and CWorldMap tests link the row object, so all eight were rewritten minimally on the legacy header:
  - each defines the landed `FABLE_RETAIL_CMAP_ENGINETHEME_PROTECTED`;
  - the CMap tests call the protected member through a test-only derived class with a using-declaration (`CMapTestAccess`);
  - the CWorldMap tests also define the NF81 opt-in WORLDMAPTHEME_C2DCOORDI_SHARED (engine/C2DCoordI.h, `.x/.y` -> `.X/.Y`);
  - the 0x0081F170 test's GetAtmosBlend stub now takes `long` (Ego_r QBEEJ).
  Each PassPattern is kept.
- **world_map_theme.h** is now used by no landed row; its only users are tests. Four stale scratch files,
  `rebuild/src/compiled/00/81/0081ee60.pre.cpp`, `0081efc0.pre.cpp`, `0081f090.pre.cpp` and `0081f170.pre.cpp`, still include it.

## q10988 (0x008C2550 / 0x008C1600)
- **Grade 1, declaration only:** DIFFER, frame 0x178 v 0x16C. Retail sees the body; NF74's Shape note says the same.
- **The body moved to a new overlay header**, `engine/CCombatAction_StrikeResponseJackOfBladesIntoRush_GetValidRushPosition.h`. It
  holds the landed 0x008C1600 text, unchanged. The header follows the CMap GetStrongestEngineThemeAt BODY pattern: a visible
  `inline __declspec(noinline)` COMDAT, which becomes the out-of-line definition under _DEFINE.
- **0x008C2550** includes the header and calls 0x008C1600 (RM on grade 2). With FABLE_RETAIL_C3DVECTOR_PACKED added on grade 3, its
  COMDAT is byte-identical to 0x008C1600's function (761 bytes; compared section to section).
- **0x008C1600** defines the function from that header (RM). So the address has one name and one text.
- **The older copy is gone.** It was the rejected CVectorFilter_FurtherFromPointThan phase filter without PACKED.

## q10975 / q10985 / q10986 (STLport trees)
- **Explicit instantiations on stlport/fable_map.h.** Each matched on grade 1:
  - 0x004E6E0C, 0x00450903: `_M_erase` (the 0x005BA78B recipe);
  - 0x0045128D: `clear`, size TU (the 0x004E7720 recipe);
  - 0x004E81E4: `~_Rb_tree`, size TU, CLEAR_OUTOFLINE;
  - 0x006DC020: `~_Rb_tree`, speed TU, MERASE_NOINLINE.
- **0x004EBB1C** (~CHeroExperienceDef): only the ComboMultiplierMap stand-in was swapped for the shared std::map. This is NF78's
  0x004F914A swap, with TREE_CTOR_DTOR_OUTOFLINE. VC7.1 then binds the member's destruction to the tree destructor 0x004E81E4, which
  backs that name. The other stand-ins in that row are unchanged.
- **0x00868A40** (ClearAllDamage) is on engine/CHitParameters.h with the NF81 opt-in CLEARALLDAMAGE. It took 2 compiles: SRC_FAIL,
  then RM.
- **0x006DC350** (GFSerialiseMapBinaryOut) was ported off its local std / traits / stream models and its renaming-macro include. It
  uses the NF81 CPersistTraits<CComboMultiplierData> opt-in.
  - Grade 1 was DIFFER 273v244: the _M_erase body was expanded one level. Grade 2 was RM with TREE_MERASE_NOINLINE.
  - Its own leaf is now the PDB spelling that its landed caller 0x006D8F10 already bound.
  - Its traits callee 0x006DB060 still defines a stand-in leaf (NAME_FIXES.tsv).
- **Not touched:** 0x00869900 (`_global::RbTree_Clear_00869900`, a 51-byte speed-TU clear outside the slice) also calls 0x00450903
  through a stand-in.
  - It must not take the tree `clear` name: 0x0045128D holds that name, and retail has no identical-code folding.
  - It is probably `map<EDamageAttribute,float>::clear` (with the tree clear inlined), but that is unproven. It was not graded.

## Descriptive names (by library or made up)
- **Descriptive by library (STLport; both PDBs are Dinkumware builds):**
  - 0x004E6E0C / 0x00450903 `_Rb_tree::_M_erase`;
  - 0x0045128D `_Rb_tree::clear`;
  - 0x004E81E4 / 0x006DC020 `_Rb_tree::~_Rb_tree`;
  - NAME_FIXES 0x004E3625 `pair<const long, CComboMultiplierData>` scalar deleting destructor.
- **Map versus tree destructor (0x004E81E4, 0x006DC020):** the bytes cannot tell whether VC7.1 emitted the map's implicit destructor
  or the tree's own. I used the tree destructor:
  - it is the function that holds the text;
  - the 0x004EBB1C port binds it so;
  - the landed 0x0056D3D0 / 0x006F1BA0 and the size-TU set twin 0x00451B07 (bound by 0x00465C54) use the same name.
  0x006DC020 is not referenced anywhere in Fable.exe.
- **Made up:** the EThemePaintType enumerators `THEME_PAINT_ENGINE` (0: the height map's engine theme layers) and `THEME_PAINT_ATMOS`
  (1: the game map's atmos themes). Neither PDB name set lists them, and Ego_d was read only through egod_locals.py. The coordinator
  should unify them with the legacy spellings `EThemePaintType_Default` / `_Replace` (world_map_theme.h) and the former
  `THEME_PAINT_NORMAL` (0x0063EB20).
- **CIsOutsideRadius** (moved with the 0x008C1600 body) remains that landed row's descriptive predicate.

## For the coordinator
- **File moves (self renames that change the catalog name):**
  - 0x004E81E4 `std::list<T_004e81e4>::~list` -> the CComboMultiplierData `_Rb_tree::~_Rb_tree`;
  - 0x009EDC50 `_global::DeleteViaVtable0_009edc50` -> `CCountedPointer<CConsoleInputBase>::DeleteData`;
  - 0x009EE540 `CCountedPointer_009edc50::Reset` -> `CCountedPointer<CConsoleInputBase>::Reset`;
  - 0x004E6E0C / 0x00450903 `LTextBinTree<...>::BuildTreeArray` -> `_Rb_tree::_M_erase`;
  - 0x006DC020 `CRetailTree_erase00450903::~...` -> `_Rb_tree::~_Rb_tree`;
  - 0x0045128D `_global::RbTree_Clear_0045128d` -> `_Rb_tree::clear`.
  The other section-1 lines change only the leaf. No Unwind funclet includes any changed row: I grepped every changed row's file name
  over rebuild/src/compiled. The only hit was the legacy CFrontEndManager_Action_0059a238_match.cpp companion, which is not a funclet.
- **Tests kept on the old stand-in names** (standalone models that do not link the row):
  - 0x004E81E4 (`list_T_004e81e4`);
  - 0x0045128D, 0x006DC020, 0x00450903, 0x004E6E0C (their old local node models);
  - 0x009EE540.
  Only the 0x009EDC50 test was rewritten onto the real name.
- **0x00599D5C, pre-existing and unchanged:** its same-TU stand-in body of GetProfileNames (0x005955AB) is public QAE. Both PDBs say
  protected IAE, and the stand-in body defines that retail name with a fake text.
- **RENAMES:** section 1 has 18 lines and Section 2 has 23. The old spellings come from the landed sources and reloc_bindings, and were
  checked by compiling where they were not literal:
  - old 0x0063AA50 (ABU / QBE): one verification grade;
  - old 0x006DC350 leaf: one verification grade.

## Review fixes (after a self-review, before hand-back)
- **0x006DB060** was claimed and ported (RM, grade 1). Without it, the 0x006DC350 port would have opened a link split: the caller bound
  the PDB writer spelling while the self row defined the stand-in.
- **0x009EE540** now models CConsoleInputBase with its virtual destructor. Its DeleteData COMDAT is then byte-identical to the
  0x009EDC50 row (11 bytes). A forward declaration had given an incomplete-type delete under the same name.
- **0x004EBB1C** got TREE_CLEAR_OUTOFLINE. Its ~_Rb_tree COMDAT is then byte-identical to the 0x004E81E4 row (23 bytes).
- **Caller enumeration** (callers.py on every self address, absrefs.py on the address-taken 0x009EDC50):
  - Two landed binders outside the slice were found. They bind the q10985 / q10986 _M_erase under the stand-ins
    `?Detach_004e6e0c@CTCCarryable@@QAEXI@Z` (0x006DC970) and `?Detach_00450903@CTCCarryable@@QAEXI@Z` (0x00869900).
  - Each was graded unmodified once (RM). Both define the same own symbol `?CTCCarryable_OnKill@@YIXPAVCTCCarryable@@@Z`.
  - They are recorded in NAME_FIXES.tsv and not changed: their own names are unproven, and the tree clear names belong to
    0x004E7720 / 0x0045128D.
  - The LoadProfile binders 0x00598A1C and 0x00599E3F (NF79's lock) already bind IAE.
  - 0x009EDC50 has one more absolute reference at 0x009EE4D6, in no landed row.

## Deviation for the coordinator to rule on
- **q10988:** the brief says 0x008C2550 "must call, not define" 0x008C1600. Its call does bind 0x008C1600, but its object still emits
  GetValidRushPosition as an `inline __declspec(noinline)` COMDAT. That COMDAT is byte-identical to the 0x008C1600 function and comes
  from the one shared text.
  - A declaration-only form is DIFFER (frame 0x178 v 0x16C): retail saw the body in that TU.
  - This is the engine/CMap.h GetStrongestEngineThemeAt pattern. If the coordinator rejects it, hold 0x008C2550 and keep the landed
    form.

## Rule slips
- Two Bash calls exited non-zero:
  - a Python test-writer helper hit a TypeError, which I fixed and reran for the one file it missed;
  - callers.py was run after a `cd` into my own directory, so its relative repo path failed. I reran it correctly.
- 0x0059A238 ends `ported` for its binder respells while its own entry q10981 is blocked. APPLIED.tsv carries the block through
  overrides.tsv.
- q10981 was blocked after a single graded form, without the port onto engine/CFrontEndManager.h being attempted. The row models about
  15 classes locally; the brief allows blocked/port with the classes named.
- No git, no writes under /home/user/FableDecomp, no asm, no address casts in rows. The tests keep their pre-existing object-buffer
  casts.
