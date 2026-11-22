// CInputProcessFirstPersonTargeting::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- a held button event that is not game action 10 ends the player's mode 0x16 and marks the input handled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690390, length 54 bytes. Vtable 0x01237C98 slot 4.
// Callees: CInputProcessGameBase::IsEventButtonReleased(const CInputEvent&) @ 0x00687BD0, CInputProcessGameBase::IsEventGameAction((EGameAction)(long action), const CInputEvent&)
// @ 0x00687CF0 (forwards to CGamePlayerInterface::IsEventGameAction @ 0x00445BD0 through +0x24), CPlayer::RemoveMode(long) @ 0x00634190
// (MyPlayer +0x18), CProcessedInput::SetAsPerformedEvent @ 0x00A0D390 (NoGameEvents = 0, Type = 2).
// Shape: the event reference rides in edi for both predicate calls, nested ifs share the single exit, `ret 8`.
// Name fix (agentNF5 2026-11-10): CPlayer::RemoveMode 0x00634190 takes EPlayerMode (?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z, Ego_r.pdb / FableWin.pdb), was long.
enum EPlayerMode;                                  // MS forward enum (CPlayer::RemoveMode 0x00634190 takes it)
class CInputEvent;
class CProcessedInput { public:
    void SetAsPerformedEvent();                              // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
class CPlayer { public:
    void RemoveMode(EPlayerMode mode);                     // 0x00634190
};
#include "engine/EGameAction.h"
struct CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
    virtual void v00();
    unsigned char _pad04[0x10];                     // +0x04 CAInputProcess
    void*         World;                            // +0x14
    CPlayer*      MyPlayer;                         // +0x18
    void*         DefinitionManager;                // +0x1c
    void*         Component;                        // +0x20
    void*         PGamePlayerInterface;             // +0x24
protected:
    bool IsEventButtonReleased(const CInputEvent& event) const;                  // 0x00687BD0; NF236: protected const (Ego_r / FableWin IBE)
public:
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;     // 0x00687CF0
};
class CInputProcessFirstPersonTargeting : public CInputProcessGameBase { public:   // vtable 0x01237C98
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessFirstPersonTargeting::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventButtonReleased(event)) {
        if (!IsEventGameAction((EGameAction)(10), event)) {
            MyPlayer->RemoveMode((EPlayerMode)(0x16));
            processed.SetAsPerformedEvent();
        }
    }
}