#include <cstdio>
// Behaviour model of the CScriptConversationManager forwarders (0x006E5800 / 0x006E5950): find the conversation by index and
// forward to it, nothing when the index is unknown (the map is a small array here).
class CThing { public: int id; };
class CCharString { public: const char* text; };
class CScriptConversation { public:
    long index; CThing* lastPerson; CThing* lastSpeaker; CThing* lastListener; const CCharString* lastLine; bool lastCutScene; int calls;
    void AddPersonToConversation(CThing* person) { lastPerson = person; ++calls; }
    void AddLineToConversation(CThing* s, CThing* l, const CCharString* line, bool cut) { lastSpeaker = s; lastListener = l; lastLine = line; lastCutScene = cut; ++calls; }
};
class CScriptConversationManager { public:
    CScriptConversation Conversations[2]; int count;
    CScriptConversation* find(long key) { for (int i = 0; i < count; ++i) if (Conversations[i].index == key) return &Conversations[i]; return 0; }
    void AddPersonToConversation(long conversation, CThing* person) { CScriptConversation* it = find(conversation); if (it) it->AddPersonToConversation(person); }
    void AddLineToConversation(long conversation, CThing* s, CThing* l, const CCharString* line, bool cut) { CScriptConversation* it = find(conversation); if (it) it->AddLineToConversation(s, l, line, cut); }
};
int main()
{
    CScriptConversationManager m; m.count = 2; m.Conversations[0].index = 4; m.Conversations[1].index = 9;
    for (int i = 0; i < 2; ++i) { m.Conversations[i].calls = 0; m.Conversations[i].lastPerson = 0; m.Conversations[i].lastSpeaker = 0; }
    CThing a = { 1 }, b = { 2 }; CCharString line = { "Hello" };
    int ok = 1;
    m.AddPersonToConversation(9, &a); m.AddPersonToConversation(5, &b);
    ok = m.Conversations[1].lastPerson == &a && m.Conversations[1].calls == 1 && m.Conversations[0].calls == 0;
    if (!ok) { std::printf("006e5800_TEST FAIL\n"); return 1; }
    std::printf("006e5800_TEST PASS\n");
    return 0;
}