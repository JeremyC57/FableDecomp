// Behaviour test for 00691130: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CInputProcessQuestCompletionUI::ProcessInput(const CInputEvent&, CProcessedInput&)
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CInputProcessQuestCompletionUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- route input to the front end while the hero's quest-completion screen (component 0xFC) is up: pressed / held / released confirm, back, page-left and page-right actions become front-end events, action 0x48 closes the screen, mouse moves update the cursor and the wheel scrolls the menu
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) @ 0x00691130, 755 bytes, `ret 8`.
// Shape: the action-0x47 arm copies the event's mouse position (a C2DVector, floats, inside UInputEventData at +0x0c)
//   into the front-end manager's MousePosition (+0xB0) through an 8-byte frame temporary that lives across the
//   GetInstance call (retail `sub esp,8`, stores to [esp+0xc]/[esp+0x10] before the call, reloads after). VC7.1
//   scalar-replaces a plain `T position = event.Position;` local into edi/esi (push ecx frame) whatever the member
//   type or constructors are; the memory home comes from returning the NON-POD C2DVector BY VALUE from an inline
//   getter (`GetMousePosition(event)` -> hidden return buffer = the local) and handing the local BY REFERENCE to an
//   inline manager setter (`SetMousePosition(CMousePosition&)`), the same form as the landed sibling
//   CInputProcessPhotojournalCapture::ProcessInput 0x00691AE0. Float members are required: with longs the same form
//   stays in registers.
#ifdef _MSC_VER
#include "engine/CInputProcessGameBase.h"
#else   // 64-bit g++ cannot satisfy the header's pointer-size asserts: same layout by hand (only MyPlayer is used)
struct CInputProcessGameBase { void* __vftable; unsigned char _pad_0x04[0x10]; struct CWorld* World; struct CPlayer* MyPlayer; struct CGameDefinitionManager* DefinitionManager; struct CMainGameComponent* Component; struct CGamePlayerInterface* PGamePlayerInterface; };
#endif
struct CMousePosition {
    float X, Y;
    CMousePosition() {}
    CMousePosition(const CMousePosition& other) : X(other.X), Y(other.Y) {}
};
class CInputEvent {
public:
    unsigned char  _pad00[0x0c];
    CMousePosition Position;                       // +0x0C
    float GetWheelDelta() const;                   // 0x00A03BB0
};
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
float __cdecl ceil(float value);                   // 0x00689370
namespace NUISystem {
class CFrontEndManager {
public:
    virtual void SendEvent(long event);            // slot 0
    unsigned char  _pad04[0xac];
    CMousePosition MousePosition;                  // +0xB0
    void SetMousePosition(CMousePosition& position) { MousePosition = position; }
    void ScrollMenu(long amount);                  // 0x0041DA2D
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
struct CTCQuestCompletionUI {
    unsigned char _pad00[0x0c];
    bool          Active;                          // +0x0C
    unsigned char _pad0d[0x0f];
    bool          Closing;                         // +0x1C
    unsigned char _pad1d[3];
    long          State;                           // +0x20
    void Deactivate();                             // 0x005E3DF0
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long                  m_kind;
    CTCQuestCompletionUI* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCQuestCompletionUI* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20: which component kinds the thing holds
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCQuestCompletionUI* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CPlayer {
public:
    CThing* GetHeroCreature();                     // 0x00487DC0
};
class CInputProcessQuestCompletionUI : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    CMousePosition GetMousePosition(const CInputEvent& event) const { return event.Position; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessQuestCompletionUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetHeroCreature();
    if (!hero->HasTC(0xfc))
        return;
    CTCQuestCompletionUI* ui = hero->GetTC(0xfc);
    if (!ui->Active || ui->Closing)
        return;
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x19, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1a);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x4c, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(5);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x58, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(6);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x59, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(7);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x48, event)) {
            if (ui->State == 0 || ui->State == 1)
                ui->Deactivate();
            processed.SetHandled();
        }
        return;
    }
    if (IsEventButtonHeld(event)) {
        if (IsEventGameAction(0x19, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1c);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x4c, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1d);
            processed.SetHandled();
        }
        return;
    }
    if (IsEventButtonReleased(event)) {
        if (IsEventGameAction(0x19, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1f);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x4c, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x20);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x58, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(6);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x59, event)) {
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(7);
            processed.SetHandled();
        }
        return;
    }
    if (IsEventGameAction(0x47, event)) {
        CMousePosition position = GetMousePosition(event);
        NUISystem::CFrontEndManager::GetInstance()->SetMousePosition(position);
        NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x19);
        processed.SetHandled();
        return;
    }
    if (IsEventGameAction(0x65, event)) {
        NUISystem::CFrontEndManager::GetInstance()->ScrollMenu((long)ceil(event.GetWheelDelta() * 0.008333334f));
        processed.SetHandled();
    }
}

