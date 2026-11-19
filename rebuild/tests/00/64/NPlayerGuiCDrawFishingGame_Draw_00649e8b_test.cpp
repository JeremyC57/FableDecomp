// Behaviour test for NPlayerGui::CDrawFishingGame::Draw (0x00649E8B): the row body and its same-TU
// CDrawQuestInfo::UpdateMiniGame(const CRGBColour&) verbatim over models of their callees. Kills: drawing a dead / missing hero or an
// uninitialised stage, not marking the drawer, acting in stage 0 / 1 / 5, the wrong distance fraction (stages 2 / 3 push 0, stage 4
// distance / max length), the strain colour (red = 255 * strain, green = 255 * (1 - strain), blue 0, alpha 255) or its clamp at 1,
// the rod-def maxima ignored, the strain / distance texts not built from the truncated values, and the colour update reaching a
// non-mini-game element, a non-updatable bar or a bar when the mini-game index is 0.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <list>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
struct CEnginePrimitivePrepareInfo { int unused; };
struct CRGBColour {
    unsigned char R, G, B, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b) : R(r), G(g), B(b), A(255) {}
};
struct CWideString { std::string Text; long Value; bool Built; };
namespace NGameText {
class CDataBank { public: CWideString GetTextBySymbol(const char* s) const { CWideString w; w.Text = s; w.Value = -1; w.Built = false; return w; } };
static long built_strain = -99, built_distance = -99;
static void ConstructString(CWideString& w, long v) {
    w.Value = v; w.Built = true;
    if (w.Text == "TEXT_GUI_FISHING_STRAIN") built_strain = v; else if (w.Text == "TEXT_GUI_FISHING_DISTANCE") built_distance = v;
}
}
using NGameText::built_strain; using NGameText::built_distance;
struct CFontBank { int unused; };
struct CMainGameComponent {
    NGameText::CDataBank Bank; CFontBank Font;
    NGameText::CDataBank& GetTextBank() { return Bank; }
    const CFontBank& PeekCutSceneMainFont() const { return Font; }
};
enum EFishingStage { };
class CFishingRodDef { public: float MaxStrain, MaxLengthMetres; };
class CTCFishing {
public:
    EFishingStage Stage; bool StageInitialised; float Distance, Strain; const CFishingRodDef* RodDef;
    EFishingStage GetStage() const { return Stage; }
    bool IsStageInitialised() const { return StageInitialised; }
    float GetDistance() const { return Distance; }
    float GetStrain() const { return Strain; }
    const CFishingRodDef* GetPFishingRodDef() const { return RodDef; }
};
class CThingPlayerCreature {
public:
    bool Alive; CTCFishing* Fishing;
    bool PeekTC(const CTCFishing** p) const { *p = Fishing; return Fishing != 0; }
};
template<class T> static bool IS_THING_ALIVE(const T& thing) { return thing != 0 && thing->Alive; }
struct CPlayer { const CThingPlayerCreature* Hero; const CThingPlayerCreature* PeekPControlledCreature() const { return Hero; } };
namespace NPlayerGui {
class CDrawQuestInfo {
public:
    enum EElementType { QUEST_INFO_ELEMENT_BAR = 1, QUEST_INFO_ELEMENT_MINIGAME_BAR = 6 };
    class CQuestInfoElement { public: long ID; EElementType Type; EElementType GetType() const { return Type; } virtual ~CQuestInfoElement() {} };
    class CQuestInfoElementMiniGameBar : public CQuestInfoElement { public: bool Updatable; CRGBColour Colour; };
    template<class T> struct CCountedPointer { T* Data; T* Get() const { return Data; } };
    long MiniGameElementIndex;
    std::list<CCountedPointer<CQuestInfoElement> > Elements;
    int FloatCalls; float LastPercentage;
    void UpdateMiniGame(float percentage, long, long) { ++FloatCalls; LastPercentage = percentage; }
    void UpdateMiniGame(const CRGBColour& colour);
};
// ---- row TU (verbatim) ----
void CDrawQuestInfo::UpdateMiniGame(const CRGBColour& colour)
{
    if (MiniGameElementIndex > 0) {
        for (std::list<CCountedPointer<CQuestInfoElement> >::iterator it = Elements.begin(); it != Elements.end(); ++it) {
            CQuestInfoElement* pelement = (*it).Get();
            if (pelement->ID == MiniGameElementIndex) {
                if (pelement->GetType() == QUEST_INFO_ELEMENT_MINIGAME_BAR) {
                    CQuestInfoElementMiniGameBar* pbar = static_cast<CQuestInfoElementMiniGameBar*>(pelement);
                    if (pbar->Updatable)
                        pbar->Colour = colour;
                }
                return;
            }
        }
    }
}

struct CPlayerGui { CDrawQuestInfo Info; CDrawQuestInfo& GetQuestInfo() { return Info; } };
class CDrawBase {
public:
    CPlayerGui* PlayerGui; CMainGameComponent* Component; bool Drawing; CPlayer* Player;
    CPlayer& GetPlayer() { return *Player; }
    void SetAsDrawing(bool d) { Drawing = d; }
};
class CDrawFishingGame : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };
void CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* phero = GetPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(phero)) {
        const CTCFishing* ptc_fishing;
        if (phero->PeekTC(&ptc_fishing) && ptc_fishing->IsStageInitialised()) {
            SetAsDrawing(true);
            if (ptc_fishing->GetStage()) {
                const CFontBank& font = Component->PeekCutSceneMainFont();
                switch (ptc_fishing->GetStage()) {
                case 2:
                case 3:
                case 4: {
                    const CFishingRodDef* pfishing_rod_def = ptc_fishing->GetPFishingRodDef();
                    CWideString strain_text = Component->GetTextBank().GetTextBySymbol("TEXT_GUI_FISHING_STRAIN");
                    NGameText::ConstructString(strain_text, (long)ptc_fishing->GetStrain());
                    float max_strain = ptc_fishing->GetStrain();
                    if (pfishing_rod_def)
                        max_strain = pfishing_rod_def->MaxStrain;
                    float strain_percentage = ptc_fishing->GetStrain() / max_strain;
                    if (strain_percentage > 1.0f)
                        strain_percentage = 1.0f;
                    unsigned char strain_colour_red = (unsigned char)(strain_percentage * 255.0f);
                    unsigned char strain_colour_green = (unsigned char)((1.0f - strain_percentage) * 255.0f);
                    CWideString distance_text = Component->GetTextBank().GetTextBySymbol("TEXT_GUI_FISHING_DISTANCE");
                    NGameText::ConstructString(distance_text, (long)ptc_fishing->GetDistance());
                    float max_length = ptc_fishing->GetDistance();
                    if (pfishing_rod_def)
                        max_length = pfishing_rod_def->MaxLengthMetres;
                    float distance_percentage = ptc_fishing->GetDistance() / max_length;
                    if (distance_percentage > 1.0f)
                        distance_percentage = 1.0f;
                    unsigned char distance_colour_red = (unsigned char)(distance_percentage * 255.0f);
                    unsigned char distance_colour_green = (unsigned char)((1.0f - distance_percentage) * 255.0f);
                    if (ptc_fishing->GetStage() == 3 || ptc_fishing->GetStage() == 2)
                        PlayerGui->GetQuestInfo().UpdateMiniGame(0.0f, 0, 0);
                    else
                        PlayerGui->GetQuestInfo().UpdateMiniGame(ptc_fishing->GetDistance() / max_length, 0, 0);
                    PlayerGui->GetQuestInfo().UpdateMiniGame(CRGBColour(strain_colour_red, strain_colour_green, 0));
                    break;
                }
                }
            }
        }
    }
}
// ---- end row ----
}
using namespace NPlayerGui;
typedef CDrawQuestInfo::CQuestInfoElementMiniGameBar Bar;

