# 2026-11-17 landing nf56: name-fix round 7, NF56 (62 rows: the follow-up queue entries with exact symbols)

Owner: "Continue the naming fix queue with two agents until complete" and "Keep going until queue is done".

NF56 took the 61 follow-up entries (`land1116fu` / `land1117fu` in NAME_FIXES_QUEUE.tsv) whose correct symbol is an exact
decoration: 95 rows (43 binder rows from reloc_bindings.tsv as of daed7abdfd, plus the entries' landed self rows). The other 33
follow-up entries went to NF57 (32) and NF58 (the STLport stand-in family). Brief, worklist and records:
`naming/2026-11-17_agentNF56/`.

**How it was landed.** An interrupt stopped NF56 after every row and entry had a terminal status and its final recheck had run,
but before it rewrote NOTES.md, wrote LESSONS.md and reported. The coordinator landed it from its files: STATUS.tsv, APPLIED.tsv,
RENAMES.tsv, NAME_FIXES.tsv, hdr/ and recheck_final.log (63 graded, all PASS). NOTES.md is the in-progress stub; `slips.txt` is
its slip log.

## Rows (95, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 62 (+3 rewritten tests: 0x005D96F0, 0x00A0AA80, 0x00B3B8C0) |
| not-a-binder | 6 (0x004A6AB0, 0x004FB470, 0x00506BB0, 0x00A0AAA0, 0x00BF2E50: self rows already right; 0x009378F0: worklist artefact) |
| blocked/port | 27 (unchanged; each with its probe grade and the local model that must move first) |

- The landed tests of 0x00A0AA80 and 0x00B3B8C0 drove the old stand-ins and no longer linked; NF56 rewrote them as model tests and
  kept their PassPattern.

## Entries (APPLIED.tsv, 61)
- **35 done, 3 done-already** (q10410, q10413, q10473: the self rows were already right).
- **23 blocked**, each naming the rows left (APPLIED.tsv). Most are a ported self row with a binder that is blocked/port.
- **1 claim rejected and applied under the proven spelling (REJECTED.tsv):** q10468's `CRetailList_node0x10::Buynode` is a
  made-up owner. 0x005D84D1 is the STLport `list<pair<CWideString, long> >::_M_create_node`, and its caller 0x005D8626 is that
  list's `insert`. Both are re-authored and catalogued so, which settles the catalog name the nf54 landing kept for this row.

## Rows and checks
- **Self renames (section 1, 47 rows; 62 file moves for sources and tests).** Among them:
  - CDisplayEngine::ReceiveFeedbackGameEvent and CWorld::ReceiveLocalEvent (were the `*_Methods` stand-ins);
  - `~CWorld` 0x004A6AB0 (was catalogued `NHeroInformationScreens::CBase::~CBase`);
  - `CPersistContext::TransferVector<bool>`, `vector<bool>::push_back`, `vector<map<unsigned long, NUISystem::CUIState> >::push_back`,
    `vector<CIntelligentPointer<CThing> >::reserve`, `__destroy_aux<CEngineTextureMorph*>`;
  - `list<pair<CWideString, long> >` `_M_create_node` / `insert` (0x005D84D1 / 0x005D8626);
  - the `_Rb_tree` instances `find` 0x00696680 (RegisteredActions), `_M_insert` 0x0069D520 and the iterator `operator--` 0x0069CA00
    (the NCameraModes map);
  - CCountedPointer<CAFile> DeleteData / Reset and CCountedPointer DeleteData for C3DSkeletalMorphParameter / CVertexBufferWin32
    (were `DeleteViaVtable0_*` / `gapscan::sub_*`);
  - CTCInventoryItem GetUIInventoryCategory / GetInventoryType, NPlayerGui::CDrawNPCName::ShowScreenshotMessage (was
    `CAIStateGroup_BuyFromSeller::BoughtSomething`), both `CTCTalk::PlaySpeech` overloads, CTCStealth::IsAvailableForAttack;
  - the anonymous-namespace strike-event helpers `IsValidStrikeEventName`, `CGetMatchingStrikeEndEventName::operator()` and its
    `for_each` instance;
  - `CAIStateGroup_WorkBase<CAIStateGroupBase>::StayInShop` (was IsInWorkHours), the cover IsRunnableLeaf / RefreshCoverList
    members and the four private OpinionReactionRoot `Wife*Update` members;
  - CRenderManagerCore SetMaterial / SetAWindow, CParticle::SetPrimitiveInfo, CEngine::PeekVideoOptionsDef (was
    `CTextureManager::GetPoolTextureSize`), CEngineWorldMap::OnGameMapLoaded, CEngineScreenEffectDisplacementRenderer::
    FinishDisplacementStencilRendering, CEngineOcclusionManager::AddFlareTest;
  - libpk: `??_GCSegmentList`, the PKCAutoFile ctor 0x00C31D80 (was `__EH_epilog3`), CBufferedRangeReader::SetRange.
- **Descriptive names (no PDB names them; NF56 hdr/README.md):** `CWorld::GetFramesPerSecond` (inline only),
  `CTCInventoryItem::s_CachedItemDef`, libpk `PKCBufferedReader::GetSize` / `Seek` and `CBufferedRangeReader::Reset`. Each is the
  spelling the landed binders already use.
- **Coordinator fix, 0x0095ED80:** NF55 landed `CAIStateGroup_OpinionReactionRoot::WaitForTextToLoadThenSpeak` as public (QAE)
  only because its caller 0x00982F70 was NF56's row. NF56 moved that caller to the private PDB spelling (AAE, Ego_r.pdb 0x00983300
  + FableWin.pdb 0x02C28520), so the coordinator made the row's declaration private in the same landing. Regraded:
  RELOCATION_MATCH, PASS (`logs/recheck_0095ed80.log`). No landed row binds the QAE spelling any more.
