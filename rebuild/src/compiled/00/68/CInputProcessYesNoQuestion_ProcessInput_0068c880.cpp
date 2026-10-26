// CInputProcessYesNoQuestion::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- answer the on-screen yes/no question: on PC keyboard input keys 4 (yes) / 10 (no), otherwise the yes / no / cancel game actions 0x16 / 0x17 / 0x18 (cancel only while the gui allows it) queue game event 0x1D carrying (answer, previous player mode) and mark the input handled
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068C880, 576 bytes, `ret 8` (/GS cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
#include "engine/CInputProcessGameBase.h"
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
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetHandled();                             // 0x00A0D390
};
class CInputEvent {
public:
    long GetKey_00a03b40() const;                           // 0x00A03B40
    long GetDeviceType() const;                    // 0x00A03B50
};
struct CPlayerGuiQuestion {
    bool CanCancel();                              // 0x0048DB98
};
struct CPlayerGuiCalls {
    unsigned char      _pad00[0x18];
    CPlayerGuiQuestion Question;                   // +0x18
    unsigned char      _pad19[0xfa - 0x19];
    bool               UsingKeyboard;              // +0xFA
};
extern CPlayerGuiCalls* g_pPlayerGui;              // 0x013B8790
// Layout: engine/CInputProcessYesNoQuestion.h is a flattened view of the same object (PreviousMode +0x28); the
// game-base methods need the CInputProcessGameBase base, so the derived member is declared here.
class CInputProcessYesNoQuestion : public CInputProcessGameBase {
public:
    long PreviousMode;                             // +0x28 EPlayerMode
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessYesNoQuestion::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonPressed(event))
        return;
    if (g_pPlayerGui->UsingKeyboard) {
        if (event.GetDeviceType() == 3 && event.GetKey_00a03b40() == 4) {
            CGameEvent gameEvent(0x1d);
            gameEvent.AddLong(1);
            gameEvent.AddLong(PreviousMode);
            processed.SetHandled();
            processed.AddGameEvent(gameEvent);
        } else if (event.GetDeviceType() == 3 && event.GetKey_00a03b40() == 10) {
            CGameEvent gameEvent(0x1d);
            gameEvent.AddLong(0);
            gameEvent.AddLong(PreviousMode);
            processed.SetHandled();
            processed.AddGameEvent(gameEvent);
        }
    } else if (IsEventGameAction(0x16, event)) {
        CGameEvent gameEvent(0x1d);
        gameEvent.AddLong(1);
        gameEvent.AddLong(PreviousMode);
        processed.SetHandled();
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(0x17, event)) {
        CGameEvent gameEvent(0x1d);
        gameEvent.AddLong(0);
        gameEvent.AddLong(PreviousMode);
        processed.SetHandled();
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(0x18, event) && g_pPlayerGui->Question.CanCancel()) {
        CGameEvent gameEvent(0x1d);
        gameEvent.AddLong(2);
        gameEvent.AddLong(PreviousMode);
        processed.SetHandled();
        processed.AddGameEvent(gameEvent);
    }
}