// CInputProcessFirstPersonTargeting::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- a held button event that is not game action 10 ends the player's mode 0x16 and marks the input handled
// Leaf: ?ProcessInput@CInputProcessFirstPersonTargeting@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690390, length 54 bytes. Vtable 0x01237C98 slot 4.
// Callees: CInputProcessGameBase::IsEventButtonReleased(const CInputEvent&) @ 0x00687BD0, CInputProcessGameBase::IsEventGameAction((EGameAction)(long action), const CInputEvent&)
// @ 0x00687CF0 (forwards to CGamePlayerInterface::IsEventGameAction @ 0x00445BD0 through +0x24), CPlayer::RemoveMode(long) @ 0x00634190
// (MyPlayer +0x18), CProcessedInput::SetAsPerformedEvent @ 0x00A0D390 (NoGameEvents = 0, Type = 2).
// Shape: the event reference rides in edi for both predicate calls, nested ifs share the single exit, `ret 8`.
// Name fix (agentNF5 2026-11-10): CPlayer::RemoveMode 0x00634190 takes EPlayerMode (?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z, Ego_r.pdb / FableWin.pdb), was long.
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the non-virtual ?ProcessInput@CInputProcessFirstPersonTargeting@@QAE... on a local
//   class model; respelt onto engine/CInputProcessFirstPersonTargeting.h FABLE_RETAIL_INPUTPROCESS_VTABLE (Ego_r.pdb / FableWin.pdb public
//   virtual, retail vftable 0x01237C98 slot 4), so the family vftables 0x00488D10 emits name this row. The base queries come from the
//   retail view of engine/CInputProcessGameBase.h (FABLE_RETAIL_INPUTPROCESS_ZTARGET: the same PDB spellings). No body change.
enum EPlayerMode;                                  // MS forward enum (CPlayer::RemoveMode 0x00634190 takes it)
class CInputEvent;
class CProcessedInput { public:
    void SetAsPerformedEvent();                              // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
class CPlayer { public:
    void RemoveMode(EPlayerMode mode);                     // 0x00634190
};
#include "engine/EGameAction.h"
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#include "engine/CInputProcessFirstPersonTargeting.h"

void CInputProcessFirstPersonTargeting::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventButtonReleased(event)) {
        if (!IsEventGameAction((EGameAction)(10), event)) {
            MyPlayer->RemoveMode((EPlayerMode)(0x16));
            processed.SetAsPerformedEvent();
        }
    }
}
