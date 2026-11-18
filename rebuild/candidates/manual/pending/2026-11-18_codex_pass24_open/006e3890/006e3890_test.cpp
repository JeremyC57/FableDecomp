#include <stdio.h>
#include <string>

struct CCharString {
    std::string Text;
    CCharString(const char* s=""):Text(s) {}
};
CCharString operator+(const char* prefix,const CCharString& suffix)
{
    return CCharString((std::string(prefix)+suffix.Text).c_str());
}
struct CWideString {
    unsigned long Length;
    explicit CWideString(unsigned long length=0):Length(length) {}
    unsigned long GetLength() const { return Length; }
};
namespace NGameText {
struct CTextEntry {
    CCharString SpeechBank,SoundName,Narrator;
    unsigned long TextLength;
    CTextEntry():SpeechBank("Speech"),SoundName("Hello"),Narrator("Narrator"),TextLength(20) {}
    CWideString GetText() const { return CWideString(TextLength); }
};
}
template<class T> struct CIVCountedPointer {
    T* Data;
    explicit CIVCountedPointer(T* data):Data(data) {}
    bool IsNull() const { return !Data; }
    T* Get() const { return Data; }
};
namespace NGameText {
struct CDataBank {
    CTextEntry Entry;
    bool Group,Missing,Found;
    CDataBank():Group(false),Missing(false),Found(false) {}
    int GetEntryDataType(unsigned long) const { return Group?1:0; }
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long,bool) const { return CIVCountedPointer<const CTextEntry>(Missing?0:&Entry); }
    const CTextEntry* GetTextEntryFromGroup(unsigned long,const CCharString&,bool& found,long* index,bool) const
    {
        found=Found;
        if(!Missing) *index=11;
        return Missing?0:&Entry;
    }
};
}
struct CMainGameComponent {
    const NGameText::CDataBank* PDataBank;
    static CMainGameComponent* Instance;
    static CMainGameComponent* Get() { return Instance; }
    const NGameText::CDataBank& GetTextBank() const { return *PDataBank; }
};
CMainGameComponent* CMainGameComponent::Instance=0;
bool g_bForceSpeech=false,g_bShowSpeechEmotes=false;
struct CASoundBank {
    long Index;
    CASoundBank():Index(3) {}
    long GetSoundIndexFromSymbol(const CCharString& name) { return name.Text=="SND_Hello"?Index:0; }
};
struct CASoundManager {
    CASoundBank Bank;
    CASoundBank* GetPSoundBank(const CCharString& name) { return name.Text=="Speech"?&Bank:0; }
};
CASoundManager* PSoundManager=0;
struct CTCEmoteIcon {
    enum { EMOTE_PRIORITY_DEBUG_LOW=7 };
    bool Replaceable,InvalidArgs;
    unsigned Calls;
    float Duration;
    CTCEmoteIcon():Replaceable(true),InvalidArgs(false),Calls(0),Duration(0) {}
    void SetReplaceable(bool replaceable) { Replaceable=replaceable; }
    void SetEmoteText(long priority,const CWideString&,float duration,float fade)
    {
        InvalidArgs=priority!=7||fade!=0.3f;
        Duration=duration; ++Calls;
    }
};
enum { TCI_EMOTEICON=0x45 };
struct CThing {
    bool HasEmote;
    CTCEmoteIcon Icon;
    CThing():HasEmote(true) {}
    bool HasTC(long id) const { return HasEmote && id==0x45; }
    CTCEmoteIcon& GetTC(long) { return Icon; }
};
namespace NSpeechGainManager { enum EDialogueLayer { DIALOGUE_LAYER_DEFAULT }; }
float GFLimitReturn(float value,float lo,float hi) { return value<lo?lo:value>hi?hi:value; }
struct CTCTalk {
    enum ESpeechPlaybackResult { SPEECH_PLAYBACK_STARTED,SPEECH_PLAYBACK_FAILED_FINAL,SPEECH_PLAYBACK_FAILED_RETRY };
    CThing* ParentThing;
    long LastTextIndex;
    unsigned Calls;
    bool BadArgs;
    ESpeechPlaybackResult PlaybackResult;
    explicit CTCTalk(CThing& speaker):ParentThing(&speaker),LastTextIndex(0),Calls(0),BadArgs(false),
        PlaybackResult(SPEECH_PLAYBACK_STARTED) {}
    ESpeechPlaybackResult PlaySpeech(CASoundBank&,long index,bool speech,const CCharString& narrator,NSpeechGainManager::EDialogueLayer,bool first,bool second)
    {
        ++Calls;
        if(index!=3 || speech || narrator.Text!="Narrator" || first || !second) BadArgs=true;
        return PlaybackResult;
    }
    ESpeechPlaybackResult PlaySpeech(unsigned long,const CCharString&,NSpeechGainManager::EDialogueLayer,bool,bool);
};

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

int main()
{
    NGameText::CDataBank entries;
    CMainGameComponent component={&entries};
    CMainGameComponent::Instance=&component;
    CThing speaker;
    CTCTalk talk(speaker);
    if(talk.PlaySpeech(0,"Hero",NSpeechGainManager::DIALOGUE_LAYER_DEFAULT,false,true)!=CTCTalk::SPEECH_PLAYBACK_FAILED_FINAL)
        return 1;
    entries.Missing=true;
    if(talk.PlaySpeech(12,"Hero",NSpeechGainManager::DIALOGUE_LAYER_DEFAULT,false,true)!=CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY)
        return 2;
    g_bForceSpeech=true;
    if(talk.PlaySpeech(12,"Hero",NSpeechGainManager::DIALOGUE_LAYER_DEFAULT,false,true)!=CTCTalk::SPEECH_PLAYBACK_FAILED_FINAL)
        return 3;
    g_bForceSpeech=false;
    entries.Group=true;
    entries.Found=true;
    if(talk.PlaySpeech(12,"Hero",NSpeechGainManager::DIALOGUE_LAYER_DEFAULT,false,true)!=CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY)
        return 4;
    entries.Missing=false;
    CASoundManager sounds;
    PSoundManager=&sounds;
    g_bShowSpeechEmotes=true;
    if(talk.PlaySpeech(12,"Hero",NSpeechGainManager::DIALOGUE_LAYER_DEFAULT,false,true)!=CTCTalk::SPEECH_PLAYBACK_STARTED ||
        talk.Calls!=1 || talk.LastTextIndex!=11 || talk.BadArgs || speaker.Icon.Calls)
        return 5;
    talk.PlaybackResult=CTCTalk::SPEECH_PLAYBACK_FAILED_FINAL;
    entries.Entry.TextLength=100;
    if(talk.PlaySpeech(12,"Hero",NSpeechGainManager::DIALOGUE_LAYER_DEFAULT,false,true)!=CTCTalk::SPEECH_PLAYBACK_STARTED ||
        talk.Calls!=2 || speaker.Icon.Calls!=1 || speaker.Icon.Replaceable || speaker.Icon.InvalidArgs || speaker.Icon.Duration!=6.0f)
        return 6;
    entries.Entry.TextLength=2;
    PSoundManager=0;
    talk.PlaySpeech(12,"Hero",NSpeechGainManager::DIALOGUE_LAYER_DEFAULT,false,true);
    if(speaker.Icon.Calls!=2 || speaker.Icon.Duration!=0.5f || talk.BadArgs)
        return 7;
    puts("006e3890_TEST PASS");
    return 0;
}
