#ifndef _MSC_VER
#define __cdecl
#define __forceinline inline
#endif
// Leaf: ProcessInput
// Flags: /GS
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
inline float Abs(float x) { return (float)fabs(x); }
class CCharString { public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const char* text) const;            // 0x0099E960
};
class CInputEvent {
public:
    unsigned char _pad00[0x28];
    long          Type;                                 // +0x28 (descriptive; read inline)
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
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
class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                        // +0x91 bit 0
    template<class T> __forceinline T* GetInterfaceUnchecked(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCInventoryAbilities { public:
    void TriggerSpecialAbility(long index);        // 0x005EED4C (label)
    void CycleAssignedAbilities(bool forward);     // 0x005F64DD
};
struct CTCHeroState {                              // interface 0xa7 (descriptive)
    unsigned char _pad00[0x10];
    long          State;                           // +0x10
    bool CheckTermination();                       // 0x0072A860
    CCharString GetName() const;                   // 0x0072B870 (CCharString by hidden pointer)
};
class CPlayer {
public:
    unsigned char _pad00[0x28];
    signed char   PlayerNumber;                    // +0x28 (descriptive)
    unsigned char _pad29[0x218 - 0x29];
    bool          AbilitiesEnabled;                // +0x218 (descriptive)
    CThing* GetHeroCreature();                     // 0x00487DC0
    CThing* GetControlledCreature();               // 0x00487DD0
    long GetCurrentMode() const;                   // 0x00633BE0
};
class CGamePlayerInterface { public:
    long GetAssignableSpecialMoveIndexForEvent(const CInputEvent& event) const;   // 0x00445FA0
};
class CWorld; class CDefinitionManager;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CDefinitionManager*     DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventAssignableSpecialMoveButton(const CInputEvent& event);   // 0x00687C70
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    float GetEventScrollAmount(const CInputEvent& event);             // 0x00687D40 (descriptive)
};
class CInputProcessSpecialAbilities : public CInputProcessGameBase {
public:
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessSpecialAbilities::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetHeroCreature();
    if (MyPlayer->AbilitiesEnabled && hero && !hero->Dead && (hero->m_presentBits[3] & 0x8000)) {
        CTCInventoryAbilities* abilities = hero->GetInterfaceUnchecked<CTCInventoryAbilities>(0x6f);
        CTCHeroState* state = 0;
        hero->GetInterface(&state, 0xa7);
        if (IsEventAssignableSpecialMoveButton(event)) {
            if (IsEventButtonPressed(event)) {
                abilities->TriggerSpecialAbility(PGamePlayerInterface->GetAssignableSpecialMoveIndexForEvent(event));
                processed.SetHandled();
            } else if (IsEventButtonHeld(event)) {
                processed.SetHandled();
            }
        } else if (IsEventGameAction(0x39, event)) {
            if (state && !state->CheckTermination()) {
                if (event.Type == 0xe) {
                    float amount = GetEventScrollAmount(event);
                    float size = Abs(amount);
                    if (size > 0.0001f) {
                        long steps = (long)size / 120;
                        for (long i = 0; i < steps; ++i) {
                            if (amount > 0.0f) {
                                abilities->CycleAssignedAbilities(true);
                                processed.SetHandled();
                            } else {
                                abilities->CycleAssignedAbilities(false);
                                processed.SetHandled();
                            }
                        }
                    }
                } else if (IsEventButtonHeld(event)) {
                    abilities->CycleAssignedAbilities(true);
                    processed.SetHandled();
                }
            }
        } else if (IsEventGameAction(0x5e, event) && state && !state->CheckTermination() && IsEventButtonPressed(event)) {
            abilities->CycleAssignedAbilities(true);
            processed.SetHandled();
        }
    }
    CThing* creature = MyPlayer->GetControlledCreature();
    CTCHeroState* heroState;
    if (creature->GetInterface(&heroState, 0xa7)) {
        switch (heroState->State) {
        case 0x10: {
            long mode = MyPlayer->GetCurrentMode();
            if (mode != 0x23 && mode != 2) {
                CCharString name = heroState->GetName();
                if (name != "") {
                    CGameEvent gameEvent(0x3b);
                    gameEvent.Player = MyPlayer->PlayerNumber;
                    g_pMainGameComponent->AddGameEvent(gameEvent);
                }
            }
            break;
        }
        case 0xb:
            if (MyPlayer->GetCurrentMode() != 0x28) {
                CGameEvent gameEvent(0x40);
                gameEvent.Player = MyPlayer->PlayerNumber;
                g_pMainGameComponent->AddGameEvent(gameEvent);
            }
            break;
        }
    }
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int g_cycles = 0, g_handled = 0, g_trigger = 0, g_events = 0;
CCharString::~CCharString() {}
bool CCharString::operator!=(const char*) const { return false; }
void CMemoryBuffer::Copy(unsigned long, const void*) {}
void CMainGameComponent::AddGameEvent(const CGameEvent&) { ++g_events; }
static CMainGameComponent g_component;
CMainGameComponent* g_pMainGameComponent = &g_component;
void CProcessedInput::SetHandled() { ++g_handled; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { const SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CTCInventoryAbilities::TriggerSpecialAbility(long) { ++g_trigger; }
void CTCInventoryAbilities::CycleAssignedAbilities(bool forward) { g_cycles += forward ? 1 : -1; }
bool CTCHeroState::CheckTermination() { return false; }
CCharString CTCHeroState::GetName() const { CCharString s; s.PStringData = 0; return s; }
static CThing g_hero;
static CTCHeroState g_state;
CThing* CPlayer::GetHeroCreature() { return &g_hero; }
CThing* CPlayer::GetControlledCreature() { return &g_hero; }
long CPlayer::GetCurrentMode() const { return 2; }
long CGamePlayerInterface::GetAssignableSpecialMoveIndexForEvent(const CInputEvent&) const { return 0; }
void CInputProcessGameBase::v00() {}
bool CInputProcessGameBase::IsEventButtonPressed(const CInputEvent&) { return false; }
bool CInputProcessGameBase::IsEventButtonHeld(const CInputEvent&) { return false; }
bool CInputProcessGameBase::IsEventAssignableSpecialMoveButton(const CInputEvent&) { return false; }
bool CInputProcessGameBase::IsEventGameAction(long action, const CInputEvent&) { return action == 0x39; }
static float g_scroll = 0.0f;
float CInputProcessGameBase::GetEventScrollAmount(const CInputEvent&) { return g_scroll; }
int main()
{
    CTCInventoryAbilities abilities;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x6f; entries[0].component = (CTCBase*)&abilities;
    entries[1].type = 0xa7; entries[1].component = (CTCBase*)&g_state;
    memset(&g_hero, 0, sizeof(g_hero));
    g_hero.m_presentBits[3] |= 0x8000;
    g_hero.m_presentBits[0xa7 >> 5] |= 1UL << (0xa7 & 31);
    g_hero.m_interfaces.first = entries; g_hero.m_interfaces.last = entries + 2;
    g_state.State = 0;
    CPlayer player; memset(&player, 0, sizeof(player)); player.AbilitiesEnabled = true;
    CGamePlayerInterface gpi;
    CInputProcessSpecialAbilities proc; proc.MyPlayer = &player; proc.PGamePlayerInterface = &gpi;
    CInputEvent event; memset(&event, 0, sizeof(event)); event.Type = 0xe;
    CProcessedInput processed;
    g_scroll = 360.0f; proc.ProcessInput(event, processed);
    int ok = (g_cycles == 3 && g_handled == 3);
    g_scroll = -250.0f; proc.ProcessInput(event, processed);
    ok = ok && (g_cycles == 1 && g_handled == 5);
    g_scroll = 0.0f; proc.ProcessInput(event, processed);
    ok = ok && (g_cycles == 1 && g_handled == 5 && g_trigger == 0 && g_events == 0);
    printf(ok ? "00690030_TEST PASS\n" : "00690030_TEST FAIL cycles=%d handled=%d\n", g_cycles, g_handled);
    return ok ? 0 : 1;
}