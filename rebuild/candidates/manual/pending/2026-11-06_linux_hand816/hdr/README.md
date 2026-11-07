# Chunk 816 hdr/ hand-back

Each file was copied fresh from rebuild/include (or, for the Beggar header, from chunk 813's pending hdr/). Every edit is an opt-in branch.
Before hand-back, every file was diffed against rebuild/include again: the repo copies had not changed, so the only differences are the blocks below.

| header | change | evidence | path |
|---|---|---|---|
| engine/CQ_GuildTrainingScript.h | adds nested `CApprenticeSpeedTest` (Alloc 0x00D3E1F0, GetParentScript 0x00D3E290, Main 0x00D3E2E0, Init 0x00D3E2A0, OnPredicateFail 0x00D3E2B0; ParentClass +0x14, PMasterData +0x18, RaceMode +0x1c; sizeof 0x20) | Ego_r.pdb layout (struct_layouts_egor.tsv, 32 bytes); landed Alloc allocates 0x20; entity vtable 0x012CDA40; both PDB name sets | opt-in: `FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_SPEEDTEST` && `FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN`; one block appended after the existing class, default path textually unchanged |
| engine/CV_BanditTollScript.h | adds `CV_BanditTollScript::CheckForBanditTollPaid()`, `CTollBandit::SetUpBanditToll()` and `CTollBandit::Should_I_Flee(CScriptGameResourceObjectScriptedThingBase&)` | both PDB name sets; called / spawned by CTollBandit::Main 0x00E4B5B0 (0x00E4E0E0, 0x00E4DF20; CSpawnedFunc vtable 0x012E4B90) | opt-in: `FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT`; two inserted `#ifdef` blocks, no existing line touched |
| engine/CV_BeggarAndChildScript.h | NEW (not in repo): chunk 813's pending draft verbatim + nested `CLookoutPointBeggar` (Alloc 0x00E58C60, GetParentScript 0x00E58D00, Init 0x00E58D10, Main 0x00E58D40; ParentClass +0x14, PMasterData +0x18; sizeof 0x1c) replacing 813's forward declaration | Ego_r.pdb layout (28 bytes); entity vtable 0x012E68C4; both PDB name sets | whole header behind `#ifdef FABLE_RETAIL_NSCRIPT`, no #error branch |
| engine/CQ_GuildTrainingSkillScript.h | NEW (not in repo): `CQ_GuildTrainingSkillScript : CLevelScriptBase<CQ_SunnyvaleMasterData>` (TargetsHit +0x48, TutorialState +0x4c, GenericTutorialCounter +0x50, TotalTrainingDummiesCounter +0x54; sizeof 0x58) + nested `CTheRealGuildmaster` (Alloc 0x00D5ABE0, GetParentScript 0x00D5AC80, Init 0x00D5AC90, Main 0x00D5AE70; ParentClass +0x14, PMasterData +0x18, PlayerNotWarned +0x1c; sizeof 0x20) | Ego_r.pdb layout (108 / 32 bytes; retail = PDB - 0x14); entity vtable 0x012D09D8; both PDB name sets | whole header behind `#ifdef FABLE_RETAIL_NSCRIPT`, no #error branch |

Includers rechecked: all 19 landed includers of the overlay set (rebuild/src, listed in ../SIBLINGS.tsv) were preprocessed with g++ -E,
with and without `-I hdr`. The output is identical except for the `__LINE__`-derived `fable_static_assert_<N>` typedef names in the 11
CV_BanditTollScript.h includers (line shift, no code). So the default path cannot change their bytes. A VC7.1 vland recheck was not run;
it is moot for this reason. The opt-in parked rows compile only with the overlay, because they use the opt-in-only declarations.
