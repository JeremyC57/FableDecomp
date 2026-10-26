// CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const CMatrix3x4& transform) -- distance from the engine camera's world position to the transform's translation
// Leaf: ?CalcDistanceFromCamera@CEnginePrimitiveRenderUtil@@SIMABVCMatrix3x4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB6450, 67 bytes, static __fastcall (transform ecx), float in st0, `ret`.
// Identity: Ego_r.pdb ?CalcDistanceFromCamera@CEnginePrimitiveRenderUtil@@SIMABVCMatrix3x4@@@Z (static, float, one
//   CMatrix3x4 reference): the body reads the matrix translation (+0x24) and the camera position, nothing else. No direct
//   callers in retail. Catalog "ComputeDistanceToCamera" is descriptive.
// Layout: CMatrix3x4 = three axis rows + Pos (+0x24); the camera global 0x01436EA0 with its world position at +0x54
//   (CameraDesc +0x14, its WorldPosition +0x40), modelled locally as the landed WeatherMask RenderPrimitive 0x00BB6AB0
//   does (engine/CEngineCamera.h keeps CameraDesc opaque).
// Shape: the C3DVector overload (also Ego_r) inlined. GetPos() builds its C3DVector member by member (a block copy of
//   Pos gives three integer moves and no direct loads), so X and Y are used straight from the matrix and only Z goes
//   through the frame copy; the difference is a second frame temporary (X stored); the single-expression length
//   `sqrt(X*X + Y*Y + Z*Z)` squares Z first, then Y, then X, as retail.
#include <math.h>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    C3DVector Pos;                                 // +0x24
    C3DVector GetPos() const { return C3DVector(Pos.X, Pos.Y, Pos.Z); }
};

class CEngineCamera { public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                   // +0x54 (CameraDesc +0x14, its WorldPosition +0x40)
};
extern CEngineCamera* g_pEngineCamera;             // 0x01436EA0

class CEnginePrimitiveRenderUtil {
public:
    static float __fastcall CalcDistanceFromCamera(const C3DVector& position)
    {
        return (position - g_pEngineCamera->WorldPosition).GetLength();
    }
    static float __fastcall CalcDistanceFromCamera(const CMatrix3x4& transform);
};

float __fastcall CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const CMatrix3x4& transform)
{
    return CalcDistanceFromCamera(transform.GetPos());
}