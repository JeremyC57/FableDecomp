// CTCParticleAttacher::CTCParticleAttacher(CThing* parent) -- CTCBase, own vtable, empty def pointer and particle list
// Leaf: CTCParticleAttacher
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE7FD, length 38 bytes (size-optimised def/factory module: unaligned
// entry and `push dword ptr [esp+8]` -> #pragma optimize("s", on)). Vtable 0x0124176C is CTCParticleAttacher's (RTTI); the
// manifest label "CTCDHeroSuit::CTCDHeroSuit" is propagated (the HeroSuit body has the same shape).
// Callee: CTCBase::CTCBase(CThing*) @ 0x00686800.
// Shape: every zero store follows the vtable store -- PDef (+0x0c) and ParticleList (+0x10, the 12-byte retail vector,
// PDB 16) are member objects with inline zeroing constructors, sharing one zero register.
#pragma optimize("s", on)

class CThing;

class CTCBase { public:
    CTCBase(CThing* parent);                // 0x00686800
    virtual void v00();
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
};

struct CDefPointer {                        // CDefPointer<CParticleAttacherDef const>
    void* Object;
    CDefPointer() : Object(0) { }
};

struct CThingPointerVector {                // vector<CIntelligentPointer<CThing> > (12 bytes)
    void* First;
    void* Last;
    void* End;
    CThingPointerVector() : First(0), Last(0), End(0) { }
};

class CTCParticleAttacher : public CTCBase { public:
    CDefPointer PDef;                       // +0x0c
    CThingPointerVector ParticleList;       // +0x10
    CTCParticleAttacher(CThing* parent);
    virtual void v00();
};

CTCParticleAttacher::CTCParticleAttacher(CThing* parent)
    : CTCBase(parent)
{
}