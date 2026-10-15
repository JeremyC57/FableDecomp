// Behaviour test for CTCTalk::PlayAISpeech(name, force, layer, b1, b2) @ 0x006E3B30: gate on IsPureAISpeaker || force, layer pinned to 1.
#include <cstdio>
struct CCharString { const char* m_pData; };
struct CTCTalk {
    bool IsPureAISpeaker; int calls; long lastLayer; bool lastB1, lastB2; const CCharString* lastName;
    long PlaySpeech(const CCharString& name, long layer, bool b1, bool b2)
    { ++calls; lastName = &name; lastLayer = layer; lastB1 = b1; lastB2 = b2; return 0; }
    long PlayAISpeech(const CCharString& name, bool force, long layer, bool b1, bool b2)
    {
        if (!IsPureAISpeaker && !force) return 1;
        return PlaySpeech(name, 1, b1, b2);
    }
};
int main()
{
    CTCTalk t; t.calls = 0; t.IsPureAISpeaker = false; CCharString n; n.m_pData = "LINE";
    if (t.PlayAISpeech(n, false, 5, true, true) != 1 || t.calls != 0) { std::printf("006e3b30_TEST FAIL\n"); return 1; }
    if (t.PlayAISpeech(n, true, 5, true, false) != 0 || t.calls != 1 || t.lastLayer != 1 || t.lastName != &n || !t.lastB1 || t.lastB2) { std::printf("006e3b30_TEST FAIL\n"); return 1; }
    t.IsPureAISpeaker = true;
    if (t.PlayAISpeech(n, false, 9, false, true) != 0 || t.calls != 2 || t.lastLayer != 1) { std::printf("006e3b30_TEST FAIL\n"); return 1; }
    std::printf("006e3b30_TEST PASS\n");
    return 0;
}