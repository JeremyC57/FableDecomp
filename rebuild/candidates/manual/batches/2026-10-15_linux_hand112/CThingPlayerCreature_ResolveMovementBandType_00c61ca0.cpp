// CThingPlayerCreature::ResolveMovementBandType(SParticle*) -- picks this initialiser's value from the emitter's random table at the owner's current time, then hands itself to the particle's slot-9 sink
// Retail: Fable.exe 0x00C61CA0, 55 bytes. The class/method label is a propagated manifest name: the body
// sits in the 0x00C6xxxx particle-component code and uses no creature member (see NOTES label corrections);
// the model below is descriptive.
// Callees: 0x00C1BB00 (frame-data time: word +0x0e times a float scale; labelled CGraphicFrameData::
// GetFloatTLX) on the owner's member at +8, 0x00C2D100 (a float-argument picker returning a word, rand-
// based; labelled CTCLook::UpdateLookTimer) on the table at +0x70 of the particle's three-link owner
// chain, slot 9 (+0x24) of the particle's embedded sink at +0x20.
// Shape: the time is computed first (x87 result stored into the pushed argument slot), the owner chain
// walked after it (right-to-left argument evaluation), the result stored to +4 after the call.
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

struct CEmitterLink2 { CEmitterDef*   m_pDef; };
struct CEmitterLink1 { CEmitterLink2* m_pNext; };
struct CEmitterLink0 { CEmitterLink1* m_pNext; };

struct CInitialiser;

struct CParticleSink {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008();
    virtual void Accept(CInitialiser* initialiser);      // slot 9 (+0x24)
};

struct SParticle {
    CEmitterLink0* m_pLink;                              // +0x00
    unsigned char  _pad04[0x1c];
    CParticleSink  m_sink;                               // +0x20
};

struct CInitialiser {
    COwner* m_pOwner;                                    // +0x00
    long    m_value;                                     // +0x04
    void ResolveMovementBandType(SParticle* particle);
};

void CInitialiser::ResolveMovementBandType(SParticle* particle)
{
    m_value = particle->m_pLink->m_pNext->m_pNext->m_pDef->m_table.Pick(m_pOwner->m_frameData.GetTime());
    particle->m_sink.Accept(this);
}
