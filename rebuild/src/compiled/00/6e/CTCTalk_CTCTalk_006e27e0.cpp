// CTCTalk::CTCTalk(CThing* owner) -- talk component constructor: CTCBase(owner), speech state cleared, attitude 1, no sample loaded (-1.0f), empty narrator and voice strings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E27E0, length 95 bytes (ret 4). Vtable 0x0125FFEC (CTCTalk).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CCharString::CCharString(const char*, long) @ 0x0099EBF0 (with the
// empty literal @ 0x0122D70E and -1), CCharString::CCharString() @ 0x0099E4B0. Created by Create_CTCTalk @ 0x004D2E0A
// (operator new(0x3c)).
// Ego_r.pdb CTCTalk (0x3c bytes): SpeechSample +0x0c, SampleStartedFrame +0x10, SampleDurationFrames +0x14, Attitude +0x18,
// MimeStartTime +0x1c, SampleLoadedTime +0x20, PSpeechBank +0x24, SampleID +0x28 (left uninitialised), LastTextIndex +0x2c,
// LastNarrator +0x30, IsPureAISpeaker +0x34, LipSynching +0x35, UseLipSynching +0x36, DialogueVoice +0x38.
// Shape: an initialiser list in declaration order; the scalars before LastNarrator are stored right after the vtable
// (sharing the ebx zero register) while LastNarrator's constructor arguments are already pushed; the bools between the
// two strings are stored before DialogueVoice's default constructor.
class CThing;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                     // +0x04
    bool    ToBeRemoved;                     // +0x08
    bool    IsInGlobalUpdateTCs;             // +0x09
    CTCBase(CThing* owner);                  // 0x00686800
};

class CCharString { public:
    void* m_pData;
    CCharString();                           // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
};

class CASoundBank;
typedef long EConversationAttitude;

class CTCTalk : public CTCBase { public:                   // vtable 0x0125FFEC
    long                  SpeechSample;          // +0x0c
    long                  SampleStartedFrame;    // +0x10
    long                  SampleDurationFrames;  // +0x14
    EConversationAttitude Attitude;              // +0x18
    float                 MimeStartTime;         // +0x1c
    float                 SampleLoadedTime;      // +0x20
    CASoundBank*          PSpeechBank;           // +0x24
    unsigned long         SampleID;              // +0x28
    long                  LastTextIndex;         // +0x2c
    CCharString           LastNarrator;          // +0x30
    bool                  IsPureAISpeaker;       // +0x34
    bool                  LipSynching;           // +0x35
    bool                  UseLipSynching;        // +0x36
    CCharString           DialogueVoice;         // +0x38
    virtual void v00();
    CTCTalk(CThing* owner);
};

CTCTalk::CTCTalk(CThing* owner)
    : CTCBase(owner)
    , SpeechSample(0)
    , SampleStartedFrame(0)
    , SampleDurationFrames(0)
    , Attitude(1)
    , MimeStartTime(0.0f)
    , SampleLoadedTime(-1.0f)
    , PSpeechBank(0)
    , LastTextIndex(0)
    , LastNarrator("")
    , IsPureAISpeaker(true)
    , LipSynching(false)
    , UseLipSynching(true)
{
}