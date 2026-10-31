// behaviour test for CQuestManager::OnEntityInterrupted 0x004AFBB0: only quests whose script exists and has the thing's
// binding name bound are told, with the thing's 64-bit unique id
#include <cstdio>
#include <string>
#include <vector>
struct Script { std::vector<std::string> bound; std::vector<unsigned long long> got;
    bool IsBound(const std::string& n) const { for (size_t i = 0; i < bound.size(); ++i) if (bound[i] == n) return true; return false; } };
struct Thing { unsigned long long id; std::string name; };
static void OnEntityInterrupted(std::vector<Script*>& quests, const Thing& t)
{
    const std::string& name = t.name; const unsigned long long& uid = t.id;
    for (size_t i = 0; i < quests.size(); ++i)
        if (quests[i] && quests[i]->IsBound(name)) quests[i]->got.push_back(uid);
}
int main()
{
    Script a, b; a.bound.push_back("Guard"); b.bound.push_back("Other");
    std::vector<Script*> q; q.push_back(&a); q.push_back(0); q.push_back(&b);
    Thing t = { 0x100000002ULL, "Guard" };
    OnEntityInterrupted(q, t);
    int ok = a.got.size() == 1 && a.got[0] == 0x100000002ULL && b.got.empty();
    std::printf(ok ? "004afbb0_TEST PASS\n" : "004afbb0_TEST FAIL\n");
    return !ok;
}