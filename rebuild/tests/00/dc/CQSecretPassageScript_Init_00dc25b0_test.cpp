// Behaviour test for the graveyard quest script Init (0x00DC25B0): three AddQuestRegion calls with the quest
// name and the region names, every temporary destroyed, then the parent flag and both state bools cleared.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct CGameScriptInterface {
    int calls; const char* quests[3]; const char* regions[3]; int liveAtCall;
    virtual void AddQuestRegion(const CCharString& quest, const CCharString& region)
    { quests[calls] = quest.PStringData; regions[calls] = region.PStringData; liveAtCall = g_live; ++calls; }
};
struct CGraveyardParentData { bool m_bRegionScriptActive; };
struct CQ_SecretPassageScript {
    CGameScriptInterface* m_pInterface;
    CGraveyardParentData* m_pParent;
    bool QuestStartScreenShown;
    bool HaveOpenedDemonDoor;
    void Init();
};
void CQ_SecretPassageScript::Init()
{
    m_pInterface->AddQuestRegion("Q_OpeningGraveyardSecretPassage", "Graveyard");
    m_pInterface->AddQuestRegion("Q_OpeningGraveyardSecretPassage", "GraveyardPath");
    m_pInterface->AddQuestRegion("Q_OpeningGraveyardSecretPassage", "GraveyardCircle");
    m_pParent->m_bRegionScriptActive = false;
    HaveOpenedDemonDoor = false;
    QuestStartScreenShown = false;
}
int main()
{
    CGameScriptInterface gi; gi.calls = 0; gi.liveAtCall = 0;
    CGraveyardParentData parent; parent.m_bRegionScriptActive = true;
    CQ_SecretPassageScript s; s.m_pInterface = &gi; s.m_pParent = &parent;
    s.QuestStartScreenShown = true; s.HaveOpenedDemonDoor = true;
    s.Init();
    int bad = 0;
    if (gi.calls != 3 || g_live != 0 || gi.liveAtCall != 2) bad = 1;
    for (int i = 0; i < 3 && !bad; ++i)
        if (std::strcmp(gi.quests[i], "Q_OpeningGraveyardSecretPassage") != 0) bad = 1;
    if (!bad && (std::strcmp(gi.regions[0], "Graveyard") || std::strcmp(gi.regions[1], "GraveyardPath") || std::strcmp(gi.regions[2], "GraveyardCircle"))) bad = 1;
    if (parent.m_bRegionScriptActive || s.QuestStartScreenShown || s.HaveOpenedDemonDoor) bad = 1;
    if (bad) { std::printf("00dc25b0_TEST FAIL\n"); return 1; }
    std::printf("00dc25b0_TEST PASS\n");
    return 0;
}