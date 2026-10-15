// Behaviour test for 0x00BDB260 CEngineVSConstantLayoutLightsBones ctor: bone range (0x26, 0x36), particle range (0x5c, 4).
#include <cstdio>

static int g_live = 0;
struct CBaseClass { CBaseClass() { ++g_live; } CBaseClass(const CBaseClass&) { ++g_live; } ~CBaseClass() { --g_live; } virtual void v00() {} };
struct CShaderConstantRange : CBaseClass {
    long m_firstRegister, m_registerCount;
    CShaderConstantRange() : m_firstRegister(0), m_registerCount(0) {}
    CShaderConstantRange(long f, long c) : CBaseClass(), m_firstRegister(f), m_registerCount(c) {}
};
struct CEngineVSConstantLayoutLights {
    CShaderConstantRange ParticleRange;
    CShaderConstantRange BoneRange;
    virtual ~CEngineVSConstantLayoutLights() {}
};
struct CEngineVSConstantLayoutLightsBones : CEngineVSConstantLayoutLights {
    CEngineVSConstantLayoutLightsBones()
    {
        BoneRange = CShaderConstantRange(0x26, 0x36);
        ParticleRange = CShaderConstantRange(0x5c, 4);
    }
};

int main()
{
    {
        CEngineVSConstantLayoutLightsBones l;
        if (l.BoneRange.m_firstRegister != 0x26 || l.BoneRange.m_registerCount != 0x36
            || l.ParticleRange.m_firstRegister != 0x5c || l.ParticleRange.m_registerCount != 4 || g_live != 2) {
            std::printf("00bdb260_TEST FAIL\n"); return 1;
        }
    }
    if (g_live != 0) { std::printf("00bdb260_TEST FAIL\n"); return 1; }
    std::printf("00bdb260_TEST PASS\n");
    return 0;
}