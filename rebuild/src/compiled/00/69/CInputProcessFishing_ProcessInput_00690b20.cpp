// CInputProcessFishing::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- on a button press: game action 0x2b / 0x2c tells the hero's fishing component (0xB1) that reel-in / the second fishing button was pressed and marks the input handled; game action 3 (when the hero has component 0xC6) queues game event 0x1a carrying (long 0x17, true)
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690B20, length 325 bytes, `ret 8` (/GS cookie 0x0139C8A8,
//   __security_check_cookie @ 0x00BFE9F9).
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventGameAction @ 0x00687CF0,
//   CPlayer::GetHeroCreature @ 0x00487DC0 (MyPlayer +0x18), the component map's LowerBound @ 0x0040F020,
//   CTCFishing::ReelInButtonPressed @ 0x007B0BA0 and the fishing component's 0x007B0CC0 (a one-line `Xxx(0)` forwarder
//   to 0x007B0950; manifest label "_Dest_val<..CFlash>" is propagated, named CastButtonPressed here),
//   CProcessedInput::SetHandled @ 0x00A0D390 / AddGameEvent(const CGameEvent&) @ 0x00A0D340, CMemoryBuffer::Copy
//   @ 0x009F1760.
// Layout: `this` is engine/CInputProcessGameBase.h (MyPlayer +0x18); CThing component bitset +0x20 (0xB1 = word 5 bit
//   17, 0xC6 = word 6 bit 6) and map +0x44 as in the landed CInputProcessBetting 0x00691560; CGameEvent (Ego_r.pdb, 40
//   bytes) as in the landed CInputProcessCombat::ProcessUnarmedInput 0x0068E8F0.
// Shape: `T* p = 0; if (HasTC) p = GetTC;` gives the `xor esi,esi` preload; the lookup key and both payload values sit
//   in the dead `event` argument slot (the event itself lives in ebx).
#include "engine/CInputProcessGameBase.h"
class CInputEvent;
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
    void AddLong(long value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetHandled();                             // 0x00A0D390
};
struct CTCFishing {
    void ReelInButtonPressed();                    // 0x007B0BA0
    void CastButtonPressed();                      // 0x007B0CC0
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long        m_kind;
    CTCFishing* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCFishing* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                     // +0x20: which component kinds the thing holds
    unsigned char _pad40[4];
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCFishing* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CPlayer {
public:
    CThing* GetHeroCreature();                     // 0x00487DC0
};
class CInputProcessFishing : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessFishing::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonPressed(event))
        return;
    CThing* hero = MyPlayer->GetHeroCreature();
    CTCFishing* fishing = 0;
    if (hero->HasTC(0xb1))
        fishing = hero->GetTC(0xb1);
    if (IsEventGameAction(0x2b, event)) {
        fishing->ReelInButtonPressed();
        processed.SetHandled();
    } else if (IsEventGameAction(0x2c, event)) {
        fishing->CastButtonPressed();
        processed.SetHandled();
    } else if (IsEventGameAction(3, event) && hero->HasTC(0xc6)) {
        CGameEvent gameEvent(0x1a);
        gameEvent.AddLong(0x17);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    }
}