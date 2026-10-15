// CTCCoopSpirit::CTCCoopSpirit(CThing*) -- CTCBase(thing), own vtable, the def pointer cleared, the particle-emitter intelligent pointer constructed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00670050, length 48 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (the manifest label "CEngineSubPrimitive2DClockSprite" is
// propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the CIntelligentPointer
// vtable 0x01238C6C). Vtable 0x0125B264 = CTCCoopSpirit.
// Layout = Ego_r.pdb CTCCoopSpirit (0x34 bytes): CTCBase 0xc, NoMovementTimer +0xc, Score +0x10,
// OffscreenCounter +0x14, MovementVector +0x18, Master +0x24, PCoopSpiritDef +0x28 (CDefPointer, one
// word), PParticleEmitter +0x2c (CIntelligentPointer). Shape: both members are member objects with inline
// constructors (their stores follow the vtable store); the pointer member is addressed once in edi.
struct CThing;
struct CPlayer;
struct CCoopSpiritDef;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;                                          // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CDefPointer_CoopSpiritDef {                          // CDefPointer<CCoopSpiritDef const>: one word
    const CCoopSpiritDef* Object;
    CDefPointer_CoopSpiritDef() : Object(0) { }
};

struct C3DVector { float X, Y, Z; };

struct CTCCoopSpirit : CTCBase_Calls {                      // vtable 0x0125B264
    long                      NoMovementTimer;              // +0x0c
    long                      Score;                        // +0x10
    long                      OffscreenCounter;             // +0x14
    C3DVector                 MovementVector;               // +0x18
    CPlayer*                  Master;                       // +0x24
    CDefPointer_CoopSpiritDef PCoopSpiritDef;               // +0x28
    CIntelligentPointer       PParticleEmitter;             // +0x2c
    virtual void v00();
    CTCCoopSpirit(CThing* thing);
};

CTCCoopSpirit::CTCCoopSpirit(CThing* thing)
    : CTCBase_Calls(thing)
    , PCoopSpiritDef()
    , PParticleEmitter()
{
}