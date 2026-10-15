// CTCCreatureGeneratorCreator::CTCCreatureGeneratorCreator(CThing*) -- CTCBase(thing), own vtable, no regeneration frame, not generated, the generator intelligent pointer constructed, generator UID cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C9720, length 57 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (the manifest label "CEngineSubPrimitive2DClockSprite"
// is propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the
// CIntelligentPointer vtable 0x01238C6C). Vtable 0x0126EB3C = CTCCreatureGeneratorCreator.
// Layout = Ego_r.pdb CTCCreatureGeneratorCreator (0x28 bytes): FrameToRegenerate +0xc, Generated +0x10,
// PGenerator +0x11 (CIntelligentPointer has alignment 1: declared under pack(1)), GeneratorUID +0x20
// (an 8-aligned unsigned __int64).
// Shape: plain initialisers in declaration order sharing the ebx zero register; the pointer member
// addressed once in edi; the UID halves stored after its constructor.
struct CThing;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;                                          // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
#pragma pack(pop)

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CTCCreatureGeneratorCreator : CTCBase_Calls {        // vtable 0x0126EB3C
    long                FrameToRegenerate;                  // +0x0c
    bool                Generated;                          // +0x10
    CIntelligentPointer PGenerator;                         // +0x11
    unsigned __int64    GeneratorUID;                       // +0x20
    virtual void v00();
    CTCCreatureGeneratorCreator(CThing* thing);
};

CTCCreatureGeneratorCreator::CTCCreatureGeneratorCreator(CThing* thing)
    : CTCBase_Calls(thing)
    , FrameToRegenerate(0)
    , Generated(false)
    , PGenerator()
    , GeneratorUID(0)
{
}
