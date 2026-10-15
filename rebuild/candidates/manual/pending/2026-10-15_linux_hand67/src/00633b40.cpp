// CPlayer::UninitModeQuestCompletionUI(EPlayerMode mode, CProcessedInput* input) -- leaving the quest-completion UI mode re-attaches the main input process to the player's interface and detaches the quest-completion process
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633B40, length 33 bytes.
// Identity: Ego_r.pdb `?UninitModeQuestCompletionUI@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z`.
// Callees: the main process's virtual AddToInterface(CGamePlayerInterface*) (slot 1, `call [eax+4]`),
// CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 (non-virtual).
// Shape: the mirror of the landed CPlayer::InitModeQuestCompletionUI (0x00633B10): engine/CPlayer.h
// members PlayerInterface +0x0C, InputProcessMain_ptr +0x100, InputProcessQuestCompletionUI_ptr
// +0x1D0; both hook arguments unused (`ret 8`).
#include "engine/CPlayer.h"

typedef long EPlayerMode;
struct CProcessedInput;

struct CAInputProcessCalls {
    void RemoveFromInterface();                       // 0x00A0D2D0 (non-virtual)
};

struct CInputProcessVirtuals {
    virtual void v00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

struct CPlayer_Methods : CPlayer {
    void UninitModeQuestCompletionUI(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::UninitModeQuestCompletionUI(EPlayerMode mode, CProcessedInput* input)
{
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessMain_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CAInputProcessCalls*>(this->InputProcessQuestCompletionUI_ptr)->RemoveFromInterface();
}
