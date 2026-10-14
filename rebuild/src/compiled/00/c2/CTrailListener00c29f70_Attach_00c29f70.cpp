// CTrailListener_00c29f70::Attach(CHost* host, void* owner, long param) -- remember the owner at +4, register with the host's observable at +0x104 (virtual slot 9 with `this`), then update (0x00C29EC0 with the parameter)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29F70, length 48 bytes.
// Callees: virtual slot 9 (+0x24) of the object at host+0x104 (this listener by address), the update @ 0x00C29EC0 (on `this`; labelled "CEngineInternalPrimitiveParticleSpriteTrailGroup::Update",
// propagated). Shape: the owner stored before the virtual call, `this` in esi, `ret 0Ch`. The chunk label "CGameScriptInterface::GetWaterHeightAtPosition" is propagated; named by address.
struct CTrailListener_00c29f70;
struct CObservable_00c29f70 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008();
    virtual void AddObserver(CTrailListener_00c29f70* observer);   // slot 9 (+0x24)
};
struct CHost_00c29f70 {
    unsigned char        _pad[0x104];
    CObservable_00c29f70 m_observable;             // +0x104
};

struct CTrailListener_00c29f70 {
    long  m_0;
    void* m_pOwner;                                // +0x04
    void Update(long param);                       // 0x00C29EC0
    void Attach(CHost_00c29f70* host, void* owner, long param);
};

void CTrailListener_00c29f70::Attach(CHost_00c29f70* host, void* owner, long param)
{
    m_pOwner = owner;
    host->m_observable.AddObserver(this);
    Update(param);
}