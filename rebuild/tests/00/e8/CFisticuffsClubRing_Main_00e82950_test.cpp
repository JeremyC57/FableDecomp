// 00e82950 behaviour test: CFisticuffsClubRing::Main (match.cpp body verbatim, pragmas dropped; for(;;) loop with the
// terminate check at the top), host stand-ins for the game interface. C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <string.h>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const std::string& s) { g_log += s; g_log += ";"; }
static std::string num(long v) { char b[24]; sprintf(b, "%ld", v); return b; }

struct CCharString {
    std::string s;
    CCharString(const char* text, long length = -1) : s(text) { (void)length; }
};
struct CScriptThing {
    std::string Name;
    CScriptThing() {}
    CScriptThing(const char* n) : Name(n) {}
};
struct CTimer;
struct CGameScriptInterface;
extern CGameScriptInterface* g_pGameScriptInterface;
struct CTimer {
    long TimerIndex;
    CTimer();
    ~CTimer();
    void SetTime(long time);
    long GetTime() const;
};
struct CGameScriptInterface {
    CScriptThing Hero;
    long Timers[8];
    long NextTimer;
    long Frames;
    long FrameLimit;
    long Deregistered;
    long NextInfo;
    CGameScriptInterface() : Hero("hero"), NextTimer(0), Frames(0), FrameLimit(1), Deregistered(0), NextInfo(100) { memset(Timers, 0, sizeof(Timers)); }
    void NewScriptFrame() const { CGameScriptInterface* g = const_cast<CGameScriptInterface*>(this); ++g->Frames; logs("frame"); }
    CScriptThing& GetHero() { return Hero; }
    CScriptThing GetThingWithScriptName(const CCharString& a0) const { logs("get-" + a0.s); return CScriptThing(a0.s.c_str()); }
    CScriptThing GetNearestWithScriptName(const CScriptThing& a0, const CCharString& a1) const { logs("nearest-" + a0.Name + "-" + a1.s); return CScriptThing(a1.s.c_str()); }
    long RegisterTimer() { return NextTimer++; }
    void DeregisterTimer(long a0) { ++Deregistered; (void)a0; }
    void SetTimer(long a0, long a1) { Timers[a0] = a1; logs("settimer-" + num(a0) + "-" + num(a1)); }
    long GetTimer(long a0) const { return Timers[a0]; }
    long AddQuestInfoTimer(const CTimer& a0, const CCharString& a1, float a2) const
    { logs("info-" + num(a0.TimerIndex) + "-" + a1.s + (a2 == 1.0f ? "-1" : "-?")); return const_cast<CGameScriptInterface*>(this)->NextInfo++; }
    void RemoveQuestInfoElement(long a0) const { logs("remove-" + num(a0)); }
    long AddNewConversation(const CScriptThing& a0, bool a1, bool a2) const { logs("conv-" + a0.Name + (a1 || a2 ? "-t" : "-f")); return 7; }
    void AddPersonToConversation(long a0, const CScriptThing& a1) const { logs("person-" + num(a0) + "-" + a1.Name); }
    void AddLineToConversation(long a0, const CCharString& a1, bool a2, const CScriptThing& a3, const CScriptThing& a4) const
    { logs("line-" + num(a0) + "-" + a1.s + (a2 ? "-t-" : "-f-") + a3.Name + "-" + a4.Name); }
};
CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
void CTimer::SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
CGameScriptInterface* g_pGameScriptInterface = 0;
struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
};
struct CScriptDef {
    float FC_OutsideRingRange;
    float FC_RingTimeOut;
    float FC_RingRadius;
    long  FC_InsideRingMessageDelay;
};
static CScriptDef g_def = { 20.0f, 30.0f, 5.0f, 9 };
CScriptDef* g_pScriptDef = &g_def;
static bool g_under = true;     // hero distance from Me under the radius
static bool g_over = false;     // over the given distance
static float g_lastUnder = 0.0f;
static float g_lastOver = 0.0f;
bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance) { (void)a; (void)b; g_lastUnder = distance; return g_under; }
bool IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance) { (void)a; (void)b; g_lastOver = distance; return g_over; }
struct CV_FisticuffsClubScript {
    bool  FighterOneInRing, FighterTwoInRing, HeroInsideRing, HeroPaidEntryFee, HeroInFisticuffsClub, TylerInFight, TimeOut, RoundStarted;
    bool  EverybodyAttack, LostWhileOutside, FisticuffsActive;
    float CheerNumber, BooNumber;
    int   Inits;
    void InitialiseVariables() { ++Inits; }
    CCharString GetQuestText(CCharString suffix) { return CCharString(("Q_" + suffix.s).c_str()); }
};
inline void ClampMeter(float& meter)
{
    if (meter < 0.0f)
        meter = 0.0f;
    else if (meter > 12.0f)
        meter = 12.0f;
}
struct CEntityScriptBase {
    CGameScriptInterface* Game;
    CScriptThing          Me;
    int Registered;
    long TerminateAtCheck;        // IsActiveThreadTerminating returns true from this call number on (-1: never by count)
    long Checks;
    void RegisterFilter(CIsThingAlive& filter) { ++Registered; logs("filter-" + filter.m_thing.Name); }
    bool IsActiveThreadTerminating() const
    {
        CEntityScriptBase* s = const_cast<CEntityScriptBase*>(this);
        ++s->Checks;
        if (TerminateAtCheck >= 0 && Checks >= TerminateAtCheck) return true;
        return Game->Frames > Game->FrameLimit;
    }
};
struct CFisticuffsClubRing : CEntityScriptBase {
    CV_FisticuffsClubScript* ParentClass;
    int Setups;
    void Main();
    void SetUpRing() { ++Setups; }
};

