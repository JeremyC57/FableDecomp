// 007e5230 behaviour test: CTCBetting::ProcessInput (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <stdio.h>
#include <string.h>

static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }

class CCharString { public:
    const char* text;
    CCharString(const char* t, long length = -1) : text(t) { (void)length; }
};
class C2DVector { public: float x, y; };
static long g_eventType;
class CInputEvent { public: long GetEventType() const { return g_eventType; } };
static int g_handled;
class CProcessedInput { public: void SetHandled() { ++g_handled; } };
static bool g_vecOk, g_pressed, g_act16, g_act17;
static C2DVector g_stick;
struct CInputProcessGameBase {
    bool IsEventButtonPressed(const CInputEvent&) { return g_pressed; }
    bool IsEventGameAction(long action, const CInputEvent&) { return action == 0x16 ? g_act16 : (action == 0x17 ? g_act17 : false); }
    bool GetEventGameActionVector(long action, const CInputEvent&, C2DVector* v) { if (action != 0x72) return false; *v = g_stick; return g_vecOk; }
};
static int g_clears;
class CSubtitleRenderer { public: void Clear(bool) { ++g_clears; } };
class CPlayerGui { public: CSubtitleRenderer SubtitleRenderer; };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
class CTCHeroStats { public: long Gold; };
struct STCEntry { long type; CTCHeroStats* component; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind)
    {
        STCEntry* first = m_pFirst;
        long count = m_pLast - m_pFirst;
        while (count > 0) {
            long half = count >> 1;
            STCEntry* mid = first + half;
            if ((unsigned long)mid->type < kind) {
                first = mid + 1;
                count -= half + 1;
            } else
                count = half;
        }
        return first;
    }
    __forceinline CTCHeroStats* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->type) ? m_pLast : it)->component;
    }
};
class CThing { public:
    unsigned long m_tcMask[9];
    CTCMap        m_tcs;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCHeroStats* GetTC(long kind) { return m_tcs.Find(kind); }
};
static CThing* g_heroThing;
class CPlayer { public: CThing* GetHeroCreature() { return g_heroThing; } };
static CPlayer g_player;
class CPlayerManager { public: CPlayer* GetMainPlayer() const { return &g_player; } };
static long g_frame;
class CMainGameComponent { public: CPlayerManager* PlayerManager; long GetFrame() { return g_frame; } };
static CPlayerManager g_pm;
static CMainGameComponent g_mgc = { &g_pm };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
static bool g_BettingActive, g_BetPlaced;
static long g_CurrentBet;
static long g_BetRepeatDelay = 10, g_BetRepeatSlow = 5, g_BetRepeatFast = 2, g_BetMultiplierRate = 20;

static int g_destroys;
class CBettingDef { public:
    long RefCount;
    long MinBet, MaxBet, BetStep;
    void Destroy() { ++g_destroys; }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
static CBettingDef* g_subDef;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CBettingDef>* out) { if (!g_subDef) return false; ++g_subDef->RefCount; out->Object = g_subDef; return true; }
};
struct CParentThing {
    CDefClassBase* m_pDef;
    CDefClassBase* GetDef() const { return m_pDef; }
};
class CTCBetting { public:
    CParentThing* ParentThing;
    long          FrameBetWasLastChanged;
    long          FrameBetChangeStarted;
    long          Mult;
    long          FrameToStart;
    long          PreviousDonation;
    void PlaySound(const CCharString& name) { Log(name.text); }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessGameBase* input);
};

