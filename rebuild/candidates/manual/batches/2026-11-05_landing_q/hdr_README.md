# chunk 790 header overlay (hdr/engine/)

Three NEW headers. No existing rebuild/include header changes. Each is a retail opt-in model and errors out unless
`FABLE_RETAIL_NSCRIPT` is defined.

| header | class | used by | layout evidence |
|---|---|---|---|
| engine/CQ_DragonBossFightScript.h | NScript::CQ_DragonBossFightScript : CLevelScriptBase<CQ_SunnyvaleMasterData> | src/00d26190.cpp (DoMission) | Ego_r.pdb layout (184 bytes) at PDB - 0x14, a further - 4 after each STLport vector: DragonState +0x48, MinionSpawnPoints +0x4c, SummonerSpawnPoints +0x58, ... cs_flags +0x84, MinionSpawnDelay / SummonerSpawnDelay +0x90 / +0x94; sizeof 0x98 (Alloc 0x00D27240 news 0x98). DoMission reads +0x48 and passes &cs_flags = this+0x84, and the landed PlayCutscene 0x00D27050 passes the same address. Declares RunEnemySpawning 0x00D26BA0 and JackTaunts 0x00D26A50 (spawned by DoMission through CSpawnedFunc<CQ_DragonBossFightScript>). |
| engine/CExpression_FishScript.h | NScript::CExpression_FishScript : CScriptBase (global expression script: ctor takes only the interface) | src/00ee95c0.cpp (Main) | Ego_r.pdb layout (92 bytes) at PDB - 0x14: Game +0x40 (ctor 0x00EE94A0 stores it), LastFishingLevel +0x44 (OnPersist 0x00EE9590 transfers it), sizeof 0x48. Vtable 0x012F54E4: slot 1 RegisterMain 0x00EE94D0, slot 2 Main 0x00EE95C0, slot 4 OnPersist 0x00EE9590. |
| engine/CQ_HeroSoulsScript.h | NScript::CQ_HeroSoulsScript : CLevelScriptBase<CQ_SunnyvaleMasterData> | src/00d78670.cpp (Main) | Ego_r.pdb layout (100 bytes) at PDB - 0x14: HeardFirstBriarRoseMessage +0x48 .. AmLoadingGame +0x4d, sizeof 0x50 (Alloc 0x00D7AC50 news 0x50; OnPersist 0x00D785E0 transfers +0x48..+0x4c by name). Vtable 0x012D2F48: RegisterMain 0x00D78520 / Main 0x00D78670 / Init 0x00D78500 / OnPersist 0x00D785E0; PlayCutscene 0x00D7AA30. The nested CNotForODG / CEndScythe / CEndBriarRose are forward-declared only: the row defines them locally with their Alloc. |

None of the three has an includer in the tree before chunk 790. The landed sibling rows of each class keep their local models,
and nothing has to be migrated. The two parked rows use existing headers unchanged: engine/NScript_CV_ChickenKickingScript.h and
engine/CQ_OpeningGraveyardSecretPassageScript.h.
