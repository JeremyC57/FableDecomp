// CInputProcessOracleMinigame::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- on a button press: game actions 0x42..0x45 push oracle symbols 0/2/3/1 into the hero's oracle minigame component (0x107), 0x46 quits it, game action 3 (when the hero has component 0xC6) queues game event 0x1a carrying (long 0x17, true); every press is marked handled
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00691680, length 370 bytes, `ret 8` (/GS cookie 0x0139C8A8,
//   __security_check_cookie @ 0x00BFE9F9).
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventGameAction @ 0x00687CF0,
//   CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18), the component map's LowerBound @ 0x0040F020,
//   CTCOracleMinigame::PushSymbolToQueue(long) @ 0x007E5F70 and Quit() @ 0x007E62D0, CMemoryBuffer::Copy @ 0x009F1760,
//   CProcessedInput::AddGameEvent(const CGameEvent&) @ 0x00A0D340 / SetHandled @ 0x00A0D390.
// Layout: `this` is engine/CInputProcessGameBase.h (MyPlayer +0x18); CThing component bitset +0x20..+0x43 (0x107 = word 8
//   bit 7, read as the sign of the byte at +0x40; 0xC6 = word 6 bit 6) and map +0x44 as in the landed
//   CInputProcessBetting 0x00691560; CGameEvent (Ego_r.pdb, 40 bytes) as in the landed 0x0068E8F0.
// Shape: the same press/fishing-style event arm as CInputProcessFishing::ProcessInput 0x00690B20; SetHandled closes every
//   pressed path, including the no-match one.
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
class CTCOracleMinigame { public:
    void PushSymbolToQueue(long symbol);           // 0x007E5F70
    void Quit();                                   // 0x007E62D0
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long               m_kind;
    CTCOracleMinigame* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCOracleMinigame* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20: which component kinds the thing holds
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCOracleMinigame* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CInputProcessOracleMinigame : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessOracleMinigame::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonPressed(event))
        return;
    CThing* hero = MyPlayer->GetPControlledCreature();
    CTCOracleMinigame* oracle = 0;
    if (hero->HasTC(0x107))
        oracle = hero->GetTC(0x107);
    if (IsEventGameAction(0x42, event))
        oracle->PushSymbolToQueue(0);
    else if (IsEventGameAction(0x43, event))
        oracle->PushSymbolToQueue(2);
    else if (IsEventGameAction(0x44, event))
        oracle->PushSymbolToQueue(3);
    else if (IsEventGameAction(0x45, event))
        oracle->PushSymbolToQueue(1);
    else if (IsEventGameAction(0x46, event))
        oracle->Quit();
    else if (IsEventGameAction(3, event) && hero->HasTC(0xc6)) {
        CGameEvent gameEvent(0x1a);
        gameEvent.AddLong(0x17);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    }
    processed.SetHandled();
}