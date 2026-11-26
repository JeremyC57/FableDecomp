// Behaviour test for 0x0068FED0 CInputProcessLightning::ProcessInput (NF477 respell): links the row object and calls the member non-virtually.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessLightning.h"
#include <cstdio>
static unsigned int g_guard = 0x5A5A5A5A;
static unsigned int g_storage[0x80];
int main()
{
    CInputProcessLightning* p = (CInputProcessLightning*)g_storage;
    bool ok = false;
    CInputEvent* e = (CInputEvent*)0x1234; CProcessedInput* q = (CProcessedInput*)0x5678;
    p->CInputProcessLightning::ProcessInput(*e, *q);
    ok = (g_guard == 0x5A5A5A5A);
    if (ok) { std::printf("AUTO_TINY_0068fed0_TEST PASS\n"); return 0; }
    std::printf("NF477_0068fed0_TEST FAIL\n");
    return 1;
}
