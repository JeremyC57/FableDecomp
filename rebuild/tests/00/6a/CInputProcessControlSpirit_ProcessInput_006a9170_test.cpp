// Behaviour test for 0x006A9170 CInputProcessControlSpirit::ProcessInput (NF478 respell): links the row object, calls the member
// non-virtually and checks that neither the process nor the event / processed-input records change (the retail body is `ret 8`).
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessControlSpirit.h"
#include <cstring>
#include <cstdio>
static unsigned char g_process[0x40], g_event[0x40], g_input[0x100], g_copy[0x180];
int main()
{
    for (int i = 0; i < 0x40; ++i) { g_process[i] = (unsigned char)i; g_event[i] = (unsigned char)(0x80 + i); }
    std::memset(g_input, 0x5A, sizeof(g_input));
    std::memcpy(g_copy, g_process, 0x40); std::memcpy(g_copy + 0x40, g_event, 0x40); std::memcpy(g_copy + 0x80, g_input, 0x100);
    CInputProcessControlSpirit* p = (CInputProcessControlSpirit*)g_process;
    p->CInputProcessControlSpirit::ProcessInput(*(const CInputEvent*)g_event, *(CProcessedInput*)g_input);
    bool ok = !std::memcmp(g_copy, g_process, 0x40) && !std::memcmp(g_copy + 0x40, g_event, 0x40) && !std::memcmp(g_copy + 0x80, g_input, 0x100);
    std::printf(ok ? "AUTO_TINY_006a9170_TEST PASS\n" : "NF478_006a9170_TEST FAIL\n");
    return ok ? 0 : 1;
}
