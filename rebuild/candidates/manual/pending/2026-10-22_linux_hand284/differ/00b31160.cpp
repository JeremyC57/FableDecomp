// CEngineCamera::ProcessDebugScript() -- play the console debug camera script: advance through the looping script elements whose durations have elapsed since the element start time, then build an engine camera description interpolated between the current and next elements (position, look vector, FOV), with right = look x world-up and up = right x look, and apply it
// Grade: DIFFER(891v886) -- structure exact (loop inversion, `!(t < 1)` compare, element pointers in ebp/edi, lerps, crosses); left: the in-loop index recompute uses eax/edx in the other roles (+5 bytes), integer-copy register rotation (eax/ecx/edx) for the WorldPosition and rotation stores, and two `lea ecx` Normalise-this slots scheduled earlier
// Leaf: ProcessDebugScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B31160, length 889 bytes (`ret`, 8-aligned frame for the double start time).
// Callees: GFGetTime 0x009E1BC0 (__fastcall, double), C3DVector::Normalise 0x00A14440, CEngineCamera::SetCameraState
// 0x00B30B50. Globals: the default z-buffer start 0x01399D44, the display object 0x01436E1C (screen width/height +0x18c /
// +0x190). Constants 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout: as the matched Process 0x00B314E0 (DebugScript 12-byte vector +0x208, CurrentDebugScriptElement +0x214,
// DebugScriptRunning +0x218); DebugScriptElementStartTime (double) +0x220; Ego_r.pdb CDebugScriptElement (32 bytes).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    void Normalise();                                   // 0x00A14440
};
__forceinline void CrossProduct(C3DVector& r, const C3DVector& a, const C3DVector& b)
{
    r.X = a.Y * b.Z - a.Z * b.Y;
    r.Y = a.Z * b.X - a.X * b.Z;
    r.Z = a.X * b.Y - a.Y * b.X;
}
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
double __fastcall GFGetTime();                         // 0x009E1BC0
struct CDisplayGlobal {
    unsigned char _pad00[0x18c];
    long          ScreenWidth;                          // +0x18c
    long          ScreenHeight;                         // +0x190
};
extern CDisplayGlobal* g_pDisplay_01436e1c;             // 0x01436E1C
struct CMatrix3x4 { float m[12]; };
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
struct CDebugScriptElement {                            // 32 bytes
    C3DVector WorldPos;                                 // +0x00
    C3DVector LookVector;                               // +0x0c
    float     FOV;                                      // +0x18
    float     TimeToNextElement;                        // +0x1c
};
struct CDebugScriptArray {                              // retail 12-byte STLport vector
    CDebugScriptElement* _M_start;
    CDebugScriptElement* _M_finish;
    CDebugScriptElement* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const CDebugScriptElement& operator[](unsigned long i) const { return _M_start[i]; }
};
struct CEngineCamera {
    void*             __vftable;                        // +0x00
    unsigned char     _pad04[0x200 - 4];
    long              ConsoleOverrideViewPortWidth;     // +0x200
    long              ConsoleOverrideViewPortHeight;    // +0x204
    CDebugScriptArray DebugScript;                      // +0x208
    unsigned long     CurrentDebugScriptElement;        // +0x214
    bool              DebugScriptRunning;               // +0x218
    double            DebugScriptElementStartTime;      // +0x220
    void SetCameraState(const CEngineCameraDesc& desc, bool a, bool b);   // 0x00B30B50
    void ProcessDebugScript();
};

void CEngineCamera::ProcessDebugScript()
{
    CEngineCameraDesc desc(false);
    desc.Window.TLX = 0.0f;
    desc.Window.TLY = 0.0f;
    if (ConsoleOverrideViewPortWidth > 0) {
        desc.Window.BRX = (float)ConsoleOverrideViewPortWidth;
        desc.Window.BRY = (float)ConsoleOverrideViewPortHeight;
    } else {
        desc.Window.BRX = (float)g_pDisplay_01436e1c->ScreenWidth;
        desc.Window.BRY = (float)g_pDisplay_01436e1c->ScreenHeight;
    }
    const CDebugScriptElement* current = &DebugScript[CurrentDebugScriptElement];
    const CDebugScriptElement* next = &DebugScript[(CurrentDebugScriptElement + 1) % DebugScript.size()];
    float t = (float)((GFGetTime() - DebugScriptElementStartTime) / current->TimeToNextElement);
    while (!(t < 1.0f)) {
        DebugScriptElementStartTime += current->TimeToNextElement;
        CurrentDebugScriptElement = (CurrentDebugScriptElement + 1) % DebugScript.size();
        current = &DebugScript[CurrentDebugScriptElement];
        next = &DebugScript[(CurrentDebugScriptElement + 1) % DebugScript.size()];
        t = (float)((GFGetTime() - DebugScriptElementStartTime) / current->TimeToNextElement);
    }
    desc.WorldPosition = current->WorldPos * (1.0f - t) + next->WorldPos * t;
    C3DVector forward = current->LookVector * (1.0f - t) + next->LookVector * t;
    forward.Normalise();
    float fov = current->FOV;
    desc.Use2DFOV = false;
    desc.HorizontalFOV = (next->FOV - fov) * t + fov;
    C3DVector right;
    CrossProduct(right, forward, C3DVector(0.0f, 0.0f, 1.0f));
    right.Normalise();
    C3DVector up;
    CrossProduct(up, right, forward);
    up.Normalise();
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
    SetCameraState(desc, true, true);
}
