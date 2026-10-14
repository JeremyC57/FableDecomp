// CPoolSurfaceOwner_004d959d::ReleaseDefaultPoolSurfaces() -- when the owner at +0xc exists: delete it (its non-virtual destructor, then operator delete), release the surface at +0x10 through its virtual slot 23 with flag 1, and clear both slots
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D959D, length 50 bytes.
// Callees: the owner's destructor @ 0x00A1C950 (direct, non-virtual), operator delete @ 0x00BFE9BC (`pop ecx` cleanup: a size-optimised TU), the surface's
// virtual slot 23 (+0x5c, one argument: 1). Shape: `delete m_pOwner` under the null test needs no test of its own; the zero stores are `and [..],0`
// (`#pragma optimize("s", on)`). The chunk label (CEngineLandscapeMap::ReleaseDefaultPoolSurfaces) is propagated: 0x004D959D sits in the game module and the
// destructor label (~CSteeringAgent) is itself propagated, so the two members are modelled locally.
#pragma optimize("s", on)
struct CSurfaceOwner_00a1c950 {
    ~CSurfaceOwner_00a1c950();                     // 0x00A1C950
};

struct CSurface_004d959d {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void Release(long flags);              // slot 23 (+0x5c)
};

struct CPoolSurfaceOwner_004d959d {
    unsigned char            _pad[0xc];
    CSurfaceOwner_00a1c950*  m_pOwner;             // +0x0c
    CSurface_004d959d*       m_pSurface;           // +0x10
    void ReleaseDefaultPoolSurfaces();
};

void CPoolSurfaceOwner_004d959d::ReleaseDefaultPoolSurfaces()
{
    if (m_pOwner) {
        delete m_pOwner;
        if (m_pSurface)
            m_pSurface->Release(1);
        m_pOwner = 0;
        m_pSurface = 0;
    }
}
