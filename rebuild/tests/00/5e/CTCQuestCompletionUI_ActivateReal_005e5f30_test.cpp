// Behaviour test for CTCQuestCompletionUI::ActivateReal (0x005E5F30): a copy of the row's decision logic -- the music set per
// completion type, the sound fade gate, the menu / frame / screen / viewport / container string picks, the start-screen snapshot
// of renown / gold / experience per quest card, and the bar targets computed from (and erasing) those snapshots with the clamp
// rules of each bar (renown and gold clamp at zero, experience does not), plus the count-up sound gates.
#include <cstdio>
#include <cstring>
#include <map>

struct Hero { long RenownTotal, TotalMoneyEarned, TotalExperiencePoints; };
struct Misc { long QuestStartScreenMusic, QuestCompleteScreenMusic, QuestFailureScreenMusic, DeathScreenMusic; };

struct Model {
    long Completion, QuestCardDef; bool HasCard; long GoldReward, RenownReward;
    long TargetGoldGained, InitialGold, TargetExperienceGained, InitialExperience, TargetRenownGained, InitialRenown;
    std::map<long, unsigned long> StartQuestExperience, StartQuestGold, StartQuestRenown;
    const char* Menu; const char* Frame; const char* Screen; const char* Viewport; const char* Container;
    long Music; bool MusicSet; bool Faded; int Sounds;
};

static void Run(Model& m, const Hero& h, const Misc& misc)
{
    m.Faded = m.Completion != 0 && m.Completion != 1;
    switch (m.Completion) {
    case 0: m.Music = misc.QuestStartScreenMusic; break;
    case 1: m.Music = misc.QuestCompleteScreenMusic; break;
    case 2: m.Music = misc.QuestFailureScreenMusic; break;
    case 3: m.Music = misc.DeathScreenMusic; break;
    default: m.Music = misc.QuestStartScreenMusic; break;
    }
    m.MusicSet = m.Completion != 0 && m.Completion != 1;
    m.InitialGold = m.TargetGoldGained = m.InitialExperience = m.TargetExperienceGained = m.InitialRenown = m.TargetRenownGained = 0;
    if (m.Completion == 0 && m.QuestCardDef != 0) {
        m.StartQuestRenown[m.QuestCardDef] = h.RenownTotal;
        m.StartQuestGold[m.QuestCardDef] = h.TotalMoneyEarned;
        m.StartQuestExperience[m.QuestCardDef] = h.TotalExperiencePoints;
    }
    switch (m.Completion) {
    case 1: m.Menu = "PC_QUEST_COMPLETION_MENU_COMPLETE"; m.Frame = "PC_UI_QUEST_FINISHED_FRAME"; break;
    case 2: m.Menu = "PC_QUEST_COMPLETION_MENU_FAIL"; m.Frame = "PC_UI_QUEST_FAILED_FRAME"; break;
    case 3: m.Menu = "PC_QUEST_COMPLETION_MENU_DEATH"; m.Frame = "PC_UI_QUEST_DEATH_FRAME"; break;
    default: m.Menu = "PC_QUEST_COMPLETION_MENU_START"; m.Frame = "PC_UI_QUEST_START_FRAME"; break;
    }
    bool failed = m.Completion != 0 && m.Completion != 1;
    m.Screen = failed ? "PC_QUEST_COMPLETION_MENU_SCREEN" : "PC_QUEST_COMPLETION_MENU_SCREEN_START_SUCCESS";
    m.Viewport = failed ? "PC_DESCRIPTION_VIEWPORT_QUEST_FAILED" : "PC_DESCRIPTION_VIEWPORT_QUEST_COMPLETION";
    m.Container = failed ? "PC_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_LIST" : "PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_QUEST_COMPLETION";
    if (m.Completion == 0 || m.Completion == 1) {
        if (m.Completion == 0) { if (m.HasCard) m.TargetRenownGained = m.RenownReward; }
        else if (m.QuestCardDef) {
            std::map<long, unsigned long>::iterator it = m.StartQuestRenown.find(m.QuestCardDef);
            if (it != m.StartQuestRenown.end()) {
                m.TargetRenownGained = h.RenownTotal - it->second; m.InitialRenown = 0;
                if (m.TargetRenownGained < 0) m.TargetRenownGained = 0;
                m.StartQuestRenown.erase(it);
            } else { m.TargetRenownGained = h.RenownTotal; m.InitialRenown = 0; if (m.TargetRenownGained < 0) m.TargetRenownGained = 0; }
        }
        if (m.Completion == 0) { if (m.HasCard) m.TargetGoldGained = m.GoldReward; }
        else if (m.QuestCardDef) {
            std::map<long, unsigned long>::iterator it = m.StartQuestGold.find(m.QuestCardDef);
            if (it != m.StartQuestGold.end()) {
                m.TargetGoldGained = h.TotalMoneyEarned - it->second; m.InitialGold = 0;
                if (m.TargetGoldGained < 0) m.TargetGoldGained = 0;
                m.StartQuestGold.erase(it);
            } else { m.TargetGoldGained = h.TotalMoneyEarned; m.InitialGold = 0; if (m.TargetGoldGained < 0) m.TargetGoldGained = 0; }
        }
    }
    if (m.Completion == 1 && m.QuestCardDef) {
        std::map<long, unsigned long>::iterator it = m.StartQuestExperience.find(m.QuestCardDef);
        if (it != m.StartQuestExperience.end()) {
            m.TargetExperienceGained = h.TotalExperiencePoints - it->second; m.InitialExperience = 0;
            m.StartQuestExperience.erase(it);
        } else { m.TargetExperienceGained = h.TotalExperiencePoints; m.InitialExperience = 0; }
    }
    m.Sounds = 0;
    if (m.InitialGold != m.TargetGoldGained) ++m.Sounds;
    if (m.InitialRenown != m.TargetRenownGained) ++m.Sounds;
    if (m.InitialExperience != m.TargetExperienceGained) ++m.Sounds;
}

