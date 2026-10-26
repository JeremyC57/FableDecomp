#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
// Behaviour test for CQ_MinionClifftopChaseScript::GuardComment (0x00DA8970): a random follower speaks
// TEXT_QST_067_<its data string>_<comment> to the hero, but only when that text entry exists and the thread lives on.
struct CScriptThing { std::string Data; std::string GetDataString() const { return Data; } };
struct CGSI {
    std::vector<CScriptThing> Followers; std::string Known; int Conversations; int People; std::string LastLine; CScriptThing Hero;
    CScriptThing& GetHero() { return Hero; }
    bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing>& out) { out = Followers; return true; }
    bool TextEntryExists(const std::string& tag) { return tag == Known; }
    long AddNewConversation(const CScriptThing&, bool, bool) { return ++Conversations; }
    void AddPersonToConversation(long, const CScriptThing&) { ++People; }
    void AddLineToConversation(long, const std::string& line, bool, const CScriptThing&, const CScriptThing&) { LastLine = line; }
};
struct CQ_MinionClifftopChaseScript {
    CGSI* GSI; bool Terminating;
    bool IsActiveThreadTerminating() { return Terminating; }
    void GuardComment(std::string comment)
    {
        std::vector<CScriptThing> followers;
        GSI->GetFollowingEntityList(GSI->GetHero(), followers);
        if (followers.size() != 0) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing guard = followers[std::rand() % followers.size()];
            if (GSI->TextEntryExists("TEXT_QST_067_" + guard.GetDataString() + "_" + comment)) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = GSI->AddNewConversation(guard, false, false);
                GSI->AddPersonToConversation(conversation, GSI->GetHero());
                GSI->AddLineToConversation(conversation, "TEXT_QST_067_" + guard.GetDataString() + "_" + comment, false, guard, GSI->GetHero());
            }
        }
    }
};
int main()
{
    CGSI gsi; gsi.Conversations = 0; gsi.People = 0;
    CQ_MinionClifftopChaseScript s; s.GSI = &gsi; s.Terminating = false;
    s.GuardComment("HELLO");                            // no followers: nothing
    CScriptThing g; g.Data = "GUARD1"; gsi.Followers.push_back(g);
    gsi.Known = "TEXT_QST_067_GUARD1_HELLO";
    s.GuardComment("BYE");                              // text entry missing: nothing
    bool ok = gsi.Conversations == 0;
    s.GuardComment("HELLO");
    ok = ok && gsi.Conversations == 1 && gsi.People == 1 && gsi.LastLine == "TEXT_QST_067_GUARD1_HELLO";
    s.Terminating = true; s.GuardComment("HELLO");
    ok = ok && gsi.Conversations == 1;
    if (!ok) { std::printf("00da8970_TEST FAIL\n"); return 1; }
    std::printf("00da8970_TEST PASS\n");
    return 0;
}