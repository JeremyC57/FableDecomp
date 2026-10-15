// CCamera::CCamera(const CCamera&) -- the camera copy constructor: copies the world position, the
// right-handed orientation set, the two lock flags and the field-of-view block member by member.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D380, length 97 bytes. No callees.
// Layout (Ego_r.pdb CCamera, 0x38 bytes): WorldPosition C3DVector +0x00, RHSet CRightHandedSet
// {Up +0x0c, Forward +0x18} +0x0c, HeightLocked bool +0x24, ViewVecZLocked bool +0x25, FOVFlags +0x28,
// HorizontalFOV +0x2c, VerticalFOV +0x30, Zoom +0x34.
// Shape: every dword goes through edx one at a time (`mov edx,[ecx+N] ; mov [eax+N],edx`) and the two
// bools are byte moves, so this is a memberwise user copy constructor over members whose own copy
// constructors are memberwise too (a POD copy of 0x38 bytes would be a `rep movsd` block). The float
// members are copied as raw dwords: the vector/float members are modelled as `unsigned long` words so
// VC moves them through an integer register like retail (a float copy would go through the FPU).
struct C3DVectorBits {                        // C3DVector {X, Y, Z} copied as raw words
    unsigned long X, Y, Z;
    C3DVectorBits(const C3DVectorBits& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet {                      // 24 bytes
    C3DVectorBits Up;                         // +0x00
    C3DVectorBits Forward;                    // +0x0c
    CRightHandedSet(const CRightHandedSet& o) : Up(o.Up), Forward(o.Forward) { }
};
class CCamera {
public:
    C3DVectorBits   WorldPosition;            // +0x00
    CRightHandedSet RHSet;                    // +0x0c
    bool            HeightLocked;             // +0x24
    bool            ViewVecZLocked;           // +0x25
    unsigned long   FOVFlags;                 // +0x28
    unsigned long   HorizontalFOV;            // +0x2c (float)
    unsigned long   VerticalFOV;              // +0x30 (float)
    unsigned long   Zoom;                     // +0x34 (float)
    CCamera(const CCamera& o);
};

CCamera::CCamera(const CCamera& o)
    : WorldPosition(o.WorldPosition), RHSet(o.RHSet), HeightLocked(o.HeightLocked),
      ViewVecZLocked(o.ViewVecZLocked), FOVFlags(o.FOVFlags), HorizontalFOV(o.HorizontalFOV),
      VerticalFOV(o.VerticalFOV), Zoom(o.Zoom)
{
}
