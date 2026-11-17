// CTCTalk::PlaySpeech(const CCharString& text, EDialogueLayer layer, bool, bool) -- look the text symbol up in the main text bank (by CRC) and play that group's speech in this speaker's own dialogue voice
// Leaf: ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@_N2@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3AB0: CMainGameComponent::Get()->GetTextBank(), NGameText::CDataBank /
//   CBankFile::FindIndexBySymbolCRC(text.GetCRC()), then the (index, voice) overload 0x006E3890 with DialogueVoice (+0x38).
// Identity (NF56, round 7, queue q10426): Ego_r.pdb 0x0070E010 / FableWin.pdb 0x023E6D50
//   ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@_N2@Z, between the (index,
//   voice) overload 0x006E3890 and the index overload 0x006E3B10 as in the Ego_r order. Was the address-named stand-in
//   CMemoryDataInputStream_006e3ab0::OpenStringSource (a local model of CTCTalk's +0x38 voice); now engine/CTCTalk.h (FABLE_RETAIL_CTCTALK,
//   overlay opt-in _PLAYSPEECH_OVERLOADS).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_PLAYSPEECH_OVERLOADS
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTalk.h"

CTCTalk::ESpeechPlaybackResult CTCTalk::PlaySpeech(const CCharString& text, NSpeechGainManager::EDialogueLayer layer, bool b1, bool b2)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    unsigned long textGroup = bank.FindIndexBySymbolCRC(text.GetCRC());
    return PlaySpeech(textGroup, DialogueVoice, layer, b1, b2);
}