int main()
{
    CEnginePrimitivePrepareInfo info;
    CMainGameComponent comp; CPlayerGui gui; CPlayer player;
    CFishingRodDef rod; rod.MaxStrain = 8.0f; rod.MaxLengthMetres = 40.0f;
    CTCFishing fishing; fishing.StageInitialised = true; fishing.Distance = 10.5f; fishing.Strain = 2.0f; fishing.RodDef = &rod;
    CThingPlayerCreature hero; hero.Alive = true; hero.Fishing = &fishing;
    player.Hero = &hero;
    Bar other; other.ID = 3; other.Type = CDrawQuestInfo::QUEST_INFO_ELEMENT_MINIGAME_BAR; other.Updatable = true; other.Colour = CRGBColour(1, 1, 1);
    Bar plain; plain.ID = 5; plain.Type = CDrawQuestInfo::QUEST_INFO_ELEMENT_BAR; plain.Updatable = true; plain.Colour = CRGBColour(2, 2, 2);
    Bar bar; bar.ID = 5; bar.Type = CDrawQuestInfo::QUEST_INFO_ELEMENT_MINIGAME_BAR; bar.Updatable = true; bar.Colour = CRGBColour(3, 3, 3);
    CDrawQuestInfo::CCountedPointer<CDrawQuestInfo::CQuestInfoElement> p;
    p.Data = &other; gui.Info.Elements.push_back(p);
    p.Data = &bar; gui.Info.Elements.push_back(p);
    gui.Info.MiniGameElementIndex = 5;
    CDrawFishingGame d; d.PlayerGui = &gui; d.Component = &comp; d.Player = &player;
    long cases = 0;
#define RESET() (d.Drawing = false, gui.Info.FloatCalls = 0, gui.Info.LastPercentage = -1.0f, bar.Colour = CRGBColour(3, 3, 3))

    // Stage 4 with a rod: strain 2/8 = 0.25 -> red 63, green 191; distance fraction 10.5/40.
    fishing.Stage = (EFishingStage)4; RESET(); d.Draw(info);
    CHECK(d.Drawing); ++cases;
    CHECK(gui.Info.FloatCalls == 1 && gui.Info.LastPercentage == 10.5f / 40.0f); ++cases;
    CHECK(bar.Colour.R == 63 && bar.Colour.G == 191 && bar.Colour.B == 0 && bar.Colour.A == 255); ++cases;
    CHECK(other.Colour.R == 1); ++cases;
    CHECK(built_strain == 2 && built_distance == 10); ++cases;                              // a different element id is not touched
    // Stages 2 and 3 push 0.
    fishing.Stage = (EFishingStage)2; RESET(); d.Draw(info);
    CHECK(gui.Info.FloatCalls == 1 && gui.Info.LastPercentage == 0.0f && bar.Colour.R == 63); ++cases;
    fishing.Stage = (EFishingStage)3; RESET(); d.Draw(info);
    CHECK(gui.Info.FloatCalls == 1 && gui.Info.LastPercentage == 0.0f && bar.Colour.G == 191); ++cases;
    // Stages 1 and 5: drawing, but no update; stage 0: drawing, nothing else.
    built_strain = built_distance = -99;
    fishing.Stage = (EFishingStage)1; RESET(); d.Draw(info);
    CHECK(d.Drawing && gui.Info.FloatCalls == 0 && bar.Colour.R == 3 && built_strain == -99); ++cases;
    fishing.Stage = (EFishingStage)5; RESET(); d.Draw(info);
    CHECK(d.Drawing && gui.Info.FloatCalls == 0 && bar.Colour.R == 3); ++cases;
    fishing.Stage = (EFishingStage)0; RESET(); d.Draw(info);
    CHECK(d.Drawing && gui.Info.FloatCalls == 0); ++cases;
    // Strain above the maximum clamps to red 255 / green 0.
    fishing.Stage = (EFishingStage)4; fishing.Strain = 12.0f; RESET(); d.Draw(info);
    CHECK(bar.Colour.R == 255 && bar.Colour.G == 0); ++cases;
    // No rod def: the maxima are the current values (fraction 1, red 255).
    fishing.Strain = 2.0f; fishing.RodDef = 0; RESET(); d.Draw(info);
    CHECK(bar.Colour.R == 255 && bar.Colour.G == 0 && gui.Info.LastPercentage == 1.0f); ++cases;
    fishing.RodDef = &rod;
    // A non-updatable bar keeps its colour.
    bar.Updatable = false; RESET(); d.Draw(info);
    CHECK(bar.Colour.R == 3 && gui.Info.FloatCalls == 1); ++cases;
    bar.Updatable = true;
    // The first element with the index decides: a plain bar with id 5 ahead of the mini-game bar stops the search.
    gui.Info.Elements.push_front(p); gui.Info.Elements.front().Data = &plain;
    RESET(); d.Draw(info);
    CHECK(bar.Colour.R == 3 && plain.Colour.R == 2); ++cases;
    gui.Info.Elements.pop_front();
    // Mini-game index 0: no colour update.
    gui.Info.MiniGameElementIndex = 0; other.ID = 0; RESET(); d.Draw(info);
    CHECK(bar.Colour.R == 3 && other.Colour.R == 1 && gui.Info.FloatCalls == 1); ++cases;   // even an element with id 0 is not touched
    other.ID = 3;
    gui.Info.MiniGameElementIndex = 5;
    // Not initialised / dead / no hero / no fishing component: nothing at all.
    fishing.StageInitialised = false; RESET(); d.Draw(info);
    CHECK(!d.Drawing && gui.Info.FloatCalls == 0 && bar.Colour.R == 3); ++cases;
    fishing.StageInitialised = true; hero.Alive = false; RESET(); d.Draw(info);
    CHECK(!d.Drawing && gui.Info.FloatCalls == 0); ++cases;
    hero.Alive = true; player.Hero = 0; RESET(); d.Draw(info);
    CHECK(!d.Drawing); ++cases;
    player.Hero = &hero; hero.Fishing = 0; RESET(); d.Draw(info);
    CHECK(!d.Drawing); ++cases;
    CHECK(cases == 19);
    puts("00649e8b_TEST PASS");
    return 0;
}