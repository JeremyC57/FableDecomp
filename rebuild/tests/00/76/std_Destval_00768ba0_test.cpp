#include "rebuild_abi.h"
#include <cstdio>
// Spy for CCharString::~CCharString (0x0099EAE0): records the order of the destroyed member addresses.
class CCharString
{ public:
    void* PStringData;
    ~CCharString();
};
static int g_calls = 0;
static void* g_seen[2];
CCharString::~CCharString() { if (g_calls < 2) g_seen[g_calls] = this; g_calls++; }
struct FableDestVal_00768ba0_t;
extern "C" void FABLE_FASTCALL FableDestVal_00768ba0(FableDestVal_00768ba0_t* self);
int main()
{
    static char buf[0x4 + 8];
    FableDestVal_00768ba0((FableDestVal_00768ba0_t*)buf);
    if (g_calls != 2 || g_seen[0] != buf + 0x4 || g_seen[1] != buf) { std::printf("BAD\n"); return 1; }
    std::printf("DESTVAL_00768ba0_OK\n");
    return 0;
}
