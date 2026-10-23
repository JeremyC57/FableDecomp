#include <cstdio>
// behaviour test: CEngineVSConstantLayoutRepeatedMesh ctor zero-builds four ranges then assigns them and the user range
static int g_ctors = 0, g_dtors = 0;
struct CBaseClass {
    CBaseClass() { ++g_ctors; }
    ~CBaseClass() { ++g_dtors; }
};
struct CShaderConstantRange : CBaseClass {
    long m_firstRegister, m_registerCount;
    CShaderConstantRange() : m_firstRegister(0), m_registerCount(0) {}
    CShaderConstantRange(long f, long c) : m_firstRegister(f), m_registerCount(c) {}
};
namespace N_00bdbb70 { struct CVertexShaderConstantLayout {
    CShaderConstantRange User;
    CVertexShaderConstantLayout() {}
}; } using N_00bdbb70::CVertexShaderConstantLayout;
struct CEngineVSConstantLayoutRepeatedMesh : CVertexShaderConstantLayout {
    CShaderConstantRange LightingResults, MainLightLightingResults, ObjectMatricies, ObjectOffsets;
    CEngineVSConstantLayoutRepeatedMesh();
};
CEngineVSConstantLayoutRepeatedMesh::CEngineVSConstantLayoutRepeatedMesh()
    : CVertexShaderConstantLayout()
{
    LightingResults = CShaderConstantRange(0x33, 0x10);
    MainLightLightingResults = CShaderConstantRange(0x43, 0x10);
    ObjectMatricies = CShaderConstantRange(0x13, 0x10);
    ObjectOffsets = CShaderConstantRange(0x23, 0x10);
    User = CShaderConstantRange(0x53, 0xd);
}
static bool is(const CShaderConstantRange& r, long f, long c) { return r.m_firstRegister == f && r.m_registerCount == c; }
int main()
{
    CEngineVSConstantLayoutRepeatedMesh m;
    bool ok = is(m.LightingResults, 0x33, 0x10) && is(m.MainLightLightingResults, 0x43, 0x10)
        && is(m.ObjectMatricies, 0x13, 0x10) && is(m.ObjectOffsets, 0x23, 0x10) && is(m.User, 0x53, 0xd)
        && g_ctors == 5 + 5 && g_dtors == 5;
    if (!ok) { std::printf("00bdb080_TEST FAIL\n"); return 1; }
    std::printf("00bdb080_TEST PASS\n");
    return 0;
}