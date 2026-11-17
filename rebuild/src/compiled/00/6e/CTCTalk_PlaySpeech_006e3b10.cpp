// CTCTalk::PlaySpeech(unsigned long textGroup, EDialogueLayer layer, bool, bool) -- play the text group's speech in this speaker's own dialogue voice
// Leaf: ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@KW4EDialogueLayer@NSpeechGainManager@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3B10: forwards to the (index, voice) overload 0x006E3890 with DialogueVoice (+0x38).
// Identity (NF56, round 7, queue q10426): Ego_r.pdb 0x0070E050 / FableWin.pdb 0x023E6DA0
//   ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@KW4EDialogueLayer@NSpeechGainManager@@_N1@Z; its callee 0x006E3890 is Ego_r.pdb
//   0x0070DC40 / FableWin.pdb 0x023E6DE0 ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@KABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@_N2@Z.
//   Was a local CTCTalk model (long result / layer typedefs); now engine/CTCTalk.h (FABLE_RETAIL_CTCTALK, overlay opt-in
//   _PLAYSPEECH_OVERLOADS).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_PLAYSPEECH_OVERLOADS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTalk.h"

CTCTalk::ESpeechPlaybackResult CTCTalk::PlaySpeech(unsigned long textGroup, NSpeechGainManager::EDialogueLayer layer, bool b1, bool b2)
{
    return PlaySpeech(textGroup, DialogueVoice, layer, b1, b2);
}
