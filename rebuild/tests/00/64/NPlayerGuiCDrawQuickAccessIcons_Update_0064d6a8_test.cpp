// Behaviour test for 0064d6a8: src/0064d6a8.cpp copied verbatim; the player chain, IS_THING_ALIVE, the activation time,
// CMemoryBuffer::Copy and AddGameEvent stubbed so the charge build-up, the completion event and the fade are observable.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// Retry 10b (B-12 sweep): the hero is a const REFERENCE to the call result (`CThingCreatureBase* const& hero = ...`): the game-event constructor keeps retail's dead `Player = -1` store and its store order before the Copy set-up (same fix as 0x0047D21C); `if (level <= ChargeLevel)` keeps the event arm first.
#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuickAccessIcons::Update() -- advance the quick-access charge: while charging, a held button whose activation time passes the charge level fires the "charge complete" game event (0x27, the hero's player number, the ability) and starts the completion fade, otherwise the level grows by one frame; the completion fade then runs down by one frame
// Leaf: Update
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D6A8, length 254 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetCharacterThing @ 0x00487DC0, IS_THING_ALIVE<CThing*> @ 0x0043DBD0,
// CDrawQuickAccessIcons::GetActivationTimeForButton(long) @ 0x0064B5C1 (float), CMemoryBuffer::Copy @ 0x009F1760,
// CMainGameComponent::AddGameEvent @ 0x0041649C (on 0x013B86A0), __security_check_cookie @ 0x00BFE9F9. Globals: g_fEpsilon 0x0129BA3C,
// g_framesPerSecond 0x01375550 (int), 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout: CDrawQuickAccessIcons as ToggleCharge 0x0064D92A (ChargingAbility +0x2c, ChargeLevel +0x30, Charging +0x34, ChargeComplete
// +0x35, ChargeFade +0x38); CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527; the hero's player byte +0x90.
struct CThing { unsigned char _pad[0x90]; unsigned char PlayerNumber; };   // +0x90 (descriptive)
struct CThingCreatureBase : CThing { };
template <class T> bool __fastcall IS_THING_ALIVE_0043dbd0(const T& thing);   // 0x0043DBD0 (CThing*)
struct CPlayer { CThingCreatureBase* GetCharacterThing(); };          // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };                  // 0x00449970
struct CMemoryBuffer { void Copy(unsigned long length, const void* data); };   // 0x009F1760
struct CGameEvent : CMemoryBuffer {
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLONG(long value) { Copy(4, &value); }
};
struct CMainGameComponent { void AddGameEvent(const CGameEvent& event); };   // 0x0041649C
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
extern float g_fEpsilon;                          // 0x0129BA3C
extern long  g_framesPerSecond;                   // 0x01375550
namespace NPlayerGui {
struct CDrawQuickAccessIcons {
    void*           _vtable;
    void*           PlayerGui;
    CPlayerManager* PlayerManager;                 // +0x08
    unsigned char   _base0c[0x2c - 0x0c];
    long            ChargingAbility;               // +0x2c
    float           ChargeLevel;                   // +0x30
    bool            Charging;                      // +0x34
    bool            ChargeComplete;                // +0x35
    float           ChargeFade;                    // +0x38
    float GetActivationTimeForButton(long ability);   // 0x0064B5C1
    void Update();
};
void CDrawQuickAccessIcons::Update()
{
    CThingCreatureBase* const& hero = PlayerManager->GetMainPlayer()->GetCharacterThing();
    if (!IS_THING_ALIVE_0043dbd0<CThing*>(hero))
        return;
    if (Charging) {
        float level = GetActivationTimeForButton(ChargingAbility);
        if (level > g_fEpsilon) {
            if (level <= ChargeLevel) {
                CGameEvent event(0x27);
                event.Player = hero->PlayerNumber;
                event.AddLONG(ChargingAbility);
                g_pMainGameComponent->AddGameEvent(event);
                Charging = false;
                ChargeComplete = true;
                ChargeFade = 0.5f;
            } else
                ChargeLevel += 1.0f / g_framesPerSecond;
        }
    }
    if (ChargeComplete) {
        ChargeFade -= 1.0f / g_framesPerSecond;
        if (ChargeFade <= 0.0f)
            ChargeComplete = false;
    }
}
}
// ---- end source ----
#pragma optimize("", on)
static CThingCreatureBase g_hero; static CPlayer g_player; static bool g_alive = true;
static float g_activation = 0.1f;
static int g_events = 0; static long g_eventType = 0; static signed char g_eventPlayer = 0; static long g_payload = 0;
float g_fEpsilon = 0.0001f;
long g_framesPerSecond = 10;
CThingCreatureBase* CPlayer::GetCharacterThing() { return &g_hero; }
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
template <class T> bool __fastcall IS_THING_ALIVE_0043dbd0(const T& thing) { return g_alive && thing == (CThing*)&g_hero; }
void CMemoryBuffer::Copy(unsigned long length, const void* data) { if (length == 4) memcpy(&g_payload, data, 4); }
void CMainGameComponent::AddGameEvent(const CGameEvent& e) { ++g_events; g_eventType = e.Type; g_eventPlayer = e.Player; }
static CMainGameComponent g_mgc;
CMainGameComponent* g_pMainGameComponent = &g_mgc;
float NPlayerGui::CDrawQuickAccessIcons::GetActivationTimeForButton(long) { return g_activation; }
int main()
{
    g_hero.PlayerNumber = 1;
    static CPlayerManager manager;
    static unsigned char raw[sizeof(NPlayerGui::CDrawQuickAccessIcons)];
    NPlayerGui::CDrawQuickAccessIcons* d = (NPlayerGui::CDrawQuickAccessIcons*)raw;
    d->PlayerManager = &manager; d->ChargingAbility = 42; d->ChargeLevel = 0.0f; d->Charging = true; d->ChargeComplete = false;
    d->Update();                                         // 0 < 0.1: build up by 1/10
    CHECK(g_events == 0 && d->ChargeLevel > 0.099f && d->ChargeLevel < 0.101f && d->Charging);
    d->Update();                                         // 0.1 <= 0.1: complete
    CHECK(g_events == 1 && g_eventType == 0x27 && g_eventPlayer == 1 && g_payload == 42);
    CHECK(!d->Charging && d->ChargeComplete && d->ChargeFade > 0.39f && d->ChargeFade < 0.41f);   // 0.5 then the same frame's fade
    for (int i = 0; i < 5; ++i) d->Update();
    CHECK(!d->ChargeComplete);
    g_alive = false; d->Charging = true; d->ChargeLevel = 5.0f;
    d->Update();
    CHECK(g_events == 1);
    if (fails == 0) printf("0064d6a8_TEST PASS\n"); else printf("0064d6a8_TEST FAIL\n");
    return fails;
}