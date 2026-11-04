// Behaviour test for CQuestManager::FailAllActiveQuests (0x004B2AD0): a model of the row -- the active-quest list is copied first,
// then every copied quest that has a script is failed silently (show screen false, empty reason, silent true); the source list is
// left alone even though failing a quest would change it.
#include <stdio.h>
#include <list>
#include <string>
#include <vector>
struct Quest { std::string Name; bool HasScript; };
struct Manager {
    std::list<Quest*> Active;
    std::vector<std::string> Failed;
    void SetQuestAsFailed(const std::string& name, bool show, const std::wstring& reason, bool silent)
    {
        if (!show && reason.empty() && silent) Failed.push_back(name);
        for (std::list<Quest*>::iterator it = Active.begin(); it != Active.end(); ++it)
            if ((*it)->Name == name) { Active.erase(it); break; }
    }
    void FailAllActiveQuests()
    {
        std::list<Quest*> quests(Active);
        for (std::list<Quest*>::iterator it = quests.begin(); it != quests.end(); ++it)
            if ((*it)->HasScript) { std::wstring reason(L""); SetQuestAsFailed((*it)->Name, false, reason, true); }
    }
};
int main()
{
    Quest a = { "A", true }, b = { "B", false }, c = { "C", true };
    Manager m; m.Active.push_back(&a); m.Active.push_back(&b); m.Active.push_back(&c);
    m.FailAllActiveQuests();
    int ok = m.Failed.size() == 2 && m.Failed[0] == "A" && m.Failed[1] == "C" && m.Active.size() == 1 && m.Active.front() == &b;
    printf(ok ? "004b2ad0_TEST PASS\n" : "004b2ad0_TEST FAIL\n");
    return ok ? 0 : 1;
}