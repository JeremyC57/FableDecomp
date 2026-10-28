// CCamera::SetView(const C3DVector& viewDirection, const C3DVector& upDirection) -- set the camera's right-handed set from a view and an up direction, both normalised
// Grade: DIFFER(70v68) -- the two vector copies: retail copies through ecx (`mov ecx,esi ; mov [ecx],edx`, then `lea ecx,[esi+0Ch]`) with the source pointers in eax/edx and no edi push; ours copies through edx and `add ecx,18h` with edi as the scratch
// Leaf: ?SetView@CCamera@@QAEXABVC3DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BE30, 68 bytes, `ret 8`. Name: Ego_r/FableWin PDB
//   ?SetView@CCamera@@QAEXABVC3DVector@@0@Z (the only two-vector CCamera setter; the catalog row agrees).
// Callee: C3DVector::AccurateNormalise @ 0x00A14440, on RHSet.Up then RHSet.Forward.
// Layout: Ego_r.pdb CCamera: WorldPosition +0x00, RHSet (CRightHandedSet: Up +0x00, Forward +0x0C) +0x0C.
// Shape: Up = up, Forward = view as direct member assignments, then the inline CRightHandedSet::Normalise (esi = &RHSet).
#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector& AccurateNormalise();      // 0x00A14440
};
#pragma pack(pop)

class CRightHandedSet {
public:
    C3DVector Up;        // +0x00
    C3DVector Forward;   // +0x0c
    void Normalise()
    {
        Up.AccurateNormalise();
        Forward.AccurateNormalise();
    }
    void Init(const C3DVector& forward, const C3DVector& up)
    {
        Up = up;
        Forward = forward;
        Normalise();
    }
};

class CCamera {
public:
    C3DVector       WorldPosition;      // +0x00
    CRightHandedSet RHSet;              // +0x0c
    bool            HeightLocked;       // +0x24
    bool            ViewVecZLocked;     // +0x25
    unsigned long   FOVFlags;           // +0x28
    float           HorizontalFOV;      // +0x2c
    float           VerticalFOV;        // +0x30
    float           Zoom;               // +0x34
    void SetView(const C3DVector& viewDirection, const C3DVector& upDirection);
};

void CCamera::SetView(const C3DVector& viewDirection, const C3DVector& upDirection)
{
    RHSet.Up = upDirection;
    RHSet.Forward = viewDirection;
    RHSet.Normalise();
}
