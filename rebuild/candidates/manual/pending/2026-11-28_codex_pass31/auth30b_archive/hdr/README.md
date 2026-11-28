# AUTH30 header overlay (FABLE_HDR_OVERLAY=$S/w22/auth30/hdr)
Each file is the repo header (rebuild/include/<path>) plus Codex pass 29's PARTIAL_PATCHES opt-ins, applied by tools/applyp.py (orig/ keeps
the repo copy). Every addition sits under an `#ifdef FABLE_RETAIL_*` that only the named rows define; with no macro defined each header
compiles as the repo's. FABLE_STATIC_ASSERT -> FABLE_STATIC_ASSERT_AT(<original line>, ...) edits are the project's pin_asserts pins (they keep
the line-keyed assert names after lines are added), not content changes. stlport/fable_map.h line 146 is split so an opt-in
`__declspec(noinline)` can sit between `template<>` and `inline`; unchanged text when the macro is off.
| file | opt-in | rows |
|---|---|---|
| engine/CAnimSet.h, engine/CAnimateThing.h, engine/CTCSoundPlayer.h, engine/CTCTalk.h | FABLE_RETAIL_TALK_UPDATE_PASS29 | 006e3b90 |
| engine/CCardDef.h | FABLE_RETAIL_CCARDDEF_CTOR_PASS29 | 007d75a0 |
| engine/CTCTavernGameCardPairs.h | FABLE_RETAIL_TAVERN_PAIRS_RANDOM_PASS29 | 007d75a0 |
| engine/CCountedPointer.h | FABLE_RETAIL_COUNTED_DESTRUCTOR_INLINE_PASS29 | 00d06130 |
| engine/CQ_BanditCampBossBattleScript.h | FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_TYPED_CTORS_PASS29 (+ ASSASSIN2_PARENT_DECL_PASS29) | 00d05a60, 00d06130 |
| engine/CQ_BanditCampBossBattleScript.h | FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_INCLASS_MAIN_PASS30 (AUTH30, round 2): CAssassin2/3 ::Main defined in the class (original declaration kept in #else) + the body's includes; evidence FableWin marked-inline + hpp line records (PAIR_FACTS.md) | 00d05a60 g5, 00d06130 g2 |
| engine/CDiskFileWin32.h | FABLE_RETAIL_TGA_DISK_POSITION_PASS29 | 009f4590 |
| engine/SurfaceTGAPixels.h | NEW header; FABLE_RETAIL_TGA_PIXEL_RECORDS_PASS29 (CATGAPixel, CNATGAPixel, CRGBALong; no repo definition, CRGBColour.h has only a forward decl) | 009f4590 |
| engine/CHeroSoulsCreatureDef.h | FABLE_RETAIL_DEFS_FILL_INSERT_PASS29, FABLE_RETAIL_WAVE_DEFAULT_DTOR_PASS29 | 00f277f0 |
| engine/CLevelScriptNoMasterBase.h | FABLE_RETAIL_LEVEL_NOMASTER_ORIGINAL_REFERENCE_PASS29 (Game as reference; original kept in #else) | 00ee91e0 |
| engine/CGlobal_DebugCycleThroughSpeechScript.h | NEW header (no repo class definition) | 00ee91e0 |
| stlport/fable_map.h | FABLE_RETAIL_SPEECH_GLOBAL_NOINLINE26, _EXPAND_INCREMENT26, _CTOR_DECL26, _SET_DTOR26 | 00ee91e0 |
| engine/CQ_EndGameFocalSitesScript.h | FABLE_RETAIL_END_GAME_BATTLE_MINION_PASS29 | 00d2d1a0 |
| engine/CQ_MinionClifftopChaseScript.h, engine/fable_nscript_forward.h | FABLE_RETAIL_MINION_GUARD_CHATTER_PASS29 | 00da71e0 |
| engine/EAIStateGroupType.h | NEW header with the four Ego_d enumerators. The repo has forward decls (`enum EAIStateGroupType;`, 36 lines) and an empty `enum EAIStateGroupType { };` in fable_nscript_forward.h's portable path; no enumerators anywhere. fable_nscript_forward.h includes it in both paths under the MINION opt-in | 00da71e0 |
| engine/CQ_UndeadRisingScript.h | FABLE_RETAIL_UNDEAD_RISING_FIREFLY_PASS29 (enum names in #ifdef, original in #else) | 00e0ca50 |
Removed: my own stlport/fable_containers.h size-body opt-in for 005458a0 (it made the copy EQ but broke the leaf; deleted with its orig/).
No row has matched, so none of these is proposed for the repo yet.
