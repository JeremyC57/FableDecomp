// CWorld::RenderUpdate(const CInterpolationInfo* info) -- per-render-frame world update: make the world's thing manager current, render-update its things and the environment, then point the sound listener along the current sound camera when both camera axes are non-degenerate
// Leaf: ?RenderUpdate@CWorld@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049E080, 284 bytes, thiscall `ret 4`. Catalog "CWorld_RenderUpdate" (name only).
// Callees: SetCurrentThingManager @ 0x004C74F0 (__fastcall), CThingManager::RenderUpdateThings @ 0x0051EBD0,
//   CEnvironment::RenderUpdate(float) @ 0x006B42F0, CWorld::GetCurrentSoundCamera @ 0x0049DAA0 (CCamera by hidden pointer),
//   the sound system's slot 11 (+0x2C, the listener) and slot 3 (+0x0C, update), the listener's slots 1 (+0x04,
//   position) and 3 (+0x0C, forward + up). Globals: the sound system @ 0x013B8394, epsilon 0.0001f @ 0x0129BA3C.
// Layout: engine/CWorld.h agrees (PEnvironment +0x18, PThingManager +0x50) but is a flattened struct that cannot host
//   the member functions, so CWorld is a local model with its member names; CCamera (Ego_r.pdb, 0x38): WorldPosition +0,
//   RHSet {Up, Forward} +0xC; CInterpolationInfo.Current.WFInterpolate +4.
// Shape: epsilon squared is computed once and kept on the x87 stack across both tests; the axis copies (forward
//   declared before up) are taken before the tests, which read the camera itself (testing the copies loads them back
//   from the copies' slots); GetLengthSquared = X*X + Y*Y + Z*Z (VC7.1 sums z, y, x); the shared Update call is
//   duplicated into both arms.
class C3DVector {
public:
    float X, Y, Z;
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CRightHandedSet {
public:
    C3DVector Up;                                    // +0x00
    C3DVector Forward;                               // +0x0c
};
class CCamera {                                      // 0x38 bytes (Ego_r.pdb)
public:
    C3DVector       WorldPosition;                   // +0x00
    CRightHandedSet RHSet;                           // +0x0c
    bool            HeightLocked;                    // +0x24
    bool            ViewVecZLocked;                  // +0x25
    unsigned long   FOVFlags;                        // +0x28
    float           HorizontalFOV;                   // +0x2c
    float           VerticalFOV;                     // +0x30
    float           Zoom;                            // +0x34
};
class CInterpolationInfoSet {
public:
    float GTPredictedTimeSinceLastRenderFrame;       // +0x00
    float WFInterpolate;                             // +0x04
};
class CInterpolationInfo {
public:
    CInterpolationInfoSet Current;                   // +0x00
    CInterpolationInfoSet Last;                      // +0x08
};
class CThingManager {
public:
    void RenderUpdateThings(const CInterpolationInfo* info);   // 0x0051EBD0
};
class CEnvironment {
public:
    void RenderUpdate(float interpolate);            // 0x006B42F0
};
class CSoundListener {
public:
    virtual void v00();
    virtual void SetPosition(const C3DVector& position);                          // slot 1
    virtual void v02();
    virtual void SetOrientation(const C3DVector& forward, const C3DVector& up);   // slot 3
};
class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Update();                           // slot 3
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CSoundListener* GetListener();           // slot 11
};
extern CSoundSystem* g_pSoundSystem;                 // 0x013B8394
extern const float g_Epsilon;                        // 0x0129BA3C (0.0001f)
void __fastcall SetCurrentThingManager(CThingManager* manager);   // 0x004C74F0

class CWorld {
public:
    unsigned char  _pad00[0x18];
    CEnvironment*  PEnvironment_ptr;                 // +0x18 scoped_ptr<CEnvironment>
    unsigned char  _pad1c[0x50 - 0x1c];
    CThingManager* PThingManager_ptr;                // +0x50 scoped_ptr<CThingManager>
    CCamera GetCurrentSoundCamera(const CInterpolationInfo* info);   // 0x0049DAA0
    void RenderUpdate(const CInterpolationInfo* info);
};

void CWorld::RenderUpdate(const CInterpolationInfo* info)
{
    SetCurrentThingManager(PThingManager_ptr);
    PThingManager_ptr->RenderUpdateThings(info);
    PEnvironment_ptr->RenderUpdate(info->Current.WFInterpolate);
    if (g_pSoundSystem) {
        CCamera camera = GetCurrentSoundCamera(info);
        C3DVector forward = camera.RHSet.Forward;
        C3DVector up = camera.RHSet.Up;
        if (camera.RHSet.Forward.GetLengthSquared() > g_Epsilon * g_Epsilon &&
            camera.RHSet.Up.GetLengthSquared() > g_Epsilon * g_Epsilon) {
            CSoundListener* listener = g_pSoundSystem->GetListener();
            listener->SetPosition(camera.WorldPosition);
            listener->SetOrientation(forward, up);
        }
        g_pSoundSystem->Update();
    }
}