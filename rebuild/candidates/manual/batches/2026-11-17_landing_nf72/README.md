# 2026-11-17 landing nf72: name-fix round 7, NF72 (65 rows: follow-up entries queued at nf67-nf69)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF72 took the 51 follow-up entries queued at the nf67, nf68 and nf69 landings (q10859..q10909): 83 rows plus 2 it claimed. Brief,
entries, worklist and records: `naming/2026-11-17_agentNF72/`.

## Rows (STATUS.tsv, 85)

| Status | Rows |
|---|---|
| ported and landed | 65 |
| ported, held by the coordinator | 1: 0x006C4430 (below) |
| not-a-binder | 7 |
| blocked | 11 (0x00A79930, 0x00B42530, 0x00B41FA0, 0x00970CD0, and the seven q10884 binders) |
| deferred | 1: 0x00E12580 (held by NF71 at the time; NF71 has since landed it) |

**Among the landed rows:**
- **The STLport containers of the nf69 maps:** the four trees' `_M_erase`, `operator--` and member-template `find` (were
  `_unattributed::FreeTree_CTreeOwner_*`, `CRetailTreeIterator::Decrement`, `CRetailTree_longkey::find`), each in the module spelling
  of its landed sibling rows.
- **STLport helpers with made-up names** now under their library names: 0x009F6720 `std::_Destroy<CInputEvent*>` (was
  `_global::_Destroy_Record34`), 0x00AD7B50 / 0x00AD7120 / 0x00AD7B90 (COptimisedVertex uninitialized copy / copy_backward /
  uninitialized_fill_n), 0x00A84160 `std::sort<...>`, 0x004B8940 `~list<CIntelligentPointer<CThing> >`, 0x004406D0
  `~vector<CIntelligentPointer<CThing> >`.
- **PDB names:** 0x007BED60 `NThingShot::CTCShotDefinitionBase::IsAbleToBeDeflected` (was `_global::GetMember_byte0x68`),
  0x009D68B0 `CBankFileAsync::AsyncUpdatePending`, 0x009C80F0 / 0x009C8700 / 0x009C8220 NGameText's BeginLoading / BuildGroup /
  BeginLoadingGroup, 0x00B32300 `CEnginePrimitiveRenderer::InitialiseGlobalLights`, 0x00970590 `SetAtFailed`, and others.
- **0x008B9F20** CCreatureAction_TeleportIn::Initialise now uses the same text as its twin TeleportOut.
- **0x0047EEE4** is on engine/NGameText_CDataBank.h (CGroupEntry : CResource); its call 0x009C7980 binds NGameText::ConstructString
  (both PDBs) instead of the made-up SubstituteFileName. Seven other local models remain in that row (CAFile, CDateAndTime,
  CTCHeroStats, CMainGameComponent, CUserProfileManager, CThing, CTCBase).
- **0x00E13310** is `EndMission` in both PDBs (Ego_r 0x00E59960, FableWin 0x038E7910): WaspMenace is rejected.

**0x006C4430 is held:** the CTCFadeOutAndIn scalar deleting destructor stand-in reached the protected FadingThings by
`reinterpret_cast` at `this + 0x18`. The row stays as it was; the draft is in `held/`, and a friend opt-in or a real `??_G` TU is
queued (q10901 stays open).

## Entries (APPLIED.tsv, 51): 46 done, 1 done-already, 4 blocked
Blocked: q10878 (0x00A79930), q10881 (0x00B42530, 0x00B41FA0), q10884 (seven derived CCreatureAction destructor binders: retail's
are compiler-generated and VC7.1 drops their vtable reset, so each class needs an IMPLICIT_DTOR opt-in graded from a caller's TU),
q10886 (0x00970CD0). With 0x006C4430 held, q10901 is open again.

## Rows and checks
- **Section 1: 46 self renames (62 file moves). Section 2: 54 respelled bindings.** Module spellings checked against the sibling
  rows of each container instance.
- **Headers merged three-way:** NF72's overlays were copied before the assert pins (2034b9e969). Each was merged with
  `git merge-file` against NF72's own base copies (`hdr/orig/`, not landed). CBankFile.h conflicted only on its asserts: NF72 had
  pinned them to its copy's lines (266..), the repo to their 49b943df65 lines (246..); the repo's pins were kept.
  `lane_merge/assert_names_check.py` reports 0 mismatches against 49b943df65 after the merge.
- **`__forceinline`:** NF72's new STLport sort body uses `__forceinline` inside `#if defined(_MSC_VER)`, the guarded form the
  portability check tolerates. A coordinator attempt to respell it `FABLE_FORCEINLINE` broke 0x00A84160 (the macro is not defined at
  that point of fable_containers.h) and was reverted before landing; `logs/recheck_00a84160.log` is the regrade.
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`, `logs/recheck_00a84160.log`):** all 65 rows graded from the repo after landing: 54
  RELOCATION_MATCH and 11 MATCH, every test PASS. (0x00A84160 first showed SRC_FAIL from the reverted respelling.)
- **Checks:** reloc_bindings.tsv: 85 lines removed, 47 added; bindings_multi_check is at its baseline (`NullCoord`).

## Headers (8 opt-ins in 6 files)
CBankFile.h (PACKRUNTIMEDATA, ALIGN), CPackedUIntArray.h (PACK), NGameText_CDataBank.h (CGROUPENTRY_CTOR),
CAIStateGroup_CreatureFindCover.h (CREATUREFINDCOVER_UPDATE: the retail polymorphic class on CAIStateGroup_CreatureUseCoverBase),
CBankFileAsync.h (UPDATEPENDING_BODY), fable_containers.h (STLP_SORT_FASTCALL_BODY). The old made-up `CREATEENTRY` opt-in in
CBankFileAsync.h has no landed user left; three pending 0x009FDE80 candidates still define it (NF72 NOTES).

## Name fixes queued (`land1117fu`, 12)
NF72's NAME_FIXES.tsv (11) plus the held 0x006C4430.

## Rule slips (NF72)
- Nine rows were blocked without a grade (the six heavy q10884 binders, 0x00B42530, 0x00B41FA0, 0x00970CD0); the q10884 binders were
  handled out of queue order; 0x009D68B0 needed a fourth grade after three failed forms; 0x00CD28F4 was graded twice with the same form.
- One write to `/tmp/x` outside the work directory (deleted); two non-zero exits.
- 0x006C4430's offset cast (held, above). Three rows keep local typedef names for the shared std::vector / std::list (NOTES).
