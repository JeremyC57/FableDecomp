// Behaviour test for CTCTalk::DoesSpeechExist @ 0x006E2910: the data bank's `found` flag is the result.
#include <cstdio>
struct CCharString { const char* m_pData; };
struct CDataBank {
    unsigned long known; const CCharString* group;
    void* GetTextEntryFromGroup(unsigned long id, const CCharString& g, bool* found, long* index, bool flag)
    { group = &g; if (index || flag) return 0; if (id == known) { *found = true; return this; } return 0; }
};
struct CTCTalk {
    CCharString DialogueVoice; CDataBank* bank;
    bool DoesSpeechExist(unsigned long id)
    { CDataBank* b = bank; bool found = false; b->GetTextEntryFromGroup(id, DialogueVoice, &found, 0, false); return found; }
};
int main()
{
    CDataBank bank; bank.known = 12; bank.group = 0; CTCTalk t; t.bank = &bank;
    if (!t.DoesSpeechExist(12) || bank.group != &t.DialogueVoice || t.DoesSpeechExist(13)) { std::printf("006e2910_TEST FAIL\n"); return 1; }
    std::printf("006e2910_TEST PASS\n");
    return 0;
}