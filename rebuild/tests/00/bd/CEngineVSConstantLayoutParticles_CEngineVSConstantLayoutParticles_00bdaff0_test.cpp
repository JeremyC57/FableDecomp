#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_base = 0, g_baseDtor = 0, g_lights = 0;
struct CBaseClass { virtual void v00() { } CBaseClass() { ++g_base; } ~CBaseClass() { ++g_baseDtor; } };
struct CShaderConstantRange : CBaseClass { long m_firstRegister; long m_registerCount; virtual void v00() { }
    CShaderConstantRange(long f, long c) : CBaseClass(), m_firstRegister(f), m_registerCount(c) { } };
struct CEngineVSConstantLayoutLights { virtual void v00() { } unsigned char _pad0[0xc]; CShaderConstantRange ParticleRange; unsigned char _pad1[0x150 - 0x1c];
    CEngineVSConstantLayoutLights() : ParticleRange(0, 0) { ++g_lights; } };
struct CEngineVSConstantLayoutParticles : CEngineVSConstantLayoutLights { virtual void v00() { }
    CEngineVSConstantLayoutParticles() : CEngineVSConstantLayoutLights() { ParticleRange = CShaderConstantRange(0x26, 0x3a); } };
int main()
{
    CEngineVSConstantLayoutParticles* p = new CEngineVSConstantLayoutParticles();
    bool ok = sizeof(CEngineVSConstantLayoutLights) == 0x150 && g_lights == 1 && g_base == 2 && g_baseDtor == 1 && p->ParticleRange.m_firstRegister == 0x26 && p->ParticleRange.m_registerCount == 0x3a;
    if (!ok) { std::printf("00bdaff0_TEST FAIL\n"); return 1; }
    std::printf("00bdaff0_TEST PASS\n"); return 0;
}