void CFisticuffsClubRing::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        {
            return;
        }
    SetUpRing();
    ParentClass->InitialiseVariables();
    CTimer warningTimer;
    CTimer interruptTimer;
    float ringRadius = g_pScriptDef->FC_RingRadius;
    long warningInfo = 0;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), ringRadius)) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->HeroInsideRing = true;
            if (warningTimer.GetTime() != 0) {
                if (IsActiveThreadTerminating())
                    return;
                Game->RemoveQuestInfoElement(warningInfo);
                warningTimer.SetTime(0);
            }
            if (ParentClass->FisticuffsActive && !ParentClass->HeroInFisticuffsClub && !ParentClass->HeroPaidEntryFee
                && !ParentClass->EverybodyAttack && interruptTimer.GetTime() < 1) {
                if (IsActiveThreadTerminating())
                    {
                        return;
                    }
                CScriptThing member = Game->GetNearestWithScriptName(Me, "FisticuffsMember");
                long conversation = Game->AddNewConversation(member, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, ParentClass->GetQuestText("INTERRUPT_FIGHT_10"), false, member, Game->GetHero());
                interruptTimer.SetTime(g_pScriptDef->FC_InsideRingMessageDelay);
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->HeroInFisticuffsClub && ParentClass->HeroInsideRing) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptThing tyler = Game->GetThingWithScriptName("Tyler");
                long conversation = Game->AddNewConversation(tyler, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, ParentClass->GetQuestText("STEP_OUTSIDE_RING_10"), false, tyler, Game->GetHero());
                warningTimer.SetTime((long)g_pScriptDef->FC_RingTimeOut);
                warningInfo = Game->AddQuestInfoTimer(warningTimer, "HUD_CLOCK_ICON", 1.0f);
            }
            ParentClass->HeroInsideRing = false;
        }
        if (ParentClass->HeroInFisticuffsClub) {
            if (IsActiveThreadTerminating())
                return;
            if ((ParentClass->RoundStarted || ParentClass->TylerInFight)
                && IsDistanceBetweenThingsOver(Me, Game->GetHero(), ringRadius)) {
                if (IsActiveThreadTerminating())
                    {
                        return;
                    }
                ParentClass->BooNumber += 1.0f / 15.0f;
                if (warningTimer.GetTime() == 0
                    || IsDistanceBetweenThingsOver(Me, Game->GetHero(), g_pScriptDef->FC_OutsideRingRange)) {
                    if (IsActiveThreadTerminating())
                        {
                            return;
                        }
                    ParentClass->LostWhileOutside = true;
                    ParentClass->TimeOut = true;
                    Game->RemoveQuestInfoElement(warningInfo);
                }
            } else if (ParentClass->TylerInFight) {
                if (IsActiveThreadTerminating())
                    {
                        return;
                    }
                ParentClass->CheerNumber += 1.0f / 15.0f;
            } else {
                if (IsActiveThreadTerminating())
                    {
                        return;
                    }
                ParentClass->CheerNumber += 1.0f / 30.0f;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->FighterTwoInRing) {
                if (ParentClass->FighterOneInRing) {
                    if (IsActiveThreadTerminating())
                        {
                            return;
                        }
                    ParentClass->CheerNumber += 1.0f / 75.0f;
                }
            } else if (ParentClass->FighterOneInRing) {
                if (IsActiveThreadTerminating())
                    {
                        return;
                    }
                ParentClass->CheerNumber += 1.0f / 150.0f;
            }
        }
        ClampMeter(ParentClass->CheerNumber);
        ClampMeter(ParentClass->BooNumber);
        Game->NewScriptFrame();
        continue;
    }
}

static void Setup(CGameScriptInterface& g, CV_FisticuffsClubScript& p, CFisticuffsClubRing& r, long frames)
{
    memset(&p, 0, sizeof(p));
    g_log = "";
    g.FrameLimit = frames;
    r.Game = &g; r.Me = CScriptThing("ring"); r.Registered = 0; r.TerminateAtCheck = -1; r.Checks = 0;
    r.ParentClass = &p; r.Setups = 0;
}

