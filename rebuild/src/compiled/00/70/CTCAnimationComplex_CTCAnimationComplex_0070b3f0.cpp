// CTCAnimationComplex::CTCAnimationComplex(CThing& owner) -- CTCBase(owner), this class's vtable, then PAnimation = new CAnimateThing(owner) (null when the allocation fails)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B3F0, length 67 bytes.
// Leaf: ??0CTCAnimationComplex
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, operator new @ 0x00BFEA1A (0xbc bytes = sizeof(CAnimateThing) in retail),
// CAnimateThing::CAnimateThing(CThing&) @ 0x0070E710 (the manifest's "CAnimSet::Ctor" is propagated). Vtable 0x012650A4 =
// CTCAnimationComplex (rtti-vtables.tsv). Ego_r.pdb CTCAnimationComplex (0x10 bytes): CAnimateThing* PAnimation +0x0c (the
// landed `??_G` @ 0x0070BB80 deletes it).
// Shape: a member initialised by a call lands after the vtable store; the `new` null test splits the store into two
// tails (the constructor result, or a zero register), each with its own epilogue. No EH frame (the module is not /EHsc).
class CThing;

class CAnimateThing { public:
    unsigned char _body[0xbc];                 // sizeof(CAnimateThing) = 0xbc in retail
    CAnimateThing(CThing& owner);              // 0x0070E710
};

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CTCBase(CThing& owner);                    // 0x00686800
};

class CTCAnimationComplex : public CTCBase { public:         // vtable 0x012650A4
    CAnimateThing* PAnimation;                 // +0x0c
    virtual void v00();
    CTCAnimationComplex(CThing& owner);
};

CTCAnimationComplex::CTCAnimationComplex(CThing& owner)
    : CTCBase(owner), PAnimation(new CAnimateThing(owner))
{
}