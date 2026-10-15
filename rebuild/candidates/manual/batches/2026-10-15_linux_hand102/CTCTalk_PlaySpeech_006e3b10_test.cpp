// Behaviour test for CTCTalk::PlaySpeech(id, layer, b1, b2) @ 0x006E3B10: forwards with the talker's own DialogueVoice.
#include <cstdio>
struct CCharString { const char* m_pData; };
struct CTCTalk {
    CCharString DialogueVoice;
    unsigned long lastId; const CCharString* lastVoice; long lastLayer; bool lastB1, lastB2;
    long PlaySpeech(unsigned long id, const CCharString& voice, long layer, bool b1, bool b2)
    { lastId = id; lastVoice = &voice; lastLayer = layer; lastB1 = b1; lastB2 = b2; return 7; }
    long PlaySpeech(unsigned long id, long layer, bool b1, bool b2) { return PlaySpeech(id, DialogueVoice, layer, b1, b2); }
};
int main()
{
    CTCTalk t; t.DialogueVoice.m_pData = "VOICE";
    long r = t.PlaySpeech(42, 3, true, false);
    if (r != 7 || t.lastId != 42 || t.lastVoice != &t.DialogueVoice || t.lastLayer != 3 || !t.lastB1 || t.lastB2) { std::printf("006e3b10_TEST FAIL\n"); return 1; }
    std::printf("006e3b10_TEST PASS\n");
    return 0;
}
