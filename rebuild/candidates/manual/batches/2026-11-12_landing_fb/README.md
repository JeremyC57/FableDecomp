# 2026-11-12 landing fb -- author workflow wf14 (Opus only): nine of ten rows landed, one not matched

Source: `pending/2026-11-12_linux_handWF14/` (workflow `lane_tools/workflows/author_rows.js`, 58 agents, all Opus). Nine rows
byte-matched in about 110 minutes. Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x00959800
CAIStateGroup_HoverWanderAroundThing::IsRunnable, 0x004C9920 CThing::IsAffectedByBulletTime, 0x00760E70
NShape::CTrack::GetMinDistanceToTrack, 0x006AC430 CThingPlayerCreature::LearnExpression, 0x0076C9A0 CTCEnemy::OnSerialise,
0x00AF5430 NParticleEngine::CParticleSystem::ClearDecal, 0x00B68DA0 CEngineLandscapeRenderer::RenderSortedPatches, 0x00941690
CAIStateGroup_KidPlayHopscotch::Update, 0x009C4440 CFileInstaller::IsFileCopyNecessary (per-row flag /d2inlT120).

**Not matched:** 0x009F20B0 CMemoryAllocator::CMemoryAllocator, DIFFER(596v598) after three Opus author passes (about 150
grades). Only register allocation differs: retail keeps the constant zero in ebx through the first loop; VC7.1 here colours `heap`
first. A priority trace shows the first loop is byte-exact once the second loop and the tail store are removed. This is the first
row of the Opus-only workflows (wf10..wf14, 49 rows) that Opus could not match, so it goes to ONE Fable author pass
(`args.authorModel = 'fable'`, the owner's "absolutely necessary" case) with all notes; best source and notes stay in scratch
until then.

**Coordinator fixes at landing (both regraded: match, test PASS):**
- 0x009C4440 called 0x0099ED10 `CCharString::ToLower` through a new header opt-in. Both PDBs name that address
  `?GetAsLowerCase@CCharString@@QBE?AV1@XZ` and landing ey already added FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE; the row now uses
  that, and the ToLower opt-in was dropped (one address, one name; no fable_string.h change).
- 0x00B68DA0: the CEngineLightingManager.h overlay had rewritten a default-path comment in place. The original line is restored and
  the correction (SetEnableFadedShadowFactor does have an out-of-line copy, 0x00B66EB0) is added as new lines; an Inline-budget line
  added to the new force-inlined setter.
**Mechanical checks:** reserved_check 0 (0x0076C9A0 only cites 0x0076C8A0 in a comment), purity clean; pragmas are
`intrinsic(...)`, `auto_inline(off)` around real same-TU callee bodies, and `pack(1)` on CAIStateGroup_KidPlayHopscotch, which has
no shared header. No local model of a header class.
**Headers** (14; additions under new opt-ins, all unused elsewhere, defaults unchanged per hdrcheck2):
CAIStateGroup_HoverWanderAroundThing.h, CTCDParticleEmitter.h, CThingManager.h, CTCInventory.h, CExpressionDef.h,
CThingPlayerCreature.h, CTCInventoryItem.h, CTCInventoryAbilities.h, CDefString.h, CDecal.h, CParticleSystem.h,
CEngineLandscapeRenderer.h, CEngineLightingManager.h, CCreatureAction_PlayHopscotch.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fb): 0x00705170 GetPCarriedByThing, 0x005EEDCF AddLearnedExpression, 0x01436E9C GPLightingManager, 0x0099AF00.
