#include <cstdio>
#include <string>
#include <map>
#include <list>
// Behaviour test for CQuestManager::AddBoast (0x004B1720): the boast's text is the bank text for the tag (else the tag
// widened) and its title the bank text for tag + "_TITLE" (else the TEXT tag widened); the boast is appended with
// Failed/Completed clear and the boast frame is stamped.
struct CBoast { long ID, Cost, Reward; std::string ScriptName, Text, Title; bool Evil, Failed, Completed; long GroupIndex; };
static std::map<std::string, std::string> g_Bank;
static long g_Frame = 500;
struct CQuestManager {
    unsigned long FrameBoastsLastUpdated;
    std::list<CBoast> Boasts;
    static std::string Lookup(const std::string& tag, const std::string& fallback)
    {
        std::map<std::string, std::string>::const_iterator it = g_Bank.find(tag);
        return it != g_Bank.end() ? it->second : fallback;
    }
    void AddBoast(const std::string& scriptName, const std::string& textTag, long id, long cost, long reward, bool evil,
                  long groupIndex)
    {
        std::string text = Lookup(textTag, textTag);
        std::string titleTag = textTag + "_TITLE";
        std::string title = Lookup(titleTag, textTag);
        CBoast b = { id, cost, reward, scriptName, text, title, evil, false, false, groupIndex };
        Boasts.push_back(b);
        FrameBoastsLastUpdated = g_Frame;
    }
};
int main()
{
    g_Bank["BOAST_NAKED"] = "Fight unarmoured"; g_Bank["BOAST_NAKED_TITLE"] = "Naked";
    g_Bank["BOAST_NOHIT"] = "Take no hits";
    CQuestManager m; m.FrameBoastsLastUpdated = 0;
    m.AddBoast("Q_Orchard", "BOAST_NAKED", 3, 100, 400, false, 1);
    g_Frame = 501;
    m.AddBoast("Q_Orchard", "BOAST_NOHIT", 4, 50, 200, true, 2);
    const CBoast& a = m.Boasts.front();
    const CBoast& b = m.Boasts.back();
    bool ok = m.Boasts.size() == 2 && a.Text == "Fight unarmoured" && a.Title == "Naked" && a.ID == 3 && !a.Evil
              && b.Text == "Take no hits" && b.Title == "BOAST_NOHIT" && b.Evil && !b.Failed && !b.Completed
              && b.GroupIndex == 2 && m.FrameBoastsLastUpdated == 501;
    if (!ok) { std::printf("004b1720_TEST FAIL\n"); return 1; }
    std::printf("004b1720_TEST PASS\n");
    return 0;
}
