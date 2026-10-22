// CInputProcessSpecialAbilities::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- with abilities enabled and a live hero holding the abilities component: an assignable special-move button triggers the assigned ability, game action 0x39 cycles the assigned abilities (mouse-wheel steps of 120 or a held button) and action 0x5e cycles forward; then hero state 0xb queues game event 0x40 (outside mode 0x28) and state 0x10 with a named state queues game event 0x3b (outside modes 0x23 and 2)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00690030, 805 bytes, `ret 8`, /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Callees: CPlayer::GetHeroCreature 0x00487DC0 / GetControlledCreature 0x00487DD0 / GetCurrentMode 0x00633BE0, LowerBound
//   0x0040F020 (non-const, interface 0x6f) and 0x004365B0 (const, interface 0xa7), CInputProcessGameBase 0x00687A90/0x00687BD0/
//   0x00687C70/0x00687CF0/0x00687D40, GetAssignableSpecialMoveIndexForEvent 0x00445FA0, TriggerSpecialAbility 0x005EED4C,
//   CycleAssignedAbilities 0x005F64DD, CheckTermination 0x0072A860, GetName 0x0072B870, CCharString operator!= 0x0099E960 / dtor
//   0x0099EAE0, AddGameEvent 0x0041649C on g_pMainGameComponent 0x013B86A0, SetHandled 0x00A0D390, _ftol 0x00BFEA70.
// Layout: engine/CInputProcessGameBase.h fields (MyPlayer +0x18, PGamePlayerInterface +0x24); CThing interface map +0x44,
//   presence bits +0x20, Dead bit +0x91 (retail CThing); the other members are descriptive offsets read from retail.
// Shape: SetHandled written in both arms of the scroll-loop if/else (VC tail-merges the calls, and the second use gives
//   processed esi over the countdown); the hero-state test is a switch with case 0x10 first; the GetName result is a named local.
// Leaf: ProcessInput
// Flags: /GS
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
inline float Abs(float x) { return (float)fabs(x); }
struct CCharString {
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
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
struct CMainGameComponent {
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
struct CTCBase;
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
struct CTCInventoryAbilities {
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
struct CGamePlayerInterface {
    long GetAssignableSpecialMoveIndexForEvent(const CInputEvent& event) const;   // 0x00445FA0
};
struct CWorld; struct CDefinitionManager;
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
