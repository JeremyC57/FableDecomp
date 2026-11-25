// CInputProcessFishing::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- on a button press: game action 0x2b / 0x2c tells the hero's fishing component (0xB1) that reel-in / the second fishing button was pressed and marks the input handled; game action 3 (when the hero has component 0xC6) queues game event 0x1a carrying (long 0x17, true)
// Leaf: ProcessInput
// NF64 (round 7, q10652): the local IsEventButtonPressed / Held / Released redeclarations on the derived class are gone; the calls bind
//   the Ego_r.pdb + FableWin.pdb protected const CInputProcessGameBase members (overlay opt-in FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690B20, length 325 bytes, `ret 8` (/GS cookie 0x0139C8A8,
//   __security_check_cookie @ 0x00BFE9F9).
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventGameAction @ 0x00687CF0,
//   CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18), the component map's LowerBound @ 0x0040F020,
//   CTCFishing::ReelInButtonPressed @ 0x007B0BA0 and the fishing component's 0x007B0CC0 (a one-line `Xxx(0)` forwarder
//   to 0x007B0950; manifest label "_Dest_val<..CFlash>" is propagated, Ego_r.pdb / FableWin.pdb CancelButtonPressed, NF120 q6450),
//   CProcessedInput::SetAsPerformedEvent @ 0x00A0D390 / AddGameEvent(const CGameEvent&) @ 0x00A0D340, CMemoryBuffer::Copy
//   @ 0x009F1760.
// Layout: `this` is engine/CInputProcessGameBase.h (MyPlayer +0x18); CThing component bitset +0x20 (0xB1 = word 5 bit
//   17, 0xC6 = word 6 bit 6) and map +0x44 as in the landed CInputProcessBetting 0x00691560; CGameEvent (Ego_r.pdb, 40
//   bytes) as in the landed CInputProcessCombat::ProcessUnarmedInput 0x0068E8F0.
// Shape: `T* p = 0; if (HasTC) p = GetTC;` gives the `xor esi,esi` preload; the lookup key and both payload values sit
//   in the dead `event` argument slot (the event itself lives in ebx).
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
class CTCFishing { public:
    void ReelInButtonPressed();                    // 0x007B0BA0
    void CancelButtonPressed();                    // 0x007B0CC0 (Ego_r.pdb / FableWin.pdb; NF120: was the made-up CastButtonPressed)
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const ETCInterfaceType& kind);   // 0x0040F020
    FABLE_FORCEINLINE CTCFishing* Find(long kind)
    {
        STCEntry* it = LowerBound((ETCInterfaceType)kind);
        return ((CTCFishing*)((it == m_pLast || kind < it->first) ? m_pLast : it)->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                     // +0x20: which component kinds the thing holds
    unsigned char _pad40[4];
    CTCMap        m_tcs;                           // +0x44
    FABLE_FORCEINLINE unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    FABLE_FORCEINLINE CTCFishing* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CInputProcessFishing : public CInputProcessGameBase {
public:
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessFishing::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonPressed(event))
        return;
    CThing* hero = MyPlayer->GetPControlledCreature();
    CTCFishing* fishing = 0;
    if (hero->HasTC(0xb1))
        fishing = hero->GetTC(0xb1);
    if (IsEventGameAction(GAME_ACTION_FISHING_REEL_IN, event)) {
        fishing->ReelInButtonPressed();
        processed.SetAsPerformedEvent();
    } else if (IsEventGameAction(GAME_ACTION_FISHING_CANCEL, event)) {
        fishing->CancelButtonPressed();
        processed.SetAsPerformedEvent();
    } else if (IsEventGameAction(GAME_ACTION_OPEN_IN_GAME_MENU, event) && hero->HasTC(0xc6)) {
        CGameEvent gameEvent(0x1a);
        gameEvent.AddLong(0x17);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    }
}