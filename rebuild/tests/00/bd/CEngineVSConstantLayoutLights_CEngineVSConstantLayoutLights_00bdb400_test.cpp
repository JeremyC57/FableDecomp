#include <cstdio>
// behaviour test: CEngineVSConstantLayoutLights ctor fills the light/spotlight/user ranges and LightSize
static int g_ctors = 0, g_dtors = 0, g_baseLayout = 0;
class CBaseClass { public:
    CBaseClass() { ++g_ctors; }
    ~CBaseClass() { ++g_dtors; }
};
class CShaderConstantRange : public CBaseClass { public:
    long m_firstRegister, m_registerCount;
    CShaderConstantRange() : m_firstRegister(-1), m_registerCount(-1) {}
    CShaderConstantRange(long f, long c) : m_firstRegister(f), m_registerCount(c) {}
};
namespace N_00bdbb70 { struct CVertexShaderConstantLayout {
    CShaderConstantRange User;
    long LightSize;
    CShaderConstantRange LightArray, LightGlobals, LightAttenuations;
    CShaderConstantRange ShadowedSpotlightFalloffTransform, ShadowedSpotlightAttenuation, ShadowedSpotlightColour;
    CVertexShaderConstantLayout() : LightSize(0) { ++g_baseLayout; }
}; } using N_00bdbb70::CVertexShaderConstantLayout;
struct CEngineVSConstantLayoutLights : CVertexShaderConstantLayout {
    CEngineVSConstantLayoutLights();
};
CEngineVSConstantLayoutLights::CEngineVSConstantLayoutLights()
    : CVertexShaderConstantLayout()
{
    ShadowedSpotlightFalloffTransform = CShaderConstantRange(0xf, 3);
    ShadowedSpotlightAttenuation = CShaderConstantRange(0x24, 1);
    ShadowedSpotlightColour = CShaderConstantRange(0x25, 1);
    LightArray = CShaderConstantRange(0x13, 0xc);
    LightAttenuations = CShaderConstantRange(0x1f, 4);
    LightGlobals = CShaderConstantRange(0x23, 1);
    LightSize = 2;
    User = CShaderConstantRange(0x26, 0x3a);
}
static bool is(const CShaderConstantRange& r, long f, long c) { return r.m_firstRegister == f && r.m_registerCount == c; }
int main()
{
    int before = g_ctors;
    CEngineVSConstantLayoutLights l;
    bool ok = g_baseLayout == 1 && l.LightSize == 2
        && is(l.ShadowedSpotlightFalloffTransform, 0xf, 3) && is(l.ShadowedSpotlightAttenuation, 0x24, 1)
        && is(l.ShadowedSpotlightColour, 0x25, 1) && is(l.LightArray, 0x13, 0xc) && is(l.LightAttenuations, 0x1f, 4)
        && is(l.LightGlobals, 0x23, 1) && is(l.User, 0x26, 0x3a)
        && g_ctors - before == 7 + 7 && g_dtors == 7;   // 7 members + 7 temporaries, temporaries destroyed
    if (!ok) { std::printf("00bdb400_TEST FAIL\n"); return 1; }
    std::printf("00bdb400_TEST PASS\n");
    return 0;
}