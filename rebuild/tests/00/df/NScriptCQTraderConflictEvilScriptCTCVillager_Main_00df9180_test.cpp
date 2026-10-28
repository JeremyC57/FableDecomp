// Behaviour test for 0x00DF9180 NScript::CQ_TraderConflictEvilScript::CTC_Villager::Main: the source model is copied
// verbatim (filler virtual slots given empty bodies) and every callee is a recording stub driven by a frame counter.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
// NScript::CQ_TraderConflictEvilScript::CTC_Villager::Main() -- a Barrowfields villager in the trader-conflict (evil) quest: alive filter on Me, quit unless BarrowFields is loaded, wait for the quest start screen, make Me a scared, non-combatant non-ally of the hero, then until the player engages heal Me when hit by anyone but the hero and flag the engagement when the hero hits Me; afterwards panic at the hero (a sex-specific scream line) whenever the scream-out timer has run out and the hero is within 15 units
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF9180, 1215 bytes (+ int3 padding); entity-script vtable slot 1 (Main);
//   PDB ?Main@CTC_Villager@CQ_TraderConflictEvilScript@NScript@@EAEXXZ.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 (the CIsThingAlive predicate, vtable 0x012C32F0), SetPredicate
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0, IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall), rand 0x00BFEB16; CScriptThing slots 21 MsgIsHitBy, 41 MsgIsHitBySpecialAbilityFrom, 42
//   MsgIsHitByAnySpecialAbilityFrom; GameCalls (this->Game, and the global interface 0x0143E8F8 for the timer) slots 7
//   NewScriptFrame, 12 IsRegionLoaded, 70 GetHero, 89 SetTimer, 90 GetTimer, 265 ModifyThingHealth, 361
//   ClearThingHasInformation, 364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation, 496
//   EntitySetAsScared, 513 EntityGetSex, 592 EntitySetCombatEnabled, 600 EntityUnsetThingAsAllyOfThing.
// Strings: "BarrowFields" 0x012C5D94, "" 0x0122D70E, "SCRIPT_NAME_HERO" 0x0125D1C8,
//   "TEXT_QST_B12_VILLAGER_FEMALE_PANIC" 0x012DEA6C, "TEXT_QST_B12_VILLAGER_MALE_PANIC" 0x012DEA48.
// Layout: Ego_r.pdb CTC_Villager (0x1c): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
//   CQ_TraderConflictEvilScript sits 0x14 below the Ego_r.pdb offsets (the quest-script shift) and its AllCreatures vector
//   is the 12-byte retail STLport vector: QuestStartScreened +0x60, PlayerEngaged +0x6a, ScreamOutTimer (CTimer) +0x6c.
// Shape: the landed CMuteGuardOutsideWoods::Main 0x00D86150 family: each hit test owns conditional CCharString temporaries
//   (flag word in ebx, destroyed in reverse) and is materialised as a bool before its branch; the timer is read through the
//   inline CTimer::GetTime (`add eax,6Ch ; mov eax,[eax]`) and written through CTimer::SetTime.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
typedef long ESex;
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void s000() {} virtual void s001() {} virtual void s002() {} virtual void s003() {} virtual void s004() {} virtual void s005() {} virtual void s006() {} virtual void s007() {}
    virtual void s008() {} virtual void s009() {} virtual void s010() {} virtual void s011() {} virtual void s012() {} virtual void s013() {} virtual void s014() {} virtual void s015() {}
    virtual void s016() {} virtual void s017() {} virtual void s018() {} virtual void s019() {} virtual void s020() {}
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() {} virtual void s023() {} virtual void s024() {} virtual void s025() {} virtual void s026() {} virtual void s027() {} virtual void s028() {} virtual void s029() {}
    virtual void s030() {} virtual void s031() {} virtual void s032() {} virtual void s033() {} virtual void s034() {} virtual void s035() {} virtual void s036() {} virtual void s037() {}
    virtual void s038() {} virtual void s039() {} virtual void s040() {}
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
struct GameCalls {                                     // CGameScriptInterface vtable 0x01260F0C (ghidra_out/fable_cgsi_slots.h)
    virtual void g000() {} virtual void g001() {} virtual void g002() {} virtual void g003() {} virtual void g004() {} virtual void g005() {} virtual void g006() {}
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void g008() {} virtual void g009() {} virtual void g010() {} virtual void g011() {}
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30)
    virtual void g013() {} virtual void g014() {} virtual void g015() {} virtual void g016() {} virtual void g017() {} virtual void g018() {} virtual void g019() {} virtual void g020() {}
    virtual void g021() {} virtual void g022() {} virtual void g023() {} virtual void g024() {} virtual void g025() {} virtual void g026() {} virtual void g027() {} virtual void g028() {}
    virtual void g029() {} virtual void g030() {} virtual void g031() {} virtual void g032() {} virtual void g033() {} virtual void g034() {} virtual void g035() {} virtual void g036() {}
    virtual void g037() {} virtual void g038() {} virtual void g039() {} virtual void g040() {} virtual void g041() {} virtual void g042() {} virtual void g043() {} virtual void g044() {}
    virtual void g045() {} virtual void g046() {} virtual void g047() {} virtual void g048() {} virtual void g049() {} virtual void g050() {} virtual void g051() {} virtual void g052() {}
    virtual void g053() {} virtual void g054() {} virtual void g055() {} virtual void g056() {} virtual void g057() {} virtual void g058() {} virtual void g059() {} virtual void g060() {}
    virtual void g061() {} virtual void g062() {} virtual void g063() {} virtual void g064() {} virtual void g065() {} virtual void g066() {} virtual void g067() {} virtual void g068() {}
    virtual void g069() {}
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void g071() {} virtual void g072() {} virtual void g073() {} virtual void g074() {} virtual void g075() {} virtual void g076() {} virtual void g077() {} virtual void g078() {}
    virtual void g079() {} virtual void g080() {} virtual void g081() {} virtual void g082() {} virtual void g083() {} virtual void g084() {} virtual void g085() {} virtual void g086() {}
    virtual void g087() {} virtual void g088() {}
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168)
    virtual void g091() {} virtual void g092() {} virtual void g093() {} virtual void g094() {} virtual void g095() {} virtual void g096() {} virtual void g097() {} virtual void g098() {}
    virtual void g099() {} virtual void g100() {} virtual void g101() {} virtual void g102() {} virtual void g103() {} virtual void g104() {} virtual void g105() {} virtual void g106() {}
    virtual void g107() {} virtual void g108() {} virtual void g109() {} virtual void g110() {} virtual void g111() {} virtual void g112() {} virtual void g113() {} virtual void g114() {}
    virtual void g115() {} virtual void g116() {} virtual void g117() {} virtual void g118() {} virtual void g119() {} virtual void g120() {} virtual void g121() {} virtual void g122() {}
    virtual void g123() {} virtual void g124() {} virtual void g125() {} virtual void g126() {} virtual void g127() {} virtual void g128() {} virtual void g129() {} virtual void g130() {}
    virtual void g131() {} virtual void g132() {} virtual void g133() {} virtual void g134() {} virtual void g135() {} virtual void g136() {} virtual void g137() {} virtual void g138() {}
    virtual void g139() {} virtual void g140() {} virtual void g141() {} virtual void g142() {} virtual void g143() {} virtual void g144() {} virtual void g145() {} virtual void g146() {}
    virtual void g147() {} virtual void g148() {} virtual void g149() {} virtual void g150() {} virtual void g151() {} virtual void g152() {} virtual void g153() {} virtual void g154() {}
    virtual void g155() {} virtual void g156() {} virtual void g157() {} virtual void g158() {} virtual void g159() {} virtual void g160() {} virtual void g161() {} virtual void g162() {}
    virtual void g163() {} virtual void g164() {} virtual void g165() {} virtual void g166() {} virtual void g167() {} virtual void g168() {} virtual void g169() {} virtual void g170() {}
    virtual void g171() {} virtual void g172() {} virtual void g173() {} virtual void g174() {} virtual void g175() {} virtual void g176() {} virtual void g177() {} virtual void g178() {}
    virtual void g179() {} virtual void g180() {} virtual void g181() {} virtual void g182() {} virtual void g183() {} virtual void g184() {} virtual void g185() {} virtual void g186() {}
    virtual void g187() {} virtual void g188() {} virtual void g189() {} virtual void g190() {} virtual void g191() {} virtual void g192() {} virtual void g193() {} virtual void g194() {}
    virtual void g195() {} virtual void g196() {} virtual void g197() {} virtual void g198() {} virtual void g199() {} virtual void g200() {} virtual void g201() {} virtual void g202() {}
    virtual void g203() {} virtual void g204() {} virtual void g205() {} virtual void g206() {} virtual void g207() {} virtual void g208() {} virtual void g209() {} virtual void g210() {}
    virtual void g211() {} virtual void g212() {} virtual void g213() {} virtual void g214() {} virtual void g215() {} virtual void g216() {} virtual void g217() {} virtual void g218() {}
    virtual void g219() {} virtual void g220() {} virtual void g221() {} virtual void g222() {} virtual void g223() {} virtual void g224() {} virtual void g225() {} virtual void g226() {}
    virtual void g227() {} virtual void g228() {} virtual void g229() {} virtual void g230() {} virtual void g231() {} virtual void g232() {} virtual void g233() {} virtual void g234() {}
    virtual void g235() {} virtual void g236() {} virtual void g237() {} virtual void g238() {} virtual void g239() {} virtual void g240() {} virtual void g241() {} virtual void g242() {}
    virtual void g243() {} virtual void g244() {} virtual void g245() {} virtual void g246() {} virtual void g247() {} virtual void g248() {} virtual void g249() {} virtual void g250() {}
    virtual void g251() {} virtual void g252() {} virtual void g253() {} virtual void g254() {} virtual void g255() {} virtual void g256() {} virtual void g257() {} virtual void g258() {}
    virtual void g259() {} virtual void g260() {} virtual void g261() {} virtual void g262() {} virtual void g263() {} virtual void g264() {}
    virtual void ModifyThingHealth(const CScriptThing& thing, float amount, bool canKill) const;   // slot 265 (+0x424)
    virtual void g266() {} virtual void g267() {} virtual void g268() {} virtual void g269() {} virtual void g270() {} virtual void g271() {} virtual void g272() {} virtual void g273() {}
    virtual void g274() {} virtual void g275() {} virtual void g276() {} virtual void g277() {} virtual void g278() {} virtual void g279() {} virtual void g280() {} virtual void g281() {}
    virtual void g282() {} virtual void g283() {} virtual void g284() {} virtual void g285() {} virtual void g286() {} virtual void g287() {} virtual void g288() {} virtual void g289() {}
    virtual void g290() {} virtual void g291() {} virtual void g292() {} virtual void g293() {} virtual void g294() {} virtual void g295() {} virtual void g296() {} virtual void g297() {}
    virtual void g298() {} virtual void g299() {} virtual void g300() {} virtual void g301() {} virtual void g302() {} virtual void g303() {} virtual void g304() {} virtual void g305() {}
    virtual void g306() {} virtual void g307() {} virtual void g308() {} virtual void g309() {} virtual void g310() {} virtual void g311() {} virtual void g312() {} virtual void g313() {}
    virtual void g314() {} virtual void g315() {} virtual void g316() {} virtual void g317() {} virtual void g318() {} virtual void g319() {} virtual void g320() {} virtual void g321() {}
    virtual void g322() {} virtual void g323() {} virtual void g324() {} virtual void g325() {} virtual void g326() {} virtual void g327() {} virtual void g328() {} virtual void g329() {}
    virtual void g330() {} virtual void g331() {} virtual void g332() {} virtual void g333() {} virtual void g334() {} virtual void g335() {} virtual void g336() {} virtual void g337() {}
    virtual void g338() {} virtual void g339() {} virtual void g340() {} virtual void g341() {} virtual void g342() {} virtual void g343() {} virtual void g344() {} virtual void g345() {}
    virtual void g346() {} virtual void g347() {} virtual void g348() {} virtual void g349() {} virtual void g350() {} virtual void g351() {} virtual void g352() {} virtual void g353() {}
    virtual void g354() {} virtual void g355() {} virtual void g356() {} virtual void g357() {} virtual void g358() {} virtual void g359() {} virtual void g360() {}
    virtual void ClearThingHasInformation(const CScriptThing& thing) const;   // slot 361 (+0x5a4)
    virtual void g362() {} virtual void g363() {}
    virtual long AddNewConversation(const CScriptThing& thing, bool a, bool b) const;   // slot 364 (+0x5b0)
    virtual void AddPersonToConversation(long conversation, const CScriptThing& person) const;   // slot 365 (+0x5b4)
    virtual void AddLineToConversation(long conversation, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
    virtual void g367() {} virtual void g368() {} virtual void g369() {} virtual void g370() {} virtual void g371() {} virtual void g372() {} virtual void g373() {} virtual void g374() {}
    virtual void g375() {} virtual void g376() {} virtual void g377() {} virtual void g378() {} virtual void g379() {} virtual void g380() {} virtual void g381() {} virtual void g382() {}
    virtual void g383() {} virtual void g384() {} virtual void g385() {} virtual void g386() {} virtual void g387() {} virtual void g388() {} virtual void g389() {} virtual void g390() {}
    virtual void g391() {} virtual void g392() {} virtual void g393() {} virtual void g394() {} virtual void g395() {} virtual void g396() {} virtual void g397() {} virtual void g398() {}
    virtual void g399() {} virtual void g400() {} virtual void g401() {} virtual void g402() {} virtual void g403() {} virtual void g404() {} virtual void g405() {} virtual void g406() {}
    virtual void g407() {} virtual void g408() {} virtual void g409() {} virtual void g410() {} virtual void g411() {} virtual void g412() {} virtual void g413() {} virtual void g414() {}
    virtual void g415() {} virtual void g416() {} virtual void g417() {} virtual void g418() {} virtual void g419() {} virtual void g420() {} virtual void g421() {} virtual void g422() {}
    virtual void g423() {} virtual void g424() {} virtual void g425() {} virtual void g426() {} virtual void g427() {} virtual void g428() {} virtual void g429() {} virtual void g430() {}
    virtual void g431() {} virtual void g432() {} virtual void g433() {} virtual void g434() {} virtual void g435() {} virtual void g436() {} virtual void g437() {} virtual void g438() {}
    virtual void g439() {} virtual void g440() {} virtual void g441() {} virtual void g442() {} virtual void g443() {} virtual void g444() {} virtual void g445() {} virtual void g446() {}
    virtual void g447() {} virtual void g448() {} virtual void g449() {} virtual void g450() {} virtual void g451() {} virtual void g452() {} virtual void g453() {} virtual void g454() {}
    virtual void g455() {} virtual void g456() {} virtual void g457() {} virtual void g458() {} virtual void g459() {} virtual void g460() {} virtual void g461() {} virtual void g462() {}
    virtual void g463() {} virtual void g464() {} virtual void g465() {} virtual void g466() {} virtual void g467() {} virtual void g468() {} virtual void g469() {} virtual void g470() {}
    virtual void g471() {} virtual void g472() {} virtual void g473() {} virtual void g474() {} virtual void g475() {} virtual void g476() {} virtual void g477() {} virtual void g478() {}
    virtual void g479() {} virtual void g480() {} virtual void g481() {} virtual void g482() {} virtual void g483() {} virtual void g484() {} virtual void g485() {} virtual void g486() {}
    virtual void g487() {} virtual void g488() {} virtual void g489() {} virtual void g490() {} virtual void g491() {} virtual void g492() {} virtual void g493() {} virtual void g494() {}
    virtual void g495() {}
    virtual void EntitySetAsScared(const CScriptThing& thing, bool scared) const;   // slot 496 (+0x7c0)
    virtual void g497() {} virtual void g498() {} virtual void g499() {} virtual void g500() {} virtual void g501() {} virtual void g502() {} virtual void g503() {} virtual void g504() {}
    virtual void g505() {} virtual void g506() {} virtual void g507() {} virtual void g508() {} virtual void g509() {} virtual void g510() {} virtual void g511() {} virtual void g512() {}
    virtual ESex EntityGetSex(const CScriptThing& thing) const;   // slot 513 (+0x804)
    virtual void g514() {} virtual void g515() {} virtual void g516() {} virtual void g517() {} virtual void g518() {} virtual void g519() {} virtual void g520() {} virtual void g521() {}
    virtual void g522() {} virtual void g523() {} virtual void g524() {} virtual void g525() {} virtual void g526() {} virtual void g527() {} virtual void g528() {} virtual void g529() {}
    virtual void g530() {} virtual void g531() {} virtual void g532() {} virtual void g533() {} virtual void g534() {} virtual void g535() {} virtual void g536() {} virtual void g537() {}
    virtual void g538() {} virtual void g539() {} virtual void g540() {} virtual void g541() {} virtual void g542() {} virtual void g543() {} virtual void g544() {} virtual void g545() {}
    virtual void g546() {} virtual void g547() {} virtual void g548() {} virtual void g549() {} virtual void g550() {} virtual void g551() {} virtual void g552() {} virtual void g553() {}
    virtual void g554() {} virtual void g555() {} virtual void g556() {} virtual void g557() {} virtual void g558() {} virtual void g559() {} virtual void g560() {} virtual void g561() {}
    virtual void g562() {} virtual void g563() {} virtual void g564() {} virtual void g565() {} virtual void g566() {} virtual void g567() {} virtual void g568() {} virtual void g569() {}
    virtual void g570() {} virtual void g571() {} virtual void g572() {} virtual void g573() {} virtual void g574() {} virtual void g575() {} virtual void g576() {} virtual void g577() {}
    virtual void g578() {} virtual void g579() {} virtual void g580() {} virtual void g581() {} virtual void g582() {} virtual void g583() {} virtual void g584() {} virtual void g585() {}
    virtual void g586() {} virtual void g587() {} virtual void g588() {} virtual void g589() {} virtual void g590() {} virtual void g591() {}
    virtual void EntitySetCombatEnabled(const CScriptThing& thing, bool enabled) const;   // slot 592 (+0x940)
    virtual void g593() {} virtual void g594() {} virtual void g595() {} virtual void g596() {} virtual void g597() {} virtual void g598() {} virtual void g599() {}
    virtual void EntityUnsetThingAsAllyOfThing(const CScriptThing& thing, const CScriptThing& ally) const;   // slot 600 (+0x960)
};
extern GameCalls* g_pGameScriptInterface;              // 0x0143E8F8
class CTimer { public:                                        // a script timer: the interface's timer index
    long TimerIndex;
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};
struct CIsThingAlive {                                 // vtable 0x012C32F0: a predicate holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern "C" int __cdecl rand();                         // 0x00BFEB16

namespace NScript {
struct CEntityScriptBase {                             // retail 0x14 bytes
    virtual void v00();
    GameCalls*   Game;                                 // +0x04
    CScriptThing Me;                                   // +0x08
    void SetPredicate(CIsThingAlive& predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
class CQ_SunnyvaleMasterData;
class CQ_TraderConflictEvilScript { public:
    unsigned char _pad[0x60];
    bool   QuestStartScreened;                         // +0x60
    long   NextTimeToSpawnGuards;                      // +0x64
    bool   MissionSucceeded;                           // +0x68
    bool   HeroAttackedBandit;                         // +0x69
    bool   PlayerEngaged;                              // +0x6a
    CTimer ScreamOutTimer;                             // +0x6c
    class CTC_Villager;
};
class CQ_TraderConflictEvilScript::CTC_Villager : public CEntityScriptBase {
public:
    CQ_TraderConflictEvilScript* ParentClass;          // +0x14
    CQ_SunnyvaleMasterData*      PMasterData;          // +0x18
    void Main();
};
}

void NScript::CQ_TraderConflictEvilScript::CTC_Villager::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    bool notLoaded = !Game->IsRegionLoaded("BarrowFields");
    if (notLoaded) {
        IsActiveThreadTerminating();
        return;
    }
    while (!ParentClass->QuestStartScreened) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsScared(Me, true);
    Game->ClearThingHasInformation(Me);
    Game->EntitySetCombatEnabled(Me, false);
    Game->EntityUnsetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntityUnsetThingAsAllyOfThing(Game->GetHero(), Me);
    while (!ParentClass->PlayerEngaged) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        bool hitByOther = Me.MsgIsHitBy("")
                       && !Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                       && (!Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                           || Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"));
        if (hitByOther) {
            if (IsActiveThreadTerminating()) return;
            Game->ModifyThingHealth(Me, 100.0f, false);
        }
        else {
            bool hitByHero = Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                          || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                              && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"));
            if (hitByHero) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->PlayerEngaged = true;
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (!IsActiveThreadTerminating()) {
        if (ParentClass->ScreamOutTimer.GetTime() == 0
            && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (Game->EntityGetSex(Me) == 2) {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation, "TEXT_QST_B12_VILLAGER_FEMALE_PANIC", false, Me, Game->GetHero());
            }
            else {
                if (IsActiveThreadTerminating()) return;
                Game->AddLineToConversation(conversation, "TEXT_QST_B12_VILLAGER_MALE_PANIC", false, Me, Game->GetHero());
            }
            ParentClass->ScreamOutTimer.SetTime(rand() % 15 + 15);
        }
        Game->NewScriptFrame();
    }
}


static int g_frame = 0, g_limit = 12;
static int g_predicates = 0, g_heal = 0, g_scared = 0, g_cleared = 0, g_combatOff = 0, g_unsetAlly = 0;
static int g_conversations = 0, g_people = 0, g_lines = 0, g_female = 0, g_timerSets = 0;
static long g_timer = 0, g_lastTimerValue = -1;
static long g_sex = 2;
static bool g_regionChecked = false, g_regionLoaded = true;
static NScript::CQ_TraderConflictEvilScript g_parent;
static GameCalls g_game;
GameCalls* g_pGameScriptInterface = &g_game;

CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; (void)length; }
CCharString::~CCharString() { }
static const char* Text(const CCharString& s) { return (const char*)s.PStringData; }
CScriptThing::CScriptThing(const CScriptThing& other) { PImp_Data = other.PImp_Data; PImp_Info = other.PImp_Info; }
CScriptThing::~CScriptThing() { }
static CScriptThing* g_me = 0;
static CScriptThing* g_hero = 0;
bool CScriptThing::MsgIsHitBy(const CCharString& name) const
{
    if (std::strcmp(Text(name), "") == 0) return g_frame == 4 || g_frame == 6;   // hit by anybody
    if (std::strcmp(Text(name), "SCRIPT_NAME_HERO") == 0) return g_frame == 6;    // hit by the hero
    return false;
}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const { (void)ability; (void)name; return false; }
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const { (void)name; return false; }
void CIsThingAlive::v00() { }
void NScript::CEntityScriptBase::v00() { }
void NScript::CEntityScriptBase::SetPredicate(CIsThingAlive& predicate) { if (predicate.m_thing.PImp_Data == Me.PImp_Data) ++g_predicates; }
bool NScript::CEntityScriptBase::IsActiveThreadTerminating() const { return g_frame >= g_limit; }
void GameCalls::NewScriptFrame() const { ++g_frame; if (g_frame == 2) g_parent.QuestStartScreened = true; }
bool GameCalls::IsRegionLoaded(const CCharString& region) const { g_regionChecked = std::strcmp(Text(region), "BarrowFields") == 0; return g_regionLoaded; }
CScriptThing& GameCalls::GetHero() { return *g_hero; }
void GameCalls::SetTimer(long timer, long time) { if (timer == 77) { g_timer = time; g_lastTimerValue = time; ++g_timerSets; } }
long GameCalls::GetTimer(long timer) const { return timer == 77 ? g_timer : -1; }
void GameCalls::ModifyThingHealth(const CScriptThing& thing, float amount, bool canKill) const { if (&thing == g_me && amount == 100.0f && !canKill) ++g_heal; }
void GameCalls::ClearThingHasInformation(const CScriptThing& thing) const { if (&thing == g_me) ++g_cleared; }
long GameCalls::AddNewConversation(const CScriptThing& thing, bool a, bool b) const { if (&thing == g_me && !a && !b) ++g_conversations; return 5; }
void GameCalls::AddPersonToConversation(long conversation, const CScriptThing& person) const { if (conversation == 5 && &person == g_hero) ++g_people; }
void GameCalls::AddLineToConversation(long conversation, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener) const
{
    if (conversation == 5 && !a && &speaker == g_me && &listener == g_hero) ++g_lines;
    if (std::strcmp(Text(text), "TEXT_QST_B12_VILLAGER_FEMALE_PANIC") == 0) ++g_female;
}
void GameCalls::EntitySetAsScared(const CScriptThing& thing, bool scared) const { if (&thing == g_me && scared) ++g_scared; }
ESex GameCalls::EntityGetSex(const CScriptThing& thing) const { (void)thing; return g_sex; }
void GameCalls::EntitySetCombatEnabled(const CScriptThing& thing, bool enabled) const { if (&thing == g_me && !enabled) ++g_combatOff; }
void GameCalls::EntityUnsetThingAsAllyOfThing(const CScriptThing& thing, const CScriptThing& ally) const
{
    if ((&thing == g_me && &ally == g_hero) || (&thing == g_hero && &ally == g_me)) ++g_unsetAlly;
}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{
    return &a == g_me && &b == g_hero && distance == 15.0f;
}

int main()
{
    // the model has no default constructors: build the script by copy-constructing zeroed storage onto itself
    typedef NScript::CQ_TraderConflictEvilScript::CTC_Villager CVillager;
    static double storage[(sizeof(CVillager) + sizeof(double) - 1) / sizeof(double)];
    CVillager& villager = *new (storage) CVillager(*reinterpret_cast<CVillager*>(storage));
    static CScriptThing* hero = (CScriptThing*)std::malloc(sizeof(CScriptThing));
    villager.Game = &g_game;
    villager.ParentClass = &g_parent;
    villager.Me.PImp_Data = &villager;
    g_parent.ScreamOutTimer.TimerIndex = 77;
    g_me = &villager.Me;
    g_hero = hero;
    villager.Main();
    bool ok = g_regionChecked && g_predicates == 1 && g_scared == 1 && g_cleared == 1 && g_combatOff == 1 && g_unsetAlly == 2;
    ok = ok && g_heal == 1 && g_parent.PlayerEngaged;                       // healed once (frame 4), engaged by the hero (frame 6)
    ok = ok && g_conversations == 1 && g_people == 1 && g_lines == 1 && g_female == 1;   // one panic line, timer then running
    ok = ok && g_timerSets == 1 && g_lastTimerValue >= 15 && g_lastTimerValue < 30;
    ok = ok && g_frame == g_limit;
    // a region that is not loaded ends the script before the quest wait
    g_frame = 0; g_scared = 0; g_regionLoaded = false; g_parent.QuestStartScreened = false;
    villager.Main();
    ok = ok && g_frame == 1 && g_scared == 0 && !g_parent.QuestStartScreened;
    if (!ok) { std::printf("00df9180_TEST FAIL\n"); return 1; }
    std::printf("00df9180_TEST PASS\n");
    return 0;
}