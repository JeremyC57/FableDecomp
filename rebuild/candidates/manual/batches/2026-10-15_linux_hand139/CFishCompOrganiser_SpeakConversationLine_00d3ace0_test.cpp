// Behaviour test for 0x00D3ACE0 SpeakConversationLine (FireHeart entity): speech started by Me (1, 0), listener = hero, parent's line added, flag cleared.
#include <cstdio>
struct CCharString { const char* s; };
struct CScriptThing { int id; };
struct Game {
    int started, listener, lines; const CCharString* line; const CScriptThing* speaker; int a1, a2;
    int GetHero() { return 77; }
    int StartSpeech(const CScriptThing& me, int a, int b) { ++started; speaker = &me; a1 = a; a2 = b; return 5; }
    void SetSpeechListener(int speech, int who) { if (speech == 5) listener = who; }
    void AddSpeechLine(int speech, const CCharString& l, int, const CScriptThing& me, int who) { if (speech == 5 && who == 77 && &me == speaker) { ++lines; line = &l; } }
};
struct Parent { bool* shouldSpeak; CCharString line; };
struct Entity {
    Game* G; CScriptThing Me; Parent* ParentClass; int MyID;
    void SpeakConversationLine()
    {
        int speech = G->StartSpeech(Me, 1, 0);
        G->SetSpeechListener(speech, G->GetHero());
        G->AddSpeechLine(speech, ParentClass->line, 0, Me, G->GetHero());
        *(ParentClass->shouldSpeak + MyID) = false;
    }
};
int main()
{
    bool flags[3] = { true, true, true };
    Game g = { 0, 0, 0, 0, 0, 0, 0 };
    Parent p; p.shouldSpeak = flags; p.line.s = "hello";
    Entity e; e.G = &g; e.Me.id = 1; e.ParentClass = &p; e.MyID = 1;
    e.SpeakConversationLine();
    if (g.started != 1 || g.a1 != 1 || g.a2 != 0 || g.listener != 77 || g.lines != 1 || g.line != &p.line || !flags[0] || flags[1] || !flags[2]) {
        std::printf("00d3ace0_TEST FAIL\n"); return 1;
    }
    std::printf("00d3ace0_TEST PASS\n");
    return 0;
}
