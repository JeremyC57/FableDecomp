// Retry 10b (B-12 sweep): the hero is a const REFERENCE to the call result (`CThingCreatureBase* const& hero = ...`): the game-event constructor keeps retail's dead `Player = -1` store and its store order before the Copy set-up (same fix as 0x0047D21C); `if (level <= ChargeLevel)` keeps the event arm first.
#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuickAccessIcons::Update() -- advance the quick-access charge: while charging, a held button whose activation time passes the charge level fires the "charge complete" game event (0x27, the hero's player number, the ability) and starts the completion fade, otherwise the level grows by one frame; the completion fade then runs down by one frame
// Leaf: Update
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D6A8, length 254 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, IS_THING_ALIVE<CThing*> @ 0x0043DBD0,
// CDrawQuickAccessIcons::GetActivationTimeForButton(long) @ 0x0064B5C1 (float), CMemoryBuffer::Copy @ 0x009F1760,
// CMainGameComponent::AddGameEvent @ 0x0041649C (on 0x013B86A0), __security_check_cookie @ 0x00BFE9F9. Globals: g_fEpsilon 0x0129BA3C,
// g_framesPerSecond 0x01375550 (int), 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout: CDrawQuickAccessIcons as ToggleCharge 0x0064D92A (ChargingAbility +0x2c, ChargeLevel +0x30, Charging +0x34, ChargeComplete
// +0x35, ChargeFade +0x38); CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527; the hero's player byte +0x90.
class CThing { public: unsigned char _pad[0x90]; unsigned char PlayerNumber; };   // +0x90 (descriptive)
class CThingCreatureBase : public CThing { public: };
class CThingPlayerCreature : public CThingCreatureBase { };
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBD0 (T = CThingPlayerCreature*: fed by GetPControlledCreature 0x00487DC0 at 25 of 32 retail call sites)
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };          // 0x00487DC0
class CPlayerManager { public: CPlayer* GetMainPlayer(); };                  // 0x00449970
struct CMemoryBuffer { void Copy(unsigned long length, const void* data); };   // 0x009F1760
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLONG(long value) { Copy(4, &value); }
};
class CMainGameComponent { public: void AddGameEvent(const CGameEvent& event); };   // 0x0041649C
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
extern float g_fEpsilon;                          // 0x0129BA3C
extern long  g_framesPerSecond;                   // 0x01375550
namespace NPlayerGui {
class CDrawQuickAccessIcons { public:
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
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer()->GetPControlledCreature();
    if (!IS_THING_ALIVE(hero))
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