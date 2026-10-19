// Grade: DIFFER(223v238) -- retail spills the camera-space X to the frame (screen Y preloaded) and sums the world X term as (y*Up + d*Look) + x*Right; ours keeps X on the x87 stack and sums in source order (3x4 transform term-order lane gap, cf. 0x987BF0)
// CEngine::ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) -- unproject a screen position: when the engine is active, build the camera-space point ((X - centreX) / halfWidth * d, (centreY - Y) / halfHeight * d, d) and transform it by the engine camera's inverse view transform into `out`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B24400, 238 bytes (+ int3 padding).
// Callees: CIEngine::IsActive @ 0x00B23550 (CEngine vtable 0x012A0F3C slot 51, +0xcc; reads the byte at +8); the engine camera
//   singleton pointer @ 0x01436EA0.
// Layout: engine/CEngineCamera.h agrees with retail here (OneOverHalfWindowWidth +0xc0, OneOverHalfWindowHeight +0xc4,
//   WindowCenterX +0xc8, WindowCenterY +0xcc, InverseViewTransform +0xe4 = CMatrix3x4 rows X, Y, Z, Pos); the header keeps the
//   matrix as opaque bytes, so the transform reads it through a local CMatrix3x4 view of that member.
// Shape: a by-value C3DVector result (no user copy ctor) built in the frame and copied into `out` with integer moves.
#include "engine/CEngineCamera.h"

struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CMatrix3x4 {
    C3DVector X, Y, Z, Pos;
};
static inline C3DVector operator*(const C3DVector& v, const CMatrix3x4& m)
{
    return C3DVector(v.X * m.X.X + v.Y * m.Y.X + v.Z * m.Z.X + m.Pos.X,
                     v.X * m.X.Y + v.Y * m.Y.Y + v.Z * m.Z.Y + m.Pos.Y,
                     v.X * m.X.Z + v.Y * m.Y.Z + v.Z * m.Z.Z + m.Pos.Z);
}

extern CEngineCamera* g_pEngineCamera;                 // 0x01436EA0

struct CEngine {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050();
    virtual bool IsActive();                           // slot 51 (+0xcc) 0x00B23550
    bool ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screenPos, C3DVector& out);
};

bool CEngine::ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screenPos, C3DVector& out)
{
    if (IsActive()) {
        CEngineCamera* camera = g_pEngineCamera;
        C3DVector local((screenPos.X - camera->WindowCenterX) * camera->OneOverHalfWindowWidth * distance,
                        (camera->WindowCenterY - screenPos.Y) * camera->OneOverHalfWindowHeight * distance,
                        distance);
        out = local * *reinterpret_cast<const CMatrix3x4*>(camera->InverseViewTransform);
        return true;
    }
    return false;
}
