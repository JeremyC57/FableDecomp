// Behaviour test for 0x0049E080 CWorld::RenderUpdate(const CInterpolationInfo*): the thing manager is made current and
// render-updated, the environment gets the WF interpolation, and with a sound system the listener follows the sound
// camera (position, forward, up) only while both camera axes are longer than the 0.0001 epsilon; Update runs every time.
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CCamera { public:
    C3DVector WorldPosition;
    CRightHandedSet RHSet;
    bool HeightLocked, ViewVecZLocked;
    unsigned int FOVFlags;
    float HorizontalFOV, VerticalFOV, Zoom;
};
struct CInterpolationInfoSet { float GTPredictedTimeSinceLastRenderFrame; float WFInterpolate; };
class CInterpolationInfo { public: CInterpolationInfoSet Current; CInterpolationInfoSet Last; };

static int g_currentSets = 0, g_thingUpdates = 0, g_listenerSets = 0, g_updates = 0;
static float g_envInterpolate = -1.0f;
static C3DVector g_lastPosition, g_lastForward, g_lastUp;
class CThingManager { public: void RenderUpdateThings(const CInterpolationInfo*) { ++g_thingUpdates; } };
class CEnvironment { public: void RenderUpdate(float t) { g_envInterpolate = t; } };
struct CSoundListener {
    void SetPosition(const C3DVector& p) { g_lastPosition = p; }
    void SetOrientation(const C3DVector& f, const C3DVector& u) { g_lastForward = f; g_lastUp = u; ++g_listenerSets; }
};
struct CSoundSystem {
    CSoundListener listener;
    CSoundListener* GetListener() { return &listener; }
    void Update() { ++g_updates; }
};
static CSoundSystem* g_pSoundSystem = 0;
static const float g_Epsilon = 0.0001f;
static CThingManager* g_currentManager = 0;
static void SetCurrentThingManager(CThingManager* m) { g_currentManager = m; ++g_currentSets; }

class CWorld { public:
    CEnvironment* PEnvironment_ptr;
    CThingManager* PThingManager_ptr;
    CCamera soundCamera;
    CCamera GetCurrentSoundCamera(const CInterpolationInfo*) { return soundCamera; }
    void RenderUpdate(const CInterpolationInfo* info)
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
};

static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }

int main()
{
    int ok = 1;
    CThingManager manager; CEnvironment environment; CSoundSystem sound;
    CWorld world; world.PEnvironment_ptr = &environment; world.PThingManager_ptr = &manager;
    world.soundCamera.WorldPosition = V(1, 2, 3);
    world.soundCamera.RHSet.Up = V(0, 0, 1);
    world.soundCamera.RHSet.Forward = V(0, 1, 0);
    CInterpolationInfo info; info.Current.GTPredictedTimeSinceLastRenderFrame = 0.0f; info.Current.WFInterpolate = 0.25f;
    world.RenderUpdate(&info);                                  // no sound system
    if (g_currentManager != &manager || g_currentSets != 1 || g_thingUpdates != 1 || g_envInterpolate != 0.25f ||
        g_updates != 0 || g_listenerSets != 0) ok = 0;
    g_pSoundSystem = &sound;
    world.RenderUpdate(&info);                                  // valid axes: listener follows the camera
    if (g_updates != 1 || g_listenerSets != 1 || g_lastPosition.Z != 3.0f || g_lastForward.Y != 1.0f || g_lastUp.Z != 1.0f) ok = 0;
    world.soundCamera.RHSet.Forward = V(0.00005f, 0, 0);       // degenerate forward: no listener update
    world.RenderUpdate(&info);
    if (g_updates != 2 || g_listenerSets != 1) ok = 0;
    world.soundCamera.RHSet.Forward = V(1, 0, 0);
    world.soundCamera.RHSet.Up = V(0, 0, 0);                    // degenerate up
    world.RenderUpdate(&info);
    if (g_updates != 3 || g_listenerSets != 1 || g_thingUpdates != 4) ok = 0;
    if (!ok) { std::printf("0049e080_TEST FAIL\n"); return 1; }
    std::printf("0049e080_TEST PASS\n");
    return 0;
}