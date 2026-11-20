# NF154 notes (round 10, PRIORITY CTCBase / CTCPhysicsBase / CTCLook / CTCCreatureNavigation, 24 entries)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 24 entries: 15 done, 7 done-already, 1 blocked (q11661, one definition:
coordinator decision), 1 deferred (q4045, held by NF153), 0 rejected, 0 open. 44 STATUS rows: 23 ported, 18 not-a-binder, 3 blocked
(0x00751E80 / 0x007525C0 one definition, drafts in work/; 0x007E00D0 NF153 lock, not touched).
recheck_final.log: one vland --recheck-landed run over the 23 ported rows. selfcheck: 0 problems.
Extra lock outside rows.txt: 0x008006D0 (claimed NF154 in nflocks; not in UNMATCHED_ROWS.tsv or COORDINATION_CLAIMS.tsv).

## Census (work/census_a.final.tsv, work/census_b.tsv)
- Every REFS.tsv row (373 unique rows, 387 compiles: 183 + 204, over 21 entry addresses; q11771 alone 204) was compiled from the working tree and its relocation at each
  entry address read with reloc_identity (tools/census.sh). All compiled MATCH / RELOCATION_MATCH.
- Wrong binders found = the WORKLIST rows plus 0x005DAB10 (PeekComponent, not in reloc_bindings) and 0x00752930 (ChangeEyeAngles QAE).
  Stale table witnesses: 0x007E27F0 for PeekWorldMap (it binds the PDB spelling; it was wrong only for AddToGlobalTCs) and 0x0057CB7E for
  GetDefinitionManager (binds the PDB spelling).
- After this hand-back every entry address is bound under one spelling by landed rows, except 0x00686D20 (0x007E00D0, NF153's lock).
  Exe scan of the self-renamed addresses: 0x00751820 {0x00752120, 0x00752190}, 0x00752190 {0x007528A0},
  0x00752200 {0x00752930, 0x00753040, 0x00753080}, 0x00753080 {0x00753C70}, 0x00752930 {0x007530D0}, 0x00800540 {0x008006D0},
  0x00723730 {} (vtables only). Each referencer is in this hand-back or already binds the new spelling (census / bindchk).

## q11661 blocked: one definition (coordinator decision)
- engine/CTCLook.h defines GetTurningAngles and GetTurningAnglesAndCheckForTurningBody in-class (private, noinline). The landed includer
  0x00752E00 (SetBodyMoving) calls the latter, so its object emits both ?GetTurningAngles@CTCLook@@AAE_NAAM0@Z and
  ?GetTurningAnglesAndCheckForTurningBody@CTCLook@@AAE_NAAM0@Z as pick-any COMDATs (llvm-readobj: Selection Any). The respelled self row
  0x00751E80 would define GetTurningAngles AAE NoDuplicates -> LNK2005 (nf146: pick-any vs nodup clashes in either order). While the self
  row is QAE there is no clash, so the pair (0x00751E80 + its only referencer 0x007525C0) is held back; both drafts grade
  RELOCATION_MATCH (work/00751e80_blocked.cpp, work/007525c0_blocked.cpp).
- Existing clash found the same way: the landed 0x007525C0 (nf120) already defines GetTurningAnglesAndCheckForTurningBody AAE
  NoDuplicates against 0x00752E00's pick-any copy. The nf120 landing README has no ruling on it.
- The other 14 CTCLook.h includers were compiled at HEAD: none defines a symbol this hand-back renames (only 0x00752E00 defines the two above).
- Options for the coordinator: the nf86 / nf88 pick-any scaffolding on the two self rows (only helps if every definer is pick-any), or
  taking the in-class bodies out of CTCLook.h (a header claim).

## Self renames (RENAMES section 1, 7)
- In-place access respells of the pre-existing local CTCLook models (0x00751820, 0x00752190, 0x00752200, 0x00753080, 0x00752930): both PDBs make each private. 0x00753080 (FrameUpdateOverriddenByParent) and 0x00752930 (SetEyesLooking) are outside the
  entries; they were in the q11664 family anyway, both PDBs agree, and their only callers end on the new spelling (0x00753C70 respelled
  here, 0x007530D0 already AAE), so no split is created and two existing ones close.
- 0x00723730: the stand-in CCreatureAction_TrollWhackGroundBase_Methods::Initialise is CTCPhysicsStandard::UpdateInterpolation (catalog
  already says so; vtable slot 94 proof in the row). Ported onto engine/CTCPhysicsStandard.h with the overlay opt-in. Its test is a
  standalone stand-in model with no class token (V / Action), left unchanged.
- 0x00800540: own signature to the PDB private const vector& form (two PDBs), needed to hand its route to CTCCreatureNavigation::
  CalculateRouteFromTo; the local CTCBase model was replaced by engine/CTCBase.h retail. Its only caller 0x008006D0 (extra lock, claimed
  NF154) follows; it keeps its CNavStepVector view and passes it with a reinterpret_cast to the vector& (the retail steps are 9-byte
  packed pairs; porting the vector itself would need the packed retail C2DVector and was not attempted).

## Header-opt-ins
- engine/CTCPhysicsStandard.h: FABLE_RETAIL_CTCPHYSICSSTANDARD_UPDATEINTERPOLATION (hdr/README.md). No other overlay.
- Existing opt-ins used: FABLE_RETAIL_CTCBASE_COMPONENT, FABLE_RETAIL_CTCWEAPON, FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR (0x005DAB10);
  FABLE_RETAIL_CTCCREATURENAVIGATION (0x0097F5B0, 0x00800540); FABLE_RETAIL_CTCBASE (0x00800540).

## Acceptance (objdump -t defined symbols, HEAD vs new; tools/symdiff.sh) and binding diff
- Unfiltered binds.py diff HEAD vs new for all 23 rows: the only changed pairs are exactly RENAMES section 2 (work/bh_*.txt / bn_*.txt).
- Only the row's own leaf changes, except: 0x00714730 (row-local CThing::GetInterface<T> forceinline COMDAT now instantiated on
  CTCCreatureNavigation instead of the CTCNavigator stand-in; unreferenced) and 0x00800540 (shared-header CThing::PeekTC<T> COMDAT on
  CTCCreatureNavigation instead of CTCNavigationBase). Every other row: no defined-symbol change.
