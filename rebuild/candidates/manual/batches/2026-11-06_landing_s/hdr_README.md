# Chunk 818 hdr/ hand-back

Each file was copied fresh from rebuild/include. The two chunk 816 drafts are byte-identical copies of `agent816/hdr`. Every edit is an
opt-in branch that adds lines only; no existing line was touched or reflowed. Before hand-back every file was diffed against
rebuild/include again (plain `diff`). The only differences are the blocks below. CQ_GuildTrainingSkillScript.h is not in the repo yet.

| header | change | evidence | path |
|---|---|---|---|
| engine/CV_MayorsInvitationScript.h | adds nested `CLadyGrey` (Alloc 0x00EB78B0, GetParentScript 0x00EB7950, Init 0x00EB7960, Main 0x00EB9980, OnPredicateFail; ParentClass +0x14, PMasterData +0x18, HeroFollowed +0x1c; sizeof 0x20), defined after the class so the existing forward declaration stays | Ego_r.pdb layout (struct_layouts_egor.tsv, retail = PDB - 0x14); the landed Alloc allocates 0x20 and stores vtable 0x012F05F4 at 0x00EB78E6; both PDB name sets hold the ctor / Main / Init / OnPredicateFail | opt-in `FABLE_RETAIL_CV_MAYORSINVITATIONSCRIPT_LADYGREY`; one block appended |
| engine/NScript_CQR_EscortTraderScript.h | adds nested `CTraderToEscort : CEntityScriptBase` (Alloc 0x00E1E7A0, GetParentScript 0x00E1E840, GetRandomSpeech 0x00E23F30, Init 0x00E1F7E0, Main 0x00E20600, OnPredicateFail 0x00E1E850; ParentClass +0x14, PMasterData +0x18, lastRandomSpeechFollowIdx1/2 +0x1c/+0x20, lastRandomSpeechWaitIdx1/2 +0x24/+0x28; sizeof 0x2C) | Ego_r.pdb layout (member names as the PDB); vtable 0x012E151C (6 slots) stored by Alloc 0x00E1E7A0; both PDB name sets | opt-in `FABLE_RETAIL_CQR_ESCORTTRADERSCRIPT_TRADERTOESCORT`; one block before the final `#endif` |
| engine/NScript_CV_ChickenKickingScript.h | adds nested `CChickenMaster : CEntityScriptBase` (Alloc 0x00E634B0, GetParentScript 0x00E63550, OnPersist 0x00E641B0, Init 0x00E63560, Main 0x00E64FB0; ParentClass +0x14, PMasterData +0x18, HaveTalked / HeroHasPlayed / KickingChickens / GhostChat +0x1c..+0x1f; sizeof 0x20) | Ego_r.pdb layout; vtable 0x012E747C stored by Alloc 0x00E634B0; both PDB name sets | opt-in `FABLE_RETAIL_CV_CHICKENKICKINGSCRIPT_CHICKENMASTER`; one block appended |
| engine/CScriptGameResourceObjectScriptedThingBase.h | adds a `__forceinline operator=` (CBaseObject::operator= 0x0099A3B0, then the PImp share) | retail CChickenMaster::Main expands the assignment at 0x00E65052 (seh_ChickenMaster = seh_me) as the landed PlayCutscene 0x00E68B20 of the same TU does; the other NScript TUs call the implicit COMDAT 0x008ABD10 | opt-in `FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN`; one block inside the class |
| engine/CV_BanditTollScript.h | chunk 816 draft unchanged (CheckForBanditTollPaid, CTollBandit::SetUpBanditToll / Should_I_Flee) | see agent816/hdr/README.md | opt-in `FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT` |
| engine/CQ_GuildTrainingSkillScript.h | chunk 816 draft unchanged (NEW header: CQ_GuildTrainingSkillScript + CTheRealGuildmaster) | see agent816/hdr/README.md | whole header behind `FABLE_RETAIL_NSCRIPT` |

Includers rechecked: all 360 landed includers of the overlay set (rebuild/src/compiled, transitive through rebuild/include; listed in
../SIBLINGS.tsv: 359 for CScriptGameResourceObjectScriptedThingBase.h, 11 CV_BanditTollScript.h, 3 NScript_CV_ChickenKickingScript.h, 1 each
for the Mayor / EscortTrader headers, with overlap) were preprocessed with g++ -E, with and without `-I hdr`. Every output is identical (after
masking the `__LINE__`-based `fable_static_assert_<N>` typedef names) and non-empty. No landed source defines any of the opt-in macros, so the
default path cannot change their bytes. A VC7.1 vland recheck of them was not run because the preprocessed text is identical.
The parked rows compile only with the overlay, because they use the opt-in-only declarations.