// ---- stubs ----
static int g_pressed, g_held, g_released;          // which IsEventButton* answers true
static long g_action;                              // the one game action the event carries
static float g_wheel;
static int g_sends; static long g_lastEvent; static long g_events[8];
static int g_handled, g_deactivated, g_scrolls; static long g_scrollAmount;
static NUISystem::CFrontEndManager* g_mgr;
static CThing* g_hero;
static float g_lastWheelArg;
CThing* CPlayer::GetHeroCreature() { return g_hero; }
STCEntry* CTCMap::LowerBound(const unsigned long& kind)
{
    STCEntry* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it;
    return it;
}
bool CInputProcessQuestCompletionUI::IsEventButtonPressed(const CInputEvent&) { return g_pressed != 0; }
bool CInputProcessQuestCompletionUI::IsEventButtonReleased(const CInputEvent&) { return g_released != 0; }
bool CInputProcessQuestCompletionUI::IsEventButtonHeld(const CInputEvent&) { return g_held != 0; }
bool CInputProcessQuestCompletionUI::IsEventGameAction(long action, const CInputEvent&) { return action == g_action; }
NUISystem::CFrontEndManager* __fastcall NUISystem::CFrontEndManager::GetInstance() { return g_mgr; }
void NUISystem::CFrontEndManager::SendEvent(long event) { if (g_sends < 8) g_events[g_sends] = event; ++g_sends; g_lastEvent = event; }
void NUISystem::CFrontEndManager::ScrollMenu(long amount) { ++g_scrolls; g_scrollAmount = amount; }
void CTCQuestCompletionUI::Deactivate() { ++g_deactivated; }
float CInputEvent::GetWheelDelta() const { return g_wheel; }
void CProcessedInput::SetHandled() { ++g_handled; }
float __cdecl ceil(float value) { g_lastWheelArg = value; long t = (long)value; if ((float)t < value) ++t; return (float)t; }

