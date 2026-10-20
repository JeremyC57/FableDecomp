// Behaviour test for CQ_PrisonEscapeRescueMotherScript::MakePrisonerComment (0x00DD5860).
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
struct CQ_PrisonEscapeRescueMotherScript : CQuestScriptBase {
    long ConversationIndex;
    bool MakePrisonerComment(const CScriptThing& prisoner, const CCharString& line);
};
bool CQ_PrisonEscapeRescueMotherScript::MakePrisonerComment(const CScriptThing& prisoner, const CCharString& line)
{
    if (GSI->IsConversationActive(ConversationIndex))
        return false;
    long conversation = GSI->AddNewConversation(prisoner, false, false);
    GSI->AddLineToConversation(conversation, "TEXT_QST_055_PRISONER_" + line, false, prisoner, CScriptThing());
    ConversationIndex = conversation;
    return true;
}
int main()
{
    int bad = 0;
    CGameScriptInterface g; CQ_PrisonEscapeRescueMotherScript q; q.GSI = &g; q.ConversationIndex = 3; g.Active = false; g.Next = 11;
    CScriptThing who("SPEAKER");
    if (!q.MakePrisonerComment(who, CCharString("07"))) bad |= 1;
    if (std::strcmp(g_log, "Active(3);New(SPEAKER,0,0);Line(11,TEXT_QST_055_PRISONER_07,0,SPEAKER,NULL);") || q.ConversationIndex != 11) bad |= 2;
    g_log[0] = 0; g.Active = true; g.Next = 12;
    if (q.MakePrisonerComment(who, CCharString("08")) || std::strcmp(g_log, "Active(11);") || q.ConversationIndex != 11) bad |= 4;
    if (bad) { std::printf("00dd5860_TEST FAIL\n"); return 1; }
    std::printf("00dd5860_TEST PASS\n");
    return 0;
}
