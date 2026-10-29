// Behaviour test for 0x00D00440 NScript::CQ_AwakeningTheOracleScript::CLostBayCutsceneTrigger::Main: unless the Lost Bay
// cutscene has played, persist Me, age the hero by 1, play CS_LOSTBAY_INTRO, mark it played, pause, start the message
// conversation and wait on it; always finish with RemoveThing(Me, false, true) unless the thread terminates first.
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
class CScriptThing { public: const char* Name; };

class CGameScriptInterface { public:
    CScriptThing Hero; float Age; int ActiveAfter; int Frames;
    void NewScriptFrame() const { ++const_cast<CGameScriptInterface*>(this)->Frames; Log("frame"); }
    CScriptThing& GetHero() { return Hero; }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { char x[64]; std::sprintf(x, "remove(%s,%d,%d)", t.Name, a, b); Log(x); }
    void SetHeroAge(float a) const { const_cast<CGameScriptInterface*>(this)->Age = a; Log("age"); }
    float GetHeroAge() const { return Age; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) const { char x[64]; std::sprintf(x, "conv(%s,%d,%d)", t.Name, a, b); Log(x); return 7; }
    void AddLineToConversation(long c, const CCharString& s, bool a, const CScriptThing& p, const CScriptThing& q) const
    { char x[128]; std::sprintf(x, "line(%ld,%s,%d,%s,%s)", c, s.Text, a, p.Name, q.Name); Log(x); }
    bool IsConversationActive(long c) const { return c == 7 && Frames >= ActiveAfter; }
    void Pause(float s) const { char x[32]; std::sprintf(x, "pause(%d)", (int)s); Log(x); }
    void SetThingPersistent(const CScriptThing& t, bool p) const { char x[64]; std::sprintf(x, "persist(%s,%d)", t.Name, p); Log(x); }
};

struct CQ_AwakeningTheOracleScript {
    bool PlayedLostBayCS;
    void PlayCutscene(CCharString name, bool wait) { char x[64]; std::sprintf(x, "cs(%s,%d)", name.Text, wait); Log(x); }
};

struct CLostBayCutsceneTrigger {
    CGameScriptInterface* Game; CScriptThing Me; CQ_AwakeningTheOracleScript* ParentClass; int TermAt, TermCalls;
    bool IsActiveThreadTerminating() const { CLostBayCutsceneTrigger* s = const_cast<CLostBayCutsceneTrigger*>(this); return s->TermAt && ++s->TermCalls >= s->TermAt; }
    void Main()
    {
        if (!ParentClass->PlayedLostBayCS) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetThingPersistent(Me, true);
            Game->SetHeroAge(Game->GetHeroAge() + 1.0f);
            ParentClass->PlayCutscene("CS_LOSTBAY_INTRO", true);
            ParentClass->PlayedLostBayCS = true;
            Game->Pause(10.0f);
            long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
            Game->AddLineToConversation(conversation, "TEXT_QST_B03_BRIAR_LOSTBAY_MESSAGE", false, Game->GetHero(), Game->GetHero());
            while (!Game->IsConversationActive(conversation)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->RemoveThing(Me, false, true);
    }
};

int main()
{
    CGameScriptInterface game; game.Hero.Name = "hero"; game.Age = 20.0f; game.ActiveAfter = 2; game.Frames = 0;
    CQ_AwakeningTheOracleScript quest; quest.PlayedLostBayCS = false;
    CLostBayCutsceneTrigger t; t.Game = &game; t.Me.Name = "me"; t.ParentClass = &quest; t.TermAt = 0; t.TermCalls = 0;
    t.Main();
    int ok = !std::strcmp(g_log, "persist(me,1);age;cs(CS_LOSTBAY_INTRO,1);pause(10);conv(hero,0,0);"
                                 "line(7,TEXT_QST_B03_BRIAR_LOSTBAY_MESSAGE,0,hero,hero);frame;frame;remove(me,0,1);")
             && quest.PlayedLostBayCS && game.Age == 21.0f;
    g_log[0] = 0; t.Main();   // already played: only the removal
    ok = ok && !std::strcmp(g_log, "remove(me,0,1);");
    g_log[0] = 0; quest.PlayedLostBayCS = false; t.TermAt = 1; t.TermCalls = 0; t.Main();   // terminating at once
    ok = ok && g_log[0] == 0;
    if (!ok) { std::printf("00d00440_TEST FAIL\n"); return 1; }
    std::printf("00d00440_TEST PASS\n");
    return 0;
}