# 2026-11-17 landing nf59: name-fix round 7, NF59 (59 rows: follow-up entries queued at nf55-nf57)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF59 took the 53 follow-up entries queued at the nf55, nf56 and nf57 landings (NAME_FIXES_QUEUE.tsv q10497..q10551, less the
`pGlobalScriptDef` pair, which NF60 had): 96 rows (the entries' landed self rows, the binders of their addresses in
reloc_bindings.tsv as of 481314fbd4, and callers the evidence names). Brief, worklist and records: `naming/2026-11-17_agentNF59/`.

## Rows (96, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 59 (14 tests rewritten with them) |
| not-a-binder | 33 |
| blocked | 4 (unchanged) |

**Blocked rows:**
- **0x00467831** (q10530, 3 grades): needs an opt-in in stlport/fable_containers.h that force-inlines `__copy_trivial` / `fill_n`.
- **0x0053CB9B** (q10531): the map copy constructor (FableWin.pdb 0x021A7386); stlport/fable_map.h has no `_Rb_tree(const _Rb_tree&)`
  body yet. Its caller 0x0053CCA5 is ported and already binds the right name.
- **0x006A0150** (q10502): needs GetAnimationToUse, GetAnimationFromName 0x006924B0, the `CAnimationEntry(const CAnimationEntry&, long)`
  copy and the CCountedPointer deleter on shared headers first.
- **0x0064F6DC** (q10525): needs a DrawOracleSymbols declaration in engine/NPlayerGui_CDrawQuestInfo.h and its local sprite, engine
  and gui models moved onto shared headers.

## Entries (APPLIED.tsv, 53)
- **34 done**, among them q10506 (0x0095ED80 private, done at the nf56 landing) and q10523: 0x00745120 no longer defines its own
  `CTCPerceiveThing::GetFOV` (opt-in `FABLE_RETAIL_CTCPERCEIVETHING_GETFOV_INLINE`), so it and 0x007445D0 no longer both define it.
- **6 done-already.**
- **13 blocked:** q10500 and q10516 (no landed row to change); q10502, q10525, q10530 and q10531 (the blocked rows above); q10524,
  q10527, q10528, q10529 and q10532 (the callers still on the old name are outside the slice); q10543 (~CTCTargetingPlayer
  0x004E37BF still reaches 0x004E1AAF through a made-up member destructor); q10550 (0x007AE190, 0x008AE380, 0x01229D00, 0x01229D60
  still bind stand-ins). Every such caller has a NAME_FIXES line.

## Rows and checks
- **Self renames (section 1: 45 rows from NF59 + 1 coordinator row; 54 file moves).** Among them:
  - nine CTCInventoryItem statics that NF56 had left on stand-in names (GetUseButtonActionType, IsConfiscatable, GetInventoryCategory,
    GetPDef, GetItemToSelectUponRemoval, GetMaxNumberOfItems, IsSellable, IsBuyable, GetTutorialCategory);
  - `CTCQuickAccessMenu::IsQuitting` (was `GetMember_byte0x1c_00625940`), `CEngineLandscapeMap::OnGameMapLoaded` (was
    `Nop_ret0_00bdbfe0`), `CEngineScreenEffectRadialBlurRenderer::SetDefaultParams` (was `SetShadowClipVolumesToNeverShadow`);
  - libpk: the CBufferedRangeReader ctor and Reset, the SSampleIndexNode ctor, CAudioBankTableReader Load / LoadCriteria (were
    `AudioBankTableReader_00c3bb50` / `AudioCriteriaLoader_00c3bcb0`), CLockedRingBuffer::Pop;
  - the access / constness spellings of CTCTalk IsTalking / PlayAISpeech, CAnimateThing::IsAnimationFinished,
    CCreatureActionBase::GetFrameUpdateTime, CTCPhysicsBase::GetBoundingInfo, CTCContainer::AddItemToInventory,
    CTCContainerRewardHero::CreateNewObject, OpinionReactionRoot::SayToThingReactingTo and the CTCPhysicsControlled /
    CTCPhysicsStandard Move steps;
  - STLport instances: `vector<CIntelligentPointer<CThing> >::_M_clear` 0x004AC1D0 (was catalogued `~vector<CConversation>`),
    `__uninitialized_copy` / `__destroy_aux` instances, `~vector<NPlayerTargeting::CTargetPoint>` 0x004E1AAF,
    `map<CCharString, vector<CCharString> >::~map` 0x007AEB70 and `_M_erase` 0x007AE0E0, GFSerialiseVectorBinaryIn / Out `<bool>`,
    `??_GCEngineTextureMorph`, `CPersistContext::Transfer<CFaction>`; the CGameScriptInterface RemoveRumourCategory /
    RemoveGossipVillages / RemoveGossipFactions virtuals.
