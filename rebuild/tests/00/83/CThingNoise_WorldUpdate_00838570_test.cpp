// Behaviour test for 0x00838570 CThingNoise::WorldUpdate(): runs the base update, then returns false.
#include <cstdio>

static int g_baseCalls = 0;

class Thing {
public:
    virtual ~Thing() { }
    virtual bool WorldUpdate() { ++g_baseCalls; return true; }
};
class ThingNoise : public Thing {
public:
    virtual bool WorldUpdate() { Thing::WorldUpdate(); return false; }
};

int main()
{
    ThingNoise n;
    Thing& t = n;
    if (t.WorldUpdate() != false || g_baseCalls != 1) { std::printf("00838570_TEST FAIL\n"); return 1; }
    if (t.WorldUpdate() != false || g_baseCalls != 2) { std::printf("00838570_TEST FAIL\n"); return 1; }
    std::printf("00838570_TEST PASS\n");
    return 0;
}