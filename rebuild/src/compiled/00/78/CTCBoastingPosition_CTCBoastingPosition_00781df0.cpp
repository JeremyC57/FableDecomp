// CTCBoastingPosition::~CTCBoastingPosition() -- kill the still-alive particle effect, destroy both intelligent pointers, then ~CTCBase
// Leaf: ??1CTCBoastingPosition
//
// Retail 0x00781DF0, 80 bytes (ends in the tail jump to ~CTCBase). Callees:
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on +0x2e), CThing::Kill(bool) @ 0x004C9B80,
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (+0x2e then +0x0c),
// CTCBase::~CTCBase @ 0x00686830 (tail jump; the chunk label "~CChunkedFileChunk" is propagated).
// Vtable 0x0126B24C = CTCBoastingPosition, stored first (a concrete class with a destructor body).
// Body: `if (effect && !effect->Dead) effect->Kill(true);` with every use of the pointer going
// through the out-of-line GetPItem (three calls, nothing cached); `test byte ptr [eax+91h],1` is the
// CThing::Dead bit (retail CThing sits 0x10 bytes below the PDB offset 0xa1). The member pointer
// objects are destroyed in reverse declaration order with no CIntelligentPointer vtable store, so the
// derived pointer template has no user-declared destructor. Byte-packed layout (PParticleEffect at
// +0x2e) as in the constructor 0x00781DA0; modelled locally because the engine header declares the
// pointer members as opaque byte arrays.

class CBaseObject;

class CThing
{ public:
    unsigned char _pad_0x00[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0
    void Kill(bool immediately);                    // 0x004C9B80
};

struct CBaseIntelligentPointer
{
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer   // vtable 0x01238C6C for <CThing>
{ public:
    virtual void v00();
    CIntelligentPointer() {}
    T* Get() const { return (T*)GetPItem(); }
};

class C3DVector { public: float X, Y, Z; };

#pragma pack(push, 1)
class CTCBase
{ public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    unsigned char _pad_0x0a[0x2];                   // +0x0a
    CTCBase(CThing* owner);                         // 0x00686800
    ~CTCBase();                                     // 0x00686830
};

class CTCBoastingPosition : public CTCBase                // vtable 0x0126B24C
{ public:
    CIntelligentPointer<CThing> PBoastingPodium;    // +0x0c
    C3DVector     OrbPosition;                      // +0x14
    bool          PositionActivated;                // +0x20
    unsigned char _pad_0x21[0x3];                   // +0x21
    long          BoastIndex;                       // +0x24
    long          Cost;                             // +0x28
    bool          BoastOn;                          // +0x2c
    bool          HeroInsideRadius;                 // +0x2d
    CIntelligentPointer<CThing> PParticleEffect;    // +0x2e (unaligned: byte-packed hierarchy)
    unsigned char _pad_0x36[0x2];                   // +0x36

    virtual void v00();
    ~CTCBoastingPosition();
};
#pragma pack(pop)

CTCBoastingPosition::~CTCBoastingPosition()
{
    if (PParticleEffect.Get() && !PParticleEffect.Get()->Dead)
        PParticleEffect.Get()->Kill(true);
}