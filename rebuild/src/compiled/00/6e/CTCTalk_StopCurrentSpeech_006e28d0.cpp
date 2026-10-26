// CTCTalk::StopCurrentSpeech(CTCSoundPlayer* emitter) -- stops the playing sample, ends the frame window now and resets the speech state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E28D0, length 59 bytes (ret 4).
// Callees: the emitter's stop-sample member @ 0x0066ADD0 (takes the sample id; manifest label
// "CGameScriptThing::SetToKillOnLevelUnload" is propagated), the world frame getter @ 0x00416225 on g_pWorld @ 0x013B86A0.
// Ego_r.pdb CTCTalk: SpeechSample +0x0c, SampleStartedFrame +0x10, SampleDurationFrames +0x14, MimeStartTime +0x1c,
// SampleLoadedTime +0x20 (float, reset to -1.0f), SampleID +0x28, LipSynching +0x35.
// Shape: the duration is cut to `frame - started - 1` so IsTalking() turns false at once; the zero stores share
// one `xor eax,eax` register (the bool, the sample, the mime time and the id) while -1.0f is an immediate.
class CTCSoundPlayer { public:
public:
    void StopAttachedSound(unsigned long);   // 0x0066ADD0
};
class CWorld { public:
    long GetFrame();                         // 0x00416225
};
extern CWorld* g_pWorld;                     // 0x013B86A0

class CTCTalk { public:
    unsigned char _base[0x0c];               // CTCBase: vptr, ParentThing, flags
    long          SpeechSample;              // +0x0c
    long          SampleStartedFrame;        // +0x10
    long          SampleDurationFrames;      // +0x14
    long          Attitude;                  // +0x18 EConversationAttitude
    float         MimeStartTime;             // +0x1c
    float         SampleLoadedTime;          // +0x20
    void*         PSpeechBank;               // +0x24 CASoundBank*
    unsigned long SampleID;                  // +0x28
    long          LastTextIndex;             // +0x2c
    unsigned char LastNarrator[4];           // +0x30 CCharString
    bool          IsPureAISpeaker;           // +0x34
    bool          LipSynching;               // +0x35
    bool          UseLipSynching;            // +0x36
    void StopCurrentSpeech(CTCSoundPlayer* emitter);
};

void CTCTalk::StopCurrentSpeech(CTCSoundPlayer* emitter)
{
    emitter->StopAttachedSound(SampleID);
    SampleDurationFrames = g_pWorld->GetFrame() - SampleStartedFrame - 1;
    LipSynching = false;
    SpeechSample = 0;
    MimeStartTime = 0.0f;
    SampleLoadedTime = -1.0f;
    SampleID = 0;
}