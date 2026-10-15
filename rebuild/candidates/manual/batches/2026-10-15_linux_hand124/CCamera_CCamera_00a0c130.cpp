// CCamera::CCamera(const C3DVector& position, const C3DVector& forward, const C3DVector& up, float horizontalFOV, float verticalFOV) -- build a camera from a position, an orientation pair and the two fields of view
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0C130, length 120 bytes (ret 14h, five arguments). No callees;
// the only relocation is the 0.0f float constant @ 0x0122DEDC.
// Layout (Ego_r.pdb CCamera, 0x38 bytes): WorldPosition +0x00, RHSet {Up +0x0c, Forward +0x18},
// HeightLocked +0x24, ViewVecZLocked +0x25, FOVFlags +0x28, HorizontalFOV +0x2c, VerticalFOV +0x30, Zoom +0x34
// (left uninitialised by this constructor).
// Shape: the three vectors are copied word by word through edx (C3DVector's memberwise copy constructor),
// the up vector (third argument) fills +0x0c before the forward vector (second argument) fills +0x18, so
// CRightHandedSet's constructor takes (up, forward) and the camera passes them swapped from its own argument
// order. Both lock bytes share one `xor al,al`. FOVFlags(1) sits in the init list (a body store schedules
// the +0x2c store before the `test ah`) and is overwritten with 3 when the
// vertical field of view is positive (`fcomp [0.0f] ; test ah,41h ; jne` = skip when vfov <= 0).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet {
    C3DVector Up;                             // +0x00
    C3DVector Forward;                        // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
class CCamera {
public:
    C3DVector       WorldPosition;            // +0x00
    CRightHandedSet RHSet;                    // +0x0c
    bool            HeightLocked;             // +0x24
    bool            ViewVecZLocked;           // +0x25
    unsigned long   FOVFlags;                 // +0x28
    float           HorizontalFOV;            // +0x2c
    float           VerticalFOV;              // +0x30
    float           Zoom;                     // +0x34
    CCamera(const C3DVector& position, const C3DVector& forward, const C3DVector& up,
            float horizontalFOV, float verticalFOV);
};

CCamera::CCamera(const C3DVector& position, const C3DVector& forward, const C3DVector& up,
                 float horizontalFOV, float verticalFOV)
    : WorldPosition(position), RHSet(up, forward), HeightLocked(false), ViewVecZLocked(false),
      FOVFlags(1), HorizontalFOV(horizontalFOV), VerticalFOV(verticalFOV)
{
    if (verticalFOV > 0.0f)
        FOVFlags = 3;
}
