// CDisplayEngine::CDisplayEngine(const CDisplayEngineInit& init) -- the display engine: preload flags 1, the component/player/definition managers and banks from the init block, the camera at (0, 0, 4) looking along +X with a 0.25 FOV, game drawing on, the letter-box and screen-fade infos reset (black, fully opaque colour), no view manager, 30 initial-fade readiness frames, fade duration -1, the world pointer, and gamma 1.0 with primitive preparation on
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00434E10, length 332 bytes (+ int3 padding), `ret 4`. Vtable 0x01231574 (CDisplayEngine).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (the polymorphic root), CCamera::CCamera(const C3DVector& position,
// const C3DVector& direction, float fov, long flags) @ 0x00A0BF20 on the Camera member (+0x30).
// Layout: engine/CDisplayEngine.h (PDB offsets match retail): EnginePreloadFlags +4, Component +8, PlayerManager +0xc, DefinitionManager
// +0x10, EngineGraphicBank +0x14, MeshBank +0x18, Engine3D +0x2c, Camera +0x30, DrawGame +0x6c, LetterBox +0x70 (a CFadeInFadeOutBase
// plus one long), PreparePrimitives +0xb0, GammaRamp +0xb4, OldGammaRamp +0xb8, ScreenFadeOutInfo +0xbc, ScreenFadeOutLocked +0xd8,
// DrawMemoryUse +0xd9, DrawDebugPage +0xdc, PViewManager +0xe0, InitialFadeReadynessCount +0xe8, InitialFadeActive +0xec,
// ScreenFadeDuration +0xf0, TimeSinceFadeStarted +0xf4, World +0xf8. The init block: Component +0, DefinitionManager +4, PlayerManager
// +8, World +0xc, EngineGraphicBank +0x10, MeshBank +0x20.
// Shape: members in declaration order (one ebx zero register), the two camera vectors built as temporaries in the frame (the direction
// first: right-to-left), the CFadeInFadeOutBase inline ctor assigning its members in the body (Active, Duration, StartFrame, FadingOut, FadingIn) and
// then `Colour = CRGBColour(0, 0, 0, 255)` (the temporary built byte-wise in a frame word and copied as a dword); the readiness count, fade duration, world pointer and the gamma trio are assigned in the body (stored out of declaration order).
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRGBColour {
    unsigned char R, G, B, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { A = a; B = b; G = g; R = r; }
};
struct CFadeInFadeOutBase {                     // 0x1c bytes
    bool Active;                                // +0x00
    unsigned char _pad01[0x0c - 0x01];
    bool FadingIn;                              // +0x0c
    bool FadingOut;                             // +0x0d
    long StartFrame;                            // +0x10
    long Duration;                              // +0x14
    CRGBColour Colour;                          // +0x18
    CFadeInFadeOutBase()
    {
        Active = false;
        Duration = 0;
        StartFrame = 0;
        FadingOut = false;
        FadingIn = false;
        Colour = CRGBColour(0, 0, 0, 255);
    }
};
struct CLetterBoxModeInfo : CFadeInFadeOutBase {   // 0x20 bytes
    long Mode;                                  // +0x1c (left uninitialised)
};
struct CCamera {                                // 0x38 bytes
    unsigned char _body[0x38];
    CCamera(const C3DVector& position, const C3DVector& direction, float fov, long flags);   // 0x00A0BF20
};
struct CMainGameComponent; struct CPlayerManager; struct CGameDefinitionManager; struct CGraphicDataBank; struct CMeshDataBank;
struct CIEngine; struct CDisplayViewManager; struct CCPPointerInfo; struct CWorld;
struct CDisplayEngineInit {
    CMainGameComponent*     Component;          // +0x00
    CGameDefinitionManager* DefinitionManager;  // +0x04
    CPlayerManager*         PlayerManager;      // +0x08
    CWorld*                 World;              // +0x0c
    CGraphicDataBank*       EngineGraphicBank;  // +0x10
    unsigned char           _pad14[0x20 - 0x14];
    CMeshDataBank*          MeshBank;           // +0x20
};
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                    // 0x0099A2F0
};
class CDisplayEngine : public CBaseClassNonCopyable {   // vtable 0x01231574
public:
    unsigned long           EnginePreloadFlags; // +0x04
    CMainGameComponent*     Component;          // +0x08
    CPlayerManager*         PlayerManager;      // +0x0c
    CGameDefinitionManager* DefinitionManager;  // +0x10
    CGraphicDataBank*       EngineGraphicBank;  // +0x14
    CMeshDataBank*          MeshBank;           // +0x18
    float                   MainWindow[4];      // +0x1c C2DBoxF
    CIEngine*               Engine3D;           // +0x2c
    CCamera                 Camera;             // +0x30
    long                    Frame;              // +0x68
    bool                    DrawGame;           // +0x6c
    CLetterBoxModeInfo      LetterBox;          // +0x70
    bool                    CameraInputsOn;     // +0x90
    double                  LastRefreshTime;    // +0x98
    double                  LastRenderTimeLength;        // +0xa0
    double                  LastWorldUpdateRenderTime;   // +0xa8
    bool                    PreparePrimitives;  // +0xb0
    float                   GammaRamp;          // +0xb4
    float                   OldGammaRamp;       // +0xb8
    CFadeInFadeOutBase      ScreenFadeOutInfo;  // +0xbc
    bool                    ScreenFadeOutLocked;         // +0xd8
    bool                    DrawMemoryUse;      // +0xd9
    long                    DrawDebugPage;      // +0xdc
    CDisplayViewManager*    PViewManager_Data;  // +0xe0
    CCPPointerInfo*         PViewManager_Info;  // +0xe4
    long                    InitialFadeReadynessCount;   // +0xe8
    bool                    InitialFadeActive;  // +0xec
    float                   ScreenFadeDuration; // +0xf0
    float                   TimeSinceFadeStarted;        // +0xf4
    CWorld*                 World;              // +0xf8
    CDisplayEngine(const CDisplayEngineInit& init);
};

CDisplayEngine::CDisplayEngine(const CDisplayEngineInit& init)
    : CBaseClassNonCopyable(), EnginePreloadFlags(1), Component(init.Component), PlayerManager(init.PlayerManager),
      DefinitionManager(init.DefinitionManager), EngineGraphicBank(init.EngineGraphicBank), MeshBank(init.MeshBank), Engine3D(0),
      Camera(C3DVector(0.0f, 0.0f, 4.0f), C3DVector(1.0f, 0.0f, 0.0f), 0.25f, 0), DrawGame(true), LetterBox(), ScreenFadeOutInfo(),
      ScreenFadeOutLocked(false), DrawMemoryUse(false), DrawDebugPage(0), PViewManager_Data(0), PViewManager_Info(0),
      InitialFadeActive(false), TimeSinceFadeStarted(0.0f)
{
    InitialFadeReadynessCount = 30;
    ScreenFadeDuration = -1.0f;
    World = init.World;
    OldGammaRamp = 0.0f;
    PreparePrimitives = true;
    GammaRamp = 1.0f;
}
