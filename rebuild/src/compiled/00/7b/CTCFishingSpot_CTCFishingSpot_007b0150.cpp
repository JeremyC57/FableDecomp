// CTCFishingSpot::CTCFishingSpot(CThing*) -- CTCBase(thing), the two particle pointers and the fish particle pointer constructed empty, enabled
// Leaf: ??0CTCFishingSpot
//
// Retail 0x007B0150, 106 bytes (`ret 4`). Callees: CTCBase::CTCBase(CThing*) @ 0x00686800,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 and ::SetPItem @ 0x00A01B90 (three of
// each; the CIntelligentPointer<CThing> vtable 0x01238C6C between them). Vtable 0x0126D51C =
// CTCFishingSpot.
// Layout = Ego_r.pdb CTCFishingSpot (0x28, no retail shift): PParticle1 +0xc, PParticle2 +0x14,
// PParticleFish +0x1c, Enabled +0x24.
// Shape: each pointer is built from a null item (`CIntelligentPointer<CThing>(0)`: base ctor, vtable
// store, SetPItem(0)), addressed once in edi; Enabled is the last initialiser.
struct CBaseObject;
struct CThing;

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
};

struct CTCFishingSpot : CTCBase {                           // vtable 0x0126D51C
    CIntelligentPointer<CThing> PParticle1;                 // +0x0c
    CIntelligentPointer<CThing> PParticle2;                 // +0x14
    CIntelligentPointer<CThing> PParticleFish;              // +0x1c
    bool                        Enabled;                    // +0x24
    virtual void v00();
    CTCFishingSpot(CThing* thing);
};

CTCFishingSpot::CTCFishingSpot(CThing* thing)
    : CTCBase(thing)
    , PParticle1(0)
    , PParticle2(0)
    , PParticleFish(0)
    , Enabled(true)
{
}