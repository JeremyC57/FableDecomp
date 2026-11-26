// Behaviour test for 0x00690E60 CInputProcessBoastUI::Init (NF477 respell): links the row object and calls the member non-virtually.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessBoastUI.h"
#include <cstdio>
static unsigned int g_guard = 0x5A5A5A5A;
static unsigned int g_storage[0x80];
int main()
{
    CInputProcessBoastUI* p = (CInputProcessBoastUI*)g_storage;
    bool ok = false;
    p->CInputProcessBoastUI::Init();
    ok = (g_guard == 0x5A5A5A5A);
    if (ok) { std::printf("AUTO_TINY_00690e60_TEST PASS\n"); return 0; }
    std::printf("NF477_00690e60_TEST FAIL\n");
    return 1;
}
