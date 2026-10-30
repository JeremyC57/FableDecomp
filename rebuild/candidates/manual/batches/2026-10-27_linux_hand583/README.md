# Chunk 583 (2026-10-30): 8 of 10 quest-script-range released pass 9 rows landed

All RELOCATION_MATCH + PASS (g++ and VC7.1), on the retail NScript headers. Rows: CV_TourGuideScript::Main,
CV_IntroductionToTrophiesScript::WatchForCrowdVolume, CV_TalentlessBardScript::CTB_Bard::Init, CV_ParanoidWhispersScript::Main,
CV_TempleOfLightScript::OnPersist, CQ_ArenaScript::WatchForTermination, CQR_EscortTraderScript::OnPersist,
CV_MayorsInvitationScript::WatchForTermination.
Parked (../../pending/2026-10-27_linux_hand583/differ/), both on per-exit-site destructor inlining that retail decided
differently: 0x00EE91E0 CGlobal_DebugCycleThroughSpeechScript::Main (575v613; retail destroys a local set<unsigned long>
three ways: inlined ~set -> clear 0x004CFF70 + ~_Rb_tree_base 0x004CFFB0 at in-loop exits, 0x004D0740 at the pre-loop exit,
0x004D0E20 at the end; explicit/__forceinline ~set, real STLport allocator chain, visible _M_erase, QFE-4035 and /GL+/LTCG
all tried) and 0x00D2D1A0 CQ_EndGameFocalSitesScript::CBattleMinion::Main (556v620; retail inlines
~CScriptGameResourceObjectScriptedThingBase with its own epilogue at the first early return only).
Header debt (NOTES.md): local CSpawnedFunc<T> over the shared CSpawnedFuncBase (no shared template yet), local
CPersistContext (as landed 0x00E4F7F0), opaque CScriptDef / CQ_SunnyvaleMasterData; the parked rows also model CBaseObject /
CScriptGameResourceObjectBase / ...ScriptedThingBase locally (flat shared headers) -- the wave 4 header candidates.
NAME_FIXES.tsv: 12 rows queued (GetDefaultVal<EscortTraderQuestStage>, GFIntToCharString, _Rb_global<bool>::_M_increment
with ~25 wrong spellings, the set<unsigned long> tree members, ~CScriptGameResourceObjectScriptedThingBase,
IsPerformingScriptTask UBE).
