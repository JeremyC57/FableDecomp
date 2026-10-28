// CInputProcessFireheartMinigame::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- feed the hero's fireheart minigame: the stick (game action 0x3C) past +-0.5 on Y then X sends inputs 0/2/1/3; a press of 0x2C quits (unless already quitting), 0x16 / 0x17 answer yes / no, and game action 3 queues game event 0x1A (0x17, true) when the hero has component 0xC6
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00691870, 592 bytes, `ret 8` (/GS cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Layout: engine/CInputProcessFireheartMinigame.h is a flattened view (base +0x04..+0x28); the game-base methods need
// the CInputProcessGameBase base, modelled as in the landed CInputProcessOracleMinigame 0x00691680.
#include "engine/CInputProcessGameBase.h"
class CInputEvent;
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
    void AddLong(long value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetHandled();                             // 0x00A0D390
};
class C2DVector { public: float X, Y; };
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_presentBits[kind >> 5] & (1UL << (kind & 31)); }
    template<class T> __forceinline T* GetInterface(long interfaceType)
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            result = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
        return result;
    }
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CThingPlayerCreature : public CThing { };
class CTCFireheartMinigame { public:
    unsigned char _pad00[0x88];
    bool          Quitting;                        // +0x88 (descriptive)
    void ProcessInput(long input);                 // 0x007E79C0 (EFireheartMinigameInput)
    void DelayedQuit();                            // 0x007E7AC0
    void Answer(bool yes);                         // 0x007E7C00
};
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CInputProcessFireheartMinigame : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    bool GetEventGameActionVector(long action, const CInputEvent& event, C2DVector& vector);   // 0x00687D00
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessFireheartMinigame::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventGameAction(0x3c, event)) {
        CTCFireheartMinigame* minigame = 0;
        MyPlayer->GetPControlledCreature()->GetInterface(&minigame, 0x108);
        C2DVector stick;
        if (GetEventGameActionVector(0x3c, event, stick)) {
            if (stick.Y > 0.5) {
                minigame->ProcessInput(0);
                processed.SetHandled();
            } else if (stick.Y < -0.5) {
                minigame->ProcessInput(2);
                processed.SetHandled();
            } else if (stick.X > 0.5) {
                minigame->ProcessInput(1);
                processed.SetHandled();
            } else if (stick.X < -0.5) {
                minigame->ProcessInput(3);
                processed.SetHandled();
            }
        }
    }
    if (!IsEventButtonPressed(event))
        return;
    CThing* hero = MyPlayer->GetPControlledCreature();
    CTCFireheartMinigame* minigame = hero->GetInterface<CTCFireheartMinigame>(0x108);
    if (IsEventGameAction(0x2c, event) && !minigame->Quitting) {
        minigame->DelayedQuit();
        processed.SetHandled();
        return;
    }
    if (IsEventGameAction(0x16, event)) {
        minigame->Answer(true);
        processed.SetHandled();
        return;
    }
    if (IsEventGameAction(0x17, event)) {
        minigame->Answer(false);
        processed.SetHandled();
        return;
    }
    if (IsEventGameAction(3, event) && hero->HasTC(0xc6)) {
        CGameEvent gameEvent(0x1a);
        gameEvent.AddLong(0x17);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    }
}