int main()
{
    // 1: terminating straight after the first frame: filter registered, no setup, no timers
    {
        CGameScriptInterface g; g_pGameScriptInterface = &g;
        CV_FisticuffsClubScript p; CFisticuffsClubRing r; Setup(g, p, r, 0);
        r.Main();
        CHECK(r.Registered == 1 && r.Setups == 0 && p.Inits == 0);
        CHECK(g.NextTimer == 0 && g.Deregistered == 0);
        CHECK(g_log == "filter-ring;frame;");
    }
    // 2: hero inside the ring with a running warning timer and an idle club fight: warning cancelled, member interrupts
    {
        CGameScriptInterface g; g_pGameScriptInterface = &g;
        CV_FisticuffsClubScript p; CFisticuffsClubRing r; Setup(g, p, r, 2);
        p.FisticuffsActive = true;
        g.Timers[0] = 4;                     // warningTimer (index 0) running
        g_under = true; g_over = false;
        r.Main();
        CHECK(r.Setups == 1 && p.Inits == 1);
        CHECK(p.HeroInsideRing);
        CHECK(g.Timers[0] == 0 && g.Timers[1] == 9);
        CHECK(g_lastUnder == 5.0f);
        CHECK(g.Deregistered == 2);
        CHECK(g_log == "filter-ring;frame;remove-0;settimer-0-0;nearest-ring-FisticuffsMember;conv-FisticuffsMember-f;person-7-hero;"
                       "line-7-Q_INTERRUPT_FIGHT_10-f-FisticuffsMember-hero;settimer-1-9;frame;frame;");
    }
    // 3: hero steps out while in the club during a round: Tyler warns, warning clock added, boo rises, timeout (clock at 0 -> 30)
    {
        CGameScriptInterface g; g_pGameScriptInterface = &g;
        CV_FisticuffsClubScript p; CFisticuffsClubRing r; Setup(g, p, r, 1);
        p.HeroInFisticuffsClub = true; p.HeroInsideRing = true; p.RoundStarted = true;
        g_under = false; g_over = true;
        r.Main();
        CHECK(!p.HeroInsideRing);
        CHECK(g.Timers[0] == 30);
        CHECK(p.BooNumber == 0.0f + 1.0f / 15.0f);
        CHECK(p.LostWhileOutside);           // the warning clock is running, but being over FC_OutsideRingRange also loses
        CHECK(g_lastOver == 20.0f);
        CHECK(p.TimeOut && g_log.find("remove-100;") != std::string::npos);
        CHECK(g_log.find("get-Tyler;conv-Tyler-f;person-7-hero;line-7-Q_STEP_OUTSIDE_RING_10-f-Tyler-hero;settimer-0-30;info-0-HUD_CLOCK_ICON-1;") != std::string::npos);
        CHECK(g.Deregistered == 2);
    }
    // 4: in the club, Tyler fighting, hero inside: cheer rises by 1/15 per frame and clamps at 12
    {
        CGameScriptInterface g; g_pGameScriptInterface = &g;
        CV_FisticuffsClubScript p; CFisticuffsClubRing r; Setup(g, p, r, 3);
        p.HeroInFisticuffsClub = true; p.TylerInFight = true; p.CheerNumber = 11.9375f; p.BooNumber = -2.0f;
        g_under = true; g_over = false;
        r.Main();
        CHECK(p.CheerNumber == 12.0f);
        CHECK(p.BooNumber == 0.0f);
    }
    // 5: not in the club: both fighters in ring -> 1/75; only fighter one -> 1/150; fighter two alone -> nothing
    {
        CGameScriptInterface g; g_pGameScriptInterface = &g;
        CV_FisticuffsClubScript p; CFisticuffsClubRing r; Setup(g, p, r, 1);
        p.FighterOneInRing = true; p.FighterTwoInRing = true;
        r.Main();
        CHECK(p.CheerNumber == 0.0f + 1.0f / 75.0f);
        Setup(g, p, r, 1); g.Frames = 0;
        p.FighterOneInRing = true;
        r.Main();
        CHECK(p.CheerNumber == 0.0f + 1.0f / 150.0f);
        Setup(g, p, r, 1); g.Frames = 0;
        p.FighterTwoInRing = true;
        r.Main();
        CHECK(p.CheerNumber == 0.0f);
    }
    // 6: in the club, no round: cheer 1/30; terminate on the in-club check (4th check) returns before the meter changes
    {
        CGameScriptInterface g; g_pGameScriptInterface = &g;
        CV_FisticuffsClubScript p; CFisticuffsClubRing r; Setup(g, p, r, 5);
        p.HeroInFisticuffsClub = true;
        g_under = true;
        r.TerminateAtCheck = 4;              // 1 = after the first frame, 2 = loop top, 3 = inside-ring arm, 4 = in-club test
        r.Main();
        CHECK(p.CheerNumber == 0.0f);
        CHECK(p.HeroInsideRing);
        CHECK(g.Deregistered == 2);
        Setup(g, p, r, 1); g.Frames = 0;
        p.HeroInFisticuffsClub = true;
        r.Main();
        CHECK(p.CheerNumber == 0.0f + 1.0f / 30.0f);
    }
    if (!g_fail) printf("00e82950_TEST PASS\n");
    return g_fail;
}