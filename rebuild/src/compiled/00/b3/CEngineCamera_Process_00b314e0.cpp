// CEngineCamera::Process(bool flag) -- run the debug camera script when one is running, else build the engine camera description from the game camera (window or console override viewport, FOVs in radians, position, the orthonormal rotation from the camera's up/forward set) and apply it with SetCameraState
// Leaf: Process
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B314E0, length 541 bytes (`ret 4`).
// Callees: CEngineCamera::ProcessDebugScript 0x00B31160, GFVBoxToABox 0x009E1E70 (__fastcall), CCamera FOV/flag getters
// 0x00A0BE90 (horizontal FOV), 0x00A0BE80 (FOV flags), 0x00A0BEA0 (vertical FOV) (catalog labels CMap/CWorld "DrawGet*" are
// propagated), C3DVector::AccurateNormalise @ 0x00A14440 (catalog "GetScaled" is propagated), CEngineCamera::SetCameraState
// 0x00B30B50. Constants 360.0f 0x01238020, 1/360 0x01238E00, 2*pi 0x0128F608; default z-buffer start global 0x01399D44.
// Layout: engine/CEngineCamera.h for GameCamera +0x0c, GameWindow +0x10, ConsoleOverrideViewPortWidth/Height +0x200/+0x204;
// the header's DebugScript is the PDB's 16-byte CArray, retail's is 12 bytes, so DebugScriptRunning (PDB +0x21c) sits at
// retail +0x218 -- the class is modelled locally. CEngineCameraDesc (Ego_r.pdb, 108 bytes) and CCamera (WorldPosition +0,
// RHSet {Up +0xc, Forward +0x18}) from the PDB.
// Shape: the desc's constructor body stores StateValid first; the FOV conversion turns -> degrees -> radians multiplies by
// the three pooled constants (written as literals VC7.1 folds `* 360 / 360` away, so they are named constants); up and
// forward are copied with C3DVector's user copy ctor and normalised in the frame; the right axis comes from an inline
// out-parameter cross product (forward x up), which keeps retail's x87 operand order; the rotation is stored column-wise.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector& AccurateNormalise();                                   // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
__forceinline void CrossProduct(C3DVector& r, const C3DVector& a, const C3DVector& b)
{
    r.X = a.Y * b.Z - a.Z * b.Y;
    r.Y = a.Z * b.X - a.X * b.Z;
    r.Z = a.X * b.Y - a.Y * b.X;
}
extern const float g_DegreesPerTurn;                    // 0x01238020 (360.0f)
extern const float g_TurnsPerDegree;                    // 0x01238E00 (1/360)
extern const float g_TwoPi;                             // 0x0128F608
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
void __fastcall GFVBoxToABox(const C2DBoxF* vbox, C2DBoxF* abox);   // 0x009E1E70
class CMatrix3x4 { public: float m[12]; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CCamera { public:
    C3DVector       WorldPosition;                      // +0x00
    CRightHandedSet RHSet;                              // +0x0c
    float GetHorizontalFOV() const;                     // 0x00A0BE90
    unsigned long GetFOVFlags() const;                  // 0x00A0BE80
    float GetVerticalFOV() const;                       // 0x00A0BEA0
};
extern float g_DefaultZBufferStart;                     // 0x01399D44
struct CEngineCameraDesc {                              // 108 bytes
    C2DBoxF    Window;                                  // +0x00
    CMatrix3x4 RotationMatrix;                          // +0x10
    C3DVector  WorldPosition;                           // +0x40
    float      HorizontalFOV;                           // +0x4c
    float      VerticalFOV;                             // +0x50
    bool       Use2DFOV;                                // +0x54
    float      NearZPlane;                              // +0x58
    float      FarZPlane;                               // +0x5c
    float      ZBufferStart;                            // +0x60
    float      ZBufferEnd;                              // +0x64
    bool       StateValid;                              // +0x68
    CEngineCameraDesc(bool valid)
    {
        StateValid = valid;
        NearZPlane = 0.1f;
        FarZPlane = 4000.0f;
        ZBufferStart = g_DefaultZBufferStart;
        ZBufferEnd = 0.99f;
    }
};
class CEngineCamera { public:
    void*         __vftable;                            // +0x00
    unsigned char _pad04[8];
    CCamera*      GameCamera;                           // +0x0c
    C2DBoxF*      GameWindow;                           // +0x10
    unsigned char _pad14[0x200 - 0x14];
    long          ConsoleOverrideViewPortWidth;         // +0x200
    long          ConsoleOverrideViewPortHeight;        // +0x204
    unsigned char DebugScript[0xc];                     // +0x208 CArray<CDebugScriptElement> (12 bytes in retail)
    long          CurrentDebugScriptElement;            // +0x214
    bool          DebugScriptRunning;                   // +0x218
    void ProcessDebugScript();                          // 0x00B31160
    void SetCameraState(const CEngineCameraDesc& desc, bool a, bool b);   // 0x00B30B50
    void Process(bool flag);
};

void CEngineCamera::Process(bool flag)
{
    if (DebugScriptRunning) {
        ProcessDebugScript();
        return;
    }
    CEngineCameraDesc desc(true);
    if (ConsoleOverrideViewPortWidth > 0) {
        desc.Window.TLX = 0.0f;
        desc.Window.TLY = 0.0f;
        desc.Window.BRX = (float)ConsoleOverrideViewPortWidth;
        desc.Window.BRY = (float)ConsoleOverrideViewPortHeight;
    } else {
        GFVBoxToABox(GameWindow, &desc.Window);
    }
    desc.HorizontalFOV = GameCamera->GetHorizontalFOV() * g_DegreesPerTurn * g_TurnsPerDegree * g_TwoPi;
    if (GameCamera->GetFOVFlags() & 2) {
        desc.VerticalFOV = GameCamera->GetVerticalFOV() * g_DegreesPerTurn * g_TurnsPerDegree * g_TwoPi;
        desc.Use2DFOV = true;
    } else {
        desc.VerticalFOV = 0.0f;
        desc.Use2DFOV = false;
    }
    desc.WorldPosition = GameCamera->WorldPosition;
    C3DVector up = GameCamera->RHSet.Up;
    C3DVector forward = GameCamera->RHSet.Forward;
    up.AccurateNormalise();
    forward.AccurateNormalise();
    C3DVector right;
    CrossProduct(right, forward, up);
    right.AccurateNormalise();
    desc.RotationMatrix.m[0] = right.X;
    desc.RotationMatrix.m[3] = right.Y;
    desc.RotationMatrix.m[6] = right.Z;
    desc.RotationMatrix.m[1] = up.X;
    desc.RotationMatrix.m[4] = up.Y;
    desc.RotationMatrix.m[7] = up.Z;
    desc.RotationMatrix.m[2] = forward.X;
    desc.RotationMatrix.m[5] = forward.Y;
    desc.RotationMatrix.m[8] = forward.Z;
    desc.RotationMatrix.m[9] = 0.0f;
    desc.RotationMatrix.m[10] = 0.0f;
    desc.RotationMatrix.m[11] = 0.0f;
    SetCameraState(desc, true, flag);
}