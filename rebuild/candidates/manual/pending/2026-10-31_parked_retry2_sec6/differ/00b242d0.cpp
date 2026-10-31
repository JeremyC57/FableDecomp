// CEngine::ConvertWorldPosToScreenPos(const C3DVector& world, C2DVector& screen) -- project a world position into window coordinates through the engine camera's view transform; false when the engine is not ready or the point is not in front of the camera
// Grade: DIFFER(289v293) -- the view transform matches; X is computed first into position.X and Y stored straight to
//   screen.Y, then screen.X = position.X (diff 31 -> 8 lines): left: retail spills X into the dead transform-result slot
//   [esp+0Ch] and stores X then Y from the FPU stack at the end (`fld [esp+0Ch] ; fstp [eax] ; fstp [eax+4]`), ours
//   stores Y first. Tried this pass: direct screen stores (295v293), a C2DVector(x, y) temporary (297), Transform(world)
//   without the copy (265)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B242D0, length 292 bytes (`ret 8`).
// Callees: CEngine virtual slot 51 (+0xcc, the readiness test -> bool). Globals: the engine camera @ 0x01436EA0.
// Layout: engine/CEngineCamera.h (ViewTransform CMatrix3x4 +0x80, HalfWindowWidth +0xb8, HalfWindowHeight +0xbc, WindowCenterX +0xc8,
// WindowCenterY +0xcc) -- a flat record, so the camera is viewed through a local model with those offsets; 0.0f @ 0x0122DEDC,
// 1.0f @ 0x0122DED8.
// Shape: the position is copied into a local, transformed into a NAMED result built field by field (statement by statement, the
// first row starting with the Y term as retail) and assigned back (dword copies); `z > 0` is the `test ah,41h ; jne` early out;
// the reciprocal depth is shared by both screen coordinates.
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector& operator=(const C2DVector& o) { X = o.X; Y = o.Y; return *this; }
};
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

struct CEngineCameraView {
    unsigned char _pad00[0x80];
    CMatrix3x4    ViewTransform;                   // +0x80
    float         WindowWidth;                     // +0xb0
    float         WindowHeight;                    // +0xb4
    float         HalfWindowWidth;                 // +0xb8
    float         HalfWindowHeight;                // +0xbc
    float         OneOverHalfWindowWidth;          // +0xc0
    float         OneOverHalfWindowHeight;         // +0xc4
    float         WindowCenterX;                   // +0xc8
    float         WindowCenterY;                   // +0xcc
};
extern CEngineCameraView* g_pEngineCamera_01436ea0;   // 0x01436EA0

static __forceinline C3DVector Transform(const CMatrix3x4& m, const C3DVector& v)
{
    C3DVector r;
    float x = v.Y * m.E21;
    x += v.Z * m.E31;
    x += v.X * m.E11;
    r.X = x + m.E41;
    float y = v.X * m.E12;
    y += v.Y * m.E22;
    y += v.Z * m.E32;
    r.Y = y + m.E42;
    float z = v.X * m.E13;
    z += v.Y * m.E23;
    z += v.Z * m.E33;
    r.Z = z + m.E43;
    return r;
}

struct CEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50();
    virtual bool IsReady();                        // slot 51 (+0xcc)
    bool ConvertWorldPosToScreenPos(const C3DVector& world, C2DVector& screen);
};

bool CEngine::ConvertWorldPosToScreenPos(const C3DVector& world, C2DVector& screen)
{
    if (IsReady()) {
        C3DVector position = world;
        position = Transform(g_pEngineCamera_01436ea0->ViewTransform, position);
        if (position.Z > 0.0f) {
            float inverseZ = 1.0f / position.Z;
            position.X = position.X * g_pEngineCamera_01436ea0->HalfWindowWidth * inverseZ + g_pEngineCamera_01436ea0->WindowCenterX;
            screen.Y = g_pEngineCamera_01436ea0->WindowCenterY - inverseZ * g_pEngineCamera_01436ea0->HalfWindowHeight * position.Y;
            screen.X = position.X;
            return true;
        }
    }
    return false;
}
