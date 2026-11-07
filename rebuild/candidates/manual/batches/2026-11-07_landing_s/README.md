# Landing 2026-11-07s: authoring chunk 842 (1 of 6 landed, 5 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00D40CF0 NScript::CQ_GuildTrainingScript::CMeleeApprentice::Main | **RELOCATION_MATCH + PASS, landed** | `for (;; Game->NewScriptFrame()) { if (T) return; ... }` puts the expanded ScriptedThingBase dtor pad at the loop exit; row-local `__forceinline CScriptThing::GetPos` |
| 0x00EC4A00 CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main | DIFFER, 15 | CScriptThing copy split (one site expanded in retail) |
| 0x00E5E5A0 CV_BodyGuardScript::MakeHenchmanPerformIdleComment | DIFFER, 116 | per-exit dtor split |
| 0x0092F310 CAIStateGroup_MinionReactToPlayerInteraction::GetGossipPhrase | DIFFER, 288 | frame / register allocation; the map getter wants the named-iterator form |
| 0x00EF9280 Cjoss_test_scriptsScript::Main | DIFFER, 795 | per-exit dtor / caller-size budget |
| 0x00EF5FF0 CPersonalScript_EmptyScript::CCinemaMan::Main | DIFFER, 47 | CPredicate_And dtor inline depth |

- **Header:** additive opt-in `FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MELEEAPPRENTICE` in `engine/CQ_GuildTrainingScript.h` (used by the landed row).
  The CPredicate_And / CTCTalk / RockTroll / joss opt-ins serve only parked rows; they stay in `pending/2026-11-07_linux_hand842/hdr/`.
- **Names:** queued 0x004FAEE0 GetDialogueVoice (`land0711s`). 0x009CC410 CBankFile::FindIndexBySymbolCRC is PROBABLE only (see the pending
  NAME_FIXES.tsv); not applied. The 0x00EC4A00 and 0x0092F310 labels were already corrected in earlier landings.
