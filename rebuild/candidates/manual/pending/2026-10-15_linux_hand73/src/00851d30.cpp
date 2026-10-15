// NCameraModes::CScriptModuleBase::SetCoordAxis(const CRightHandedSet& axis) -- CoordAxis = axis
// Leaf: SetCoordAxis
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00851D30, length 44 bytes. No callees.
// Caller: CGameScriptInterface::CameraUseCameraPoint (0x0089E462, the optional `const CRightHandedSet*`).
// Shape: the plain 24-byte record (PDB CRightHandedSet {C3DVector Up; C3DVector Forward;}) assigned as a
// whole: VC re-bases `this` to the member (`add ecx,20h`) and moves six dwords through edx.
// Layout: PDB NCameraModes::CScriptModuleBase (0x4c bytes): PCamPoint +4, PThingOrigin +0xc, CoordOrigin
// +0x14, CoordAxis +0x20, POwner +0x38, PCameraManager +0x3c, flags +0x40/+0x41, CutOutOverride +0x44,
// FOVOverride +0x48 (the landed spline/camera-module constructors use the same 0x4c-byte base).
// No engine header exists for NCameraModes::CScriptModuleBase.
struct C3DVector {
    float X, Y, Z;
};

struct CRightHandedSet {                       // 24 bytes, plain record
    C3DVector Up;                              // +0x00
    C3DVector Forward;                         // +0x0c
};

namespace NCameraModes {

struct CScriptModuleBase {
    virtual void v00();
    unsigned char   _pad04[0x1c];              // +0x04 PCamPoint, PThingOrigin, CoordOrigin
    CRightHandedSet CoordAxis;                 // +0x20
    unsigned char   _pad38[0x14];              // +0x38 POwner .. FOVOverride
    void SetCoordAxis(const CRightHandedSet& axis);
};

void CScriptModuleBase::SetCoordAxis(const CRightHandedSet& axis)
{
    CoordAxis = axis;
}

}
