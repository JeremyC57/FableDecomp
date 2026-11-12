# agentFAM08 header overlay

Grade with `FABLE_HDR_OVERLAY=$S/w22/agentFAM08/hdr`. Two kinds of file:

**Full copies of the current repo header with one `#ifdef` opt-in (default path byte-identical, declaration only, no layout change):**

| header | opt-in | rows |
|---|---|---|
| engine/CQ_SunnyvaleMasterData.h | `FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_CTOR` (declares `CQ_SunnyvaleMasterData()`, FableWin ??0, retail 0x00CDB9E0) | 0x00CDB9E0 |
| engine/CScriptDataBase.h | `FABLE_RETAIL_CSCRIPTDATABASE_CTOR` (declares `CScriptDataBase()`, FableWin ??0, retail 0x00CD4500) | 0x00CD4530 |

**New headers (no repo header existed; layouts from the Ego_r.pdb records on the retail STLport sizes, sizes asserted):**

| header | class | retail evidence | rows |
|---|---|---|---|
| engine/CScriptDefinitionManager.h | CScriptDefinitionManager : CDefinitionManager (0xD0) | vtable 0x012C2648, ctor 0x00CD3F00; needs FABLE_RETAIL_CDEFINITIONMANAGER (+CRESOURCELIST, DEF_CLASS_BASE) | 0x00CD3EC0 |
| engine/CalextestData.h, engine/Cjoss_test_scriptsData.h, engine/CDanyalTestScriptsData.h | NScript::*Data : CScriptDataBase (4 bytes), modelled on engine/CEmptyData.h | vtables 0x012C2718 / 0x012C2730 / 0x012C273C | 0x00CD4A90, 0x00CD4B50, 0x00CD4BB0 |
| engine/CLevelScriptNoMasterBase.h | CLevelScriptNoMasterBase : CScriptBase, Game +0x40 (0x44) | ctor 0x00CDD2F0, vtable 0x012C2F50 | 0x00CDD2F0, 0x00CDD310 |
| engine/CArenaCreatureDef.h | CArenaCreatureDef : CSubDefClassBase (0x38) | vtable 0x012FB358; opt-in `FABLE_RETAIL_CARENACREATUREDEF_IMPLICIT_DTOR` (retail ~ 0x00F25E40 stores no vptr) | 0x00F25DD0, 0x00F280E0 (via CArenaWaveDef) |
| engine/CArenaWaveDef.h | CArenaWaveDef : CSubDefClassBase (0x3C, ShortWave +0x38) | vtable 0x012FB2F0; opt-in `FABLE_RETAIL_CARENAWAVEDEF_IMPLICIT_DTOR` (retail ??_G 0x00F25CB0 stores no own vptr) | 0x00F280E0, 0x00F34C30 |
| engine/CCS_OakValeRevisitedScript.h | NScript::CCS_OakValeRevisitedScript : CLevelScriptBase<CQ_SunnyvaleMasterData> (0xDC) | allocator 0x00EE8BE0 `new 0xDC`; registration pairs it with the Sunnyvale data allocator 0x00CDBD20; OakValeFlag kept opaque (map, 12 bytes) | 0x00EE82A0 |

Repo headers used unchanged: CArenaRoundDef.h, CHeroSoulsRoundDef.h, CHeroSoulsWaveDef.h, CFireHeartPatternDef.h, CConversation.h,
CSubDefClassBase.h (opt-ins FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL / FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE, as the landed CConversation
ctor), CSpawnedFuncBase.h, CActiveEntityScriptBase.h, CPredicateBase.h, CIsVariableFalse.h, CPredicate_And.h, CIsThingAlive.h,
CQ_HeroSoulsArenaScript.h, NScript_CQ_WaspBossScript.h, NScript_CV_HeroDuelScript.h, CSpawnedFunc.h, CEmptyData.h.
