# agentQ11 header overlay (grade with FABLE_HDR_OVERLAY=<this dir>)

Copies of repo HEAD headers with new opt-ins only; the default (no-macro) path of every changed header is byte-identical to the repo.
One new header (no repo file of that name existed).

| header | opt-in | for row | what it adds |
|---|---|---|---|
| engine/CQ_PrisonRaceScript.h | FABLE_RETAIL_CQ_PRISONRACESCRIPT_WINNINGPOSTGUARD | 0x00DDC070 CPrisonRaceWinningPostGuard::Main | the nested entity script CPrisonRaceWinningPostGuard : CEntityScriptBase (Ego_r.pdb 28 bytes: ParentClass +0x14 reference, PMasterData +0x18; Alloc 0x00DDBF90, vtable 0x012DC9E8 slots 1 Main / 2 Init / 3 GetParentScript), spelled as the existing CPrisonRaceGuard block, plus its sizeof assert. |
| engine/CQ_EndGameBossBattleScript.h | FABLE_RETAIL_CQ_ENDGAMEBOSSBATTLESCRIPT_CSWORDVOID | 0x00D33280 CSwordVoid::Main | the nested entity script CSwordVoid (Ego_r.pdb 0x1c: ParentClass +0x14, PMasterData +0x18; Alloc 0x00D2FCE0), spelled as the existing CTheRealGuildmaster / CEndGameSister blocks, plus its sizeof assert. |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_HORNTASK | 0x00940960 CAIStateGroup_VillageTaskHorn::Update | declarations of SetHornTaskAsAcceptedByThing (0x006584F9) and SetHornTaskAsCompleted (0x0065A00F), Ego_r.pdb / FableWin.pdb spellings, beside the TORCHTASK pair. |
| engine/CAIStateGroupBase.h | FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY (with _MEALACTIONS) | 0x00940960 | declaration of the protected AIAction_MoveToPosProximity (0x0097E890), Ego_r.pdb spelling. |
| engine/CCreatureAction_UseObject.h | FABLE_RETAIL_CCREATUREACTION_USEOBJECT_IMPLICIT_DTOR (with _USEOBJECT) | 0x00940960 | no user-declared destructor: the compiler-generated one, which the Horn TU expands (PUsedThing ~CBaseIntelligentPointer 0x00A01C10, then ~CCreatureActionBase 0x00693EF0, no vtable store, 0x00940A84) where the Torch TU calls the COMDAT 0x006290E0. Default path keeps `virtual ~CCreatureAction_UseObject();`. |
| engine/CAIStateGroup_VillageTaskHorn.h (NEW) | none (new header) | 0x00940960 | CAIStateGroup_VillageTaskHorn : CAIStateGroupBase from Ego_r.pdb (32 bytes: PTCVillageMember +0x14, PTask CIntelligentPointer<CVillageTaskHorn> +0x18; Initialise / Update / IsRunnable / Cleanup / Alloc / ctor / dtor), spelled as engine/CAIStateGroup_VillageTaskTorch.h. |
