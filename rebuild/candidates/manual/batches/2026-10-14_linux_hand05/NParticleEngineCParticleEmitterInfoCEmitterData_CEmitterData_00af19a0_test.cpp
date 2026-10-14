#include <cstdio>
struct CResource { virtual void v00() {} unsigned char _body[0x18]; CResource() {} };
struct SEmitterBuffers { void* m_pEmitters; void* m_pParticles; void* m_pTextures; void* m_pOwner; SEmitterBuffers() : m_pEmitters(0), m_pParticles(0), m_pTextures(0), m_pOwner(0) { } };
namespace NParticleEngine { struct CParticleEmitterInfo { struct CEmitterData : CResource { SEmitterBuffers m_buffers; unsigned long m_size; virtual void v00() {} CEmitterData(unsigned long size); }; }; }
NParticleEngine::CParticleEmitterInfo::CEmitterData::CEmitterData(unsigned long size) : CResource(), m_buffers() { m_size = size; }
int main()
{
    NParticleEngine::CParticleEmitterInfo::CEmitterData d(77);
    if (d.m_buffers.m_pEmitters != 0 || d.m_buffers.m_pParticles != 0 || d.m_buffers.m_pTextures != 0 || d.m_buffers.m_pOwner != 0 || d.m_size != 77) { std::printf("00af19a0_TEST FAIL\n"); return 1; }
    std::printf("00af19a0_TEST PASS\n");
    return 0;
}
