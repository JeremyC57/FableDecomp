// CInputProcessYesNoQuestion::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- answer the on-screen yes/no question: on PC keyboard input keys 4 (yes) / 10 (no), otherwise the yes / no / cancel game actions 0x16 / 0x17 / 0x18 (cancel only while the gui allows it) queue game event 0x1D carrying (answer, previous player mode) and mark the input handled
// Leaf: ProcessInput
// NF64 (round 7, q10652): the local IsEventButtonPressed / Held / Released redeclarations on the derived class are gone; the calls bind
//   the Ego_r.pdb + FableWin.pdb protected const CInputProcessGameBase members (overlay opt-in FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068C880, 576 bytes, `ret 8` (/GS cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
#define FABLE_RETAIL_INPUTPROCESS_GAMEACTIONS   // NF37: the base-class queries (Ego_r/FableWin spellings), hdr/README.md
#include "engine/EGameAction.h"
#define FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON   // NF64 (q10652): protected const base button queries, hdr/README.md
#include "engine/CInputProcessGameBase.h"
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
 public: void AddData(long, void const*); };
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetAsPerformedEvent();                             // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
// Naming (NF160, 2026-11-20, q4300): the local CInputEvent model replaced by the shared engine/CInputEvent.h (Ego_r.pdb / FableWin.pdb
//   spellings): 0x00A03B40 GetKey_00a03b40 -> GetType; GetDeviceType 0x00A03B50 unchanged.
#define FABLE_RETAIL_CINPUTEVENT_DEVICETYPE
#include "engine/CInputEvent.h"
struct CPlayerGuiQuestion {
    bool CanCancel();                              // 0x0048DB98
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char      _pad00[0x18];
    CPlayerGuiQuestion Question;                   // +0x18
    unsigned char      _pad19[0xfa - 0x19];
    bool               UsingKeyboard;              // +0xFA
};
// Layout: engine/CInputProcessYesNoQuestion.h is a flattened view of the same object (PreviousMode +0x28); the
// game-base methods need the CInputProcessGameBase base, so the derived member is declared here.
class CInputProcessYesNoQuestion : public CInputProcessGameBase {
public:
    long PreviousMode;                             // +0x28 EPlayerMode
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessYesNoQuestion::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonPressed(event))
        return;
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UsingKeyboard) {
        if (event.GetDeviceType() == 3 && event.GetType() == 4) {
            CGameEvent gameEvent(0x1d);
            gameEvent.AddLong(1);
            gameEvent.AddLong(PreviousMode);
            processed.SetAsPerformedEvent();
            processed.AddGameEvent(gameEvent);
        } else if (event.GetDeviceType() == 3 && event.GetType() == 10) {
            CGameEvent gameEvent(0x1d);
            gameEvent.AddLong(0);
            gameEvent.AddLong(PreviousMode);
            processed.SetAsPerformedEvent();
            processed.AddGameEvent(gameEvent);
        }
    } else if (IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_YES, event)) {
        CGameEvent gameEvent(0x1d);
        gameEvent.AddLong(1);
        gameEvent.AddLong(PreviousMode);
        processed.SetAsPerformedEvent();
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_NO, event)) {
        CGameEvent gameEvent(0x1d);
        gameEvent.AddLong(0);
        gameEvent.AddLong(PreviousMode);
        processed.SetAsPerformedEvent();
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_THIRD, event) && CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Question.CanCancel()) {
        CGameEvent gameEvent(0x1d);
        gameEvent.AddLong(2);
        gameEvent.AddLong(PreviousMode);
        processed.SetAsPerformedEvent();
        processed.AddGameEvent(gameEvent);
    }
}