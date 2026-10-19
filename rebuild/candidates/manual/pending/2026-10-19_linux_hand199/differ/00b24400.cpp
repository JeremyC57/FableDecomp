// CEngine::ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screenPos, C3DVector& worldPos) -- when the engine has a camera, turn a screen position into the world point at the given view distance: undo the window centring and half-size scaling at that depth, then transform by the camera's inverse view matrix
// Grade: DIFFER(223v238) -- retail loads screen.Y before the X term, spills the view X to [esp] (reused as the result X) and keeps a 0xC result temporary; ours keeps X on the FPU; the per-component sum order (Y*E21 + Z*E31, then X*E11, then E41) already agrees
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B24400, length 240 bytes (ret 0Ch).
// Callees: own virtual slot 51 (+0xCC, the "has camera" test). Globals: the engine camera 0x01436EA0.
// Layout: CEngineCamera (PDB): OneOverHalfWindowWidth +0xC0, OneOverHalfWindowHeight +0xC4, WindowCenterX +0xC8,
// WindowCenterY +0xCC, InverseViewTransform +0xE4 (CMatrix3x4 rows E11..E43).
// Shape: the view-space point is a C3DVector local (X spilled, Y on the FPU, Z the distance argument); the transform is an
// inline matrix * vector whose result is copied into worldPos with dword moves.
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
__forceinline C3DVector operator*(const CMatrix3x4& m, const C3DVector& v)
{
    C3DVector r;
    r.X = v.X * m.E11 + v.Y * m.E21 + v.Z * m.E31 + m.E41;
    r.Y = v.X * m.E12 + v.Y * m.E22 + v.Z * m.E32 + m.E42;
    r.Z = v.X * m.E13 + v.Y * m.E23 + v.Z * m.E33 + m.E43;
    return r;
}

struct CEngineCamera {
    unsigned char _pad000[0xc0];
    float         OneOverHalfWindowWidth;       // +0xc0
    float         OneOverHalfWindowHeight;      // +0xc4
    float         WindowCenterX;                // +0xc8
    float         WindowCenterY;                // +0xcc
    unsigned char _pad0d0[0xe4 - 0xd0];
    CMatrix3x4    InverseViewTransform;         // +0xe4
};
extern CEngineCamera* g_pEngineCamera;          // 0x01436EA0

class CEngine {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50();
    virtual bool HasCamera();                   // slot 51 (+0xcc)
    bool ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screenPos, C3DVector& worldPos);
};

bool CEngine::ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screenPos, C3DVector& worldPos)
{
    if (HasCamera()) {
        CEngineCamera* camera = g_pEngineCamera;
        C3DVector viewPos((screenPos.X - camera->WindowCenterX) * camera->OneOverHalfWindowWidth * distance,
                          (camera->WindowCenterY - screenPos.Y) * camera->OneOverHalfWindowHeight * distance,
                          distance);
        worldPos = camera->InverseViewTransform * viewPos;
        return true;
    }
    return false;
}
