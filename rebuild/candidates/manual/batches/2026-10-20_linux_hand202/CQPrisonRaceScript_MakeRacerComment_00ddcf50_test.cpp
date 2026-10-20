// Behaviour test for CQ_PrisonRaceScript::MakeRacerComment (0x00DDCF50).
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CCharString {
    char Text[64];
    CCharString(const char* t) { std::strcpy(Text, t); }
};
static CCharString operator+(const char* left, const CCharString& right) { CCharString r(left); std::strcat(r.Text, right.Text); return r; }
struct CScriptThing {
    const char* Name;
    CScriptThing() : Name("NULL") {}
    CScriptThing(const char* n) : Name(n) {}
};
struct CGameScriptInterface {
    bool Active;
    long Next;
    bool IsConversationActive(long c) { char t[32]; std::sprintf(t, "Active(%ld)", c); Log(t); return Active; }
    long AddNewConversation(const CScriptThing& who, bool a, bool b) { char t[64]; std::sprintf(t, "New(%s,%d,%d)", who.Name, a, b); Log(t); return Next; }
    void AddLineToConversation(long c, const CCharString& line, bool a, const CScriptThing& from, const CScriptThing& to)
    { char t[128]; std::sprintf(t, "Line(%ld,%s,%d,%s,%s)", c, line.Text, a, from.Name, to.Name); Log(t); }
};
struct CQuestScriptBase { CGameScriptInterface* GSI; };
struct CQ_PrisonRaceScript : CQuestScriptBase {
    long ConversationIndex;
    bool MakeRacerComment(const CScriptThing& racer, const CCharString& line);
};
bool CQ_PrisonRaceScript::MakeRacerComment(const CScriptThing& racer, const CCharString& line)
{
    if (GSI->IsConversationActive(ConversationIndex))
        return false;
    long conversation = GSI->AddNewConversation(racer, false, false);
    GSI->AddLineToConversation(conversation, "TEXT_QST_055_RACER_" + line, false, racer, CScriptThing());
    ConversationIndex = conversation;
    return true;
}
int main()
{
    int bad = 0;
    CGameScriptInterface g; CQ_PrisonRaceScript q; q.GSI = &g; q.ConversationIndex = 3; g.Active = false; g.Next = 11;
    CScriptThing who("SPEAKER");
    if (!q.MakeRacerComment(who, CCharString("07"))) bad |= 1;
    if (std::strcmp(g_log, "Active(3);New(SPEAKER,0,0);Line(11,TEXT_QST_055_RACER_07,0,SPEAKER,NULL);") || q.ConversationIndex != 11) bad |= 2;
    g_log[0] = 0; g.Active = true; g.Next = 12;
    if (q.MakeRacerComment(who, CCharString("08")) || std::strcmp(g_log, "Active(11);") || q.ConversationIndex != 11) bad |= 4;
    if (bad) { std::printf("00ddcf50_TEST FAIL\n"); return 1; }
    std::printf("00ddcf50_TEST PASS\n");
    return 0;
}
