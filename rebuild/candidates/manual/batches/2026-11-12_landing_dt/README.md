# 2026-11-12 landing dt -- chunk Q25: 5 of 6 parked rows matched; FAM11's 16 fixed tests

Source: `pending/2026-11-12_linux_handQ25/`. Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x00489D40 CPlayer::CreateCharacter, 0x00847520 CCreatureAction_PerformExpressionExtended::FinishAction, 0x004B5EE0
CQuestManager::SaveGameState, 0x007FC210 CTCKraken::CreateTentacles, 0x009658B0 CAIStateGroup_RecoverStealableItem::Update.

Headers (13, three-way merged against 50ef4fe46, all unchanged upstream; hdrcheck2 default=same for all 13). One EXISTING opt-in
changes: under FABLE_RETAIL_EXPRESSION_FRAME, CCreatureAction_PerformExpressionExtended.h now gives the retail class (RTTI + Ego_r
members) instead of the flattened default. So the two landed rows that define that macro (0x00846BC0, 0x00804020) and three landed
includers of the header (0x00847470, 0x0084ADC0, 0x00842C50) were rechecked in this landing: all MATCH / RELOCATION_MATCH, PASS.
New opt-ins: CPlayer.h PLAYER_CREATECHARACTER, CQuestManager.h SAVEGAMESTATE, CTCSoundPlayer.h RELEASELOOPINGSOUND, CTCEmoteIcon.h
STATTYPE, CCreatureAction_Pick/DropGenericBox.h GENERICBOX, fable_string.h CCHARSTRING_ASSIGN_CHAR, and declarations in
CAIStateGroup_RecoverStealableItem.h, CCreatureActionBase.h, CThingPlayerCreature.h, CWorldMap.h, CEnginePrimitiveRenderUtil.h.

**FAM11 follow-up in the same commit:** the 16 tests landing ds kept in their previous form are replaced by the agent's fixed
respelled tests (`naming/2026-11-12_agentFAM11/TESTS_FIXED.tsv`): values built with the pair constructor, keys read through `.first`.
All 16 recheck RELOCATION_MATCH / MATCH with PASS (`logs/recheck_fam11_tests_and_expression_frame_rows.log`, 21 rows).

Checks: reserved_check 0 reserved rows; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011dt):
- 0x0066AD90 is CTCSoundPlayer::ReleaseAttachedLoopingSound (landed as StopSample / StopSound).
- 0x01436E3C is NEngineInternal::GPScreenEffectOutlineGlow (landed as g_pWaterRenderer in 0x00B33010, and as
  g_pEngineScreenEffectOutlineGlow).
- 0x007120D0: landed `_global::Dtor_CCreatureActionBaseBase`; 0x009658B0 calls it as ~CCreatureAction_PickUpGenericBox (a folded
  derived-action dtor: pick one name by evidence).
- 0x004460C0: 0x00847520 binds IsAssignableSpecialMoveButtonInQueue, but it is landed as
  CGamePlayerInterface::IsQuickAccessEventInQueue.
