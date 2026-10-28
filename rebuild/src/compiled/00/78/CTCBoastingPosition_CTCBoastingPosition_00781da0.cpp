// CTCBoastingPosition::CTCBoastingPosition(CThing* owner) -- CTCBase(owner), this class's vtable, the podium pointer at +0xc, three cleared flags, the particle-effect pointer at +0x2e
// Leaf: ??0CTCBoastingPosition
//
// Retail 0x00781DA0, 70 bytes (`ret 4`). Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (the chunk
// label "CEngineSubPrimitive2DClockSprite" is propagated; the landed 0x00686800 source is CTCBase),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (twice). Vtables: 0x0126B24C =
// CTCBoastingPosition (rtti-vtables.tsv), 0x01238C6C = CIntelligentPointer<CThing>.
// Shape: the derived vtable store sits right after the base constructor (MSVC sets the vptr before
// member initialisation), then the members in declaration order: PBoastingPodium (+0x0c, an
// inline CIntelligentPointer<CThing> constructor = out-of-line base ctor + its own vtable store),
// the three bool flags PositionActivated (+0x20), BoastOn (+0x2c), HeroInsideRadius (+0x2d)
// sharing one `xor al,al`, then PParticleEffect at the UNALIGNED +0x2e (the PDB layout of this
// thing-component hierarchy is byte packed, as the engine header and the landed
// CTCTargetedCountedComponent model declare). OrbPosition/BoastIndex/Cost are left uninitialised.
// The engine header (engine/CTCBoastingPosition.h) flattens the base and declares the pointer
// members as opaque byte arrays, so this source models the class locally with the header's
// member names and offsets.

class CThing;

struct CBaseIntelligentPointer
{
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer   // vtable 0x01238C6C for <CThing>
{ public:
    virtual void v00();
    CIntelligentPointer() {}
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
    CTCBoastingPosition(CThing* owner);
};
#pragma pack(pop)

CTCBoastingPosition::CTCBoastingPosition(CThing* owner)
    : CTCBase(owner)
    , PositionActivated(false)
    , BoastOn(false)
    , HeroInsideRadius(false)
{
}