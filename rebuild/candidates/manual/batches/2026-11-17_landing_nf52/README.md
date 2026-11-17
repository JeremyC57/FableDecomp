# 2026-11-17 landing nf52: name-fix round 7, NF52 (58 rows: class statics, singletons, vftables, CBasicString<char>)

Owner: "Continue the naming fix queue with two agents until complete".

NF52 took 14 entries from both halves: 60 binder rows from NF41's probe and NF44's binder lists. The entries:
- **class statics and globals:**
  - CShaderRenderManager::SingletonInstance;
  - CMainGameComponent::EventDispatch;
  - COpinionReactionManager::AttitudeStrings;
  - g_pScriptRunner;
  - CTBaseSingleton<CSkeletalMorphResourceManager>::PClass;
  - CCreatureActionBase::RegisteredActions;
  - CWorldMap::LineCollisionAround4Square;
  - CEngineInternalPrimitiveBase::CurrentInterpolationInfo;
- **vftables:** CSegmentList and CTCInventoryMagic;
- **CBasicString<char>:** Compare and operator==;
- **engine members:** CTCTavernGame::PositionMesh and AddStaticMesh.

Brief, worklist and records: `naming/2026-11-17_agentNF52/`.

## Rows (60, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 56 (+2 callers of self-renamed rows: 0x0063D960, 0x004D2F46) |
| not-a-binder | 1 (0x006F5C10) |
| blocked | 3 |

**Blocked rows:**
- **0x009BEF80 (authoring):** an asm_bake row.
- **0x006D41F0 (3 grades, DIFFER 307v295):** retail expands the first of three `CBasicString<char>::operator==(const char*)` tests
  and calls the other two out of line (0x004115A0). Every fable_string.h spelling expands all three. It needs a new fable_string.h
  opt-in, and that root `#pragma once` header is not overlaid by agents.
- **0x004FE630 (1 grade, C2664):** the `X_004fe630::CAFile` stand-in, CAFile ReadVar<long>, set<CCharString>::find and the self
  name must be ported first.

## Entries (APPLIED.tsv)
- **11 done.**
  - nf44 26 RegisteredActions is done against the protected spelling (`@@1V`) that the shared header declares and the rows bind.
  - The queued "private" claim does not hold and is in REJECTED.tsv.
- **3 blocked:**
  - e27 Compare: 9 binders respelled; blocked by 0x006D41F0.
  - e199 operator==: blocked by 0x006D41F0 / 0x004FE630.
  - nf44 27 SingletonInstance: 10 binders; blocked by 0x009BEF80.

## Rows and checks
- **Self renames (section 1, 13 rows; 6 file moves):**
  - CWorld::ProcessEvent (was the stand-in CWorld_Methods);
  - CDisplayEngine::ConfirmFeedbackGameEventPackage;
  - GetOpinionAttitudeAsString / GetThingAttitudeToPlayerAsString;
  - CGameScriptThing::IsNull;
  - CGetMatchingStrikeEndEventName::operator();
  - CWorldMap::FindMax{Ground,Theme}ZInCell;
  - the CTCInventoryMagic ctor;
  - CEnginePrimitiveRenderer::AddStaticMesh;
  - **0x00C2DFE0 / 0x00C2E030:** CSegmentList ctor / dtor. These were `CVtable_012b47d0`. libpk ships no PDB, so the names are
    descriptive, named from the vftable they install.
  - **0x01229400:** the compiler-generated `_$E2` atexit destructor of `CTBaseSingleton<CSkeletalMorphResourceManager>::PClass`.
    It is catalogued in the established `` `dynamic atexit destructor for '...'' `` form (23 catalog rows use it). Its `_$E1`
    initializer produces the same bytes as retail 0x0121E180, which is still a separate row (queued).
- **Funclets:** the CSegmentList renames moved two EH parents.
  - The coordinator repointed the includes of 0x01213B60 / 0x01213B80 to the new file names; funclet_includes_check now reports
    1,355 checked, 0 broken.
  - All five funclets that include a changed parent (0x01213BC8, 0x01213BD0, 0x01213BDE, 0x01213B60 and 0x01213B80) were rechecked
    against their parents.
- **Recheck (`logs/recheck_part0..2.log`):** 63 graded from the repo after landing, the 58 rows plus the 5 funclets. All are
  RELOCATION_MATCH, every test PASS.
- **Checks:**
  - `naming/rebind.py` removed 161 binding lines and added 127;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (16 opt-ins in 13 files; lines added only, default path unchanged)
CScopedShadersEnable.h, CMainGameComponent.h, CGameEventPackage.h, CWorld.h, COpinionReactionManager.h, CGameScriptThing.h,
CWorldMap.h, CCreatureActionBase.h, CCombatActionBase.h, CTCInventoryMagic.h, libpk/CSegmentFile.h, CCard.h and
CSkeletalMorphResourceManager.h. Details: `naming/2026-11-17_agentNF52/hdr/README.md`. fable_string.h was not overlaid.

## Name fixes queued (`land1117fu`, 13)
From NF52's NAME_FIXES.tsv:
- ??_GCSegmentList;
- CWorld::ReceiveLocalEvent;
- CDisplayEngine::ReceiveFeedbackGameEvent;
- CQuestManager::GetActiveQuestFromScriptName;
- the strike-event helpers;
- four CAIStateGroup_OpinionReactionRoot Wife*Update spellings;
- the RegisteredActions map `find`;
- CCountedPointer<C3DSkeletalMorphParameter>::DeleteData;
- GetMorphTarget.

0x00C2E030 was not queued, because it is already renamed here. The unproven leads (0x01223D00 / 0x0122A4C0, SThingNullName,
CTCWife::CourtingAdviceGroupNames) stay in NOTES.md.

## Rule slips (NF52)
- Two grades ran on a missing file. One of those grade lines was deleted from grades.log afterwards; it should have been kept.
- One unquoted heredoc damaged a comment line, which was repaired before its grade.
- Three rows had comment-only edits after their last grade; all three were regraded before the final recheck.
