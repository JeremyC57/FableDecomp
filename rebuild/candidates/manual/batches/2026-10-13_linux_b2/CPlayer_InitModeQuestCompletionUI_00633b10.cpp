// CPlayer::InitModeQuestCompletionUI(EPlayerMode, CProcessedInput*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633B10, oracle length 33 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+100h] ; call CAInputProcess::RemoveFromInterface ; mov ecx,[esi+1D0h] ; mov edx,[esi+0Ch] ; mov eax,[ecx] ; push edx ; call [eax+4] ; pop esi ; ret 8
// engine/CPlayer.h (PDB): +0x0C PlayerInterface, +0x100 InputProcessMain_ptr, +0x1D0
// InputProcessQuestCompletionUI_ptr. Entering the quest-completion UI mode detaches the main input
// process (0x00A0D2D0, CAInputProcess::RemoveFromInterface) and attaches the quest-completion
// process to the player's interface through its virtual slot 1 (AddToInterface). Both arguments
// of this mode-init hook are unused.
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
    void InitModeQuestCompletionUI(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeQuestCompletionUI(EPlayerMode mode, CProcessedInput* input)
{
    reinterpret_cast<CAInputProcessCalls*>(this->InputProcessMain_ptr)->RemoveFromInterface();
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessQuestCompletionUI_ptr)->AddToInterface(this->PlayerInterface);
}
