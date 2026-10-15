// CTCTalk::PlaySpeech(unsigned long sampleID, EDialogueLayer layer, bool b1, bool b2) -- forwards to the full PlaySpeech with this talker's own DialogueVoice
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3B10, length 32 bytes (ret 10h).
// Callee: CTCTalk::PlaySpeech(unsigned long, const CCharString&, EDialogueLayer, bool, bool) @ 0x006E3890 (ret 14h).
// Ego_r.pdb CTCTalk (0x3c bytes): DialogueVoice is the CCharString at +0x38, so the inserted second argument is
// `lea edx,[ecx+38h]`. `this` never leaves ecx (nothing clobbers it before the call), and the four caller
// arguments are re-pushed right to left around the inserted voice reference; the result is returned as is.
struct CCharString { void* m_pData; };
typedef long EDialogueLayer;
typedef long ESpeechPlaybackResult;

struct CTCTalk {
    unsigned char _base[0x38];               // CTCBase + SpeechSample .. UseLipSynching (see the PDB layout)
    CCharString   DialogueVoice;             // +0x38
    ESpeechPlaybackResult PlaySpeech(unsigned long sampleID, const CCharString& voice, EDialogueLayer layer, bool b1, bool b2);   // 0x006E3890
    ESpeechPlaybackResult PlaySpeech(unsigned long sampleID, EDialogueLayer layer, bool b1, bool b2);
};

ESpeechPlaybackResult CTCTalk::PlaySpeech(unsigned long sampleID, EDialogueLayer layer, bool b1, bool b2)
{
    return PlaySpeech(sampleID, DialogueVoice, layer, b1, b2);
}
