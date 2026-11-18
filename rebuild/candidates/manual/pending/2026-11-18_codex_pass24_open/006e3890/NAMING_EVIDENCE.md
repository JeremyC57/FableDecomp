# 0x006E3890 naming evidence

The selected RTM object emits `?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@KABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@_N2@Z`. Ego_r donor 0x0070DC40 and FableWin donor 0x023E6DE0 carries this exact decorated spelling.
The Ego_d module record identifies `CTCTalk::PlaySpeech` in `tc_talk.obj`. The five-argument overload is the 1,046-byte Ego_d debug record. It records `text_index`, `narrator_name`, `dialogue_layer`, `force_synchronous`, `use_lipsycing`, `text_bank`, `ret`, `ptext_entry`, `tptext_entry`, `entry_exists`, `ptc_emote_icon` and `text_str`.
Retail directly calls this member at 0x006E3B02 in landed 0x006E3AB0 CTCTalk::PlaySpeech.
Retail directly calls this member at 0x006E3B28 in landed 0x006E3B10 CTCTalk::PlaySpeech.

## Shared headers

The selected source uses row opt-ins in `CTCEmoteIcon.h`, `GFLimit.h`, `NGameText_CDataBank.h`. See the history receipt for the exact leaf and the relocation report for operand evidence.

## Behavioral fixture

The selected body checks an empty index, unavailable direct and group entries, forced failures, sound playback, emote fallback, and both duration clamps. The pointer, text bank, sound bank, and emote renderer are fixture substitutes.
