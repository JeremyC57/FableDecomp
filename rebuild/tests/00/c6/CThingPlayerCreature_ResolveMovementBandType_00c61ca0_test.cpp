#include <cstdio>
struct CFrameData { float m_time; float GetTime() const { return m_time; } };
struct COwner { CFrameData m_frameData; };
struct CRandomTable { long m_scale; long Pick(float time) { return (long)(time * (float)m_scale); } };
struct CEmitterDef { CRandomTable m_table; };
struct CEmitterLink2 { CEmitterDef*   m_pDef; };
struct CEmitterLink1 { CEmitterLink2* m_pNext; };
struct CEmitterLink0 { CEmitterLink1* m_pNext; };
struct CInitialiser;
struct CParticleSink {
    virtual void Accept(CInitialiser* initialiser) { m_last = initialiser; }
    CInitialiser* m_last;
};
struct SParticle { CEmitterLink0* m_pLink; CParticleSink m_sink; };
struct CInitialiser {
    COwner* m_pOwner;
    long    m_value;
    void ResolveMovementBandType(SParticle* particle);
};
void CInitialiser::ResolveMovementBandType(SParticle* particle)
{
    m_value = particle->m_pLink->m_pNext->m_pNext->m_pDef->m_table.Pick(m_pOwner->m_frameData.GetTime());
    particle->m_sink.Accept(this);
}
int main()
{
    CEmitterDef def; def.m_table.m_scale = 10;
    CEmitterLink2 l2; l2.m_pDef = &def;
    CEmitterLink1 l1; l1.m_pNext = &l2;
    CEmitterLink0 l0; l0.m_pNext = &l1;
    SParticle p; p.m_pLink = &l0; p.m_sink.m_last = 0;
    COwner o; o.m_frameData.m_time = 2.5f;
    CInitialiser in; in.m_pOwner = &o; in.m_value = 0;
    in.ResolveMovementBandType(&p);
    if (in.m_value != 25 || p.m_sink.m_last != &in) { std::printf("00c61ca0_TEST FAIL\n"); return 1; }
    std::printf("00c61ca0_TEST PASS\n");
    return 0;
}