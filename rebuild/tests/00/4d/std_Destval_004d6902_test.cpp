#include "rebuild_abi.h"
#include <cstdio>
// Spy for CCharString::~CCharString (0x0099EAE0): records the order of the destroyed member addresses.
struct CCharString
{
    void* PStringData;
    ~CCharString();
};
static int g_calls = 0;
static void* g_seen[2];
CCharString::~CCharString() { if (g_calls < 2) g_seen[g_calls] = this; g_calls++; }
struct FableDestVal_004d6902_t;
extern "C" void FABLE_FASTCALL FableDestVal_004d6902(FableDestVal_004d6902_t* self);
int main()
{
    static char buf[0xc + 8];
    FableDestVal_004d6902((FableDestVal_004d6902_t*)buf);
    if (g_calls != 2 || g_seen[0] != buf + 0xc || g_seen[1] != buf) { std::printf("BAD\n"); return 1; }
    std::printf("DESTVAL_004d6902_OK\n");
    return 0;
}
