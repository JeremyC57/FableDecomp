#include "engine/CInputProcessControlCreatureRightStick.h"
#include <cstring>
#include <cstdio>


void __fastcall CInputProcessControlCreatureRightStick_Init(CInputProcessControlCreatureRightStick* process) {
    process->PushingCameraModeUp = 0;
    process->PushingCameraModeDown = 0;
    process->LookingAround = 0;
}

int main() {
    CInputProcessControlCreatureRightStick obj;
    memset(&obj, 0xFF, sizeof(obj));
    CInputProcessControlCreatureRightStick_Init(&obj);
    bool ok = (obj.PushingCameraModeUp == 0) && (obj.PushingCameraModeDown == 0) && (obj.LookingAround == 0);
    if (ok) {
        printf("CAND_INIT_ZERO_OK\n");
    } else {
        printf("CAND_INIT_ZERO_FAIL\n");
    }
    return 0;
}