static Model Fresh(long completion)
{
    Model m;
    m.Completion = completion; m.QuestCardDef = 77; m.HasCard = true; m.GoldReward = 100; m.RenownReward = 40;
    m.TargetGoldGained = m.InitialGold = m.TargetExperienceGained = m.InitialExperience = m.TargetRenownGained = m.InitialRenown = 0;
    m.Menu = m.Frame = m.Screen = m.Viewport = m.Container = 0; m.Music = 0; m.MusicSet = m.Faded = false; m.Sounds = 0;
    return m;
}

int main()
{
    Misc misc = { 11, 12, 13, 14 };
    Hero start = { 500, 1000, 2000 };
    Hero done = { 560, 1250, 2600 };
    Model m = Fresh(0);
    Run(m, start, misc);
    if (m.Music != 11 || m.MusicSet || m.Faded || m.StartQuestRenown[77] != 500 || m.StartQuestGold[77] != 1000 ||
        m.StartQuestExperience[77] != 2000 || m.TargetRenownGained != 40 || m.TargetGoldGained != 100 || m.Sounds != 2 ||
        std::strcmp(m.Menu, "PC_QUEST_COMPLETION_MENU_START") || std::strcmp(m.Screen, "PC_QUEST_COMPLETION_MENU_SCREEN_START_SUCCESS")) {
        std::printf("005e5f30_TEST FAIL\n"); return 1;
    }
    m.Completion = 1;
    Run(m, done, misc);
    if (m.Music != 12 || m.TargetRenownGained != 60 || m.TargetGoldGained != 250 || m.TargetExperienceGained != 600 ||
        !m.StartQuestRenown.empty() || !m.StartQuestGold.empty() || !m.StartQuestExperience.empty() || m.Sounds != 3 ||
        std::strcmp(m.Frame, "PC_UI_QUEST_FINISHED_FRAME")) {
        std::printf("005e5f30_TEST FAIL\n"); return 1;
    }
    Hero poorer = { 400, 900, 1500 };
    Model n = Fresh(0); Run(n, start, misc); n.Completion = 1; Run(n, poorer, misc);
    if (n.TargetRenownGained != 0 || n.TargetGoldGained != 0 || n.TargetExperienceGained != -500) {
        std::printf("005e5f30_TEST FAIL\n"); return 1;
    }
    Model d = Fresh(3); Run(d, done, misc);
    if (d.Music != 14 || !d.MusicSet || !d.Faded || d.Sounds != 0 || std::strcmp(d.Menu, "PC_QUEST_COMPLETION_MENU_DEATH") ||
        std::strcmp(d.Viewport, "PC_DESCRIPTION_VIEWPORT_QUEST_FAILED") ||
        std::strcmp(d.Container, "PC_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_LIST")) {
        std::printf("005e5f30_TEST FAIL\n"); return 1;
    }
    Model x = Fresh(7); Run(x, done, misc);
    if (x.Music != 11 || std::strcmp(x.Menu, "PC_QUEST_COMPLETION_MENU_START")) { std::printf("005e5f30_TEST FAIL\n"); return 1; }
    std::printf("005e5f30_TEST PASS\n");
    return 0;
}