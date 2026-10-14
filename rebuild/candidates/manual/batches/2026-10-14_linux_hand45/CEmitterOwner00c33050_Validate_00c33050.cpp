// CEmitterOwner_00c33050::Validate() -- tell the object at +4 (virtual slot 3), then hand `this` to the emitter at +8 (0x00C1B930)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C33050, length 32 bytes.
// Callees: virtual slot 3 (+0xc) of the object at +4, the emitter's frame update @ 0x00C1B930 (on +8, `this`; labelled "CTCDParticleEmitter::EditFrameUpdate", propagated).
// Shape: `this` in esi. The chunk label "CGameScriptInterface::Validate" is propagated; named by address.
struct CEmitterOwner_00c33050;
struct CValidatable_00c33050 {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Validate();                       // slot 3 (+0x0c)
};
struct CEmitter_00c33050 {
    void EditFrameUpdate(CEmitterOwner_00c33050* owner);   // 0x00C1B930
};

struct CEmitterOwner_00c33050 {
    long                   m_0;
    CValidatable_00c33050* m_pTarget;              // +0x04
    CEmitter_00c33050*     m_pEmitter;             // +0x08
    void Validate();
};

void CEmitterOwner_00c33050::Validate()
{
    m_pTarget->Validate();
    m_pEmitter->EditFrameUpdate(this);
}
