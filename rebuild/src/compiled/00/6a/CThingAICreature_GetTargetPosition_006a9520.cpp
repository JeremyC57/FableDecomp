// CThingAICreature::GetTargetPosition() -- the point others aim at: the creature's position raised by the height value of its physics component (+0x34)
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A9520, 40 bytes. Callees: the thing's position accessor @ 0x006A4D00
// Header: engine/CThingAICreature.h exists but names nothing at these offsets (all `_pad_*`, PDB offsets that
// sit 0x30 above retail at the tail), so the class is modelled locally like the landed 0x00833BA0.
// (returns a const C3DVector&; manifest label "CVertexBufferWin32::DoSizeof" is propagated).
// Shape: wave-24o hidden-pointer C3DVector built field by field in a NAMED local and returned through a memberwise
// user copy constructor: the three values stay on the x87 stack (x and y loaded first, then `height + z`) and
// fold into `fxch st(2)` plus three `fstp` into the hidden result. PhysicsTC is CThing's retail +0x60.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CTCPhysicsBase { public:
    unsigned char _pad00[0x34];
    float         m_height;                          // +0x34
};

class CThing { public:
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                       // +0x60 (PDB +0x70)
    const C3DVector& GetPosition_006a4d00() const;            // 0x006A4D00
};

class CThingAICreature : public CThing { public:
    C3DVector GetTargetPosition() const;
};

C3DVector CThingAICreature::GetTargetPosition() const
{
    const C3DVector& position = GetPosition_006a4d00();
    C3DVector target;
    target.X = position.X;
    target.Y = position.Y;
    target.Z = PhysicsTC->m_height + position.Z;
    return target;
}