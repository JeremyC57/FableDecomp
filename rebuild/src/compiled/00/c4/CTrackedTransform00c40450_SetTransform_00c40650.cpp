// CTrackedTransform_00c40450::SetTransform(const STrackedTransform_00c40650& transform) -- store a whole transform: the position (+0x0c, dirty bit 0x8000) and the direction (+0x18, dirty bit 0x10000) each only when changed (the inlined SetPosition / SetDirection), then the up / forward pair through SetOrientationVectors on frame copies
// Leaf: ?SetTransform@CTrackedTransform_00c40450@@QAEXABUSTrackedTransform_00c40650@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C40650, 249 bytes, thiscall `ret 4`, esp frame (0x18 bytes).
// Identity (descriptive; libpk has no PDB or RTTI): the out-of-line setters it inlines are the landed
//   CTrackedTransform_00c40450::SetPosition 0x00C40450 and SetDirection 0x00C404A0 (same flags, same members), and it
//   calls the landed COrientedEngineObject::SetOrientationVectors 0x00C404F0 on the same object (whose notes name this
//   call site, 0x00C4073E). Called once, from the script render binding at 0x00C338A9. Catalog gapscan name is a placeholder.
// Callees: SetOrientationVectors 0x00C404F0 (reached through a cast to its landed class so it keeps its one spelling).
// Shape: each setter takes a `const CTrackedVector&` built from the transform's vector (a converting temporary: x stays
//   in st0, y and z go through frame words); the orientation vectors are named frame copies (forward copied first).
//   C3DVector needs a user member-wise copy constructor: with the implicit one the temporary's x goes through an
//   integer frame word and esi (273v249).
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& operator=(const C3DVector& o) { X = o.X; Y = o.Y; Z = o.Z; return *this; }
};

struct CTrackedVector {                         // the setters' argument: a vector wrapper built from a C3DVector
    C3DVector Value;
    CTrackedVector(const C3DVector& value) : Value(value) { }
};

struct STrackedTransform_00c40650 {            // 0x30 bytes
    C3DVector Position;                         // +0x00
    C3DVector Direction;                        // +0x0c
    C3DVector Up;                               // +0x18
    C3DVector Forward;                          // +0x24
};

struct COrientedEngineObject {
    void SetOrientationVectors(const C3DVector& up, const C3DVector& forward);   // 0x00C404F0
};

struct CTrackedTransform_00c40450 {
    unsigned char _pad00[0x0c];
    C3DVector     m_position;                   // +0x0c
    C3DVector     m_direction;                  // +0x18
    unsigned char _pad24[0x24];
    unsigned long m_dirtyFlags;                 // +0x48

    void SetPosition(const CTrackedVector& position)      // 0x00C40450 (inlined here)
    {
        if (m_position.X != position.Value.X || m_position.Y != position.Value.Y || m_position.Z != position.Value.Z) {
            m_position = position.Value;
            m_dirtyFlags |= 0x8000;
        }
    }
    void SetDirection(const CTrackedVector& direction)    // 0x00C404A0 (inlined here)
    {
        if (m_direction.X != direction.Value.X || m_direction.Y != direction.Value.Y || m_direction.Z != direction.Value.Z) {
            m_direction = direction.Value;
            m_dirtyFlags |= 0x10000;
        }
    }
    void SetTransform(const STrackedTransform_00c40650& transform);
};

void CTrackedTransform_00c40450::SetTransform(const STrackedTransform_00c40650& transform)
{
    SetPosition(transform.Position);
    SetDirection(transform.Direction);
    C3DVector forward = transform.Forward;
    C3DVector up = transform.Up;
    ((COrientedEngineObject*)this)->SetOrientationVectors(up, forward);
}