- **Coordinator corrections to section 1:**
  - 0x006689E0's name was `operator` (an old catalog truncation); it is now `CThingFilter_OverlappingSphere::operator()`.
  - 0x007AE0E0 (a leaf-only change) keeps its catalog name. 0x007AEB70's `~map` uses the same container spelling as its sibling
    `map()` row 0x007AEB30.
- **Coordinator row 0x01224C99 (q10542):** pGlobalScriptDef's `_$E1` was the placeholder `g_CStatic_0122ae7e_0122ae7e` over a local
  `CStatic_0122ae7e`. It is now NF60's trial: the definition `CDefPointer<const CScriptDef> pGlobalScriptDef;` from
  engine/fable_script_def_global.h, the same as its atexit-destructor row 0x0122AE7E. It is catalogued
  `` `dynamic initializer for 'pGlobalScriptDef'' `` (Ego_r.pdb 0x0101B28A / FableWin.pdb 0x0400C45A). Rechecked: RELOCATION_MATCH,
  PASS.
- **NF59's judgement calls, accepted:**
  - `~map` rather than `~_Rb_tree` for 0x007AEB70: the bytes are identical and nothing calls it, but it sits right after the same
    map's uncalled `map()`, the layout of the landed `map()` / `~map` pair 0x006F2490 / 0x006F24D0.
  - `vector<bool>::resize` 0x00467686 keeps `const bool&`: retail pushes an address, though Ego_r.pdb passes by value.
  - Non-retail helpers `DeleteEngineTextureMorph` (0x005BBBCE) and `DestroyMapOfCCharStringVectorCCharString` (0x007AEB70) exist only
    to make the compiler emit the destructor.
- **Funclets:** the CAudioBankTableReader renames moved the parents of funclets 0x01214E60 / 0x01214E75. The coordinator repointed
  their includes (and comment lines). All three funclets that include a changed parent (those two and 0x01215C90) were rechecked;
  funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** 63 graded from the repo after landing: the 59 rows, 0x01224C99 and the 3 funclets.
  55 RELOCATION_MATCH, 1 RELOCATION_MATCH[/GS] and 7 MATCH, every test PASS. NF59's own final run agrees.
- **Checks:**
  - `naming/rebind.py` removed 121 binding lines and added 67. It warned on 0x008E60D0 (a catalog leaf it can not match) and took the
    largest function, the one vland graded;
  - bindings_multi_check is at its baseline (`NullCoord`).
- **To check at link time:** the `TransferBinaryIn<bool>` copy inside 0x004675F6 is a COMDAT. If 0x00403EB0 is later renamed to the
  same symbol, the link should be checked.

## Headers (13 opt-ins in 8 files; lines added only, default path unchanged)
CFaction.h (`CTOR_OUTOFLINE`), CThingFilter_OverlappingSphere.h, CTCPerceiveThing.h (`GETFOV_INLINE`), CTCTalk.h, CTCPhysicsBase.h,
CAnimateThing.h (2), CTCInventoryItem.h (6) and libpk/CCodaFrameStreamer.h. Details: `naming/2026-11-17_agentNF59/hdr/README.md`.

## Name fixes queued (`land1117fu`, 23)
From NF59's NAME_FIXES.tsv: the callers outside its rows that still bind spellings it proved wrong (the q10524 / q10527 / q10528 /
q10529 / q10532 / q10543 / q10550 callers above, and others its renames left behind).

## Rule slips (NF59 `slips.txt`)
- Two read-only Bash calls exited non-zero.
- One grade was wasted when 0x004675C5's final file lost its pragma line; 0x008E57D0's final file was truncated during assembly and
  rebuilt from the original.
- A status helper was called with an extra argument for 13 rows and overwrote their notes (rewritten); a rename helper failed silently
  on an unquoted `<bool>` (rerun).
- The rewritten tests of 0x004DBE58 / 0x004E1AAF first printed their own pass strings instead of the catalog's; fixed and rerun.
