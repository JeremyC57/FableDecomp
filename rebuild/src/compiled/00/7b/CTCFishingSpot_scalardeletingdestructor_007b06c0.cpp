// CTCFishingSpot::`scalar deleting destructor'(unsigned int) -- stores the CTCFishingSpot vtable, removes the spot's particles, destroys the three intelligent particle pointers (+0x1c, +0x14, +0x0c), runs CTCBase::~CTCBase and frees on bit 0
// Leaf: ??_GCTCFishingSpot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B06C0, length 67 bytes (vtable 0x0126D51C slot 0).
// Callees: CTCFishingSpot::RemoveParticles @ 0x007B01C0, CBaseIntelligentPointer::~CBaseIntelligentPointer
// @ 0x00A01C10 (three times), CTCBase::~CTCBase @ 0x00686830 (manifest "~CChunkedFileChunk", propagated),
// operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CTCFishingSpot vtable 0x0126D51C.
// Layout: PDB CTCFishingSpot (40 bytes): PParticle1 +0x0c, PParticle2 +0x14, PParticleFish +0x1c
// (CIntelligentPointer<CThing>, 8 bytes each: vptr + item), Enabled +0x24.
// Shape: own vtable store first, the user destructor body (RemoveParticles) before the member
// destructors, which run in reverse declaration order; a `new` factory pulls the `??_G` into the TU.
class CThing;

class CTCBase { public:                                   // engine/CTCBase.h offsets, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing& owner);                        // 0x00686800
    virtual ~CTCBase();                                    // 0x00686830
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class CTCFishingSpot : public CTCBase { public:                  // vtable 0x0126D51C
    CBaseIntelligentPointer PParticle1;                // +0x0c
    CBaseIntelligentPointer PParticle2;                // +0x14
    CBaseIntelligentPointer PParticleFish;             // +0x1c
    bool                Enabled;                   // +0x24
    void RemoveParticles();                        // 0x007B01C0
    CTCFishingSpot(CThing& owner) : CTCBase(owner) { }
    __forceinline virtual ~CTCFishingSpot() { RemoveParticles(); }
};

CTCFishingSpot* Create_CTCFishingSpot(CThing& owner)
{
    return new CTCFishingSpot(owner);
}