#include <cstdio>
// Behaviour test for 0x009E45D0 (links the row object): CMaintainedEvent's destructor body destroys Event (+4, ~CInputEvent) first,
//   then runs ~CBaseClass on the object.
// NF142 (2026-11-20, q4299): rewritten on engine/CMaintainedEvent.h, the header the row now uses (the old test was a standalone copy
//   over the Member / Host stand-ins); ~CInputEvent and the CBaseClass members are the test's fixtures. PassPattern unchanged.
#include "engine/CMaintainedEvent.h"

static int g_order[4];
static int g_n = 0;
static const void* g_this[4];

CInputEvent::~CInputEvent() { g_this[g_n] = this; g_order[g_n++] = 1; }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { g_this[g_n] = this; g_order[g_n++] = 2; }

extern "C" void FABLE_FASTCALL FableDestVal_009e45d0(CMaintainedEvent* self);

int main()
{
    static double storage[(sizeof(CMaintainedEvent) + 7) / 8];
    char* raw = (char*)storage;
    FableDestVal_009e45d0((CMaintainedEvent*)raw);
    if (g_n == 2 && g_order[0] == 1 && g_order[1] == 2 && g_this[0] == raw + 4 && g_this[1] == raw) {
        std::printf("DESTVAL_009e45d0_OK\n");
        return 0;
    }
    std::printf("DESTVAL_009e45d0_FAIL\n");
    return 1;
}
