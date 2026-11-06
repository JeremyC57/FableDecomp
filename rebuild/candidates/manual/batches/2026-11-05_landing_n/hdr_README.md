# Chunk 787 header overlay (hdr/engine)

| header | change | path | evidence | includers rechecked |
|---|---|---|---|---|
| CQ_BanditCampBossBattleScript.h | Repo copy plus additive declarations: six thread methods (BanditKingMissionProcess, WatchForBanditCampGates, ProcessHostageCutscenes, CheckForFirstAreaMassacre, CheckForSecondAreaMassacre, CheckForSecondAreaDoorHelp) and forward declarations of the 13 nested entity classes | default (additive only, no layout change) | PDB name set (Ego_r) + thread literals in Main 0x00D038E0 | none landed (INCLUDERS_CQ_BanditCampBossBattleScript.txt); 0x00D038E0 RELOCATION_MATCH |
| CQ_RansomVictimScript.h | Repo copy plus an opt-in `#ifdef FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_METHODS` block declaring WatchForTermination (0xDE14A0), WatchForNewRegion (0xDE1770), DisappearFollowers (0xDE4710) and GossipSetup (0xDE1280) | opt-in | PDB name set + references from Main 0x00DE0850 | 13 landed includers (INCLUDERS_CQ_RansomVictimScript.txt): 13/13 WINS with the overlay |
| CQ_MinionClifftopChaseScript.h | NEW: class on CLevelScriptBase<CQ_SunnyvaleMasterData>, sizeof 0x6c, members from Ego_r.pdb at offset -0x14, nested-class forward declarations, ProcessMission / WatchForTermination | default (new file) | struct_layouts_egor.tsv + Main 0x00DA34B0 member accesses | no includers; 0x00DA34B0 RELOCATION_MATCH |
| CQ_TraderConflictGoodScript.h | NEW: sizeof 0x68; STLport vector<CScriptThing> AllCreatures at +0x48, then the members up to +0x67 (a further -4 after the 12-byte vector) | default (new file) | struct_layouts_egor.tsv + Main 0x00DFA450 member offsets | no includers; 0x00DFA450 RELOCATION_MATCH |
| CQ_AwakeningTheOracleScript.h | NEW: sizeof 0x98; vector at +0x74, maps at +0x80 / +0x8c (stlport/fable_map.h), flags PlayedOpeningCS / AutosavedInNW3 / PlayedNecroCS / TabletsCollected | default (new file) | struct_layouts_egor.tsv + Main 0x00CFB080 member offsets | no includers; 0x00CFB080 RELOCATION_MATCH. A Codex pass13 hdr_manifest once named this header, but it never landed; this file supersedes it |

CQ_AmbushTradersScript.h is used unchanged from the repo.
