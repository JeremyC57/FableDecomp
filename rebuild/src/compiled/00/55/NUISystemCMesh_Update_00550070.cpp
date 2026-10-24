// NUISystem::CMesh::Update(float deltaTime) -- the state-component update, then spin the orientation about the rotation axis by speed x time
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00550070, 49 bytes (`ret 4`).
// Callees: NUISystem::CChangingStateComponent::Update(float) @ 0x0052C7E0 (landed), CRightHandedSet rotate-about-axis
// @ 0x00A22F80 (thiscall (const C3DVector& axis, float angle), as the landed CTCRotate::FrameUpdate 0x0072A570; manifest
// "CMatrix3x3::MultiplyByQuaternionRotation" is propagated).
// Label: CMesh vtable 0x01249F9C slot 1 (rtti-vtables.tsv); Ego_r.pdb names ?Update@CMesh@NUISystem@@UAEXM@Z.
// Layout: Orientation +0x174, RotationAxis +0x18c, RotationSpeed +0x198 (Ego_r.pdb CMesh offsets minus 0x30, as SetOrientation
// 0x00550BE0).
// Shape: `fld dt ; fmul speed` is `RotationSpeed * deltaTime`; the float argument is stored into its pushed slot.
struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    void RotateAboutAxis(const C3DVector& axis, float angle);   // 0x00A22F80
};

namespace NUISystem {
class CChangingStateComponent {
public:
    virtual void v000();
    virtual void Update(float deltaTime);         // slot 1; 0x0052C7E0
};

class CMesh : public CChangingStateComponent {
public:
    unsigned char _pad04[0x174 - 4];
    CRightHandedSet Orientation;                  // +0x174
    C3DVector RotationAxis;                       // +0x18c
    float RotationSpeed;                          // +0x198
    virtual void Update(float deltaTime);
};

void CMesh::Update(float deltaTime)
{
    CChangingStateComponent::Update(deltaTime);
    Orientation.RotateAboutAxis(RotationAxis, RotationSpeed * deltaTime);
}
}