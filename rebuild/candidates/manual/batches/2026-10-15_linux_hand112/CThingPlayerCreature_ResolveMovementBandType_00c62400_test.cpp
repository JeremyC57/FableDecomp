#include <cstdio>
struct CFrameData { float m_time; float GetTime() const { return m_time; } };
struct COwner { CFrameData m_frameData; };
struct CRandomTable { long m_scale; long Pick(float time) { return (long)(time * (float)m_scale); } };
struct CEmitterDef { CRandomTable m_table; };
struct CEmitterSlot  { CEmitterDef* m_pDef; };
struct CEmitterLink  { void* m_p00; CEmitterSlot* m_pSlot; };
struct CInitialiser;
struct CParticleSink {
    virtual void Accept(CInitialiser* initialiser) { m_last = initialiser; }
    CInitialiser* m_last;
};
struct SParticle { CEmitterLink* m_pLink; CParticleSink m_sink; };
struct CInitialiser {
    COwner* m_pOwner;
    long    m_value;
    CEmitterDef* GetEmitterDef(SParticle* particle);
    void ResolveMovementBandType(SParticle* particle);
};
CEmitterDef* CInitialiser::GetEmitterDef(SParticle* particle)
{
    return particle->m_pLink->m_pSlot->m_pDef;
}
void CInitialiser::ResolveMovementBandType(SParticle* particle)
{
    m_value = GetEmitterDef(particle)->m_table.Pick(m_pOwner->m_frameData.GetTime());
    particle->m_sink.Accept(this);
}
int main()
{
    CEmitterDef def; def.m_table.m_scale = 4;
    CEmitterSlot slot; slot.m_pDef = &def;
    CEmitterLink link; link.m_p00 = 0; link.m_pSlot = &slot;
    SParticle p; p.m_pLink = &link; p.m_sink.m_last = 0;
    COwner o; o.m_frameData.m_time = 3.0f;
    CInitialiser in; in.m_pOwner = &o; in.m_value = 0;
    in.ResolveMovementBandType(&p);
    if (in.m_value != 12 || p.m_sink.m_last != &in) { std::printf("00c62400_TEST FAIL\n"); return 1; }
    std::printf("00c62400_TEST PASS\n");
    return 0;
}
