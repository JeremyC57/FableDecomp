# 2026-11-17 landing nf76: name-fix round 7, NF76 (9 rows: ports NF72 and NF73 left blocked)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF76 took nine rows NF72 and NF73 had left blocked and ported all nine. Brief, list and records: `naming/2026-11-17_agentNF76/`.

## Rows (9 ported)

| Row | Grades | Now |
|---|---|---|
| 0x0041B550 | 1 | `NUISystem::CSprite::SetGraphicIndex`, leaf `QAE` to the virtual `UAE` |
| 0x00549B20 | 1 | `NUISystem::CTextSlider::GetDataFromDef` (Ego_r.pdb 0x005C3860, FableWin.pdb 0x02273B90, Ego_d locals; was the made-up `Initialize`) |
| 0x005778CA | 2 | `CTCInventoryExperience::CloseInventoryInternalPC` (was `vfunc_90`) |
| 0x00599E3F | 1 | `NUISystem::CFrontEndManager::Update` |
| 0x006D53C0 | 7 | `CTCFollowed::FrameUpdate`, leaf to `UAE` |
| 0x00970CD0 | 1 | `CAIStateGroup_CreatureUseCoverBase::GetHiddenCoverPosAndSetUpCoverThing`, `IAE` on the real class |
| 0x00A79930 | 1 | `CNavSwitchableLeafNode` generator constructor, on the STLport vector spelling |
| 0x00B41FA0 | 1 | `CEngineWorldMap::LoadWaterData`, leaf to `AAE` |
| 0x00B42530 | 1 | `CEngineMap::OpenDevelopmentStaticMap`, leaf with the enum argument |

- Every local model of a class with a shared header is gone. Two local declarations remain for classes with no shared header
  (`CBankFileAsyncEntry` in 0x00B41FA0, `CEngineLandscapeMap` in 0x00B42530).
- 0x00549B20's port also corrected what its stand-ins had wrong: the def is `CUIDef` (no `CTextSliderDef`), `SetTarget` 0x0053FDE0 is
  `CAction::AddParam(CActionParam*, CCountedPointer<CAction>&)`, vtable slot 80 is `AddActionOnLeftUnclicked` and slot 143
  `GetValueFromProfile`, and the local `CActionParamLong` is `CActionParamSLONG` (vtable 0x012366F4, entry nf44:76).
- `CNewFrontendGameComponent::StartGame`'s inline body is read from the retail bytes; the name is Ego_r's.

## Entries (APPLIED.tsv): 13 done
nf41:e125 / e124 / e164 / e154 by 0x0041B550; nf44:54 and nf44:7 done for this slice (the twin 0x0057770A still binds the stand-ins:
queued); nf44:76 by 0x00549B20; and the rest.

## Rows and checks
- **Section 1: 9 self renames (8 file moves). Section 2: 67 respelled bindings.**
- **Headers (24 opt-ins in 22 files):** 23 new, plus `FABLE_RETAIL_CFRONTENDMANAGER_UPDATE` extended to the full-layout branch. NF76 kept
  base copies (identical to the repo at hand-back) and pinned the asserts its opt-ins moved (7 in CNavSwitchableLeafNode.h, 1 in
  CEngineStaticMapFileBlockManager.h, 8 in CLocalDetailCacheMap.h, 10 in CVirtualKeyboard.h). `lane_merge/assert_names_check.py`: 0
  mismatches against 49b943df65. fable_containers.h's `#ifdef FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE` became an `#if / #elif` with a new
  `LIST_SIZE_DECL` branch first (the old branch is unchanged); CANavQuadTreeNode.h gains its RTTI base `CBaseClassNonCopyable` and the
  inline generator constructor under `FABLE_RETAIL_CANAVQUADTREENODE_GENERATOR_CTOR` only.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 9 rows graded from the repo after landing: 9 RELOCATION_MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 59 binding lines and added 23; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 15)
NF76's NAME_FIXES.tsv (13: the ctor 0x00549F60 that still calls 0x00549B20 as `Initialize`, binders of 0x0053FDE0, 0x0054A2A0,
0x0042AA29, 0x009FE380, 0x0059B039 and 0x0059AEE5, the deque back / pop_back names, 0x0087C3F0's label and others), plus two coordinator
lines: settle the data symbol 0x013CA818 (bound as Instance, PInstance, g_pointer_013ca818 and PClass; no second source yet), and port
the twin 0x0057770A with 0x005778CA's recipe.

## Rule slips
- One Bash call exited 2 (`ls` of a missing directory); an empty `hdr/orig` directory was created and removed.
