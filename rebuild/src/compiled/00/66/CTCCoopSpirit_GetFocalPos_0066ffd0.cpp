// CTCCoopSpirit::GetFocalPos() const -- the parent thing's position raised by 0.2 on Z, returned by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066FFD0, length 39 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0 (returns a const C3DVector&; the manifest label
// "CVertexBufferWin32::DoSizeof" is propagated). Float pool: 0.2f @ 0x012305A0.
// Layout = Ego_r.pdb CTCCoopSpirit: CTCBase::ParentThing +4.
// Shape: C3DVector has a memberwise user copy constructor, so it returns through the hidden pointer
// (`ret 4`); the result is a NAMED local built field by field and copied out, which VC folds into
// three x87 loads, the Z add and `fxch st(2)` + three `fstp` into the result (GOTCHAS wave 24o).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CThing {
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

struct CTCCoopSpirit {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    C3DVector GetFocalPos() const;
};

C3DVector CTCCoopSpirit::GetFocalPos() const
{
    const C3DVector& position = ParentThing->GetPosition();
    C3DVector focal;
    focal.X = position.X;
    focal.Y = position.Y;
    focal.Z = position.Z + 0.2f;
    return focal;
}