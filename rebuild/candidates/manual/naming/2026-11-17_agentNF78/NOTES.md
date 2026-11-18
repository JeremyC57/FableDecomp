# NF78 notes (round 7: land1117fu q10945..q10955 and eight NF70 blocked ports)

The counts below come from STATUS.tsv, APPLIED.tsv and recheck_final.log.

## Rows (STATUS.tsv: 26)
- **Totals:** 17 ported, 5 not-a-binder, 4 blocked, 0 deferred. That is the 25 rows of rows.txt plus 0x0059A238, which I claimed through
  claim.sh / nflocks for q10954.
- **recheck_final.log:** one vland run over all 17 ported rows: 17 RELOCATION_MATCH, 17 PASS.
- **Not-a-binder (5), each graded unmodified once (RM):**
  - 0x008AC060, 0x004F9660, 0x009F5170 and 0x00429288 are self rows that already define the entry spelling.
  - 0x00570332 already binds ReleaseData. Its reloc_bindings.tsv destructor line is stale.
- **Blocked (4):**
  - 0x0081EE60 is DIFFER(174v207) unmodified in this lane. It is a legacy July row (candidates/world_map_theme.h with local storage
    casts), so a respell cannot be shown to match.
  - 0x00EDC720 had 4 grades. The two-view-CScriptThing residue is in STATUS.
  - 0x008E7E70 and 0x00A37300 were triaged with 1 grade each (a vland baseline, RM + PASS). STATUS names the header work each needs
    first.

## Entries (APPLIED.tsv: 11)
- **Done (8):** q10945, q10946, q10947, q10948, q10949, q10950, q10952, q10954.
- **Done-already (2):** q10953 and q10955 (the same rows).
- **Blocked (1):** q10951 (0x0081EE60). Its worklist binder 0x0063A970 is ported: it binds SetEngineThemeAt as protected.

## Ports (BLOCKED.tsv: 8)
- **Ported (5):**
  - 0x007ADBF0 GFSerialiseMapBinaryIn<bool,CCharString>.
  - 0x00ADD020 list<CVertexGroupBlock>::operator=.
  - 0x009EC5E0 CConsole::AddInput.
  - 0x00D8F6D0 CHobbeLarder::Main and 0x00E5F2B0 CHenchmanComment::Main: the q10795 pDSTGame binder.
- **Blocked (3):** 0x00EDC720, 0x008E7E70, 0x00A37300.
- **Port recipes:**
  - The two GFSerialiseMapBinaryIn rows (0x006DCFA0 and 0x007ADBF0) use the shared engine/CPersistTraits.h body.
  - The two entity-script rows use the shared engine/CGameScriptInterfaceBase.h. This is the 0x00CBE716 / 0x00D9E930 / 0x00D67270
    recipe; it took one grade each. The 845-slot GameCalls copy and the 654-slot CThingManager_Calls copy are deleted.
    0x00E5F2B0's "thing manager" slot 653 is CGameScriptInterfaceBase::GetWorldFrame.

## Renames
RENAMES section 1 has 10 lines (9 renamed rows plus one catalog-only line for 0x00812170). Section 2 has 34 caller respellings.

**File moves.** Self renames that change the catalog name move these files; the coordinator renames the files:
- 0x007C0FB0 AddHitThing -> NThingShot::CTCShotDefinitionBase::AddThingToAvoid;
- 0x004F97B3 NPairLongMapTree::_Rb_tree -> std::map<EDamageAttribute, float, ...>::map;
- 0x004E7720 RbTree_Clear_004e7720 -> the CComboMultiplierData _Rb_tree::clear;
- 0x00812170: catalog-only.

**Funclets:** a grep of every ported row's file name (all 17, rerun after the five ports) over rebuild/src/compiled, excluding the row's
own file, found no Unwind funclet that includes one. The only earlier hit was CFrontEndManager_Action_0059a238_match.cpp, a legacy
companion file, not a funclet.

**Section 2 checks:** every 0x009EC5E0 and 0x006DCFA0 line was re-read from a fresh compile of the handed-back row (xref2.py over the
row object; one verification grade each): all ten new spellings resolve to the stated retail address. The old 0x00450903 spelling is the
landed 0x006DCFA0 row's own local `std::map<K,V,C>::_M_erase(_Rb_tree_node_base*)` model (the same string reloc_bindings.tsv:15203 holds
for 0x006DC350). The 0x008C1600 -> 0x00692940 line is `?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ` (fixed from a CCombatActionBase
typo; the grade-4 xref is the source).

**Tests:**
- One test was rewritten: rows/0063a970_test.cpp. It now defines the same access opt-in, so its CMap stub links. The first recheck gave
  LINK_FAIL; after the rewrite it gave PASS.
- Every other test is a standalone model and was kept unchanged; each passes.
- The 0x004E7720 test keeps its AutoTiny PassPattern.

