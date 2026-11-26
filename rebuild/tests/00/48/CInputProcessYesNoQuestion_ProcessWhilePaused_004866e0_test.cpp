// Behaviour test for 0x004866E0 CInputProcessYesNoQuestion::ProcessWhilePaused (NF477 respell): links the row object and calls the member non-virtually.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessYesNoQuestion.h"
#include <cstdio>
static unsigned int g_guard = 0x5A5A5A5A;
static unsigned int g_storage[0x80];
int main()
{
    CInputProcessYesNoQuestion* p = (CInputProcessYesNoQuestion*)g_storage;
    bool ok = false;
    bool r = p->CInputProcessYesNoQuestion::ProcessWhilePaused();
    ok = (r == true);
    if (ok) { std::printf("AUTO_TINY_004866e0_TEST PASS\n"); return 0; }
    std::printf("NF477_004866e0_TEST FAIL\n");
    return 1;
}
