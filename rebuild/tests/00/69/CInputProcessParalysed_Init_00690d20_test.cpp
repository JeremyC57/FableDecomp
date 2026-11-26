// Behaviour test for 0x00690D20 CInputProcessParalysed::Init (NF478 respell): links the row object, calls the member non-virtually and
// checks that ButtonsMashed (+0x28) is cleared and nothing else is written.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessParalysed.h"
#include <cstring>
#include <stdio.h>
static unsigned char g_storage[0x40];
int main()
{
    std::memset(g_storage, 0xEE, sizeof(g_storage));
    CInputProcessParalysed* p = (CInputProcessParalysed*)g_storage;
    p->CInputProcessParalysed::Init();
    bool ok = p->ButtonsMashed == 0 && g_storage[0x27] == 0xEE && g_storage[0x2c] == 0xEE;
    if (ok) printf("OK_00690d20"); else printf("FAIL_00690d20");
    return ok ? 0 : 1;
}
