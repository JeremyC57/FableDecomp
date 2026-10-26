// Behaviour test for CTCTalk::PlayAISpeech(id, force, layer, b1, b2) @ 0x006E3B60: gate, own voice, layer pinned to 1.
#include <cstdio>
class CCharString { public: const char* m_pData; };
class CTCTalk { public:
    bool IsPureAISpeaker; CCharString DialogueVoice; int calls; unsigned long lastId; long lastLayer; const CCharString* lastVoice; bool lastB2;
    long PlaySpeech(unsigned long id, const CCharString& voice, long layer, bool b1, bool b2)
    { ++calls; lastId = id; lastVoice = &voice; lastLayer = layer; lastB2 = b2; return b1 ? 3 : 4; }
    long PlayAISpeech(unsigned long id, bool force, long layer, bool b1, bool b2)
    {
        if (!IsPureAISpeaker && !force) return 1;
        return PlaySpeech(id, DialogueVoice, 1, b1, b2);
    }
};
int main()
{
    CTCTalk t; t.calls = 0; t.IsPureAISpeaker = false;
    if (t.PlayAISpeech(10, false, 2, true, true) != 1 || t.calls != 0) { std::printf("006e3b60_TEST FAIL\n"); return 1; }
    t.IsPureAISpeaker = true;
    if (t.PlayAISpeech(11, false, 2, true, true) != 3 || t.lastId != 11 || t.lastLayer != 1 || t.lastVoice != &t.DialogueVoice || !t.lastB2) { std::printf("006e3b60_TEST FAIL\n"); return 1; }
    std::printf("006e3b60_TEST PASS\n");
    return 0;
}