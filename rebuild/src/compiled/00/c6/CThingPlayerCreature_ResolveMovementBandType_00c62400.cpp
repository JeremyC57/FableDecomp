// CThingPlayerCreature::ResolveMovementBandType(SParticle*) -- picks this initialiser's value from the particle's emitter table at the owner's current time (def found through an out-of-line helper), then hands itself to the particle's slot-9 sink
// Retail: Fable.exe 0x00C62400, 57 bytes. The class/method label is a propagated manifest name (twin of
// 0x00C61CA0 in the particle-component code; no creature member is used); the model is descriptive.
// Callees: 0x00C1BB00 (frame-data time on the owner's member at +8), 0x00C623F0 (this TU's helper,
// compiled just above: returns particle->[0]->[4]->[0], `this` unused; manifest label is a propagated
// list-iterator operator--), 0x00C2D100 (the float-argument picker on the def's table at +0x70),
// slot 9 (+0x24) of the particle's embedded sink at +8.
// Shape: the helper is defined above the caller and kept out of line (`#pragma auto_inline(off)`); the
// float argument is pushed before the helper call (right-to-left evaluation), the result stored to +4.
struct CFrameData {
    float GetTime() const;                               // 0x00C1BB00
};

struct COwner {
    unsigned char _pad00[0x8];
    CFrameData    m_frameData;                           // +0x08
};

struct CRandomTable {
    long Pick(float time);                               // 0x00C2D100
};

struct CEmitterDef {
    unsigned char _pad00[0x70];
    CRandomTable  m_table;                               // +0x70
};

struct CEmitterSlot  { CEmitterDef* m_pDef; };
struct CEmitterLink  { void* m_p00; CEmitterSlot* m_pSlot; };   // +0x04

struct CInitialiser;

struct CParticleSink {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008();
    virtual void Accept(CInitialiser* initialiser);      // slot 9 (+0x24)
};

struct SParticle {
    CEmitterLink*  m_pLink;                              // +0x00
    unsigned char  _pad04[0x4];
    CParticleSink  m_sink;                               // +0x08
};

struct CInitialiser {
    COwner* m_pOwner;                                    // +0x00
    long    m_value;                                     // +0x04
    CEmitterDef* GetEmitterDef(SParticle* particle);     // 0x00C623F0
    void ResolveMovementBandType(SParticle* particle);
};

#pragma auto_inline(off)
CEmitterDef* CInitialiser::GetEmitterDef(SParticle* particle)
{
    return particle->m_pLink->m_pSlot->m_pDef;
}
#pragma auto_inline(on)

void CInitialiser::ResolveMovementBandType(SParticle* particle)
{
    m_value = GetEmitterDef(particle)->m_table.Pick(m_pOwner->m_frameData.GetTime());
    particle->m_sink.Accept(this);
}