# PARKED (owner wind-down, 2026-11-20): 0x006E5A00 CScriptConversation::FrameUpdate -- OPEN, draft does not compile yet

State: 0 grades of the new text (the only grade on file is the old baseline, 163v1772). `draft_v2.cpp` is a shared-header port
written from the retail disassembly (`retail_dis.txt`, 604 lines) and the Ego_d locals (`egod_locals.txt`); it was compiled twice
with tools/cc.py and still has accessor-name errors (below). Nothing here was graded, tested or run through vland.
To resume: copy `hdr/engine/*.h` back into `W/hdr/engine/` (they were moved out so the live overlay holds only b9..b12 files).

## What is established (from the disassembly; each item has its address)
- 1772 bytes, ebp frame with `sub esp,1Ch` (the frame comes from GFFloatToLongCeil's fistp locals), esi = this.
- Out-of-line callees: IsValid 0x006E5600 (Ego_r/FableWin ?IsValid@CScriptConversation@@ABE_NXZ), GetCurrentSpeaker 0x006E55B0,
  GetCurrentListener 0x006E55D0 (both private, PDB names), CScriptConversationManager::RemoveConversation 0x006E5990 (index, false),
  CTCTalk::StopCurrentSpeech 0x006E2E60, IsSpeechSamplePlaying 0x006E2D30, IsSpeechSampleLoadedAndPlaying 0x006E2DC0,
  PlaySpeech 0x006E3580, PlayRandomLipMovement 0x006E2A50, CTCTargetingBase::SetPScriptOverrideInterestingThing 0x007F5890,
  CTCLook::TrackNearestInterestingThing 0x00753840, CBankFile::LookupValue 0x009CC410 / GetEntryDataType 0x009CC210,
  CDataBank::GetPGroupEntry 0x009C8890 / GetPTextEntry 0x009C9070, CTextEntry::GetText 0x009C7A40, CWorld::GetWorldSeed 0x0049D810,
  GFRandom 0x004CDAE0 (fastcall), CASoundManager::GetPSoundBank(const CCharString&) 0x009917E0 / GetPLipSyncBank 0x009916F0,
  CASoundBank::GetSoundIndexFromSymbol 0x00A01920, CLipSyncDataBank::PeekLipSyncInfo 0x00A42830, CSubtitleRenderer::SetText 0x00492E4B,
  operator+(const char*, const CCharString&) 0x0099F690, operator+(const wchar_t*, const CWideString&) 0x0099BFF0,
  CCharString::ToWideString 0x0099E480, CCharString::GetCRC 0x00404310.
- PER-SITE INLINE SPLIT (the hard part): GetCurrentSpeaker is CALLED once (0x006E5A49, the cutscene block) and EXPANDED four times
  (0x006E5AAF, 0x006E5B5B, 0x006E5C25, 0x006E601D); GetCurrentListener is CALLED twice (0x006E5BC5, 0x006E5F8D) and EXPANDED once
  (0x006E6086, the last use). Expanded speaker body: `if (CurrentLine > -1) return Participants[Conversation[CurrentLine]
  .PersonSpeaking].GetPItem(); return 0`; listener adds `&& PersonSpokenTo > -1`. Likely the inline budget (try /d2inlT windows
  before anything else); the draft header carries both bodies as private inlines.
- Blocks, in order: IsValid else RemoveConversation; `!PlayDuringCutScenes` && world virtual slot 3 (+0xC) -> byte +0x54
  (draft: GetThingManager().IsNonScriptedEntitesPauseMode(), PDB name, to be verified) -> stop speech of an alive speaker,
  RemoveConversation, return; `decrement_frame_counter = CurrentLine == -1 || StartedCurrentLineSpeech`, else const CTCTalk PeekTC
  (const map find 0x004365B0) and `StartedCurrentLineSpeech = !IsSpeechSamplePlaying() || IsSpeechSampleLoadedAndPlaying()`;
  FramesLeftBeforeNewLine-- and SetAsUpdatingTimer(true) (PlayerGui +0xFF) when the line shows a subtitle;
  `ready_for_next_line = CurrentLine == -1` (sete), else `FramesLeftBeforeNewLine <= 0` (setle) and both targeting overrides
  cleared; next line: CurrentLine++, `(unsigned) CurrentLine < Conversation.size()` else RemoveConversation + return;
  StartedCurrentLineSpeech = true; speech_duration = 2.5f; text bank lookup by the line's CRC; group entry (type 1) -> random member;
  ptext_entry (released AFTER the FramesLeft store: its scope is the whole `text_index > 0` block); sound bank by
  entry->SpeechBank (+0x1C), "SND_" + GetSoundName() (+0x24), lip-sync duration (+4), PlaySpeech(bank, index, false,
  GetNarrator() (+0x20), layer 1, SoundIn2D, true); subtitle SetText(PeekCutSceneMainFont(), text, 2, speech_duration, false, true)
  then SetAsUpdatingTimer(false); `FramesLeftBeforeNewLine = GFFloatToLongCeil((speech_duration + 0.5f) * GetConstantFPS())`
  (fadd 0.5f, fimul FPS); else arm: PlayRandomLipMovement, `L"(HC)" + LineToSay.ToWideString()` (the ToWideString temporary dies
  before SetText), `GFFloatToLongCeil(GetConstantFPS() * speech_duration)` with 2.5f folded (0x01260080); listener alive and
  != talker -> the two targeting overrides; tail: CTCLook::TrackNearestInterestingThing(true) for speaker and alive listener.
- Ego_d (VC10 /Od, lines 126..378) is a LATER source version here: it has a `speech_played` bool and two `text_of_message` blocks
  that retail does not have (retail has one subtitle block per arm). Use Ego_d for names and scoping only.

## Compile errors left in draft_v2.cpp (accessor spellings, not structure)
- line 140: `GetPGroupEntry(..)->GetTextEntries()` (the group class needs its opt-in: FABLE_RETAIL_NGAMETEXT_GROUPENTRY);
  line 141: `CArray<long>::GetSize` -> use the header's size accessor; `CMainGameComponent::GetWorld` is reported missing there
  (not looked into: an opt-in or a cascade from line 140).
- line 145: `if (ptext_entry)` -> CIVCountedPointer has no bool conversion; use `!ptext_entry.IsNull()` (FableWin ?IsNull@).
- lines 147 / 154: `CASoundManager::Get()` does not exist; landed rows use the global `PSoundManager` (0x013B8394).
- line 151: CASoundBank needs its opt-in for GetSoundIndexFromSymbol (engine/CASoundBank.h).

## Draft overlay (hdr/, hdr.diff; add-only, NOT part of any READY batch)
- engine/CScriptConversation.h  FABLE_RETAIL_CSCRIPTCONVERSATION_CLASS: typed class (nested CConversationLine {PersonSpeaking,
  PersonSpokenTo, LineToSay, ShowSubtitle}; members with real vectors; sizeof 0x2C), `#else` generated layout.
- engine/CScriptConversationManager.h  FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_REMOVECONVERSATION: declaration (0x006E5990).
- engine/NGameText_CDataBank.h  FABLE_RETAIL_NGAMETEXT_TEXTENTRY_SPEECH_ACCESSORS: inline GetNarrator / GetSoundName (FableWin names).
- NEW engine/CLipSyncDataBankEntry.h: Data +0, Duration +4, inline GetDuration (FableWin), IsLoaded / BeginLoading declared.
  Would newly flag landed rows that define the class locally. Known: 0x00A42980 (BeginLoading). Candidates, not checked (their
  files name the class): 0x00A42AC0, 0x00A42FC0, 0x00A42EA0, 0x00A42B80.
