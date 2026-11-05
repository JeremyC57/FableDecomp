// Behaviour test for CTCQuestCompletionUI::OnSerialise(CPersistContext&, bool) (0x005E8220): a model of the row -- only an
// in-game serialise does anything; saving flattens the three per-quest start maps (experience, gold, renown) into parallel
// vectors keyed by the quest def, loading rebuilds all three maps from those vectors.
#include <stdio.h>
#include <map>
#include <vector>
struct Context {
    bool Saving;
    std::vector<long> Defs;
    std::vector<unsigned long> Experience, Gold, Renown;
};
struct QuestCompletionUI {
    std::map<long, unsigned long> StartQuestExperience, StartQuestGold, StartQuestRenown;
    void OnSerialise(Context& context, bool in_game)
    {
        if (!in_game) return;
        if (context.Saving) {
            context.Defs.clear(); context.Experience.clear(); context.Gold.clear(); context.Renown.clear();
            std::map<long, unsigned long>::iterator it;
            for (it = StartQuestExperience.begin(); it != StartQuestExperience.end(); ++it) context.Defs.push_back((*it).first);
            for (it = StartQuestExperience.begin(); it != StartQuestExperience.end(); ++it) context.Experience.push_back((*it).second);
            for (it = StartQuestGold.begin(); it != StartQuestGold.end(); ++it) context.Gold.push_back((*it).second);
            for (it = StartQuestRenown.begin(); it != StartQuestRenown.end(); ++it) context.Renown.push_back((*it).second);
        } else {
            for (unsigned long i = 0; i < context.Defs.size(); ++i) {
                StartQuestExperience[context.Defs[i]] = context.Experience[i];
                StartQuestGold[context.Defs[i]] = context.Gold[i];
                StartQuestRenown[context.Defs[i]] = context.Renown[i];
            }
        }
    }
};
int main()
{
    QuestCompletionUI a;
    a.StartQuestExperience[5] = 100; a.StartQuestGold[5] = 20; a.StartQuestRenown[5] = 3;
    a.StartQuestExperience[2] = 50; a.StartQuestGold[2] = 10; a.StartQuestRenown[2] = 1;
    Context c; c.Saving = true;
    a.OnSerialise(c, false);
    int ok = c.Defs.empty();
    a.OnSerialise(c, true);
    ok = ok && c.Defs.size() == 2 && c.Defs[0] == 2 && c.Experience[1] == 100 && c.Gold[0] == 10 && c.Renown[1] == 3;
    QuestCompletionUI b; c.Saving = false;
    b.OnSerialise(c, true);
    ok = ok && b.StartQuestExperience[2] == 50 && b.StartQuestGold[5] == 20 && b.StartQuestRenown[5] == 3 && b.StartQuestGold.size() == 2;
    printf(ok ? "005e8220_TEST PASS\n" : "005e8220_TEST FAIL\n");
    return ok ? 0 : 1;
}