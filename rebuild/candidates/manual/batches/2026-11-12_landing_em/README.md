# 2026-11-12 landing em -- author workflow wf4: eleven of eleven rows landed

Source: `pending/2026-11-12_linux_handWF4/` (workflow `lane_tools/workflows/author_rows.js`, 56 agents). All eleven rows byte-matched
and were VERIFIED by both independent lenses. Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x004B3CE0 CQuestManager::ActivateQuests (1,285 bytes; the callee the held row 0x004B4260 waits for), 0x00A90040
N3DPrimitive::CAnimatedBlock::ConvertToVBFormatBump, 0x0064546F NPlayerGui::CDrawQuickAccessIcons::DrawEffectAtPos, 0x008565E0
CThingFilter_CombatIsStrikeToStrike::operator(), 0x00AC8AE0 QuadricOptimiserInternals::COptimisedPrimitive::JoinAdjacentFaces,
0x008F10B0 CTCTavernGameCardBase::ShuffleCards, 0x00ADDBE0 C3DBoundingSphere::BuildFromSubSpheres, 0x0065A41C
CTCVillage::GetFineAmountForCurrentCrimes (/GS: a 10-byte bool array, cookie check before `leave`), 0x0065AA6F
CTCVillage::IsTimeToGoToTavern, 0x00BBCC70 CEnginePrimitiveManagerParticleGroup::SetupPixelShader, 0x007B8B10
CTCTavernTable::GetEatingPoint.

**0x004B3CE0** matched on three source facts (no compiler tie): the real bodies of LoadResourcesForQuests 0x004B2510 and
CBasicString<char>::operator< 0x00429950 (both landed) visible in the TU, both loops as `while (i < size) { ..; ++i; }`, and the PDB
inline accessors of CScriptInfo. One descriptive name: g_QuestScriptsEnabled 0x01375454 (in no PDB).
**Held row:** 0x004B4260 ActivateMultipleQuests is being regraded against this landed body (one-row workflow wf6); the header
declaration's parameter names are corrected here to (quest_names, game_script, load_resources).
**Mechanical checks:** reserved_check 0 (two comments mention reserved siblings), purity clean (HUD / village TU pragmas as the landed
neighbours; `#pragma pack(push, 1)` on a row-local class with no shared header; real callee bodies only), no local model of a
header class in a row source, no stand-in bodies.
**Headers** (20; additions under new opt-ins, all unused elsewhere, defaults unchanged per hdrcheck2): stlport/fable_map.h
(TREE_BOUNDS_OUTOFLINE; the ERASE_KEY `#ifdef` became its `#elif`), stlport/fable_containers.h (RANDOM_SHUFFLE_2ARG), CQuestManager.h
(ACTIVATEQUESTS + wf1's ACTIVATEMULTIPLEQUESTS), CScriptInfo.h (ACCESSORS, FASTCALL_ALLOC), CGenericVertex.h, C3DPrimitive2.h,
C2DCoordI.h, CPlayerGuiDef.h, QuadricOptimiserTypes.h, COptimisedPrimitive.h, card_sorting_types.h, CTavernGameCardBaseDef.h,
CTCTavernGame.h, C3DBoundingSphere.h, CTCVillage.h, CGameTimeManager.h, CEngineResourceManager.h, CTCNamedUserActionPoints.h,
CTCTavernTable.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011em): eleven name fixes from the rows (see NAME_FIXES.tsv).
