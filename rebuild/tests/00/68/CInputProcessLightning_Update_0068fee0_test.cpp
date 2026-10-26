// Behaviour test for CInputProcessLightning::Update @ 0x0068FEE0: queues game event 0x41 (stop the lightning spell)
// unless MyPlayer's controlled creature exists, is alive and holds component kind 0x94 (bit 20 of the presence word
// at +0x30). Checks: no creature, dead creature with the component, live creature with it (no event), live creature
// with only the neighbouring kinds 0x93/0x95/0xb4 (event), and the event's default fields.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

class CInputEvent;
struct CMemoryBuffer {
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[0x51];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
class CPlayer {
public:
    CThing_Retail* GetControlledCreature();        // 0x00487DD0
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90 (kind 0x13)
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0 (kind 0x15)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessLightning : public CInputProcessGameBase {
public:
    virtual void Update(CProcessedInput& processed);
};

void CInputProcessLightning::Update(CProcessedInput& processed)
{
    CThing_Retail* creature = MyPlayer->GetControlledCreature();
    if (creature == 0 || creature->m_dead || !creature->HasTC(0x94)) {
        CGameEvent gameEvent(0x41);
        processed.AddGameEvent(gameEvent);
    }
}

// ---- instrumented stubs ----
class CInputEvent { public: int unused; };
void CInputProcessGameBase::v00() { }
static int g_events = 0, g_creatureCalls = 0;
static CGameEvent g_last(0);
static CProcessedInput* g_expectProcessed = 0;
static CPlayer* g_expectPlayer = 0;
static CThing_Retail* g_creature = 0;
void CProcessedInput::AddGameEvent(const CGameEvent& e) { if (this != g_expectProcessed) g_fail = 1; ++g_events; g_last = e; }
CThing_Retail* CPlayer::GetControlledCreature() { ++g_creatureCalls; if (this != g_expectPlayer) g_fail = 1; return g_creature; }

static void reset() { g_events = g_creatureCalls = 0; g_last = CGameEvent(0); }
static bool stopEventOk() { return g_events == 1 && g_last.Type == 0x41 && g_last.Player == -1 && g_last.EndPos == 0 && g_last.Valid && !g_last.Replacement; }

int main()
{
    CInputProcessLightning proc; CPlayer player; CProcessedInput processed;
    proc.MyPlayer = &player; g_expectPlayer = &player; g_expectProcessed = &processed;
    CThing_Retail thing; std::memset(&thing, 0, sizeof(thing));

    // No controlled creature.
    reset(); g_creature = 0;
    proc.Update(processed);
    CHECK(g_creatureCalls == 1 && stopEventOk());

    // Dead creature holding the component.
    reset(); g_creature = &thing; thing.m_tcMask[4] = 1UL << 20; thing.m_dead = 1;
    proc.Update(processed);
    CHECK(stopEventOk());

    // Live creature holding the component: nothing queued.
    reset(); thing.m_dead = 0; thing.m_91_1 = 0x7f;
    proc.Update(processed);
    CHECK(g_creatureCalls == 1 && g_events == 0);

    // Live creature with only the neighbouring kinds (0x93, 0x95, 0xb4).
    reset(); thing.m_tcMask[4] = (1UL << 19) | (1UL << 21); thing.m_tcMask[5] = 1UL << 20;
    proc.Update(processed);
    CHECK(stopEventOk());

    // Every other presence word full but the kind-0x94 bit clear.
    reset(); for (int i = 0; i < 8; ++i) thing.m_tcMask[i] = 0xffffffffUL; thing.m_tcMask[4] &= ~(1UL << 20);
    proc.Update(processed);
    CHECK(stopEventOk());

    if (g_fail) { std::printf("0068fee0_TEST FAIL\n"); return 1; }
    std::printf("0068fee0_TEST PASS\n");
    return 0;
}