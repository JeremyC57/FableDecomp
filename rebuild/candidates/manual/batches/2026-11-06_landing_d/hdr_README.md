# Chunk 803 header overlay

Every change is additive and behind a new opt-in macro (or a new file), so no existing includer's preprocessed text changes.
Copies taken fresh from rebuild/include during the session and diffed again before hand-back (only the additions below).
The overlay copies are the ones used: the four matched rows need the opt-in classes, which exist only in these copies.

| header | change | macro | evidence | includers rechecked |
|---|---|---|---|---|
| engine/CQ_NewOakValeIntroScript.h | nested CNOVI_AffairMan (0x24: EncounterOver +0x1c, SaidFirstRangedComment +0x1d, HeroAgreedToKeepQuiet +0x1e, HeroSaidHeWouldReportMan +0x1f, BadgerIndex +0x20) and CNOVI_BarrelThug (0x24: DoneIntro +0x1c, LastTimeSpoken +0x20), both `: CEntityScriptBase`, ParentClass& +0x14, PMasterData +0x18, Alloc / GetParentScript / private Main+Init | FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES | Ego_r.pdb layouts (struct_layouts_egor.tsv, entity offsets equal retail's); vtables 0x012D89EC / 0x012D9234 slot 1 = the rows; offsets read/written by rows 0x00DB09E0 / 0x00DB6C60 | none (no landed includer) |
| engine/CQ_WhiteBalverineKnotholeGladeScript.h | nested CWB_WhiteBalverine (0x20: WBCounter +0x1c) | FABLE_RETAIL_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_ENTITIES | Ego_r.pdb 32 bytes; vtable 0x012E0A50; landed Alloc 0x00E15750 allocates 0x20 | 0x00E13F10 RELOCATION_MATCH PASS |
| engine/CV_SwordInTheStoneScript.h | nested CSwordCrowd (0x1c); WatchForDeathDuringSwordPull(CScriptThing) in place of () under the macro (#else keeps the old line) | FABLE_RETAIL_CV_SWORDINTHESTONESCRIPT_ENTITIES | Ego_r.pdb CSwordCrowd 28 bytes; vtable 0x012F3174; PDB ?WatchForDeathDuringSwordPull@...@@QAEXVCScriptThing@@@Z and the landed row 0x00ED7320 | 0x00ED5830 RELOCATION_MATCH PASS |
| engine/CQ_PrisonWardenGameScript.h | NEW: CQ_PrisonWardenGameScript (0x5c: SecretDiaryOpened +0x48, HeroHasSecretDiaryKey +0x49, BookOpened +0x4c, SThingBookOpened +0x50) + opt-in CPrisonWarden (0x1c) | FABLE_RETAIL_CQ_PRISONWARDENGAMESCRIPT_ENTITIES (entity) | Ego_r.pdb 112 bytes at PDB-0x14; offsets read by 0x00DDF090 and written by landed CSecretBook::Main 0x00DDE940; vtables 0x012DCB40 / 0x012DCC50 | none (new) -- used only by the parked row 0x00DDF090 |
| fable_string.h | CCharString::Sprintf(const char*, ...) 0x0099F1F0 declaration | FABLE_RETAIL_CCHARSTRING_SPRINTF | reloc_bindings ?Sprintf@CCharString@@QAAXPBDZZ; landed GFIntToCharString 0x0099F830 calls it cdecl with this pushed | not rechecked (opt-in only; only the parked row 0x00DDF090 defines it) |
| fable_definition_table.h | unchanged copy | -- | needed only so the overlay fable_string.h wins (`#pragma once` header reached through CDefPointer.h -> fable_definition_table.h); do not land it | -- |

Only the first three are needed by matched rows. The last three serve the parked PrisonWarden row; the coordinator may hold them
until that row lands (landing the Sprintf opt-in in the repo fable_string.h makes the fable_definition_table.h copy unnecessary).
