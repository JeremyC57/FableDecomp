
#include "engine/CInputProcessControlCreatureRightStick.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall CInputProcessControlCreatureRightStick_Init(CInputProcessControlCreatureRightStick* process) {
    process->PushingCameraModeUp = 0;
    process->PushingCameraModeDown = 0;
    process->LookingAround = 0;
}