// Behaviour test for CEngineLightingManager::CreateInternalPrimitive @ 0x00B44EB0.
#include <cstdio>
class CEngineInternalPrimitiveBase { public:
    long m_type; unsigned int m_flags;
    CEngineInternalPrimitiveBase(long type, unsigned int flags) : m_type(type), m_flags(flags) { }
    virtual int Kind() { return 0; }
    virtual ~CEngineInternalPrimitiveBase() { }
};
class CEngineInternalPrimitiveLight : public CEngineInternalPrimitiveBase { public:
    CEngineInternalPrimitiveLight() : CEngineInternalPrimitiveBase(15, 1) { }
    virtual int Kind() { return 1; }
};
class CEngineInternalPrimitiveShadowedSpotLight : public CEngineInternalPrimitiveBase { public:
    CEngineInternalPrimitiveShadowedSpotLight() : CEngineInternalPrimitiveBase(16, 1) { }
    virtual int Kind() { return 2; }
};
class CEngineLightingManager { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type)
    {
        if (type == 15) return new CEngineInternalPrimitiveLight;
        if (type == 16) return new CEngineInternalPrimitiveShadowedSpotLight;
        return 0;
    }
};
int main()
{
    CEngineLightingManager m;
    CEngineInternalPrimitiveBase* a = m.CreateInternalPrimitive(15);
    CEngineInternalPrimitiveBase* b = m.CreateInternalPrimitive(16);
    int ok = a && a->Kind() == 1 && a->m_type == 15 && a->m_flags == 1 && b && b->Kind() == 2 && b->m_type == 16 && b->m_flags == 1;
    ok = ok && m.CreateInternalPrimitive(0) == 0 && m.CreateInternalPrimitive(14) == 0 && m.CreateInternalPrimitive(17) == 0;
    delete a; delete b;
    if (!ok) { std::printf("00b44eb0_TEST FAIL\n"); return 1; }
    std::printf("00b44eb0_TEST PASS\n");
    return 0;
}