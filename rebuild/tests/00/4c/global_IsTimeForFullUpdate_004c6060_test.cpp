#include <cstdio>

static int g_div = 3;
static int g_num = 9;

// 0x004C5FA0 returns a float; the caller converts it with __ftol2 (0x00BFEA70) into the divisor.
extern "C" float __fastcall sub_4c5fa0(void*) { return (float)g_div; }
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
long __fastcall CWorld::GetFrame() { return g_num; }

class CBulletTimeManager;
extern "C" bool __fastcall IsTimeForFullUpdate(CBulletTimeManager* self);

int main()
{
    g_div = 3; g_num = 9;
    bool a = IsTimeForFullUpdate((CBulletTimeManager*)0);   // 9%3==0 -> true
    g_div = 4; g_num = 9;
    bool b = IsTimeForFullUpdate((CBulletTimeManager*)0);   // 9%4==1 -> false
    if (a && !b) printf("PARITY_OK\n");
    else printf("PARITY_FAIL a=%d b=%d\n", (int)a, (int)b);
    return 0;
}