// Behaviour test for 0x00838550 CThingNoise::OnCreate(): the override returns the base CThing::OnCreate() result
// (called non-virtually on the same object) as a bool.
#include <stdio.h>
static bool g_baseResult = true;
static long g_baseCalls = 0;
class CThing {
public:
    virtual ~CThing() { }
    virtual bool OnCreate() { ++g_baseCalls; return g_baseResult; }
};
class CThingNoise : public CThing {
public:
    virtual bool OnCreate();
};
bool CThingNoise::OnCreate()
{
    return CThing::OnCreate() ? true : false;
}
int main()
{
    CThingNoise noise;
    CThing* thing = &noise;
    bool ok = thing->OnCreate() == true && g_baseCalls == 1;
    g_baseResult = false;
    ok = ok && thing->OnCreate() == false && g_baseCalls == 2;
    if (!ok) { printf("00838550_TEST FAIL\n"); return 1; }
    printf("00838550_TEST PASS\n");
    return 0;
}
