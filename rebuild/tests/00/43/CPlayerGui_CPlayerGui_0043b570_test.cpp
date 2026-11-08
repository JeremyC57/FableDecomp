// Behaviour test for 0x0043B570 CPlayerGui::CPlayerGui: a portable model of the constructor -- the five game-object stores, the
// initialised flags / floats, the three button arrays sized to the GUI def's ability-button count (alpha 1.0f, buttons 0), the
// lazily loaded def (loaded once), the hero information screens manager replaced, IsEditorActive asked once, and every stat bar /
// illustrator created into its counted pointer (34 illustrators registered in order, the NPC-name one with counter -1).
#include <cstdio>
#include <vector>
#include <string>

struct Def { int abilityButtonCount; };
static Def gDef = { 6 };
static const Def* gGuiDef = 0;           // CPlayerGui::PGuiDef
static int gDefLoads = 0;
static const Def& PeekGuiDef() { if (!gGuiDef) { gGuiDef = &gDef; ++gDefLoads; } return *gGuiDef; }

struct Component { mutable int editorQueries; bool IsEditorActive() const { ++editorQueries; return false; } };
struct Manager { int world; };
struct Object { std::string kind; int counter; };

struct PlayerGui {
    void* World; void* PlayerManager; void* InputInterface; const Component* Comp; const void* Graphics;
    bool ShowSpecialAbilityButtons, ShowSpecialAbilityButtonsAbilities, SpecialSelected, SpecialDeselected;
    std::vector<long> AbilityButtons; std::vector<float> AbilityButtonsAlpha, QuickAccessButtonsAlpha;
    float AbilityButtonScale; bool NormalToSpecial, SpecialToNormal, DrawGui, OldDrawGui; int GuiMode; float FadeInTimer;
    bool ForcedClosed, ShowTime, ShowMoneyBag; float Alpha; unsigned long long HeroCentreUID; bool DrawingSafeAreas, DrawingDebugTGA;
    bool ScreenMessagingEnabled, ForceAllowSubtitleToRender, LiveGuiMode, DisplayBottomHalfGUI;
    Manager* HeroInfo;
    std::vector<Object*> Bars, Illustrators;
    PlayerGui(const Component& c, int& world, int& input, int& players, const int& graphics)
        : World(&world), PlayerManager(&players), InputInterface(&input), Comp(&c), Graphics(&graphics),
          ShowSpecialAbilityButtons(false), ShowSpecialAbilityButtonsAbilities(true), SpecialSelected(false), SpecialDeselected(false),
          AbilityButtonScale(0.0f), NormalToSpecial(false), SpecialToNormal(false), DrawGui(true), OldDrawGui(true), GuiMode(0),
          FadeInTimer(0.0f), ForcedClosed(false), ShowTime(true), ShowMoneyBag(false), Alpha(1.0f), HeroCentreUID(0),
          DrawingSafeAreas(false), DrawingDebugTGA(false), ScreenMessagingEnabled(true), ForceAllowSubtitleToRender(false),
          LiveGuiMode(false), DisplayBottomHalfGUI(true), HeroInfo(0)
    {
        AbilityButtons.resize(PeekGuiDef().abilityButtonCount);
        AbilityButtonsAlpha.resize(PeekGuiDef().abilityButtonCount, 1.0f);
        QuickAccessButtonsAlpha.resize(PeekGuiDef().abilityButtonCount, 1.0f);
        Manager* old = HeroInfo; HeroInfo = new Manager(); HeroInfo->world = world; delete old;
        Comp->IsEditorActive();
        const char* bars[] = { "Money", "Renown", "Morality", "Experience", "Crime", "StatUpdates", "AutoPickups" };
        for (int i = 0; i < 7; ++i) { PeekGuiDef(); Object* o = new Object(); o->kind = bars[i]; o->counter = 0; Bars.push_back(o); }
        const char* draws[] = { "Health", "Stamina", "CombatMultiplier", "SpiritsHealth", "MiniMap", "ScreenFade", "Money", "Renown",
            "Morality", "Experience", "Crime", "ReceivedItems", "FishingGame", "StatUpdates", "AutoPickups", "Perceivers", "Damage",
            "QuestInfo", "GuildSeal", "ScriptScreenText", "TargetInfo", "HighlightingTargetingCursor", "WorldTime", "LetterBoxBlackBars",
            "BettingHUD", "ProjectileTargetingCentre", "MeleeActionButtonEffects", "SpiritScores", "SpiritTargetingCursor", "2DText",
            "MouseCursor", "NPCName", "AbilityButtonsPC", "QuickAccessIconsPC" };
        for (int i = 0; i < 34; ++i) {
            Object* o = new Object(); o->kind = draws[i]; o->counter = (o->kind == "NPCName") ? -1 : 0; Illustrators.push_back(o);
        }
    }
};

int main()
{
    Component c = { 0 }; int world = 7, input = 1, players = 2; const int graphics = 3;
    PlayerGui g(c, world, input, players, graphics);
    bool ok = g.World == &world && g.PlayerManager == &players && g.InputInterface == &input && g.Comp == &c && g.Graphics == &graphics;
    ok = ok && g.AbilityButtons.size() == 6 && g.AbilityButtonsAlpha.size() == 6 && g.QuickAccessButtonsAlpha.size() == 6;
    for (int i = 0; i < 6; ++i) ok = ok && g.AbilityButtons[i] == 0 && g.AbilityButtonsAlpha[i] == 1.0f && g.QuickAccessButtonsAlpha[i] == 1.0f;
    ok = ok && gDefLoads == 1 && c.editorQueries == 1 && g.HeroInfo && g.HeroInfo->world == 7;
    ok = ok && !g.ShowSpecialAbilityButtons && g.ShowSpecialAbilityButtonsAbilities && g.DrawGui && g.OldDrawGui && g.ShowTime
            && !g.ShowMoneyBag && g.Alpha == 1.0f && g.ScreenMessagingEnabled && g.DisplayBottomHalfGUI && g.HeroCentreUID == 0;
    ok = ok && g.Bars.size() == 7 && g.Illustrators.size() == 34 && g.Illustrators[31]->kind == "NPCName" && g.Illustrators[31]->counter == -1
            && g.Illustrators[0]->kind == "Health" && g.Bars[4]->kind == "Crime";
    if (!ok) { std::printf("0043b570_TEST FAIL\n"); return 1; }
    std::printf("0043b570_TEST PASS\n");
    return 0;
}