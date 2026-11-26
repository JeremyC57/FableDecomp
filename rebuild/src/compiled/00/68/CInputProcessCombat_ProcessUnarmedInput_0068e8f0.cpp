// CInputProcessCombat::ProcessUnarmedInput(const CInputEvent& event, CProcessedInput& processed) -- on a button press: game action 0xb queues game event 0x16 carrying the CREATURE_ABILITY_ATTACK definition index (2 bytes) and the button pressure; game action 0xd / 0xe queue event 0x21 / 0x22 carrying a true byte
// Leaf: ?ProcessUnarmedInput@CInputProcessCombat@@AAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// NF33 (2026-11-16): IsEventGameAction / GetControlMovementFromGameActionEvent respelled to the Ego_r.pdb + FableWin.pdb
//   CInputProcessGameBase spellings (EGameAction, const; the movement vector by reference).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068E8F0, length 275 bytes (+ int3 padding), `ret 8`.
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventGameAction @ 0x00687CF0,
//   GetButtonPressureForEvent @ 0x00687D10 (float; catalog label "operator_new" is propagated), CCharString(const char*,
//   long) @ 0x0099EBF0 ("CREATURE_ABILITY_ATTACK" @ 0x01259AFC) / ~CCharString @ 0x0099EAE0,
//   CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, CMemoryBuffer::Copy @ 0x009F1760,
//   CProcessedInput::AddGameEvent(const CGameEvent&) @ 0x00A0D340, __security_check_cookie @ 0x00BFE9F9.
// Layout: engine/CInputProcessGameBase.h (DefinitionManager +0x1c), modelled locally as in the landed 0x00690C70;
//   CGameEvent (Ego_r.pdb, 40 bytes) as there.
// Shape: three else-if arms, each building and queueing its own event (retail tail-merges the two bool arms after
//   the type store, and the 0x16 arm's Copy + AddGameEvent tail too); the inline Add* helpers home their by-value argument (and the name string temporary) in the dead `event` slot.
class CInputEvent;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
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
    void AddUWORD(long value) { reinterpret_cast<CGameEvent*>(this)->AddData(2, &value); }
    void AddFloat(float value) { reinterpret_cast<CGameEvent*>(this)->AddData(sizeof(value), &value); }
    void AddBool(bool value) { reinterpret_cast<CGameEvent*>(this)->AddData(1, &value); }
 public: void AddData(long, void const*); };
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CPlayer;
class CWorld; class CMainGameComponent; class CGamePlayerInterface;
#include "engine/EGameAction.h"
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CDefinitionManager*     DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
protected:
    bool IsEventButtonPressed(const CInputEvent& event) const;              // 0x00687A90; NF236: protected const (Ego_r / FableWin IBE)
public:
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;    // 0x00687CF0
    float GetButtonPressureForEvent(const CInputEvent& event) const;        // 0x00687D10
};
class CInputProcessCombat : public CInputProcessGameBase {
public:
    private:
    void ProcessUnarmedInput(const CInputEvent& event, CProcessedInput& processed);
    public:
};

void CInputProcessCombat::ProcessUnarmedInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonPressed(event))
        return;
    if (IsEventGameAction((EGameAction)(0xb), event)) {
        CGameEvent gameEvent(0x16);
        gameEvent.AddUWORD(DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_ATTACK"));
        gameEvent.AddFloat(GetButtonPressureForEvent(event));
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction((EGameAction)(0xd), event)) {
        CGameEvent gameEvent(0x21);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction((EGameAction)(0xe), event)) {
        CGameEvent gameEvent(0x22);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    }
}