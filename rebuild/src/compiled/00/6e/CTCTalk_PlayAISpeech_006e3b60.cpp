// CTCTalk::PlayAISpeech(unsigned long sampleID, bool force, EDialogueLayer layer, bool b1, bool b2) -- plays a sample on the AI layer in this talker's own voice, only for a pure-AI speaker or when forced
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3B60, length 43 bytes (ret 14h via the callee).
// Callee: CTCTalk::PlaySpeech(unsigned long, const CCharString&, EDialogueLayer, bool, bool) @ 0x006E3890 (ret 14h).
// Ego_r.pdb CTCTalk: IsPureAISpeaker +0x34, DialogueVoice (CCharString) +0x38. Same gate as the named overload
// @ 0x006E3B30 (result 1 when nothing is played). The forward has the same argument count as this function, so
// VC7.1 tail-jumps: it overwrites the dead `force` slot with &DialogueVoice and the ignored `layer` slot with
// the constant layer 1, keeps sampleID/b1/b2 in place and `jmp`s to PlaySpeech.
class CCharString { public: void* m_pData; };
typedef long EDialogueLayer;
typedef long ESpeechPlaybackResult;

class CTCTalk { public:
    unsigned char _base[0x34];               // CTCBase + SpeechSample .. LastNarrator
    bool          IsPureAISpeaker;           // +0x34
    bool          LipSynching;               // +0x35
    bool          UseLipSynching;            // +0x36
    unsigned char _pad_0x37[1];
    CCharString   DialogueVoice;             // +0x38
    ESpeechPlaybackResult PlaySpeech(unsigned long sampleID, const CCharString& voice, EDialogueLayer layer, bool b1, bool b2);   // 0x006E3890
    ESpeechPlaybackResult PlayAISpeech(unsigned long sampleID, bool force, EDialogueLayer layer, bool b1, bool b2);
};

ESpeechPlaybackResult CTCTalk::PlayAISpeech(unsigned long sampleID, bool force, EDialogueLayer layer, bool b1, bool b2)
{
    if (!IsPureAISpeaker && !force)
        return 1;
    return PlaySpeech(sampleID, DialogueVoice, 1, b1, b2);
}