void CTCBetting::ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessGameBase* input)
{
    CTCHeroStats* stats = 0;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroCreature();
    if (hero->HasTC(4))
        stats = hero->GetTC(4);
    CDefPointer<CBettingDef> def;
    if (g_BettingActive && ParentThing->GetDef()->GetSubDef(&def)) {
    C2DVector stick;
    if (input->GetEventGameActionVector(0x72, event, &stick) && FrameToStart <= g_pMainGameComponent->GetFrame()
        && event.GetEventType() == 2) {
        long held = g_pMainGameComponent->GetFrame() - FrameBetChangeStarted;
        bool change = false;
        if (FrameBetWasLastChanged == g_pMainGameComponent->GetFrame() - 1) {
            held -= g_BetRepeatDelay;
            if ((held >= 0 && held % g_BetRepeatSlow == 0) || (held >= g_BetRepeatDelay && held % g_BetRepeatFast == 0))
                change = true;
            if (held != 0 && held % g_BetMultiplierRate == 0)
                Mult *= 2;
        } else {
            FrameBetChangeStarted = g_pMainGameComponent->GetFrame();
            change = true;
            Mult = 1;
        }
        if (stick.y > 0.2f) {
            FrameBetWasLastChanged = g_pMainGameComponent->GetFrame();
            if ((g_CurrentBet < def->MaxBet || def->MaxBet == 0) && change && g_CurrentBet < stats->Gold) {
                PlaySound("TAVERN_GAME_BET_INCREMENT");
                g_CurrentBet += def->BetStep * Mult;
            }
            processed.SetHandled();
        } else if (stick.y < -0.2f) {
            FrameBetWasLastChanged = g_pMainGameComponent->GetFrame();
            if (g_CurrentBet != def->MinBet && change) {
                PlaySound("TAVERN_GAME_BET_DECREMENT");
                g_CurrentBet -= def->BetStep * Mult;
                if (g_CurrentBet < def->MinBet)
                    g_CurrentBet = def->MinBet;
            }
            processed.SetHandled();
        }
        if (stick.x < -0.2f)
            g_CurrentBet = 1;
        else if (stick.x > 0.2f)
            g_CurrentBet = PreviousDonation;
        if (g_CurrentBet > stats->Gold)
            g_CurrentBet = stats->Gold;
    }
    if (input->IsEventButtonPressed(event)) {
        if (input->IsEventGameAction(0x16, event)) {
            g_pPlayerGui->SubtitleRenderer.Clear(false);
            processed.SetHandled();
            g_BettingActive = false;
            g_BetPlaced = true;
            PreviousDonation = g_CurrentBet;
        } else if (input->IsEventGameAction(0x17, event)) {
            g_pPlayerGui->SubtitleRenderer.Clear(false);
            processed.SetHandled();
            g_BettingActive = false;
            g_BetPlaced = false;
        }
    }
    }
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s (bet %ld mult %ld log %s)\n", name, g_CurrentBet, 0L, g_log); g_fail = 1; } }

int main()
{
    CTCHeroStats stats; stats.Gold = 100;
    STCEntry entries[3] = { { 2, 0 }, { 4, &stats }, { 9, 0 } };
    CThing hero; memset(hero.m_tcMask, 0, sizeof(hero.m_tcMask)); hero.m_tcMask[0] = 1UL << 4;
    hero.m_tcs.m_pFirst = entries; hero.m_tcs.m_pLast = entries + 3; hero.m_tcs.m_pEnd = entries + 3;
    g_heroThing = &hero;
    CBettingDef def; def.RefCount = 1; def.MinBet = 5; def.MaxBet = 50; def.BetStep = 3;
    CDefClassBase defBase; CParentThing parent; parent.m_pDef = &defBase;
    CTCBetting b; b.ParentThing = &parent; b.FrameBetWasLastChanged = 0; b.FrameBetChangeStarted = 0; b.Mult = 1;
    b.FrameToStart = 0; b.PreviousDonation = 42;
    CInputEvent ev; CProcessedInput processed; CInputProcessGameBase input;

#define RESET() (g_log[0] = 0, g_handled = 0, g_clears = 0, g_destroys = 0, g_eventType = 2, g_vecOk = true, \
                 g_pressed = false, g_act16 = false, g_act17 = false, g_subDef = &def, g_BettingActive = true)
    // not betting: nothing happens, def never fetched
    RESET(); g_BettingActive = false; g_CurrentBet = 10; b.ProcessInput(ev, processed, &input);
    Check("inactive", g_CurrentBet == 10 && g_handled == 0 && def.RefCount == 1);
    // no sub def
    RESET(); g_subDef = 0; b.ProcessInput(ev, processed, &input);
    Check("no def", g_CurrentBet == 10 && g_handled == 0);
    // first push up (new press): bet += step*1, mult reset
    RESET(); g_frame = 100; g_stick.x = 0; g_stick.y = 1.0f; b.Mult = 7; g_CurrentBet = 10;
    b.ProcessInput(ev, processed, &input);
    Check("up new", g_CurrentBet == 13 && b.Mult == 1 && b.FrameBetChangeStarted == 100 && b.FrameBetWasLastChanged == 100
        && g_handled == 1 && strcmp(g_log, "TAVERN_GAME_BET_INCREMENT;") == 0 && def.RefCount == 1 && g_destroys == 0);
    // held: frame 101, last changed 100 -> held = 1 - 10 < 0, no change
    RESET(); g_frame = 101; b.ProcessInput(ev, processed, &input);
    Check("held early", g_CurrentBet == 13 && g_handled == 1 && g_log[0] == 0);
    // held long: frame 120 (started 100): held 20-10 = 10, %5 == 0 -> change; 10 % 20 != 0 -> mult stays
    RESET(); g_frame = 120; b.FrameBetWasLastChanged = 119; b.ProcessInput(ev, processed, &input);
    Check("held repeat", g_CurrentBet == 16 && b.Mult == 1);
    // multiplier: frame 130: held 30-10 = 20 -> change, 20 % 20 == 0 -> mult doubles before the step
    RESET(); g_frame = 130; b.FrameBetWasLastChanged = 129; b.ProcessInput(ev, processed, &input);
    Check("mult", b.Mult == 2 && g_CurrentBet == 22);
    // cap at gold
    RESET(); g_frame = 200; b.FrameBetWasLastChanged = 0; stats.Gold = 22; b.ProcessInput(ev, processed, &input);
    Check("gold blocks", g_CurrentBet == 22 && g_log[0] == 0);
    // clamp to gold
    RESET(); g_frame = 250; b.FrameBetWasLastChanged = 0; stats.Gold = 24; b.ProcessInput(ev, processed, &input);
    Check("gold clamp", g_CurrentBet == 24);
    stats.Gold = 100;
    // down to min clamp
    RESET(); g_frame = 300; b.FrameBetWasLastChanged = 0; g_stick.y = -1.0f; g_CurrentBet = 6; b.ProcessInput(ev, processed, &input);
    Check("down clamp", g_CurrentBet == 5 && strcmp(g_log, "TAVERN_GAME_BET_DECREMENT;") == 0 && g_handled == 1);
    // at min: no change
    RESET(); g_frame = 400; b.FrameBetWasLastChanged = 0; b.ProcessInput(ev, processed, &input);
    Check("at min", g_CurrentBet == 5 && g_log[0] == 0 && g_handled == 1);
    // stick x left / right
    RESET(); g_frame = 500; g_stick.y = 0; g_stick.x = -1.0f; g_CurrentBet = 30; b.ProcessInput(ev, processed, &input);
    Check("left", g_CurrentBet == 1 && g_handled == 0);
    RESET(); g_stick.x = 1.0f; b.ProcessInput(ev, processed, &input);
    Check("right", g_CurrentBet == 42);
    // too early: FrameToStart in the future -> stick ignored
    RESET(); b.FrameToStart = 1000; g_CurrentBet = 9; b.ProcessInput(ev, processed, &input);
    Check("too early", g_CurrentBet == 9); b.FrameToStart = 0;
    // wrong event type
    RESET(); g_eventType = 1; b.ProcessInput(ev, processed, &input);
    Check("event type", g_CurrentBet == 9);
    // place bet (action 0x16)
    RESET(); g_vecOk = false; g_pressed = true; g_act16 = true; g_CurrentBet = 17; b.ProcessInput(ev, processed, &input);
    Check("place", !g_BettingActive && g_BetPlaced && b.PreviousDonation == 17 && g_clears == 1 && g_handled == 1);
    // cancel (action 0x17)
    RESET(); g_vecOk = false; g_pressed = true; g_act17 = true; g_BetPlaced = true; b.ProcessInput(ev, processed, &input);
    Check("cancel", !g_BettingActive && !g_BetPlaced && g_clears == 1);
    // def released to zero -> destroyed
    RESET(); def.RefCount = 0; g_vecOk = false; b.ProcessInput(ev, processed, &input);
    Check("release", g_destroys == 1 && def.RefCount == 0);
    if (g_fail) return 1;
    printf("007e5230_TEST PASS\n");
    return 0;
}