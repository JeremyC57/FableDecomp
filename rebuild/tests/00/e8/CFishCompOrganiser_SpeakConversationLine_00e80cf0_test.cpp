// Behaviour test for 0x00E80CF0 CFishCompOrganiser::SpeakConversationLine(CCharString): Conversation stored, listener = hero, line added, by-value line destroyed once.
#include <cstdio>
static int g_dtors = 0;
class CCharString { public: const char* s; CCharString(const char* t) : s(t) {} CCharString(const CCharString& o) : s(o.s) {} ~CCharString() { ++g_dtors; } };
class CScriptThing { public: int id; };
struct Game {
    int listener, lines; const char* text; int a1, a2;
    int GetHero() { return 77; }
    int StartSpeech(const CScriptThing&, int a, int b) { a1 = a; a2 = b; return 9; }
    void SetSpeechListener(int speech, int who) { if (speech == 9) listener = who; }
    void AddSpeechLine(int speech, const CCharString& l, int, const CScriptThing&, int who) { if (speech == 9 && who == 77) { ++lines; text = l.s; } }
};
struct CFishCompOrganiser {
    Game* G; CScriptThing Me; int Conversation;
    void SpeakConversationLine(CCharString line)
    {
        Conversation = G->StartSpeech(Me, 0, 0);
        G->SetSpeechListener(Conversation, G->GetHero());
        G->AddSpeechLine(Conversation, line, 0, Me, G->GetHero());
    }
};
int main()
{
    Game g = { 0, 0, 0, -1, -1 };
    CFishCompOrganiser o; o.G = &g; o.Me.id = 1; o.Conversation = 0;
    {
        CCharString line("fish!");
        o.SpeakConversationLine(line);
        if (g_dtors != 1) { std::printf("00e80cf0_TEST FAIL\n"); return 1; }
    }
    if (o.Conversation != 9 || g.listener != 77 || g.lines != 1 || g.a1 != 0 || g.a2 != 0 || g.text[0] != 'f') {
        std::printf("00e80cf0_TEST FAIL\n"); return 1;
    }
    std::printf("00e80cf0_TEST PASS\n");
    return 0;
}