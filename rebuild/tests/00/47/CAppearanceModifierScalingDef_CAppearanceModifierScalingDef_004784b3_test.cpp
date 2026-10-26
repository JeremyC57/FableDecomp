#include <cstdio>

static int g_baseCopies = 0;
class CParentDefClassBase { public:
    virtual void v00() { }
    unsigned char _body[0x38];
    CParentDefClassBase() { }
    CParentDefClassBase(const CParentDefClassBase& other) { ++g_baseCopies; }
};
class CAppearanceModifierScalingDef : public CParentDefClassBase { public:
    long m_scaleX, m_scaleY, m_scaleZ;
    virtual void v00() { }
    CAppearanceModifierScalingDef() : m_scaleX(10), m_scaleY(11), m_scaleZ(12) { }
    CAppearanceModifierScalingDef(const CAppearanceModifierScalingDef& o) : CParentDefClassBase(o), m_scaleX(o.m_scaleX), m_scaleY(o.m_scaleY), m_scaleZ(o.m_scaleZ) { }
};
int main()
{
    CAppearanceModifierScalingDef a; CAppearanceModifierScalingDef b(a);
    if (g_baseCopies != 1 || b.m_scaleX != 10 || b.m_scaleY != 11 || b.m_scaleZ != 12) { std::printf("004784b3_TEST FAIL\n"); return 1; }
    std::printf("004784b3_TEST PASS\n"); return 0;
}