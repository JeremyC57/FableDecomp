// Behaviour test for 0x00691550 CInputProcessBetting::Init (NF477 respell): links the row object and calls the member non-virtually.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessBetting.h"
#include <cstdio>
static unsigned int g_guard = 0x5A5A5A5A;
static unsigned int g_storage[0x80];
int main()
{
    CInputProcessBetting* p = (CInputProcessBetting*)g_storage;
    bool ok = false;
    p->CInputProcessBetting::Init();
    ok = (g_guard == 0x5A5A5A5A);
    if (ok) { std::printf("AUTO_TINY_00691550_TEST PASS\n"); return 0; }
    std::printf("NF477_00691550_TEST FAIL\n");
    return 1;
}
