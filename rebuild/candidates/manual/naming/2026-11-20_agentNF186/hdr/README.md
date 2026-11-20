# NF186 header overlay (hdr/engine)

Every header here is a copy of the repo working-tree header (orig/engine holds the base copy, byte-equal to the repo when this
hand-back was written) plus `#ifdef` opt-ins only. With none of the NF186 macros defined, each header is token-identical to its base
except the assert pins that pin_asserts.py wrote (FABLE_STATIC_ASSERT -> FABLE_STATIC_ASSERT_AT(<original line>, ...), the same
typedef names). No landed includer defines any NF186 macro (grep over rebuild/src/compiled and rebuild/tests), so the opt-ins change none
of them. Sibling overlays read first: NF185 (CGameScriptInterface.h, CScriptConversation.h), NF187 (stlport/fable_containers.h),
NF184 (CAnimInit.h, fable_map.h), NF183 (CDynamicObjectHeap.h): no overlap.

| header | NF186 opt-in (macro) | adds | rows using it | landed includers | pin_asserts |
|---|---|---|---|---|---|
| engine/CQ_BanditCampBossBattleScript.h | FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CROWDBANDITS_CTOR (inside the existing CROWDBANDITS block) | declaration of the PDB ctor ??0CCROWDBANDITS@CQ_BanditCampBossBattleScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z | 0x00D0B3C0 | 5 | pinned 1 (CCROWDBANDITS size assert, line 248) |
| engine/CQ_GuildTrainingScript.h | FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_SPEEDFRIEND (with ..._MAIN) | new block at the end: class CSpeedFriend (Ego_r layout 0x1c, vtable slots, PDB ctor declaration) + its size assert | 0x00D40890 | 14 | pinned 0 (the new assert is inside the new block, after every existing one) |
| engine/CQ_MinionClifftopChaseScript.h | FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_GUARDCOMMENT | declaration `void GuardComment(CCharString)` (Ego_r / FableWin ?GuardComment@CQ_MinionClifftopChaseScript@NScript@@QAEXVCCharString@@@Z; no layout change) | 0x00DA8510 | 2 | pinned 2 (lines 69, 90) |
| engine/CV_BeggarAndChildScript.h | FABLE_RETAIL_CV_BEGGARANDCHILDSCRIPT_BEGGARBULLY_CTOR | declaration of the PDB ctor ??0CBeggarBully@... | 0x00E5B390 | 3 | pinned 1 (line 69) |
| engine/CV_DemonDoorsScript.h | FABLE_RETAIL_CV_DEMONDOORSSCRIPT_SUMMONSCREATURES_CTOR | declaration of the PDB ctor ??0CDF_SummonsCreatures@... | 0x00E70370 | 21 | pinned 0 (no assert after the edit) |
| engine/CV_SingingStonesScript.h | FABLE_RETAIL_CV_SINGINGSTONESSCRIPT_MANWITHDOORNAME_CTOR | declaration of the PDB ctor ??0CManWithDoorName@... | 0x00ED17B0 | 2 | pinned 3 (lines 54, 55, 75) |
| engine/NScript_CQ_SecretPassageScript.h | FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_UNDEADTRIGGER | new block after the existing assert: class CSP_UndeadTrigger (Ego_r layout with STLport vector, 0x34; vtable 0x012DDF3C slots; PDB ctor declaration) + its size assert; includes engine/CEntityScriptBase.h inside the block | 0x00DEF700, 0x00DEF720 | 2 | pinned 0 (new assert only) |

No FABLE_FORCEINLINE in any opt-in. No existing member was renamed, re-typed or moved.
