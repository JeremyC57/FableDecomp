// Behaviour test for CTCActionUseSearch::OnActionUse (0x007EB6A0): a copy of the decision logic over a plain model.
#include <cstdio>
#include <cstring>
struct Model {
    bool hasMeshInfo; float zEdge;
    bool heroPresent, heroDead, inCutscene, heroHasHeroTC, tutorialShown;
    bool hasContainer, containerReward;
    bool hasOwned, ownerPresent;
    bool theft, userHasDeedLog, heroHasStats;
    long morality, theftMoralityChange;
    bool userHasStealth; long currentAction;
    // outputs
    const char* anim; int searched, messages, deeds; long permittedAction;
};
static void OnActionUse(Model& m)
{
    float height = m.hasMeshInfo ? m.zEdge : 2.0f;
    m.anim = (height > 1.15f) ? "SEARCH_HIGH" : "SEARCH_LOW";
    if (m.heroPresent && !m.heroDead && !m.inCutscene && m.heroHasHeroTC && m.tutorialShown)
        return;
    bool hadReward = false;
    if (m.hasContainer) { hadReward = m.containerReward; ++m.searched; }
    if (m.hasOwned && m.ownerPresent) ++m.messages;
    if (m.theft && m.userHasDeedLog) {
        ++m.deeds;
        if (hadReward && m.heroPresent && !m.heroDead && m.heroHasStats)
            m.morality = m.theftMoralityChange + m.morality;
    }
    if (m.userHasStealth) m.permittedAction = m.currentAction;
}
int main()
{
    Model m; std::memset(&m, 0, sizeof m);
    m.hasMeshInfo = true; m.zEdge = 0.5f; m.heroPresent = true; m.heroHasHeroTC = true; m.tutorialShown = false;
    m.hasContainer = true; m.containerReward = true; m.hasOwned = true; m.ownerPresent = true;
    m.theft = true; m.userHasDeedLog = true; m.heroHasStats = true; m.morality = 10; m.theftMoralityChange = -3;
    m.userHasStealth = true; m.currentAction = 7; m.permittedAction = -1;
    OnActionUse(m);
    bool ok = std::strcmp(m.anim, "SEARCH_LOW") == 0 && m.searched == 1 && m.messages == 1 && m.deeds == 1 && m.morality == 7 &&
              m.permittedAction == 7;
    Model n; std::memset(&n, 0, sizeof n);
    n.hasMeshInfo = false; n.heroPresent = true; n.heroHasHeroTC = true; n.tutorialShown = true; n.hasContainer = true;
    n.userHasStealth = true; n.currentAction = 5; n.permittedAction = -1;
    OnActionUse(n);
    ok = ok && std::strcmp(n.anim, "SEARCH_HIGH") == 0 && n.searched == 0 && n.permittedAction == -1;
    n.inCutscene = true; OnActionUse(n);
    ok = ok && n.searched == 1 && n.permittedAction == 5 && n.deeds == 0;
    if (!ok) { std::printf("007eb6a0_TEST FAIL\n"); return 1; }
    std::printf("007eb6a0_TEST PASS\n");
    return 0;
}