// Behaviour test for CTCTalk::Persist @ 0x006E2CB0: two transfers (IsPureAISpeaker default true, DialogueVoice) only when flagged.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* m_pData; };
class CPersistContext { public:
    int count; const char* names[2]; bool defaultSeen;
    void Transfer(const char* name, bool& value, const bool& def) { names[count++] = name; defaultSeen = def; value = !value; }
    void Transfer(const char* name, CCharString* value) { names[count++] = name; value->m_pData = "LOADED"; }
};
class CTCTalk { public:
    bool IsPureAISpeaker; CCharString DialogueVoice;
    void Persist(CPersistContext* c, bool flag)
    { if (flag) { c->Transfer("IsPureAISpeaker", IsPureAISpeaker, true); c->Transfer("DialogueVoice", &DialogueVoice); } }
};
int main()
{
    CPersistContext c; c.count = 0; c.defaultSeen = false; CTCTalk t; t.IsPureAISpeaker = false; t.DialogueVoice.m_pData = "X";
    t.Persist(&c, false);
    if (c.count != 0) { std::printf("006e2cb0_TEST FAIL\n"); return 1; }
    t.Persist(&c, true);
    if (c.count != 2 || std::strcmp(c.names[0], "IsPureAISpeaker") || std::strcmp(c.names[1], "DialogueVoice") || !c.defaultSeen
        || !t.IsPureAISpeaker || std::strcmp(t.DialogueVoice.m_pData, "LOADED")) { std::printf("006e2cb0_TEST FAIL\n"); return 1; }
    std::printf("006e2cb0_TEST PASS\n");
    return 0;
}