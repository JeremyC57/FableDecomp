#include <cstdio>
#include <cstddef>
// Behaviour model of CAIStateGroup_FlyingThingBase::CAIStateGroup_FlyingThingBase @ 0x009126C0.
static int g_n = 0;
struct CBase { virtual void v00() {} CBase() { g_n++; } };
struct CCharString { void* m_pData; CCharString() : m_pData(0) { g_n++; } };
struct CAIStateGroupBase : CBase { void* PBrain; long ScriptAIPriority; CCharString Name; unsigned char Terminated, Priority, Enabled, _pad;
    virtual void v00() {} CAIStateGroupBase() : CBase(), Name() {} };
struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
    float m_heightAboveGround, m_speed, m_minHeight, m_maxHeight;
    virtual void v00() {}
    CAIStateGroup_FlyingThingBase() : CAIStateGroupBase(), m_heightAboveGround(0.0f), m_speed(2.0f), m_minHeight(10.0f), m_maxHeight(10.0f) {}
};
int main()
{
    CAIStateGroup_FlyingThingBase g;
    bool ok = g_n == 2 && g.m_heightAboveGround == 0.0f && g.m_speed == 2.0f && g.m_minHeight == 10.0f && g.m_maxHeight == 10.0f && g.Name.m_pData == 0;
    if (sizeof(void*) == 4) ok = ok && offsetof(CAIStateGroup_FlyingThingBase, m_heightAboveGround) == 0x14 && offsetof(CAIStateGroup_FlyingThingBase, m_maxHeight) == 0x20;
    if (!ok) { std::printf("009126c0_TEST FAIL\n"); return 1; }
    std::printf("009126c0_TEST PASS\n");
    return 0;
}