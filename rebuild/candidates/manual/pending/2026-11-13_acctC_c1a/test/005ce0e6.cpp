// Behaviour test for CTCInventoryStats::ConstructPersonalityList @ 0x005CE0E6: the console (pad) personality page of the stats screen.
// Subject: the row's own statements (the function text of the row source, unchanged) over a mock engine that logs every text
// lookup, hero-stats member read, engine call and virtual UI call.  Expectation: kRetail, the retail call order cut into text
// statements (generated from the retail disassembly by tools/rtok.py, not from the row source) -- run 1 drives every branch and
// every loop once and must reproduce it statement for statement.  Runs 2 to 4 take the other arms (no title, no favourite
// ability, no tattoos / creatures / inventory / trophy games, missing buttons, slider container and screen) and check the page
// text, the title parts, the trophy sum, the four sliders and the back action.
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
typedef std::string Str;
// ---- the scenario a run drives ----
static Str gTitle; static long gFavouriteAbility; static int gTattoos; static int gItems; static bool gInventory;
static long gTrophyGames; static float gGoodness; static std::vector<Str> gMissing;      // FindChild names that return null
// ---- what a run records ----
struct Entry { Str text; bool title; };
struct Slider { Str label; long x, y, level, maxLevel; float value, width, red; bool flag; };
static std::vector<Str> gTok, gSeg, gActions, gChildren, gParts;
static std::vector<Entry> gPage; static std::vector<Slider> gSliders; static int gPositionChildren;
static Str Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static Str Hex(unsigned long v) { char b[32]; std::sprintf(b, "%lx", v); return b; }
static void Tok(const char* t) { gTok.push_back(t); }
static void Off(unsigned long o) { gTok.push_back("o" + Hex(o)); }
static void Close(const char* op)
{
    std::sort(gTok.begin(), gTok.end());
    Str s;
    for (size_t i = 0; i < gTok.size(); ++i) { if (gTok[i][0] == 'o' && i && gTok[i] == gTok[i - 1]) continue; s += gTok[i]; s += ' '; }
    gSeg.push_back(s + op); gTok.clear();
}
// ---- strings ----
struct CCharString { Str s; CCharString(const char* p) : s(p) { gTok.push_back("S:" + s); } CCharString(const Str& x, int) : s(x) { } };
static CCharString operator+(const CCharString& a, const CCharString& b) { return CCharString(a.s + b.s, 0); }
struct CWideString {
    Str s;
    CWideString() { }
    CWideString(const unsigned short* w) { while (*w) s += (char)*w++; }
#ifndef _MSC_VER
    CWideString(const wchar_t* w) { while (*w) s += (char)*w++; }      // g++ only: the row's wide literals are wchar_t there
#endif
    CWideString(const Str& x, int) : s(x) { }
    CWideString& operator=(const CWideString& o) { s = o.s; Close("="); return *this; }
    CWideString& operator+=(const CWideString& o) { s += o.s; Close("+="); return *this; }
    long GetLength() const { Tok("f99b220"); return (long)s.size(); }
    static CWideString NullString;
};
CWideString CWideString::NullString;
static CWideString operator+(const CWideString& a, const CWideString& b) { return CWideString(a.s + b.s, 0); }
static CWideString operator+(const CWideString& a, const unsigned short* b) { return CWideString(a.s + CWideString(b).s, 0); }
#ifndef _MSC_VER
static CWideString operator+(const CWideString& a, const wchar_t* b) { return CWideString(a.s + CWideString(b).s, 0); }
#endif
static CWideString GFIntToWideString(long v) { Tok("f405f00"); return CWideString(Num(v), 0); }
static CWideString GFFloatToWideString(double v) { Tok("f486c40"); char b[32]; std::sprintf(b, "%.2f", v); return CWideString(b, 0); }
static long GFFloatToLongNear(float v) { return (long)std::floor(v + 0.5f); }
static long GFFloatToLongFloor(float v) { return (long)std::floor(v); }
static float GFRoundFloatNear(float value, long places)
{
    float multiplier = 1.0f;
    while (places--) multiplier *= 10.0f;
    float rounded = (float)GFFloatToLongNear(value * multiplier);
    return rounded / multiplier;
}
namespace NGameText {
struct CDataBank { CWideString GetTextBySymbol(const CCharString& s) const { Tok("f9c95e0"); return CWideString("<" + s.s + ">", 0); } };
static bool ConstructString(CWideString& out, const std::vector<CWideString>& parts)
{
    Tok("f9c7c20");
    for (size_t i = 0; i < parts.size(); ++i) { gParts.push_back(parts[i].s); out.s += "[" + parts[i].s + "]"; }
    return true;
}
}
struct CMainGameComponent {
    NGameText::CDataBank bank;
    static CMainGameComponent* Get() { static CMainGameComponent c; return &c; }
    const NGameText::CDataBank& GetTextBank() const { return bank; }
};
// ---- enums and small values ----
enum ETCInterfaceType { TCI_HERO_EXPERIENCE, TCI_INVENTORY_EXPERIENCE };
enum EHeroExperienceStatCategory { HERO_STAT_STRENGTH, HERO_STAT_SKILL, HERO_STAT_WILL, NUMBER_OF_HERO_STAT_CATEGORIES };
enum EOpinion { OPINION_MORALITY, OPINION_RENOWN, OPINION_SCARINESS, OPINION_AGREEABLENESS, OPINION_ATTRACTIVENESS };
enum EHeroSexuality { HERO_SEXUALITY_MOCK = 2 };
struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
struct CRGBFloatColour { float R, G, B, A; CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { } };
template <class T> struct CCountedPointer {
    T* p;
    CCountedPointer() : p(0) { }
    CCountedPointer(T* x) : p(x) { }
    T* operator->() const { return p; }
    void Reset(T* x) { p = x; }
};
// ---- UI ----
namespace NUISystem {
enum { UI_ACTION_TYPE_CHANGESTATE = 3, UI_ACTION_TYPE_SEND_OWNED_EVENT = 230, UI_ACTION_TYPE_OBSERVE_EVENT = 245, UI_ACTION_TYPE_IGNORE_EVENT = 249 };
enum { UI_EVENT_RIGHT_STICK_UP = 6, UI_EVENT_RIGHT_STICK_DOWN = 7, UI_EVENT_MOUSE_MOVEMENT = 25 };
struct CActionParam { long value; Str label; CActionParam(long v) : value(v) { } };
struct CActionParamState : CActionParam { CActionParamState(unsigned long state) : CActionParam((long)state) { label = "state"; } };
struct CActionParamULONG : CActionParam { CActionParamULONG(unsigned long v) : CActionParam((long)v) { } };
struct CActionParamSLONG : CActionParam { CActionParamSLONG(long v) : CActionParam(v) { } };
class CComponent;
struct CAction {
    long ActionType; std::vector<CActionParam*> UIActionParams; Str target;
    CAction() : ActionType(0) { }
    void AddParam(CComponent* c, const CCountedPointer<CAction>&);
};
class CComponent : public CActionParam {
public:
    Str name;
    CComponent(const Str& n) : CActionParam(-1), name(n) { label = n; }
    CComponent* FindChild(const CCharString& n)
    {
        Tok("vc");
        if (std::find(gMissing.begin(), gMissing.end(), n.s) != gMissing.end()) return 0;
        return new CComponent(n.s);
    }
    void Record(const char* slot, const CCountedPointer<CAction>& a)
    {
        gActions.push_back(name + "." + slot + ":" + Num(a->ActionType) + ":" + a->target + ":" + (a->UIActionParams.empty() ? Str("-") : Num(a->UIActionParams[0]->value)));
    }
    void AddActionOnSelected(const CCountedPointer<CAction>& a) { Tok("v10c"); Record("selected", a); }
    void AddActionOnInactive(const CCountedPointer<CAction>& a) { Tok("v118"); Record("inactive", a); }
    void AddActionOnLeftClicked(const CCountedPointer<CAction>& a) { Tok("v11c"); Record("leftclicked", a); }
    void AddActionOnLeftHeld(const CCountedPointer<CAction>& a) { Tok("v120"); Record("leftheld", a); }
    void AddActionOnBack(const CCountedPointer<CAction>& a)
    {
        Tok("v110");
        Str s = name + ".back:" + Num(a->ActionType);
        for (size_t i = 0; i < a->UIActionParams.size(); ++i) s += ":" + (a->UIActionParams[i] ? a->UIActionParams[i]->label + "=" + Num(a->UIActionParams[i]->value) : Str("null"));
        gActions.push_back(s);
    }
    void AddChild(const CCountedPointer<CComponent>& c) { Tok("vec"); gChildren.push_back(name + "<-" + c->name); }
    void AddPositionChild(CComponent* c) { Tok("v1f0"); ++gPositionChildren; gChildren.push_back(name + "<=" + c->name); }
};
inline void CAction::AddParam(CComponent* c, const CCountedPointer<CAction>&) { Tok("f53fde0"); target = c ? c->name : Str("null"); }
struct CManager {
    static CManager* GetInstance() { Tok("f41e5f2"); static CManager m; return &m; }
    CComponent* CreateComponent(const CCharString& n, bool) { Tok("f41db1d"); return new CComponent(n.s); }
    CComponent* CreateTickSlider(const CWideString& label, const C2DCoordI& pos, long level, long maxLevel, const CRGBFloatColour& colour,
                                 float value, float width, bool flag, bool, const CRGBFloatColour&, float)
    {
        Tok("f427216");
        Slider s; s.label = label.s; s.x = pos.X; s.y = pos.Y; s.level = level; s.maxLevel = maxLevel; s.value = value; s.width = width; s.red = colour.R; s.flag = flag;
        gSliders.push_back(s); return new CComponent("tick");
    }
    CComponent* CreateStaticSliderBidirectional(const CWideString& label, float value, const C2DCoordI& pos)
    {
        Tok("f42819a");
        Slider s; s.label = label.s; s.x = pos.X; s.y = pos.Y; s.level = 0; s.maxLevel = 0; s.value = value; s.width = 0.0f; s.red = 0.0f; s.flag = false;
        gSliders.push_back(s); return new CComponent("static");
    }
};
}
using NUISystem::CComponent; using NUISystem::CAction; using NUISystem::CActionParamULONG; using NUISystem::CActionParamSLONG; using NUISystem::CActionParamState;
// ---- thing components ----
#define OFF_Age 0x30
#define OFF_BestScoreCoinGolfOakVale 0x184
#define OFF_BestScoreCoinGolfSnowSpire 0x188
#define OFF_BestScoreShoveHaPenny 0x18c
#define OFF_BestTimeGuessTheAddition 0x190
#define OFF_BestTimePairs 0x178
#define OFF_BestTimeSorting 0x17c
#define OFF_BestTrophyWitnesses 0xdc
#define OFF_BiggestFish 0x15c
#define OFF_Fatness 0x38
#define OFF_FinePaid 0x100
#define OFF_MaxChickenThrow 0x110
#define OFF_MaxMoney 0x40
#define OFF_MaximumMultiplier 0x98
#define OFF_Money 0x3c
#define OFF_Morality 0x28
#define OFF_NumBoastsCompleted 0xd0
#define OFF_NumBoastsTaken 0xcc
#define OFF_NumCompletedACoreQuest 0xc0
#define OFF_NumCompletedAQuest 0xbc
#define OFF_NumCompletedAnOptionalQuest 0xc8
#define OFF_NumDivorces 0x150
#define OFF_NumFailedAQuest 0xb8
#define OFF_NumLoversAcquired 0x140
#define OFF_NumMountedTrophies 0xd4
#define OFF_NumSex 0x154
#define OFF_NumSpousesMurdered 0x148
#define OFF_NumTeleports 0x10c
#define OFF_NumTrophyGamePlayed 0xd8
#define OFF_NumWeddings 0x144
#define OFF_NumberOfBuildingsRented 0xe8
#define OFF_NumberOfCrimes 0xf4
#define OFF_NumberOfDecapitations 0x24
#define OFF_NumberOfDrinks 0xf0
#define OFF_NumberOfHousesOwned 0xe4
#define OFF_NumberOfShopsOwned 0xe0
#define OFF_NumberOfVomits 0xec
#define OFF_RenownLevel 0x70
#define OFF_RenownMaxLevel 0x74
#define OFF_RentMoney 0x54
#define OFF_RewardMoney 0x50
#define OFF_TimesThrownOut 0x104
#define OFF_TotalBribeAmount 0x108
#define OFF_TotalFine 0xfc
#define OFF_TotalMoneyEarned 0x48
#define OFF_TotalMoneySpent 0x4c
#define OFF_TotalSpentExperience 0x13c
static std::map<unsigned long, long> gOverride;
static long Lv(unsigned long o) { Off(o); std::map<unsigned long, long>::iterator i = gOverride.find(o); return i != gOverride.end() ? i->second : (long)o + 1; }
static float Fv(unsigned long o) { Off(o); return (float)o + 0.25f; }
struct CTCHeroStats {
    // generated from hdr/engine/CTCHeroStats.h: each getter logs the member offset it reads and returns offset + 1 (floats + 0.25)
    long GetNumFailedAQuest() const { return Lv(0xb8); }
    long GetRenownLevel() const { return Lv(0x70); }
    long GetMorality() const { return Lv(0x28); }
    long GetNumBoastsTaken() const { return Lv(0xcc); }
    long GetMoney() const { return Lv(0x3c); }
    float GetAge() const { return Fv(0x30); }
    float GetFatness() const { return Fv(0x38); }
    long GetNumberOfDecapitations() const { return Lv(0x24); }
    long GetMaximumMultiplier() const { return Lv(0x98); }
    long GetNumberOfLoversAcquired() const { return Lv(0x140); }
    long GetNumberOfWeddings() const { return Lv(0x144); }
    long GetNumberOfDivorces() const { return Lv(0x150); }
    long GetNumberOfSpousesMurdered() const { return Lv(0x148); }
    long GetNumberOfTimesHadSex() const { return Lv(0x154); }
    long GetNumTeleports() const { return Lv(0x10c); }
    long GetNumCompletedAQuest() const { return Lv(0xbc); }
    long GetRewardMoney() const { return Lv(0x50); }
    long GetNumCompletedACoreQuest() const { return Lv(0xc0); }
    long GetNumCompletedAnOptionalQuest() const { return Lv(0xc8); }
    long GetNumBoastsCompleted() const { return Lv(0xd0); }
    long GetNumMountedTrophies() const { return Lv(0xd4); }
    long GetNumTrophyGamePlayed() const { return Lv(0xd8); }
    long GetBestTrophyWitnesses() const { return Lv(0xdc); }
    long GetNumberOfShopsOwned() const { return Lv(0xe0); }
    long GetNumberOfHousesOwned() const { return Lv(0xe4); }
    long GetNumberOfBuildingsRented() const { return Lv(0xe8); }
    long GetRentMoney() const { return Lv(0x54); }
    float GetMaxChickenThrow() const { return Fv(0x110); }
    float GetBiggestFish() const { return Fv(0x15c); }
    float GetBestScoreCoinGolfOakVale() const { return Fv(0x184); }
    float GetBestScoreCoinGolfSnowSpire() const { return Fv(0x188); }
    float GetBestScoreShoveHaPenny() const { return Fv(0x18c); }
    float GetBestTimeGuessTheAddition() const { return Fv(0x190); }
    float GetBestTimePairs() const { return Fv(0x178); }
    float GetBestTimeSorting() const { return Fv(0x17c); }
    long GetNumberOfVomits() const { return Lv(0xec); }
    long GetNumberOfDrinks() const { return Lv(0xf0); }
    long GetNumberOfCrimes() const { return Lv(0xf4); }
    long GetTotalFine() const { return Lv(0xfc); }
    long GetFinePaid() const { return Lv(0x100); }
    long GetTimesThrownOut() const { return Lv(0x104); }
    long GetTotalBribeAmount() const { return Lv(0x108); }
    long GetTotalMoneyEarned() const { return Lv(0x48); }
    long GetTotalMoneySpent() const { return Lv(0x4c); }
    long GetSpentExperience() const { return Lv(0x13c); }
    long GetRenownLevelMaxLevel() const { return Lv(0x74); }
    long GetMaxMoney() const { long a = Lv(0x40), b = Lv(0x3c); return a < b ? b : a; }
    void UpdateAppearanceStats(bool) { Tok("f57d23b"); }
    CWideString GetFavouriteWeapon(long kind) const { Tok("f57aeb4"); return CWideString("weapon" + Num(kind), 0); }
    long GetFavouriteAbility() const { Tok("f57aa5b"); return gFavouriteAbility; }
    long GetNumberOfAbilityCasts() const { Tok("f57aaa8"); return 71; }
    long GetNumberOfAvailableAbilities() const { Tok("f57b3e2"); return 72; }
    EHeroSexuality GetHeroSexuality() const { Tok("f57acc4"); return HERO_SEXUALITY_MOCK; }
    long GetNumberOfCurrentSpouses() const { Tok("f57ad1d"); return 73; }
    CWideString GetFavouriteRegion(long& count) const { Tok("f57a989"); count = 4; return CWideString("region", 0); }
    long GetNumRegionsExplored() const { Tok("f57aa1b"); return 74; }
    CWideString GetDrinkResistance() const { Tok("f57a7b9"); return CWideString("resistance", 0); }
    long GetHeroStatExperience(EHeroExperienceStatCategory c) const { Tok("f57a8af"); return 1000 * ((long)c + 1) + (long)c; }
    float GetPercentageThroughRenownLevel() const { Tok("f57aad9"); return 0.25f; }
};
struct CTCHeroMorph { void GetTattooNames(std::vector<CWideString>& v) { Tok("f71ce80"); for (int i = 0; i < gTattoos; ++i) v.push_back(CWideString("tattoo" + Num(i), 0)); } };
struct CTCHero { CWideString GetTitleGuiText() const { Tok("f6ca0a0"); return CWideString(gTitle, 0); } };
struct CTCInventoryExperience { CWideString GetExperienceAbilityName(long a) { Tok("f56f2bd"); return CWideString("ability" + Num(a), 0); } };
struct CTCInventory {
    void GetAllItems(std::vector<std::pair<long, long> >& v) const { Tok("f5bf482"); for (int i = 0; i < gItems; ++i) v.push_back(std::pair<long, long>(7 + i, 3 + i)); }
};
struct CTCInventoryItem { static bool IsTrophy(long id) { Tok("f5d9930"); return id != 8; } };
class CTCHeroExperience;
static CTCHeroStats gHeroStats; static CTCHeroMorph gHeroMorph; static CTCHero gHero; static CTCInventoryExperience gInventoryExperience; static CTCInventory gInventoryObject;
static void Fill(CTCHeroStats** p) { *p = &gHeroStats; }
static void Fill(CTCHeroMorph** p) { *p = &gHeroMorph; }
static void Fill(CTCHero** p) { *p = &gHero; }
struct CThing {
    template <class T> void GetTC(T** p) { Tok("f410de0"); Fill(p); }
    template <class T> void GetTC(ETCInterfaceType type, T** p) { Tok("f410de0"); *p = type == TCI_INVENTORY_EXPERIENCE ? (T*)(void*)&gInventoryExperience : (T*)0; }
    bool PeekTC(const CTCInventory** p) { Tok("f410de0"); if (!gInventory) return false; Tok("f4365b0"); *p = &gInventoryObject; return true; }
};
struct CGameTimeManager { long GetDayCount() const { return 42; } };
struct COpinionReactionManager {
    float GetHeroStatsInOpinionFormat(EOpinion o) const { Tok("f7009d0"); return o == OPINION_MORALITY ? gGoodness : 0.125f * (float)((long)o + 1); }
};
struct CWorld {
    CGameTimeManager time; COpinionReactionManager opinion;
    static unsigned long GetFrame() { Tok("f49d870"); return 1234; }
    const CGameTimeManager& PeekGameTimeManager() const { return time; }
    COpinionReactionManager& GetOpinionReactionManager() { return opinion; }
};
static CWorld gWorld;
class CTCInventoryStats {
public:
    CThing* ParentThing; std::map<unsigned long, long> CreaturesKilled;
    const CWorld& PeekWorld() const { Tok("f686a80"); return gWorld; }
    CWorld& GetWorld() { Tok("f686a70"); return gWorld; }
    CWideString BuildRenownString() { Tok("f5cd433"); return CWideString("renown", 0); }
    CWideString BuildHeroClass() { Tok("f5cd4c4"); return CWideString("class", 0); }
    void ConvertMap(const std::map<unsigned long, long>& m, std::list<std::pair<CWideString, long> >& l)
    {
        Tok("f5cd6b7");
        for (std::map<unsigned long, long>::const_iterator i = m.begin(); i != m.end(); ++i) l.push_back(std::pair<CWideString, long>(CWideString("creature" + Num((long)i->first), 0), i->second));
    }
    void CreateTitlePC(const CWideString& text, CComponent*, bool title) { Close("title"); Entry e; e.text = text.s; e.title = title; gPage.push_back(e); }
    void CreateTitle(const CWideString& text, CComponent* c, bool title) { CreateTitlePC(text, c, title); }
    CWideString GetFatnessAsString(float f) { Tok("f5cc862"); return CWideString("fat" + Num((long)f), 0); }
    CWideString GetHeroSexualityAsString(EHeroSexuality s) { Tok("f5ccbbd"); return CWideString("sexuality" + Num((long)s), 0); }
    static CWideString GetTimeSpentInGame(unsigned long frame, const NGameText::CDataBank*) { Tok("f5ccc91"); return CWideString("time" + Num((long)frame), 0); }
    CComponent* ConstructPersonalityListPC(CComponent* parent);
    CComponent* ConstructPersonalityList(CComponent* parent);
};
// ---- the row, statement for statement (the same text as the row source) ----
CComponent* CTCInventoryStats::ConstructPersonalityList(CComponent* parent)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    CWideString newLine(L"\n");
    CWideString colon(L": ");
    CWideString empty(L"");
    CWideString space(L" ");
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CTCHeroExperience* heroExperience;
    ParentThing->GetTC(TCI_HERO_EXPERIENCE, &heroExperience);

    CComponent* list = manager->CreateComponent("UI_STATS_PERSONALITY_LIST", false);
    CCountedPointer<CAction> backAction(new CAction);
    backAction->ActionType = NUISystem::UI_ACTION_TYPE_CHANGESTATE;
    backAction->UIActionParams.push_back(parent->FindChild("UI_TOP_LEVEL_MENU_SCREEN"));
    backAction->UIActionParams.push_back(new CActionParamState(1));
    CComponent* dummyEntry = list->FindChild("UI_STATS_PERS_DUMMY_MENU_ENTRY");
    if (dummyEntry)
        dummyEntry->AddActionOnBack(backAction);

    CComponent* screen = list->FindChild("UI_STATS_PERSONALITY_SCREEN");
    if (screen) {
        CTCHeroMorph* heroMorph;
        ParentThing->GetTC(&heroMorph);
        CTCHeroStats* heroStats;
        ParentThing->GetTC(&heroStats);
        heroStats->UpdateAppearanceStats(true);
        CComponent* container = screen->FindChild("UI_PERSONALITY_COMPONENT_CONTAINER");
        CWideString text;
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("HERO")), container, true);
        CTCHero* hero;
        ParentThing->GetTC(&hero);
        CWideString title = hero->GetTitleGuiText();
        std::vector<CWideString> titleParts;
        titleParts.push_back(BuildRenownString());
        titleParts.push_back(BuildHeroClass());
        if (title.GetLength() == 0) {
            titleParts.push_back(CWideString::NullString);
            titleParts.push_back(CWideString::NullString);
        } else {
            titleParts.push_back(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CALLED")));
            titleParts.push_back(title);
        }
        CWideString heroTitle;
        heroTitle = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TITLE_BUILDER"));
        NGameText::ConstructString(heroTitle, titleParts);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TITLE")) + colon + heroTitle + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("AGE")) + colon + GFIntToWideString((long)heroStats->GetAge()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("BODY_WEIGHT")) + colon + GetFatnessAsString(heroStats->GetFatness()) + newLine;
        text += GetTimeSpentInGame(CWorld::GetFrame(), 0) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMDAYS")) + colon + GFIntToWideString(PeekWorld().PeekGameTimeManager().GetDayCount()) + newLine + newLine;
        CreateTitle(text, container, false);
        std::vector<CWideString> tattooNames;
        heroMorph->GetTattooNames(tattooNames);
        if (tattooNames.size() != 0) {
            CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TATTOOS")), container, true);
            text = empty;
            for (unsigned long i = 0; i != tattooNames.size(); ++i) {
                text += tattooNames[i];
                text += newLine;
            }
            text += newLine;
            CreateTitle(text, container, false);
        }
        if (CreaturesKilled.size() != 0) {
            CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CREATURESKILLED")), container, true);
            text = empty;
            std::list<std::pair<CWideString, long> > creatures;
            ConvertMap(CreaturesKilled, creatures);
            std::list<std::pair<CWideString, long> >::iterator it;
            for (it = creatures.begin(); it != creatures.end(); ++it)
                text += it->first + colon + GFIntToWideString(it->second) + newLine;
            text += newLine;
            CreateTitle(text, container, false);
        }
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("COMBAT")), container, true);
        text = empty;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVWEAPON")) + colon + heroStats->GetFavouriteWeapon(0) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVMELWEAPON")) + colon + heroStats->GetFavouriteWeapon(1) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVRANGEWEAPON")) + colon + heroStats->GetFavouriteWeapon(2) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("DECAPITATIONS")) + colon + GFIntToWideString(heroStats->GetNumberOfDecapitations()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MAXMULTIPLIER")) + colon + GFIntToWideString(heroStats->GetMaximumMultiplier()) + newLine;
        text += newLine;
        CreateTitle(text, container, false);
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MAGIC")), container, true);
        long favouriteAbility = heroStats->GetFavouriteAbility();
        CTCInventoryExperience* inventoryExperience;
        ParentThing->GetTC(TCI_INVENTORY_EXPERIENCE, &inventoryExperience);
        CWideString abilityName;
        if (favouriteAbility == -1)
            abilityName = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NONE"));
        else
            abilityName = inventoryExperience->GetExperienceAbilityName(favouriteAbility);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVABILITY")) + colon + abilityName + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMCASTS")) + colon + GFIntToWideString(heroStats->GetNumberOfAbilityCasts()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMABILITIES")) + colon + GFIntToWideString(heroStats->GetNumberOfAvailableAbilities()) + newLine + newLine;
        CreateTitle(text, container, false);
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("ROMANCE")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEXUALITY")) + colon + GetHeroSexualityAsString(heroStats->GetHeroSexuality()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMSPOUSES")) + colon + GFIntToWideString(heroStats->GetNumberOfCurrentSpouses()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMINLOVE")) + colon + GFIntToWideString(heroStats->GetNumberOfLoversAcquired()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMWEDDINGS")) + colon + GFIntToWideString(heroStats->GetNumberOfWeddings()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMDIVORCES")) + colon + GFIntToWideString(heroStats->GetNumberOfDivorces()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMMURDEREDSPOUSES")) + colon + GFIntToWideString(heroStats->GetNumberOfSpousesMurdered()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMTIMESHADSEX")) + colon + GFIntToWideString(heroStats->GetNumberOfTimesHadSex()) + newLine + newLine;
        CreateTitle(text, container, false);
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("REGIONS")), container, true);
        long favouriteRegion;
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVREGION")) + colon + heroStats->GetFavouriteRegion(favouriteRegion) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPLORED_REGIONS")) + colon + GFIntToWideString(heroStats->GetNumRegionsExplored()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUM_TELEPORTS")) + colon + GFIntToWideString(heroStats->GetNumTeleports()) + newLine + newLine;
        CreateTitle(text, container, false);
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("QUESTS")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("QUESTS_COMPLETED")) + colon + GFIntToWideString(heroStats->GetNumCompletedAQuest()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("QUESTREWARD")) + colon + GFIntToWideString(heroStats->GetRewardMoney()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMCOREQUESTS")) + colon + GFIntToWideString(heroStats->GetNumCompletedACoreQuest()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMOPTQUESTS")) + colon + GFIntToWideString(heroStats->GetNumCompletedAnOptionalQuest()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("BOASTS")) + colon + GFIntToWideString(heroStats->GetNumBoastsCompleted()) + CWideString(L"/") + GFIntToWideString(heroStats->GetNumBoastsTaken()) + newLine;
        long numTrophies = 0;
        const CTCInventory* inventory;
        if (ParentThing->PeekTC(&inventory)) {
            std::vector<std::pair<long, long> > items;
            inventory->GetAllItems(items);
            for (unsigned long i = 0; i < items.size(); ++i) {
                if (CTCInventoryItem::IsTrophy(items[i].first))
                    numTrophies += items[i].second;
            }
            text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TROPHIES")) + colon + GFIntToWideString(heroStats->GetNumMountedTrophies() + numTrophies) + newLine;
        }
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TROPHIESUSED")) + colon + GFIntToWideString(heroStats->GetNumTrophyGamePlayed()) + newLine;
        if (heroStats->GetNumTrophyGamePlayed() > 0)
            text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("BESTTROPHY")) + colon + GFIntToWideString(heroStats->GetBestTrophyWitnesses()) + newLine;
        text += newLine;
        CreateTitle(text, container, false);
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOWNS")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMBUILDINGS")) + colon + GFIntToWideString(heroStats->GetNumberOfHousesOwned() + heroStats->GetNumberOfShopsOwned()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMHOUSES")) + colon + GFIntToWideString(heroStats->GetNumberOfHousesOwned()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMSHOPS")) + colon + GFIntToWideString(heroStats->GetNumberOfShopsOwned()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMRENTS")) + colon + GFIntToWideString(heroStats->GetNumberOfBuildingsRented()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOTALRENT")) + colon + GFIntToWideString(heroStats->GetRentMoney()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CHICKENKICK")) + colon + GFFloatToWideString(GFRoundFloatNear(heroStats->GetMaxChickenThrow(), 2)) + L" " + bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("METRES")) + newLine + newLine;
        CWideString grams = bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_GRAMS");
        float biggestFish = GFRoundFloatNear(heroStats->GetBiggestFish(), 2);
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FISHING")) + colon + GFFloatToWideString(biggestFish) + L" " + grams + newLine + newLine;
        CreateTitle(text, container, false);
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("PUBS")), container, true);
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("COINGOLFOV")) + colon + GFIntToWideString(GFFloatToLongFloor(heroStats->GetBestScoreCoinGolfOakVale())) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_STROKES") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("COINGOLFSS")) + colon + GFIntToWideString(GFFloatToLongFloor(heroStats->GetBestScoreCoinGolfSnowSpire())) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_STROKES") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SHOVEHAPENNY")) + colon + GFIntToWideString(GFFloatToLongFloor(heroStats->GetBestScoreShoveHaPenny())) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_POINTS") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("GUESS")) + colon + GFFloatToWideString(GFRoundFloatNear(heroStats->GetBestTimeGuessTheAddition(), 1)) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_SECONDS") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("PAIRS")) + colon + GFFloatToWideString(GFRoundFloatNear(heroStats->GetBestTimePairs(), 1)) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_SECONDS") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SORTING")) + colon + GFFloatToWideString(GFRoundFloatNear(heroStats->GetBestTimeSorting(), 1)) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_SECONDS") + newLine;
        text += newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("VOMITS")) + colon + GFIntToWideString(heroStats->GetNumberOfVomits()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("DRINKS")) + colon + GFIntToWideString(heroStats->GetNumberOfDrinks()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("DRINKERCLASS")) + colon + heroStats->GetDrinkResistance() + newLine + newLine;
        CreateTitle(text, container, false);
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRIME")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMCRIMES")) + colon + GFIntToWideString(heroStats->GetNumberOfCrimes()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOTALFINES")) + colon + GFIntToWideString(heroStats->GetTotalFine()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOTALFINESPAID")) + colon + GFIntToWideString(heroStats->GetFinePaid()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("THROWNOUT")) + colon + GFIntToWideString(heroStats->GetTimesThrownOut()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOTALBRIBE")) + colon + GFIntToWideString(heroStats->GetTotalBribeAmount()) + newLine + newLine;
        CreateTitle(text, container, false);
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MONEY")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MONEYMAX")) + colon + GFIntToWideString(heroStats->GetMaxMoney()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MONEYEARNED")) + colon + GFIntToWideString(heroStats->GetTotalMoneyEarned()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MONEYSPENT")) + colon + GFIntToWideString(heroStats->GetTotalMoneySpent()) + newLine + newLine;
        CreateTitle(text, container, false);
        CreateTitle(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPER")), container, true);
        long totalExperience = heroStats->GetHeroStatExperience(NUMBER_OF_HERO_STAT_CATEGORIES) + heroStats->GetHeroStatExperience(HERO_STAT_WILL) + heroStats->GetHeroStatExperience(HERO_STAT_SKILL) + heroStats->GetHeroStatExperience(HERO_STAT_STRENGTH);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERTOTAL")) + colon + GFIntToWideString(totalExperience) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERGENERAL")) + colon + GFIntToWideString(heroStats->GetHeroStatExperience(NUMBER_OF_HERO_STAT_CATEGORIES)) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERSTRENGTH")) + colon + GFIntToWideString(heroStats->GetHeroStatExperience(HERO_STAT_STRENGTH)) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERSKILL")) + colon + GFIntToWideString(heroStats->GetHeroStatExperience(HERO_STAT_SKILL)) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERWILL")) + colon + GFIntToWideString(heroStats->GetHeroStatExperience(HERO_STAT_WILL)) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERSPENT")) + colon + GFIntToWideString(heroStats->GetSpentExperience()) + newLine + newLine;
        CreateTitle(text, container, false);
        CComponent* sliderContainer = screen->FindChild("UI_STATS_EXP_STATIC_SLIDER_CONTAINER_NOT_SCROLLING");
        if (sliderContainer) {
            CCountedPointer<CComponent> slider;
            float percentage = heroStats->GetPercentageThroughRenownLevel();
            long renownLevel = heroStats->GetRenownLevel();
            slider.Reset(manager->CreateTickSlider(bank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_RENOWN"), C2DCoordI(5, 0), heroStats->GetRenownLevel(), heroStats->GetRenownLevelMaxLevel(),
                                                   CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f), (float)renownLevel + percentage, 275.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 0.0f));
            sliderContainer->AddChild(slider);
            float goodness = GetWorld().GetOpinionReactionManager().GetHeroStatsInOpinionFormat(OPINION_MORALITY);
            CRGBFloatColour colour(0.5f, 0.1f, 0.1f, 1.0f);
            if (goodness >= 0.0f)
                colour = CRGBFloatColour(0.8f, 0.8f, 0.8f, 1.0f);
            slider.Reset(manager->CreateTickSlider(bank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_ALIGNMENT"), C2DCoordI(5, 30), 0, 14,
                                                   colour, goodness * 7.0f, 285.0f, true, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 0.0f));
            sliderContainer->AddChild(slider);
            float attractiveness = GetWorld().GetOpinionReactionManager().GetHeroStatsInOpinionFormat(OPINION_ATTRACTIVENESS);
            slider.Reset(manager->CreateStaticSliderBidirectional(bank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_ATTRACTIVENESS"), attractiveness * 100.0f, C2DCoordI(285, 0)));
            sliderContainer->AddChild(slider);
            float scariness = GetWorld().GetOpinionReactionManager().GetHeroStatsInOpinionFormat(OPINION_SCARINESS);
            slider.Reset(manager->CreateStaticSliderBidirectional(bank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_SCARINESS"), scariness * 100.0f, C2DCoordI(285, 30)));
            sliderContainer->AddChild(slider);
        }
    }
    return list;
}
// ---- the retail call order (tools/rtok.py over the retail disassembly; one entry per text statement) ----
static const char* const kRetail[] = {
    "S:HERO S:TEXT_GUI_STATS_LABEL_ S:UI_PERSONALITY_COMPONENT_CONTAINER S:UI_STATS_PERSONALITY_LIST S:UI_STATS_PERSONALITY_SCREEN S:UI_STATS_PERS_DUMMY_MENU_ENTRY S:UI_TOP_LEVEL_MENU_SCREEN f410de0 f410de0 f410de0 f41db1d f41e5f2 f57d23b f9c95e0 v110 vc vc vc vc title",
    "S:CALLED S:TEXT_GUI_STATS_LABEL_ S:TEXT_GUI_STATS_LABEL_ S:TITLE_BUILDER f410de0 f5cd433 f5cd4c4 f6ca0a0 f99b220 f9c95e0 f9c95e0 =",
    "S:TEXT_GUI_STATS_LABEL_ S:TITLE f9c7c20 f9c95e0 =",
    "S:AGE S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o30 +=",
    "S:BODY_WEIGHT S:TEXT_GUI_STATS_LABEL_ f5cc862 f9c95e0 o38 +=",
    "f49d870 f5ccc91 +=",
    "S:NUMDAYS S:TEXT_GUI_STATS_LABEL_ f405f00 f686a80 f9c95e0 +=",
    "title",
    "S:TATTOOS S:TEXT_GUI_STATS_LABEL_ f71ce80 f9c95e0 title",
    "=",
    "+=",
    "+=",
    "+=",
    "title",
    "S:CREATURESKILLED S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "=",
    "f405f00 f5cd6b7 +=",
    "+=",
    "title",
    "S:COMBAT S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "=",
    "S:FAVWEAPON S:TEXT_GUI_STATS_LABEL_ f57aeb4 f9c95e0 +=",
    "S:FAVMELWEAPON S:TEXT_GUI_STATS_LABEL_ f57aeb4 f9c95e0 +=",
    "S:FAVRANGEWEAPON S:TEXT_GUI_STATS_LABEL_ f57aeb4 f9c95e0 +=",
    "S:DECAPITATIONS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o24 +=",
    "S:MAXMULTIPLIER S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o98 +=",
    "+=",
    "title",
    "S:MAGIC S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "f410de0 f56f2bd f57aa5b =",
    "S:FAVABILITY S:TEXT_GUI_STATS_LABEL_ f9c95e0 =",
    "S:NUMCASTS S:TEXT_GUI_STATS_LABEL_ f405f00 f57aaa8 f9c95e0 +=",
    "S:NUMABILITIES S:TEXT_GUI_STATS_LABEL_ f405f00 f57b3e2 f9c95e0 +=",
    "title",
    "S:ROMANCE S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "S:SEXUALITY S:TEXT_GUI_STATS_LABEL_ f57acc4 f5ccbbd f9c95e0 =",
    "S:NUMSPOUSES S:TEXT_GUI_STATS_LABEL_ f405f00 f57ad1d f9c95e0 +=",
    "S:NUMINLOVE S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o140 +=",
    "S:NUMWEDDINGS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o144 +=",
    "S:NUMDIVORCES S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o150 +=",
    "S:NUMMURDEREDSPOUSES S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o148 +=",
    "S:NUMTIMESHADSEX S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o154 +=",
    "title",
    "S:REGIONS S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "S:FAVREGION S:TEXT_GUI_STATS_LABEL_ f57a989 f9c95e0 =",
    "S:EXPLORED_REGIONS S:TEXT_GUI_STATS_LABEL_ f405f00 f57aa1b f9c95e0 +=",
    "S:NUM_TELEPORTS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o10c +=",
    "title",
    "S:QUESTS S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "S:QUESTS_COMPLETED S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 obc =",
    "S:QUESTREWARD S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o50 +=",
    "S:NUMCOREQUESTS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 oc0 +=",
    "S:NUMOPTQUESTS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 oc8 +=",
    "S:BOASTS S:TEXT_GUI_STATS_LABEL_ f405f00 f405f00 f9c95e0 occ od0 +=",
    "S:TEXT_GUI_STATS_LABEL_ S:TROPHIES f405f00 f410de0 f4365b0 f5bf482 f5d9930 f9c95e0 od4 +=",
    "S:TEXT_GUI_STATS_LABEL_ S:TROPHIESUSED f405f00 f9c95e0 od8 +=",
    "S:BESTTROPHY S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 od8 odc +=",
    "+=",
    "title",
    "S:TEXT_GUI_STATS_LABEL_ S:TOWNS f9c95e0 title",
    "S:NUMBUILDINGS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 oe0 oe4 =",
    "S:NUMHOUSES S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 oe4 +=",
    "S:NUMSHOPS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 oe0 +=",
    "S:NUMRENTS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 oe8 +=",
    "S:TEXT_GUI_STATS_LABEL_ S:TOTALRENT f405f00 f9c95e0 o54 +=",
    "S:CHICKENKICK S:METRES S:TEXT_GUI_STATS_LABEL_ S:TEXT_GUI_STATS_LABEL_ f486c40 f9c95e0 f9c95e0 o110 +=",
    "S:FISHING S:TEXT_GUI_SCOREBOARD_UNITS_GRAMS S:TEXT_GUI_STATS_LABEL_ f486c40 f9c95e0 f9c95e0 o15c +=",
    "title",
    "S:PUBS S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "S:COINGOLFOV S:TEXT_GUI_SCOREBOARD_UNITS_STROKES S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 f9c95e0 o184 +=",
    "S:COINGOLFSS S:TEXT_GUI_SCOREBOARD_UNITS_STROKES S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 f9c95e0 o188 +=",
    "S:SHOVEHAPENNY S:TEXT_GUI_SCOREBOARD_UNITS_POINTS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 f9c95e0 o18c +=",
    "S:GUESS S:TEXT_GUI_SCOREBOARD_UNITS_SECONDS S:TEXT_GUI_STATS_LABEL_ f486c40 f9c95e0 f9c95e0 o190 +=",
    "S:PAIRS S:TEXT_GUI_SCOREBOARD_UNITS_SECONDS S:TEXT_GUI_STATS_LABEL_ f486c40 f9c95e0 f9c95e0 o178 +=",
    "S:SORTING S:TEXT_GUI_SCOREBOARD_UNITS_SECONDS S:TEXT_GUI_STATS_LABEL_ f486c40 f9c95e0 f9c95e0 o17c +=",
    "+=",
    "S:TEXT_GUI_STATS_LABEL_ S:VOMITS f405f00 f9c95e0 oec +=",
    "S:DRINKS S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 of0 +=",
    "S:DRINKERCLASS S:TEXT_GUI_STATS_LABEL_ f57a7b9 f9c95e0 +=",
    "title",
    "S:CRIME S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "S:NUMCRIMES S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 of4 =",
    "S:TEXT_GUI_STATS_LABEL_ S:TOTALFINES f405f00 f9c95e0 ofc +=",
    "S:TEXT_GUI_STATS_LABEL_ S:TOTALFINESPAID f405f00 f9c95e0 o100 +=",
    "S:TEXT_GUI_STATS_LABEL_ S:THROWNOUT f405f00 f9c95e0 o104 +=",
    "S:TEXT_GUI_STATS_LABEL_ S:TOTALBRIBE f405f00 f9c95e0 o108 +=",
    "title",
    "S:MONEY S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "S:MONEYMAX S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o3c o40 =",
    "S:MONEYEARNED S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o48 +=",
    "S:MONEYSPENT S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o4c +=",
    "title",
    "S:EXPER S:TEXT_GUI_STATS_LABEL_ f9c95e0 title",
    "S:EXPERTOTAL S:TEXT_GUI_STATS_LABEL_ f405f00 f57a8af f57a8af f57a8af f57a8af f9c95e0 =",
    "S:EXPERGENERAL S:TEXT_GUI_STATS_LABEL_ f405f00 f57a8af f9c95e0 +=",
    "S:EXPERSTRENGTH S:TEXT_GUI_STATS_LABEL_ f405f00 f57a8af f9c95e0 +=",
    "S:EXPERSKILL S:TEXT_GUI_STATS_LABEL_ f405f00 f57a8af f9c95e0 +=",
    "S:EXPERWILL S:TEXT_GUI_STATS_LABEL_ f405f00 f57a8af f9c95e0 +=",
    "S:EXPERSPENT S:TEXT_GUI_STATS_LABEL_ f405f00 f9c95e0 o13c +=",
    "title",
    "S:TEXT_GUI_STATS_LABEL_ALIGNMENT S:TEXT_GUI_STATS_LABEL_ATTRACTIVENESS S:TEXT_GUI_STATS_LABEL_RENOWN S:TEXT_GUI_STATS_LABEL_SCARINESS S:UI_STATS_EXP_STATIC_SLIDER_CONTAINER_NOT_SCROLLING f427216 f427216 f42819a f42819a f57aad9 f686a70 f686a70 f686a70 f7009d0 f7009d0 f7009d0 f9c95e0 f9c95e0 f9c95e0 f9c95e0 o70 o74 vc vec vec vec vec end",
};
// ---- driver ----
static void Reset()
{
    gTok.clear(); gSeg.clear(); gActions.clear(); gChildren.clear(); gParts.clear(); gPage.clear(); gSliders.clear(); gPositionChildren = 0;
    gOverride.clear(); gMissing.clear();
    gTitle = "Chicken Chaser"; gFavouriteAbility = 5; gTattoos = 1; gItems = 1; gInventory = true; gGoodness = 0.5f;
}
static CComponent* Run(CTCInventoryStats& stats, bool creatures)
{
    static CThing thing; static CComponent parent("PARENT");
    stats.ParentThing = &thing; stats.CreaturesKilled.clear();
    if (creatures) stats.CreaturesKilled[3] = 9;
    CComponent* r = stats.ConstructPersonalityList(&parent);
    Close("end");
    return r;
}
static bool Has(const Str& line)
{
    for (size_t i = 0; i < gPage.size(); ++i) if (gPage[i].text.find(line) != Str::npos) return true;
    return false;
}
static bool HasTitle(const char* name)
{
    for (size_t i = 0; i < gPage.size(); ++i) if (gPage[i].title && gPage[i].text == Str("<TEXT_GUI_STATS_LABEL_") + name + ">") return true;
    return false;
}
static Str L(const char* name) { return Str("<TEXT_GUI_STATS_LABEL_") + name + ">: "; }
static Str F2(double v) { char b[32]; std::sprintf(b, "%.2f", v); return b; }
#define CHECK(c) do { if (!(c)) { ok = 0; std::printf("  failed line %d: %s\n", __LINE__, #c); } } while (0)

int main()
{
    int ok = 1;
    CTCInventoryStats stats;
    const CTCHeroStats& h = gHeroStats;

    // run 1: everything present, every loop once -- the whole retail call order, statement by statement
    Reset();
    CComponent* r = Run(stats, true);
    CHECK(r && r->name == "UI_STATS_PERSONALITY_LIST");
    CHECK(gSeg.size() == sizeof(kRetail) / sizeof(kRetail[0]));
    for (size_t i = 0; i < gSeg.size() && i < sizeof(kRetail) / sizeof(kRetail[0]); ++i)
        if (gSeg[i] != kRetail[i]) { ok = 0; std::printf("  statement %u: built [%s] retail [%s]\n", (unsigned)i, gSeg[i].c_str(), kRetail[i]); break; }
    CHECK(gPage.size() >= 3 && gPage[0].title && gPage[0].text == "<TEXT_GUI_STATS_LABEL_HERO>" && !gPage[1].title);
    CHECK(gPage.size() >= 3 && gPage[1].text.find(L("TITLE") + "<TEXT_GUI_STATS_LABEL_TITLE_BUILDER>[renown][class][<TEXT_GUI_STATS_LABEL_CALLED>][Chicken Chaser]\n" + L("AGE") + "48\n" + L("BODY_WEIGHT") + "fat56\ntime1234\n" + L("NUMDAYS") + "42\n\n") == 0);
    CHECK(HasTitle("TATTOOS") && Has("tattoo0\n\n") && HasTitle("CREATURESKILLED") && Has("creature3: 9\n\n"));
    CHECK(Has(L("FAVWEAPON") + "weapon0\n" + L("FAVMELWEAPON") + "weapon1\n" + L("FAVRANGEWEAPON") + "weapon2\n" + L("DECAPITATIONS") + Num(h.GetNumberOfDecapitations()) + "\n" + L("MAXMULTIPLIER") + Num(h.GetMaximumMultiplier()) + "\n\n"));
    CHECK(Has(L("FAVABILITY") + "ability5\n" + L("NUMCASTS") + "71\n" + L("NUMABILITIES") + "72\n\n"));
    CHECK(Has(L("SEXUALITY") + "sexuality2\n" + L("NUMSPOUSES") + "73\n") && Has(L("FAVREGION") + "region\n" + L("EXPLORED_REGIONS") + "74\n"));
    CHECK(Has(L("BOASTS") + Num(h.GetNumBoastsCompleted()) + "/" + Num(h.GetNumBoastsTaken()) + "\n" + L("TROPHIES") + Num(h.GetNumMountedTrophies() + 3) + "\n" + L("TROPHIESUSED") + Num(h.GetNumTrophyGamePlayed()) + "\n" + L("BESTTROPHY") + Num(h.GetBestTrophyWitnesses()) + "\n\n"));
    CHECK(Has(L("NUMBUILDINGS") + Num(h.GetNumberOfHousesOwned() + h.GetNumberOfShopsOwned()) + "\n" + L("NUMHOUSES") + Num(h.GetNumberOfHousesOwned()) + "\n" + L("NUMSHOPS") + Num(h.GetNumberOfShopsOwned()) + "\n"));
    CHECK(Has(L("CHICKENKICK") + F2(h.GetMaxChickenThrow()) + " <TEXT_GUI_STATS_LABEL_METRES>\n\n" + L("FISHING") + F2(h.GetBiggestFish()) + " <TEXT_GUI_SCOREBOARD_UNITS_GRAMS>\n\n"));
    CHECK(Has(L("COINGOLFOV") + Num((long)h.GetBestScoreCoinGolfOakVale()) + " <TEXT_GUI_SCOREBOARD_UNITS_STROKES>\n" + L("COINGOLFSS") + Num((long)h.GetBestScoreCoinGolfSnowSpire()) + " <TEXT_GUI_SCOREBOARD_UNITS_STROKES>\n" + L("SHOVEHAPENNY") + Num((long)h.GetBestScoreShoveHaPenny()) + " <TEXT_GUI_SCOREBOARD_UNITS_POINTS>\n"));
    CHECK(Has(L("GUESS") + F2((long)h.GetBestTimeGuessTheAddition() + 0.3) + " <TEXT_GUI_SCOREBOARD_UNITS_SECONDS>\n" + L("PAIRS") + F2((long)h.GetBestTimePairs() + 0.3) + " <TEXT_GUI_SCOREBOARD_UNITS_SECONDS>\n" + L("SORTING") + F2((long)h.GetBestTimeSorting() + 0.3) + " <TEXT_GUI_SCOREBOARD_UNITS_SECONDS>\n\n" + L("VOMITS")));
    CHECK(Has(L("DRINKERCLASS") + "resistance\n\n") && Has(L("MONEYMAX") + Num(OFF_MaxMoney + 1) + "\n"));
    CHECK(Has(L("EXPERTOTAL") + "10006\n" + L("EXPERGENERAL") + "4003\n" + L("EXPERSTRENGTH") + "1000\n" + L("EXPERSKILL") + "2001\n" + L("EXPERWILL") + "3002\n" + L("EXPERSPENT") + Num(h.GetSpentExperience()) + "\n\n"));
    CHECK(gPage.size() == 26 && !gPage.back().title);
    CHECK(gActions.size() == 1 && gPositionChildren == 0 && gChildren.size() == 4);
    CHECK(gActions.size() == 1 && gActions[0] == "UI_STATS_PERS_DUMMY_MENU_ENTRY.back:3:UI_TOP_LEVEL_MENU_SCREEN=-1:state=1");
    CHECK(gSliders.size() == 4);
    if (gSliders.size() == 4) {
        const Slider& a = gSliders[0]; const Slider& b = gSliders[1]; const Slider& c = gSliders[2]; const Slider& d = gSliders[3];
        CHECK(a.label == "<TEXT_GUI_STATS_LABEL_RENOWN>" && a.x == 5 && a.y == 0 && a.level == h.GetRenownLevel() && a.maxLevel == h.GetRenownLevelMaxLevel() && a.value == (float)h.GetRenownLevel() + 0.25f && a.width == 275.0f && a.red == 1.0f && !a.flag);
        CHECK(b.label == "<TEXT_GUI_STATS_LABEL_ALIGNMENT>" && b.x == 5 && b.y == 30 && b.level == 0 && b.maxLevel == 14 && b.value == 3.5f && b.width == 285.0f && b.red == 0.8f && b.flag);
        CHECK(c.label == "<TEXT_GUI_STATS_LABEL_ATTRACTIVENESS>" && c.x == 285 && c.y == 0 && c.value == 62.5f);
        CHECK(d.label == "<TEXT_GUI_STATS_LABEL_SCARINESS>" && d.x == 285 && d.y == 30 && d.value == 37.5f);
    }

    // run 2: no title, no favourite ability, no tattoos, creatures, inventory or trophy games, no dummy menu entry, no slider container
    Reset();
    gTitle = ""; gFavouriteAbility = -1; gTattoos = 0; gInventory = false; gGoodness = -0.5f; gOverride[OFF_NumTrophyGamePlayed] = 0;
    gMissing.push_back("UI_STATS_PERS_DUMMY_MENU_ENTRY"); gMissing.push_back("UI_STATS_EXP_STATIC_SLIDER_CONTAINER_NOT_SCROLLING");
    r = Run(stats, false);
    CHECK(r && gParts.size() == 4 && gParts[0] == "renown" && gParts[1] == "class" && gParts[2] == "" && gParts[3] == "");
    CHECK(!HasTitle("TATTOOS") && !HasTitle("CREATURESKILLED") && HasTitle("COMBAT") && gPage.size() == 22);
    CHECK(Has(L("FAVABILITY") + "<TEXT_GUI_STATS_LABEL_NONE>\n") && !Has("ability"));
    CHECK(!Has(L("TROPHIES")) && Has(L("TROPHIESUSED") + "0\n\n") && !Has(L("BESTTROPHY")));
    CHECK(gActions.empty() && gSliders.empty() && gChildren.empty());

    // run 3: two inventory items (one is not a trophy), evil hero, money above the recorded maximum, no top-level menu screen
    Reset();
    gItems = 2; gGoodness = -0.5f; gOverride[OFF_Money] = 5000; gMissing.push_back("UI_TOP_LEVEL_MENU_SCREEN");
    r = Run(stats, true);
    CHECK(Has(L("TROPHIES") + Num(h.GetNumMountedTrophies() + 3) + "\n") && Has(L("MONEYMAX") + "5000\n"));
    CHECK(gSliders.size() == 4 && gSliders[1].red == 0.5f && gSliders[1].value == -3.5f);
    CHECK(gActions.size() == 1 && gActions[0] == "UI_STATS_PERS_DUMMY_MENU_ENTRY.back:3:null:state=1");

    // run 4: no screen -- only the list and its back action
    Reset();
    gMissing.push_back("UI_STATS_PERSONALITY_SCREEN");
    r = Run(stats, true);
    CHECK(r && r->name == "UI_STATS_PERSONALITY_LIST" && gPage.empty() && gSliders.empty() && gActions.size() == 1 && gChildren.empty());

    if (ok) std::printf("005ce0e6_TEST PASS\n"); else std::printf("005ce0e6_TEST FAIL\n");
    return ok ? 0 : 1;
}