- **Funclets:** funclet 0x01211E13 includes the changed parent 0x00C17260 and was rechecked with it. funclet_includes_check reports
  1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** 63 graded from the repo after landing (62 rows + the funclet): 54 RELOCATION_MATCH and
  9 MATCH, every test PASS. NF56's own final run agrees.
- **Checks:**
  - `naming/rebind.py` removed 83 binding lines and added 56;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (23 opt-ins in 22 files; default path unchanged)
CDisplayManager.h, CEngine.h, CEnginePrimitiveHandle.h, CTCBetting.h, CAIStateGroup_OpinionReactionRoot.h,
CEngineResourceManager.h, CWorld.h (2), CDisplayEngine.h, CMainGameComponent.h, CNetworkClient.h, CQuestManager.h, the three
CAIStateGroup_Creature*Cover headers, CEngineWorldMap.h, CEngineMap.h, CEngineScreenEffectDisplacementRenderer.h,
CTCInventoryItem.h, CTCTalk.h, libpk/PKCBufferedReader.h, libpk/CSegmentFile.h and stlport/fable_containers.h. Details:
`naming/2026-11-17_agentNF56/hdr/README.md`.
- Five opt-ins wrap unchanged default text in an `#else` (the AdvanceInCover / TakeCover / OpinionReactionRoot retail class
  branches, FINISHSTENCIL and RESERVE_ALLOCATE_AND_COPY); every other one only adds lines. NF56 checked that each header
  preprocesses to the same text without its macros.

## Name fixes queued (`land1117fu`, 17)
From NF56's NAME_FIXES.tsv (18, less the 0x0095ED80 line fixed in this landing): callers and self rows outside its rows that still use spellings it proved wrong, among them the
GFSerialiseVectorBinary In/Out `<bool>` and `vector<bool>::_M_insert_overflow` instances, `??_GCEngineTextureMorph`,
`CEngineLandscapeMap::OnGameMapLoaded`, `CTCInventoryItem::CacheDef`, the `SayToThingReactingTo` access, the strike-event
`GetHelperPointNamesFromStrikeEventName` and `CBufferedRangeReader::Reset`.

## Rule slips (NF56 `slips.txt`)
- Three Bash calls exited non-zero (a missing column, a `which unifdef`, an `ls` of a file not yet written).
- One probe of 0x004091C0 included engine/CWideString.h, which does not exist; its grade came back empty and was rerun.
