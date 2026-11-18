// CTCTalk::PlaySpeech(unsigned long, const CCharString&, EDialogueLayer, bool, bool) -- look up the text entry (group or voiced), play its SND_ sample from the entry's speech bank and show the line as an emote
// Grade: DIFFER(546v529) with 11 strict instruction hunks.
// Leaf: ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@KABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@_N2@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E3890, 529 bytes, `ret 14h`; caller 0x006E3B10 (landed).

#define FABLE_RETAIL_CTCTALK_VOICED_GROUP_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTC_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT_CLEAR
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_PLAYSPEECH_OVERLOADS
#define FABLE_RETAIL_CTCTALK_CONVERSATION
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_NGAMETEXT_TEXTENTRY_SPEECH
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CTCTalk.h"
#include "engine/CASoundManager.h"
#include "engine/CASoundBank.h"
#include "engine/CTCEmoteIcon.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFLimit.h"

extern CASoundManager* PSoundManager;
extern bool g_bForceSpeech;
extern bool g_bShowSpeechEmotes;

CTCTalk::ESpeechPlaybackResult CTCTalk::PlaySpeech(unsigned long text_index, const CCharString& narrator_name,
    NSpeechGainManager::EDialogueLayer dialogue_layer, bool force_synchronous, bool use_lipsycing)
{
    const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
    if (!text_index)
        return SPEECH_PLAYBACK_FAILED_FINAL;
    const NGameText::CTextEntry* ptext_entry;
    if (text_bank.GetEntryDataType(text_index) == 0) {
        CIVCountedPointer<const NGameText::CTextEntry> tptext_entry =
            text_bank.GetPTextEntry(text_index, force_synchronous || g_bForceSpeech);
        if (tptext_entry.IsNull()) {
            if (force_synchronous || g_bForceSpeech)
                return SPEECH_PLAYBACK_FAILED_FINAL;
            return SPEECH_PLAYBACK_FAILED_RETRY;
        }
        ptext_entry = tptext_entry.Get();
    } else {
        bool entry_exists = false;
        ptext_entry = text_bank.GetTextEntryFromGroup(text_index, narrator_name, entry_exists, &LastTextIndex,
            force_synchronous || g_bForceSpeech);
        if (!ptext_entry && entry_exists) {
            if (force_synchronous || g_bForceSpeech)
                return SPEECH_PLAYBACK_FAILED_FINAL;
            return SPEECH_PLAYBACK_FAILED_RETRY;
        }
    }
    if (!ptext_entry)
        return SPEECH_PLAYBACK_FAILED_FINAL;
    if (PSoundManager) {
        CASoundBank* sound_bank = PSoundManager->GetPSoundBank(ptext_entry->SpeechBank);
        if (sound_bank) {
            long index = sound_bank->GetSoundIndexFromSymbol("SND_" + ptext_entry->SoundName);
            if (index > 0) {
                if (PlaySpeech(*sound_bank, index, false, ptext_entry->Narrator, dialogue_layer, false,
                    use_lipsycing) != SPEECH_PLAYBACK_FAILED_FINAL)
                    return SPEECH_PLAYBACK_STARTED;
            }
        }
    }
    if (g_bShowSpeechEmotes && ParentThing->HasTC(TCI_EMOTEICON)) {
        CTCEmoteIcon& ptc_emote_icon = static_cast<CTCEmoteIcon&>(ParentThing->GetTC(TCI_EMOTEICON));
        CWideString text_str = ptext_entry->GetText();
        float hi = 6.0f;
        float lo = 0.5f;
        float duration = text_str.GetLength() * 0.1f;
        ptc_emote_icon.SetEmoteText(CTCEmoteIcon::EMOTE_PRIORITY_DEBUG_LOW, text_str, GFLimitReturn(duration, lo, hi), 0.3f);
        ptc_emote_icon.SetReplaceable(false);
    }
    return SPEECH_PLAYBACK_STARTED;
}
