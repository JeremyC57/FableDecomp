# chunk 815 header overlay (hdr/engine)

Every file here is either a NEW header or the current repo copy plus an opt-in `#ifdef` block (pure additions: `diff` against
rebuild/include shows no removed or changed line). The default path of every existing header is byte-for-byte unchanged.

| header | kind | what / evidence | used by |
|---|---|---|---|
| engine/CQ_GuildTrainingMeleeScript.h | NEW, behind `FABLE_RETAIL_NSCRIPT` (no #error branch) | CQ_GuildTrainingMeleeScript : CLevelScriptBase<CQ_SunnyvaleMasterData>, fields +0x48..+0x56, sizeof 0x58 (Ego_r.pdb layout - 0x14, checked against the offsets Main reads); nested CTheRealGuildmaster (ParentClass +0x14, PMasterData +0x18, four bools +0x1c..+0x1f, sizeof 0x20); vtable 0x012D030C slots from Alloc 0x00D565C0; spellings from Ego_r.pdb + FableWin.pdb | src/00d58490.cpp (RELOCATION_MATCH) |
| engine/CV_BeardyBaldyScript.h | NEW, behind `FABLE_RETAIL_NSCRIPT` (no #error branch) | CV_BeardyBaldyScript : CLevelScriptBase<CQ_SunnyvaleMasterData>, fields +0x48..+0xa0, sizeof 0xA4; parent helpers ResetRandomSpeechTime 0x00E53C70, SetWanderPointAndDistance 0x00E53F60, WatchForAttack 0x00E53D90, IsHeroWearingAnyBeard/Tash/OddHairdo 0x00E53670/0x00E538F0/0x00E53AD0; nested CBB_BeardyBaldyMan (GetRandomSpeech 0x00E53CB0, lastRandomSpeechIdx1/2 +0x1c/+0x20, sizeof 0x24) | differ/00e50fd0.cpp |
| engine/CQ_TraderConflictGoodScript.h | opt-in `FABLE_RETAIL_CQ_TRADERCONFLICTGOODSCRIPT_TRADERTORESCUE` | defines CTraderToRescue (ParentClass +0x14, PMasterData +0x18, BarIndex +0x1c, sizeof 0x20; vtable 0x012DEC68 from Alloc 0x00DFB050; landed Init 0x00DFB1B0 clears BarIndex) | differ/00dfe0f0.cpp |
| engine/CV_ArcheryCompetitionScript.h | opt-in `FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CACOWNER_MAIN` | forward CScriptGameResourceObjectScriptedThingBase; parent ResetRandomSpeechTime 0x00E337C0, WatchForAttack(CScriptThing) 0x00E36390, DoTimesUp 0x00E36590, DoCountDown(long) 0x00E36750; `private: CCharString GetRandomSpeech();` 0x00E362B0 in CAC_Owner (PDB spellings; the call sites in CAC_Owner::Main) | differ/00e33ea0.cpp |
| engine/CV_BodyGuardScript.h | opt-in `FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN` | includes CEntityScriptBase/CScriptThing inside the block; nested CBodyGuard (Ego_r.pdb 60 bytes: HenchManType +0x1c .. MyHealthBarID +0x38; vtable 0x012E701C from the ctor 0x00E5DDE0; SetHenchmanAsHired(bool) 0x00E621F0, GetGoldCost 0x00E5FC70) | differ/00e5fd40.cpp |
| engine/CQ_HobbeCaveScript.h | opt-in `FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CBANDITSACRIFICE_MAIN` | defines CBanditSacrifice after the existing forward declaration (Ego_r.pdb 60 bytes: CurrentAIState +0x1c .. FollowingCommentsSaid +0x38, ImprisonedFX a CScriptThing at +0x28; vtable 0x012D5A94 from Alloc 0x00D90E90) | differ/00d973d0.cpp |

Includers rechecked read-only with this overlay (recheck.py + `vland.py --recheck-landed`, log tmp/recheck_includers.log, before the
work dirs were deleted): all 12 landed includers of CQ_HobbeCaveScript.h, CQ_TraderConflictGoodScript.h, CV_ArcheryCompetitionScript.h
and CV_BodyGuardScript.h MATCH / RELOCATION_MATCH + PASS (SIBLINGS.tsv). The two new headers have no repo includer.
Dropped: a probe opt-in `FABLE_RETAIL_CSPAWNEDFUNC1_PLAIN_INLINE_CTOR` in engine/CSpawnedFunc1.h (no byte effect; not handed back).