static void Reset() { g_pressed = g_held = g_released = 0; g_action = -1; g_wheel = 0.0f; g_sends = 0; g_lastEvent = -1; g_handled = g_deactivated = g_scrolls = 0; g_scrollAmount = 0; }
static void Run(CInputProcessQuestCompletionUI& ip, CInputEvent& ev, CProcessedInput& pi, int pressed, int held, int released, long action)
{
    Reset(); g_pressed = pressed; g_held = held; g_released = released; g_action = action;
    ip.ProcessInput(ev, pi);
}
int main()
{
    CInputProcessQuestCompletionUI ip; memset(&ip, 0, sizeof(ip));
    CPlayer player; ip.MyPlayer = &player;
    CThing hero; memset(&hero, 0, sizeof(hero)); g_hero = &hero;
    CTCQuestCompletionUI ui; memset(&ui, 0, sizeof(ui)); ui.Active = true; ui.Closing = false; ui.State = 0;
    CTCQuestCompletionUI other; memset(&other, 0, sizeof(other));
    STCEntry entries[3]; entries[0].m_kind = 0x10; entries[0].m_pTC = &other; entries[1].m_kind = 0xfc; entries[1].m_pTC = &ui; entries[2].m_kind = 0x120; entries[2].m_pTC = &other;
    hero.m_tcs.m_pFirst = entries; hero.m_tcs.m_pLast = entries + 3; hero.m_tcs.m_pEnd = entries + 3;
    hero.m_tcMask[0xfc >> 5] = 1UL << (0xfc & 31);
    NUISystem::CFrontEndManager mgr; memset(&mgr._pad04, 0, sizeof(mgr._pad04)); mgr.MousePosition.X = -1.0f; mgr.MousePosition.Y = -1.0f; g_mgr = &mgr;
    CInputEvent ev; memset(&ev, 0, sizeof(ev)); ev.Position.X = 123.5f; ev.Position.Y = -42.25f;
    CProcessedInput pi;

    // pressed arms
    Run(ip, ev, pi, 1, 0, 0, 0x19); CHECK(g_sends == 1 && g_lastEvent == 0x1a && g_handled == 1);
    Run(ip, ev, pi, 1, 0, 0, 0x4c); CHECK(g_sends == 1 && g_lastEvent == 5 && g_handled == 1);
    Run(ip, ev, pi, 1, 0, 0, 0x58); CHECK(g_sends == 1 && g_lastEvent == 6 && g_handled == 1);
    Run(ip, ev, pi, 1, 0, 0, 0x59); CHECK(g_sends == 1 && g_lastEvent == 7 && g_handled == 1);
    ui.State = 0; Run(ip, ev, pi, 1, 0, 0, 0x48); CHECK(g_sends == 0 && g_deactivated == 1 && g_handled == 1);
    ui.State = 1; Run(ip, ev, pi, 1, 0, 0, 0x48); CHECK(g_deactivated == 1 && g_handled == 1);
    ui.State = 2; Run(ip, ev, pi, 1, 0, 0, 0x48); CHECK(g_deactivated == 0 && g_handled == 1);
    ui.State = 0;
    Run(ip, ev, pi, 1, 0, 0, 0x47); CHECK(g_sends == 0 && g_handled == 0 && mgr.MousePosition.X == -1.0f);   // pressed swallows the mouse arms
    // held arms
    Run(ip, ev, pi, 0, 1, 0, 0x19); CHECK(g_sends == 1 && g_lastEvent == 0x1c && g_handled == 1);
    Run(ip, ev, pi, 0, 1, 0, 0x4c); CHECK(g_sends == 1 && g_lastEvent == 0x1d && g_handled == 1);
    Run(ip, ev, pi, 0, 1, 0, 0x58); CHECK(g_sends == 0 && g_handled == 0);
    // released arms
    Run(ip, ev, pi, 0, 0, 1, 0x19); CHECK(g_sends == 1 && g_lastEvent == 0x1f && g_handled == 1);
    Run(ip, ev, pi, 0, 0, 1, 0x4c); CHECK(g_sends == 1 && g_lastEvent == 0x20 && g_handled == 1);
    Run(ip, ev, pi, 0, 0, 1, 0x58); CHECK(g_sends == 1 && g_lastEvent == 6 && g_handled == 1);
    Run(ip, ev, pi, 0, 0, 1, 0x59); CHECK(g_sends == 1 && g_lastEvent == 7 && g_handled == 1);
    Run(ip, ev, pi, 0, 0, 1, 0x48); CHECK(g_sends == 0 && g_handled == 0 && g_deactivated == 0);
    // mouse move: position copied to the manager, then event 0x19
    Run(ip, ev, pi, 0, 0, 0, 0x47); CHECK(g_sends == 1 && g_lastEvent == 0x19 && g_handled == 1);
    CHECK(mgr.MousePosition.X == 123.5f && mgr.MousePosition.Y == -42.25f);
    CHECK(ev.Position.X == 123.5f && ev.Position.Y == -42.25f);
    // wheel: ceil(delta / 120) lines
    Reset(); g_action = 0x65; g_wheel = 230.0f; ip.ProcessInput(ev, pi); CHECK(g_scrolls == 1 && g_scrollAmount == 2 && g_handled == 1 && g_sends == 0);
    Reset(); g_action = 0x65; g_wheel = -130.0f; ip.ProcessInput(ev, pi); CHECK(g_scrolls == 1 && g_scrollAmount == -1 && g_handled == 1);
    g_wheel = 0.0f;
    // nothing matched
    Run(ip, ev, pi, 0, 0, 0, 0x99); CHECK(g_sends == 0 && g_handled == 0 && g_scrolls == 0);
    // gates: inactive / closing / component missing
    ui.Active = false; Run(ip, ev, pi, 1, 0, 0, 0x19); CHECK(g_sends == 0 && g_handled == 0); ui.Active = true;
    ui.Closing = true; Run(ip, ev, pi, 1, 0, 0, 0x19); CHECK(g_sends == 0 && g_handled == 0); ui.Closing = false;
    hero.m_tcMask[0xfc >> 5] = 0; Run(ip, ev, pi, 1, 0, 0, 0x19); CHECK(g_sends == 0 && g_handled == 0); hero.m_tcMask[0xfc >> 5] = 1UL << (0xfc & 31);
    // lookup miss falls back to the end sentinel's +4 (points at `other` here via entries[2]? no: m_pLast is past the array) -- model it with a map whose last entry is the hit
    STCEntry two[2]; two[0].m_kind = 0x10; two[0].m_pTC = &other; two[1].m_kind = 0xfc; two[1].m_pTC = &ui;
    hero.m_tcs.m_pFirst = two; hero.m_tcs.m_pLast = two + 2; hero.m_tcs.m_pEnd = two + 2;
    Run(ip, ev, pi, 1, 0, 0, 0x19); CHECK(g_sends == 1 && g_lastEvent == 0x1a);

    if (fails) { printf("00691130_TEST FAIL (%d)\n", fails); return 1; }
    printf("00691130_TEST PASS\n");
    return 0;
}