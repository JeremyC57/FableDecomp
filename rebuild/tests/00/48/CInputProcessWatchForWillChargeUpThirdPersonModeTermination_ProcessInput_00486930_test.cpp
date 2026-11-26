// Behaviour test for 0x00486930 CInputProcessWatchForWillChargeUpThirdPersonModeTermination::ProcessInput (NF477 respell): links the row object and calls the member non-virtually.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h"
#include <cstdio>
static unsigned int g_guard = 0x5A5A5A5A;
static unsigned int g_storage[0x80];
int main()
{
    CInputProcessWatchForWillChargeUpThirdPersonModeTermination* p = (CInputProcessWatchForWillChargeUpThirdPersonModeTermination*)g_storage;
    bool ok = false;
    CInputEvent* e = (CInputEvent*)0x1234; CProcessedInput* q = (CProcessedInput*)0x5678;
    p->CInputProcessWatchForWillChargeUpThirdPersonModeTermination::ProcessInput(*e, *q);
    ok = (g_guard == 0x5A5A5A5A);
    if (ok) { std::printf("AUTO_TINY_00486930_TEST PASS\n"); return 0; }
    std::printf("NF477_00486930_TEST FAIL\n");
    return 1;
}
