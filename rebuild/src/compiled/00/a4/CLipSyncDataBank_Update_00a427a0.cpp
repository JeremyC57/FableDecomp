// CLipSyncDataBank_Methods::Update @ 0x00a427a0
// mov al,[ecx+0x178]; test al,al; jne ret; jmp 0x9cec60

// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `m_bSuspended` -> `SkipUpdate`.
#include "engine/CLipSyncDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CLipSyncDataBank_Methods : CLipSyncDataBank {
    // +0x178
    void DoUpdate();
    // 0x9cec60, __fastcall
    void Update();
};

void CLipSyncDataBank_Methods::Update()
{
    if (this->SkipUpdate == 0)
        this->DoUpdate();
}