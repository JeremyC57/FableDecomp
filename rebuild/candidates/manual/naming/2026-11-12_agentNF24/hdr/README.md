# agentNF24 header overlays

Every file in `hdr/engine/` is a full copy of the current repo header (`rebuild/include/engine/`) plus `#ifdef` blocks only;
the default (no opt-in) text is unchanged. Rows were graded with `FABLE_HDR_OVERLAY` pointing here.

| Header | Opt-in added | Adds | Rows |
|---|---|---|---|
| CTCPerceiveThing.h | FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS_CALLS | out-of-line const declarations IsAwareOfThing 0x00643B3B / IsAwarenessOfThingDirect 0x00744430 (Ego_r QBE) for TUs that call them | 00935160 |
| CPredicate_And.h | FABLE_RETAIL_CPREDICATE_AND_INLINE_DTOR | inline virtual ~CPredicate_And (retail expands it in the scalar deleting dtors) | 00e8b020, 00ef6e40, 00ef84d0 |
| CalextestData.h, CEmptyData.h, Cjoss_test_scriptsData.h, CDanyalTestScriptsData.h | FABLE_RETAIL_NSCRIPT_DATA_CTOR | public default ctor declaration (FableWin.pdb ??0<Class>@NScript@@QAE@XZ, out of line in retail) | 00cd4620, 00cd4660, 00cd46a0, 00cd46e0 |
| CQ_SunnyvaleMasterData.h | FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_ALLOC | static fastcall Alloc declaration (Ego_r ?Alloc@CQ_SunnyvaleMasterData@NScript@@SIPAVCScriptDataBase@@XZ) | 00cdbd20 |
| CCombatSequence_FodderMainAttack.h, CCombatSequenceWhisperSortYourselfOut.h, CCombatSequence2StrikeCombo.h | FABLE_RETAIL_COMBATSEQUENCE_STATGETNAME | static StatGetName (CCharString, fastcall) declaration | 008b0020, 008b0440, 008b04c0 |
| CEngineResourceManager.h | FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES | nested `struct CExternalTextureAllocationInfo { long Handle; CXboxBufferMemoryPool* Pool; }` (Ego_r, 8 bytes) in both branches + forward decl, so `std::map<CTexture*, CExternalTextureAllocationInfo>` spells retail | 00b3e6e0, 00b3e750, 00b3e8d0, 00b3e990, 00b3e9d0 |

Opt-ins reused unchanged from the repo (no overlay): FABLE_RETAIL_STLP_MAP, FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE,
FABLE_RETAIL_DEF_CLASS_BASE(_CTOR_DECL), FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE, FABLE_RETAIL_CARENACREATUREDEF_IMPLICIT_DTOR,
FABLE_RETAIL_CSCRIPTDATABASE_CTOR, FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE, FABLE_RETAIL_CTCSOUNDPLAYER,
FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CDEFSTRINGTABLE.
