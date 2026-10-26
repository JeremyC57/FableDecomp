// Behaviour test for CEnginePrimitiveManager2DSprites::CreateInternalPrimitive @ 0x00BACFD0.
#include <cstdio>
class CEngineInternalPrimitiveBase { public:
    long m_type; unsigned int m_flags;
    CEngineInternalPrimitiveBase(long t, unsigned int f) : m_type(t), m_flags(f) { }
    virtual int Kind() { return 0; }
    virtual ~CEngineInternalPrimitiveBase() { }
};
class CEngineInternalPrimitive2DSprite : public CEngineInternalPrimitiveBase { public:
    CEngineInternalPrimitive2DSprite() : CEngineInternalPrimitiveBase(0x22, 0x80000000u) { }
    virtual int Kind() { return 1; }
};
class CEngineInternalPrimitive2DClockSprite : public CEngineInternalPrimitiveBase { public:
    CEngineInternalPrimitive2DClockSprite() : CEngineInternalPrimitiveBase(0x23, 0x80000000u) { }
    virtual int Kind() { return 2; }
};
class CEnginePrimitiveManager2DSprites { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type)
    {
        switch (type) {
        case 0x22: return new CEngineInternalPrimitive2DSprite;
        case 0x23: return new CEngineInternalPrimitive2DClockSprite;
        }
        return 0;
    }
};
int main()
{
    CEnginePrimitiveManager2DSprites m;
    CEngineInternalPrimitiveBase* a = m.CreateInternalPrimitive(0x22);
    CEngineInternalPrimitiveBase* b = m.CreateInternalPrimitive(0x23);
    int ok = a && a->Kind() == 1 && a->m_type == 0x22 && a->m_flags == 0x80000000u && b && b->Kind() == 2 && b->m_type == 0x23
          && m.CreateInternalPrimitive(0x21) == 0 && m.CreateInternalPrimitive(0x24) == 0 && m.CreateInternalPrimitive(0) == 0;
    delete a; delete b;
    if (!ok) { std::printf("00bacfd0_TEST FAIL\n"); return 1; }
    std::printf("00bacfd0_TEST PASS\n");
    return 0;
}