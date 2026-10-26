#include <cstdio>
#include <cstring>
// Behaviour test for CQuestDesc::CQuestDesc (0x0047BF90): id 999, default text, status 0, empty objectives.
class CCharString { public: const char* p; long n; CCharString(const char* t, long len) : p(t), n(len < 0 ? (long)std::strlen(t) : len) { } };
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { } };
struct CQuestDesc {
    unsigned long ScriptID; CCharString TextDescription; long QuestStatus; vector<int> QuestSubObjectives;
    CQuestDesc() : ScriptID(999), TextDescription("No description yet", -1), QuestStatus(0), QuestSubObjectives() { }
};
int main()
{
    CQuestDesc d;
    if (d.ScriptID != 999 || std::strcmp(d.TextDescription.p, "No description yet") != 0 || d.TextDescription.n != 18 ||
        d.QuestStatus != 0 || d.QuestSubObjectives._M_start != 0 || d.QuestSubObjectives._M_end_of_storage != 0) { std::printf("0047bf90_TEST FAIL\n"); return 1; }
    std::printf("0047bf90_TEST PASS\n");
    return 0;
}