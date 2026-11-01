# Naming pass wave 19 (2026-11-01): 136 landed rows respelled -- PeekThingSearchTools const, the c684/c685/c680 fixes, Transfer<T>, CBasicString::Compare

Inputs: brief `naming/briefs/BRIEF_W19.md`, QUEUE_W19_input.tsv (32 rows: 520 x1, c684 x11, c685 x20; the c686 rows arrived after the wave
closed), wave 18 DEFERRED.tsv. One Opus 5.5 agent authored the overlay in `$S/naming/w19` (AGENT_README.md; scripts/ here; the wave was
interrupted once by the account's usage limit and resumed from its saved state); the coordinator copied ov_final (143 files), applied the 9
RENAMES (git mv + build_candidates.ps1), BINDINGS (50 RETIRE / 37 ADD / 12 KEEP, 0 CONFLICT) and LABELS (7), and rechecked every changed row
against HEAD (`logs/coordinator_recheck.log`; agent's own run `logs/recheck_final.log` 136/136, 3 MATCH + 133 RELOCATION_MATCH).
`bindings_multi_check.py` 0. Name-only except the re-authored rows below, all byte-identical.

Families, committed in this order (`runs/<fam>.rows.txt`): rfail 2 (the ~vector<pair<long,CCharString>> destroy chain re-authored on the STLport
4.6 text: `__forceinline` _Destroy/__destroy so the __false_type temporary is retail's [esp+7] byte), stools 27 (`protected: const
CThingSearchTools& PeekThingSearchTools() const` with const CThingSearchTools members; plus the 0x007FD110 filter is CThingFilter_HasScriptName in
six callers that named it HasDefGlobalIndex / HasTrackName -- the one bindings CONFLICT removed), c684 8 (ProcessDisposalList, CTCDExperienceOrb::
Create, both NVectorMath overloads, bool ApplyRamps), gif 10 (nested CGraphicInfoFrame::CFrameData, private const InternalGetFrameForcedLoading),
c685 20 (ImeUi_Initialize / ImeUiCallback_DrawFans, extern "C" AMovieSetupRegisterFilter, CCountedPointer<CBoneInfo>::DeleteData, CEaxContext::
SetSlotLoadEffect / SetSlotProperty, AddSaleItemPlacesFromThing, CBankFile::GetEntryDataSize const; own rows 0x00AB3640, 0x0099F560
RemoveWhiteSpaceBeginEnd, 0x00A89320 / 0x00A89350 const, 0x00AB1480 std::__copy), c680 5 (C3DVector::Cross, CWorldMap::GetThemeSizeZAt, volatile
ExitThread, 0x00C73770 re-authored over DetectWindowsVersion), xfer 26 (CPersistContext `template <class T> void Transfer(T&)`), cmp 38
(CBasicString<char>::Compare as a protected static of the template).
Queue outcomes (QUEUE_W19.tsv): 15 respelled, 3 partial, 10 already-correct, 4 deferred. Decisions: `DECISIONS_W19_RESOLVED.tsv`.
Deferred (DEFERRED.tsv, 440 rows): 0x007E5230 pgui (register allocation under the class-static model), the 13 GetPWorldSeed rows (blocked on the
engine-header migration), PeekTypeList / HasTC / STLport set callers / 0x00CAF680 / the destructor families / CODA globals / 0x01238C8C, the mega
families AddEvent / GetInstance / GetPItem / ~CCharString (per-row class or namespace re-models) -> wave 20 with the c686-c689 queue rows.
