// CTCTalk::PlayAISpeech(const CCharString& name, bool force, EDialogueLayer layer, bool b1, bool b2) -- plays a named line on the AI layer, only for a pure-AI speaker or when forced
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3B30, length 48 bytes (ret 14h).
// Callee: CTCTalk::PlaySpeech(const CCharString&, EDialogueLayer, bool, bool) @ 0x006E3AB0 (ret 10h; it resolves the
// name to a sample id through the world's sound manager and calls the full PlaySpeech @ 0x006E3890).
// Ego_r.pdb CTCTalk: IsPureAISpeaker is the bool at +0x34. When the talker is not a pure-AI speaker and the
// caller did not force it, nothing is played and result 1 is returned (`mov eax,1 ; ret 14h`); otherwise the
// line is played with the layer pinned to 1 -- the caller's layer argument is ignored (its slot is never read).
// Shape: the two byte tests are `if (!IsPureAISpeaker && !force) return 1;` with the forward call the fall-through.
struct CCharString { void* m_pData; };
typedef long EDialogueLayer;
typedef long ESpeechPlaybackResult;

struct CTCTalk {
    unsigned char _base[0x34];               // CTCBase + SpeechSample .. LastNarrator
    bool          IsPureAISpeaker;           // +0x34
    bool          LipSynching;               // +0x35
    bool          UseLipSynching;            // +0x36
    unsigned char _pad_0x37[1];
    CCharString   DialogueVoice;             // +0x38
    ESpeechPlaybackResult PlaySpeech(const CCharString& name, EDialogueLayer layer, bool b1, bool b2);   // 0x006E3AB0
    ESpeechPlaybackResult PlayAISpeech(const CCharString& name, bool force, EDialogueLayer layer, bool b1, bool b2);
};

ESpeechPlaybackResult CTCTalk::PlayAISpeech(const CCharString& name, bool force, EDialogueLayer layer, bool b1, bool b2)
{
    if (!IsPureAISpeaker && !force)
        return 1;
    return PlaySpeech(name, 1, b1, b2);
}
