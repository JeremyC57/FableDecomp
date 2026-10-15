// Behaviour test for 0x00C61C70 CThingFilter_SolidToThing::CThingFilter_SolidToThing
#include <cstdio>
struct CThing { int x; };
struct CTCSolidToThing;
struct CThingFilter_SolidToThing {
    CThing*          SourceThing;
    CTCSolidToThing* PTCSolidToThing;
    bool             m_flag;
    long             m_index;
    CThingFilter_SolidToThing(CThing* sourceThing, bool flag)
        : SourceThing(sourceThing), PTCSolidToThing(0), m_flag(flag), m_index(-1) {}
};
int main()
{
    CThing t;
    CThingFilter_SolidToThing a(&t, true), b(0, false);
    bool ok = a.SourceThing == &t && a.PTCSolidToThing == 0 && a.m_flag && a.m_index == -1
           && b.SourceThing == 0 && !b.m_flag && b.m_index == -1;
    if (!ok) { std::printf("00c61c70_TEST FAIL\n"); return 1; }
    std::printf("00c61c70_TEST PASS\n");
    return 0;
}