- No local class model added; local stand-ins dropped: CTCBase (0x008FD050, 0x00800540), CTCType7, CTCNavigator, CTCWalk,
  CTCNavigationBase, CTCNavigation, CGameSubsystem.

## Descriptive names
- None new. Stand-in names removed: ResetNavigator (in no PDB), OnStateGroupRemoved, Reset (CTCType7 / CTCWalk), ResetNavigation,
  FindNearestWalkablePos, SetUpRoute, IsNavLineOfSight, AddToGlobalUpdateList, AddToGlobalUpdateTCs, GetGameSubsystem.

## Flags lines
- None added (0x0089B780 / 0x006CF920 keep their existing `// Flags: /GS`).

## Funclets
- No EH funclet row includes any changed row (grep of rebuild/src/compiled/01/21 for each parent file name).

## Seen in passing (not changed; NAME_FIXES.tsv)
- 0x00753C70's own leaf is public non-virtual (?FrameUpdate@CTCLook@@QAEXXZ); PDB UAEXXZ. It also carries a same-TU noinline copy of
  SetHeadTurnState under the public spelling (PDB AAEXJ), and declares UpdateHeadPos / ImplementHeadSettings / IsAbleToTurnDaBody /
  SetBodyMovingViaAnimation / UpdateLookTimer / IsLookTimeOver public where the PDBs make them private: a later CTCLook pass.
- 0x005DAB10's own leaf is the CTCWeapon_Getters stand-in (PDB ?IsUsableBy@CTCWeapon@@QBE_NABVCThing@@@Z).
- 0x0097F5B0's own leaf is public with a CThing pointer (PDB protected, const CThing&).
- CTCLook rows still model CTCLook locally although engine/CTCLook.h now exists (header debt, unchanged).

## Coordinator decisions asked
- 0x008006D0: the row keeps its CNavStepVector stand-in view and passes it to the PDB `vector&` parameter with a reinterpret_cast
  (porting the real STLport vector needs the packed retail C2DVector). Accept, or hold 0x00800540 + 0x008006D0 (q4533) together.
- 0x00714730: its row-local CThing model (a class with a shared header, pre-existing) now instantiates the forceinline
  CThing::GetInterface<CTCCreatureNavigation> COMDAT (was GetInterface<CTCNavigator>); unreferenced. Accept or hold.
- q11661 one definition (above), and the existing nf120 clash at 0x007525C0.

## Rule slips
- The one-definition check for q11661 was missed in the first pass (my GetTurningAngles grep could not match the header's
  GetTurningAnglesAndCheckForTurningBody caller); caught at the advisor review, the pair moved to work/ and the recheck rerun.
- Order: the CTCLook edits (q11660..q11664) and the q4260 / q4976 / q4722 drafts were written while the census held the lane, before
  q3906 was graded; grading then ran in entry order. No row was left in progress.
- 0x005DAB10 and 0x008FD050 / 0x00713740 each took 2 grades (first form C3861 / C2143); no row over 3 failed forms.
- grades_used in STATUS.tsv counts q.py grades + the two vland runs (b1 over 25 rows, b2 = recheck_final over 23); HEAD compiles for the census (387) and the symbol diff (26) are
  not counted per row.
