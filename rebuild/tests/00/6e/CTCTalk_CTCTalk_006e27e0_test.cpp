// Behaviour test for CTCTalk::CTCTalk @ 0x006E27E0: initial member values.
#include <cstdio>
#include <cstring>
class CThing { public: int id; };
class CCharString { public: const char* m_pData; CCharString() : m_pData("") {} CCharString(const char* s, long = -1) : m_pData(s) {} };
class CTCBase { public: CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; CTCBase(CThing* o) : ParentThing(o), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {} };
class CTCTalk : public CTCBase { public:
    long SpeechSample, SampleStartedFrame, SampleDurationFrames, Attitude; float MimeStartTime, SampleLoadedTime; void* PSpeechBank;
    unsigned long SampleID; long LastTextIndex; CCharString LastNarrator; bool IsPureAISpeaker, LipSynching, UseLipSynching; CCharString DialogueVoice;
    CTCTalk(CThing* o) : CTCBase(o), SpeechSample(0), SampleStartedFrame(0), SampleDurationFrames(0), Attitude(1), MimeStartTime(0.0f),
        SampleLoadedTime(-1.0f), PSpeechBank(0), LastTextIndex(0), LastNarrator(""), IsPureAISpeaker(true), LipSynching(false), UseLipSynching(true) {}
};
int main()
{
    CThing th; th.id = 1; CTCTalk t(&th);
    if (t.ParentThing != &th || t.SpeechSample || t.SampleStartedFrame || t.SampleDurationFrames || t.Attitude != 1 || t.MimeStartTime != 0.0f
        || t.SampleLoadedTime != -1.0f || t.PSpeechBank || t.LastTextIndex || std::strcmp(t.LastNarrator.m_pData, "") || !t.IsPureAISpeaker
        || t.LipSynching || !t.UseLipSynching) { std::printf("006e27e0_TEST FAIL\n"); return 1; }
    std::printf("006e27e0_TEST PASS\n");
    return 0;
}