## For the coordinator (holds and splits)
- **0x0063A970 / 0x0081EE60 split:** the ported 0x0063A970 now binds 0x0081EE60 as `?SetEngineThemeAt@CMap@@IAE...` (both PDBs), while
  the landed self row 0x0081EE60 still defines the `QAE` spelling (reloc_bindings.tsv:2184) and is DIFFER(174v207) here (q10951 blocked).
  Landing 0x0063A970 alone opens a link split; hold it with q10951, or land it with a 0x0081EE60 respell once that row is repaired.
- **0x008C2550 second text:** the landed 0x008C2550 still carries its own copy of GetValidRushPosition (the rejected
  CVectorFilter_FurtherFromPointThan spelling at 0x008C34A0, no FABLE_RETAIL_C3DVECTOR_PACKED). The matched text is rows/008c1600.cpp
  (RM + PASS); fold 0x008C2550's copy onto it, or drop the copy there, so one address has one name and one body.
- **0x008C1600 Shape (FITTED) locals:** world, creature, search, last, me, distance, too_close and self are the landed row's Shape levers,
  carried over to reach RM (grades 1-4: 744, 760, 761 DIFFER, then RM). They are not Ego_d names. p_jack_def, ptc_boss, validPositions,
  ptc_nav, found, random_index and rush_pos are the Ego_d names (NF74's 0x008C2550 text).

## Descriptive names
These names are descriptive, either by library or made up.
- **Descriptive by library (STLport; both PDBs are Dinkumware builds):**
  - 0x004F97B3 `map<EDamageAttribute,float>` copy constructor;
  - 0x004E7720 `_Rb_tree<long, pair<const long, CComboMultiplierData>>::clear`;
  - the _M_erase COMDATs 0x004E6E0C and 0x00450903, which are in NAME_FIXES.tsv.
- **CIsOutsideRadius:** 0x008C1600 binds remove_if 0x008C34A0 under this name, which is that landed row's own descriptive
  predicate. NF74 rejected CVectorFilter_FurtherFromPointThan for that call, because the XY and 3D operator() bodies conflict.
- **Left unchanged in the ported rows:** pre-existing local stand-ins outside these entries. In 0x00E5F2B0: Compare_00411570, the local
  CCharString and the vector specialisation. In 0x00D8F6D0: the local CScriptThing / CQ_HobbeCaveScript / CEntityScriptBase models.
  They are not renamed. They are only noted here.

## Evidence (two sources each)
- **q10945:** Ego_r.pdb and FableWin.pdb both list ?GetValidRushPosition@...@@IAE_NAAVC3DVector@@@Z. The landed 0x008C2550 binds it.
- **q10946:** the unary_negate spelling is the landed 0x00971ED0 binding and NF74's FinishAction declaration. The 0x008AC060 self row
  matches it.
- **q10947:** both PDBs list AddThingToAvoid, and Ego_d.pdb gives the parameter `thing` and the local `ip`. Landed 0x008F3630 binds it.
- **q10948:** the _M_copy callee is the EDamageAttribute tree (catalog). The 0x006DCB20 and 0x004F9784 bindings agree.
- **q10949:** the operator= caller 0x004F9660 binds it, and the other caller 0x004E81E4 is the member map destructor (from
  ~CHeroExperienceDef).
- **q10950:** NF75's self row. The 0x004F914A binder respell is RM.
- **q10951:** both PDBs list SetEngineThemeAt and AddThemeDefIndexToPalette as IAE. engine/CMap.h CWORLDMAP_FRIEND agrees.
- **q10952:** both PDBs list QBEKXZ.
- **q10954:** both PDBs list GotoProfileMenu as IAE. DoPressStart is IAE in both PDBs too, so it moved with GotoProfileMenu (its only
  caller is 0x0059A238, which I hold).

## Grades
47 grades over 26 rows (the sum of STATUS.tsv grades_used; q.py grades and vland baselines), plus the vland test batches and the final
recheck (grades.log has every compile).

## Rule slips
- One Bash call exited 1: callers.py was run before `cd` into the repo, and the relative path failed. It was re-run correctly.
- The 0x00EDC720 grade-4 draft used `#pragma inline_depth(1)` as an inline-budget probe (EXTRA_RULES preference 3). It was not handed
  back; the drafts stay in work/.
- 0x008E7E70 and 0x00A37300 end blocked with a vland baseline grade only. The brief asks for a port form with its error or diff; none was
  graded. For 0x008E7E70 the first gap needs no compile to show: retail tests `RuntimeType == 1` at CThing +0x10 before SuspendAI, and
  no shared header names that byte (engine/CThing.h has UniqueID then the UsingComponents bitset there), so a draft would have to invent
  an accessor. 0x00A37300 needs the CClassFactory / CClass list-iterator header moves listed in STATUS first.
- 0x00EDC720: the `/d2inlT` bisect with IMPLICIT_COPY (the one NF74 lever not tried) was not run; the row stays blocked on four grades.
- No git, and no writes under /home/user/FableDecomp.
