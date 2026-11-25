// CInputProcessFireheartMinigame::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- feed the hero's fireheart minigame: the stick (game action 0x3C) past +-0.5 on Y then X sends inputs 0/2/1/3; a press of 0x2C quits (unless already quitting), 0x16 / 0x17 answer yes / no, and game action 3 queues game event 0x1A (0x17, true) when the hero has component 0xC6
// Leaf: ProcessInput
// NF64 (round 7, q10652): the local IsEventButtonPressed / Held / Released redeclarations on the derived class are gone; the calls bind
//   the Ego_r.pdb + FableWin.pdb protected const CInputProcessGameBase members (overlay opt-in FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00691870, 592 bytes, `ret 8` (/GS cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Layout: engine/CInputProcessFireheartMinigame.h is a flattened view (base +0x04..+0x28); the game-base methods need
// the CInputProcessGameBase base, modelled as in the landed CInputProcessOracleMinigame 0x00691680.
#include "fable_compiler.h"
#define FABLE_RETAIL_INPUTPROCESS_GAMEACTIONS   // NF37: the base-class queries (Ego_r/FableWin spellings), hdr/README.md
#include "engine/EGameAction.h"
#define FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON   // NF64 (q10652): protected const base button queries, hdr/README.md
#include "engine/CInputProcessGameBase.h"
class CInputEvent;
struct CMemoryBuffer {
};
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x009F1760 ?AddData@CGameEvent@@QAEXJPBX@Z (was ?Copy@CMemoryBuffer@@QAEXKPBX@Z)
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { reinterpret_cast<CGameEvent*>(this)->AddData(sizeof(value), &value); }
    void AddBool(bool value) { reinterpret_cast<CGameEvent*>(this)->AddData(1, &value); }
 public: void AddData(long, void const*); };
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetAsPerformedEvent();                             // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
class C2DVector { public: float X, Y; };
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    FABLE_FORCEINLINE unsigned long HasTC(long kind) const { return m_presentBits[kind >> 5] & (1UL << (kind & 31)); }
    template<class T> FABLE_FORCEINLINE T* GetInterface(long interfaceType)
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            result = (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
        }
        return result;
    }
    template<class T> FABLE_FORCEINLINE void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
        }
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
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
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessFireheartMinigame::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventGameAction(GAME_ACTION_MOVEMENT, event)) {
        CTCFireheartMinigame* minigame = 0;
        MyPlayer->GetPControlledCreature()->GetInterface(&minigame, 0x108);
        C2DVector stick;
        if (GetControlMovementFromGameActionEvent(GAME_ACTION_MOVEMENT, event, stick)) {
            if (stick.Y > 0.5) {
                minigame->ProcessInput(0);
                processed.SetAsPerformedEvent();
            } else if (stick.Y < -0.5) {
                minigame->ProcessInput(2);
                processed.SetAsPerformedEvent();
            } else if (stick.X > 0.5) {
                minigame->ProcessInput(1);
                processed.SetAsPerformedEvent();
            } else if (stick.X < -0.5) {
                minigame->ProcessInput(3);
                processed.SetAsPerformedEvent();
            }
        }
    }
    if (!IsEventButtonPressed(event))
        return;
    CThing* hero = MyPlayer->GetPControlledCreature();
    CTCFireheartMinigame* minigame = hero->GetInterface<CTCFireheartMinigame>(0x108);
    if (IsEventGameAction(GAME_ACTION_FISHING_CANCEL, event) && !minigame->Quitting) {
        minigame->DelayedQuit();
        processed.SetAsPerformedEvent();
        return;
    }
    if (IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_YES, event)) {
        minigame->Answer(true);
        processed.SetAsPerformedEvent();
        return;
    }
    if (IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_NO, event)) {
        minigame->Answer(false);
        processed.SetAsPerformedEvent();
        return;
    }
    if (IsEventGameAction(GAME_ACTION_OPEN_IN_GAME_MENU, event) && hero->HasTC(0xc6)) {
        CGameEvent gameEvent(0x1a);
        gameEvent.AddLong(0x17);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    }
}