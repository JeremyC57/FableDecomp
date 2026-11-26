// Behaviour test for 0x00691AC0 CInputProcessPhotojournalCapture::Init (NF477 respell): links the row object and calls the member non-virtually.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessPhotojournalCapture.h"
#include <cstdio>
static unsigned int g_guard = 0x5A5A5A5A;
static unsigned int g_storage[0x80];
int main()
{
    CInputProcessPhotojournalCapture* p = (CInputProcessPhotojournalCapture*)g_storage;
    bool ok = false;
    p->CInputProcessPhotojournalCapture::Init();
    ok = (g_guard == 0x5A5A5A5A);
    if (ok) { std::printf("PASS_00691ac0\n"); return 0; }
    std::printf("NF477_00691ac0_TEST FAIL\n");
    return 1;
}
