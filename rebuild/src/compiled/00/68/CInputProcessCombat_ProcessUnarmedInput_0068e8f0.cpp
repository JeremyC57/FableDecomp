// CInputProcessCombat::ProcessUnarmedInput(const CInputEvent& event, CProcessedInput& processed) -- on a button press: game action 0xb queues game event 0x16 carrying the CREATURE_ABILITY definition index (2 bytes) and the button pressure; game action 0xd / 0xe queue event 0x21 / 0x22 carrying a true byte
// Leaf: ProcessUnarmedInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068E8F0, length 275 bytes (+ int3 padding), `ret 8`.
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventGameAction @ 0x00687CF0,
//   GetButtonPressureForEvent @ 0x00687D10 (float; catalog label "operator_new" is propagated), CCharString(const char*,
//   long) @ 0x0099EBF0 ("CREATURE_ABILITY" @ 0x01259AFC) / ~CCharString @ 0x0099EAE0,
//   CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, CMemoryBuffer::Copy @ 0x009F1760,
//   CProcessedInput::AddGameEvent(const CGameEvent&) @ 0x00A0D340, __security_check_cookie @ 0x00BFE9F9.
// Layout: engine/CInputProcessGameBase.h (DefinitionManager +0x1c), modelled locally as in the landed 0x00690C70;
//   CGameEvent (Ego_r.pdb, 40 bytes) as there.
// Shape: three else-if arms, each building and queueing its own event (retail tail-merges the two bool arms after
//   the type store, and the 0x16 arm's Copy + AddGameEvent tail too); the inline Add* helpers home their by-value argument (and the name string temporary) in the dead `event` slot.
class CInputEvent;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
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
    void AddUWORD(long value) { Copy(2, &value); }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CPlayer;
struct CWorld; struct CMainGameComponent; struct CGamePlayerInterface;
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
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    float GetButtonPressureForEvent(const CInputEvent& event);        // 0x00687D10
};
class CInputProcessCombat : public CInputProcessGameBase {
public:
    void ProcessUnarmedInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessCombat::ProcessUnarmedInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonPressed(event))
        return;
    if (IsEventGameAction(0xb, event)) {
        CGameEvent gameEvent(0x16);
        gameEvent.AddUWORD(DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY"));
        gameEvent.AddFloat(GetButtonPressureForEvent(event));
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(0xd, event)) {
        CGameEvent gameEvent(0x21);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(0xe, event)) {
        CGameEvent gameEvent(0x22);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    }
}