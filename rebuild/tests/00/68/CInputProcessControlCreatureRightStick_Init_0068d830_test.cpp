// Behaviour test for 0x0068D830 CInputProcessControlCreatureRightStick::Init (NF478 respell): links the row object, calls the member
// non-virtually on a 0xFF-filled object and checks that exactly the three stick flags (+0x28 / +0x29 / +0x2A) are cleared.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessControlCreatureRightStick.h"
#include <cstring>
#include <cstdio>
static unsigned char g_storage[0x40];
int main()
{
    std::memset(g_storage, 0xFF, sizeof(g_storage));
    CInputProcessControlCreatureRightStick* p = (CInputProcessControlCreatureRightStick*)g_storage;
    p->CInputProcessControlCreatureRightStick::Init();
    bool ok = g_storage[0x28] == 0 && g_storage[0x29] == 0 && g_storage[0x2a] == 0 && g_storage[0x27] == 0xFF && g_storage[0x2b] == 0xFF;
    std::printf(ok ? "CAND_INIT_ZERO_OK\n" : "CAND_INIT_ZERO_FAIL\n");
    return ok ? 0 : 1;
}
