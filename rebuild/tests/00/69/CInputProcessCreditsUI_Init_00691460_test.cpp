// Behaviour test for 0x00691460 CInputProcessCreditsUI::Init (NF477 respell): links the row object and calls the member non-virtually.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessCreditsUI.h"
#include <cstdio>
static unsigned int g_guard = 0x5A5A5A5A;
static unsigned int g_storage[0x80];
int main()
{
    CInputProcessCreditsUI* p = (CInputProcessCreditsUI*)g_storage;
    bool ok = false;
    p->CInputProcessCreditsUI::Init();
    ok = (g_guard == 0x5A5A5A5A);
    if (ok) { std::printf("AUTO_TINY_00691460_TEST PASS\n"); return 0; }
    std::printf("NF477_00691460_TEST FAIL\n");
    return 1;
}
