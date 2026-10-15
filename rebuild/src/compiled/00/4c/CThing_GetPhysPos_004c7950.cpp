// CThing::GetPhysPos() -- the physical position: the physics component's 2D offset added to its position in x/y, the position's z unchanged
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7950, 52 bytes. Callees: CTCPhysicsBase virtual slot 28 (+0x70),
// returning a C2DVector by hidden pointer into the 8-byte frame.
// Shape: PhysicsTC is retail +0x60 (PDB +0x70); the physics position C3DVector at +0x0c. The sum is an inline
// `C2DVector + C3DVector` operator taking the call's temporary by const reference, so the offset is read
// through the returned pointer (eax); a named local (or a reference local) re-reads it from the frame. The
// C3DVector (x, y, z) constructor's arguments are evaluated right to left (z load, then y sum, then x sum);
// C2DVector/C3DVector have user constructors (non-POD hidden returns).
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

inline C3DVector operator+(const C2DVector& offset, const C3DVector& position)
{
    return C3DVector(offset.X + position.X, offset.Y + position.Y, position.Z);
}

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual C2DVector GetPhysOffset2D();             // slot 28 (+0x70)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                          // +0x0c
};

struct CThing {
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                       // +0x60 (PDB +0x70)
    C3DVector GetPhysPos() const;
};

C3DVector CThing::GetPhysPos() const
{
    CTCPhysicsBase* physics = PhysicsTC;
    return physics->GetPhysOffset2D() + physics->Position;
}