// Behaviour test for CTCCameraPointScriptedSpline::OnInitialActivate (0x0075E270): every key camera position becomes world
// relative (plus the parent thing position), the splines are rebuilt, and playback is reset to run from key 0 to the last key.
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
static int g_baseActivations = 0;
static int g_splineUpdates = 0;
class CThing { public:
    C3DVector Position;
    const C3DVector& GetPosition() const { return Position; }
};
struct CKeyCamera {
    float     DeltaIndex;
    C3DVector Pos;
    unsigned char _rest[0x38 - 0x10];
};
class CTCBase {
public:
    virtual ~CTCBase() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    void OnInitialActivate() { ++g_baseActivations; }
};
class CTCCameraPointScriptedSpline : public CTCBase {
public:
    CKeyCamera*   KeyCameras_first;
    CKeyCamera*   KeyCameras_last;
    CKeyCamera*   KeyCameras_end;
    float         PauseFor;
    float         TimeToPlay;
    float         EditPreviewIndex;
    long          NodeToStartOn;
    long          NodeToEndOn;
    float         AnimationSpeed;
    long NumKeyCameras() const { return KeyCameras_last - KeyCameras_first; }
    void InitialiseSpline(long startNode, long endNode)
    {
        AnimationSpeed = 0.0f;
        PauseFor = 0.0f;
        NodeToStartOn = startNode;
        long& end = NodeToEndOn;
        end = endNode;
        if (end == -1)
            end = NumKeyCameras() - 1;
    }
    void UpdateSplines() { ++g_splineUpdates; }
    void OnInitialActivate();
};
void CTCCameraPointScriptedSpline::OnInitialActivate()
{
    CTCBase::OnInitialActivate();
    for (CKeyCamera* key = KeyCameras_first; key != KeyCameras_last; ++key)
    {
        const float* pp = &ParentThing->GetPosition().X;
        key->Pos = C3DVector(key->Pos.X + pp[0], key->Pos.Y + pp[1], key->Pos.Z + pp[2]);
    }
    UpdateSplines();
    InitialiseSpline(0, -1);
}
int main()
{
    int fails = 0;
    CThing thing; thing.Position = C3DVector(100.0f, 200.0f, 300.0f);
    CKeyCamera keys[3];
    for (int i = 0; i < 3; ++i) { keys[i].DeltaIndex = (float)i; keys[i].Pos = C3DVector(1.0f * i, 2.0f * i, 3.0f * i); }
    CTCCameraPointScriptedSpline spline;
    spline.ParentThing = &thing;
    spline.KeyCameras_first = keys; spline.KeyCameras_last = keys + 3; spline.KeyCameras_end = keys + 3;
    spline.PauseFor = 5.0f; spline.AnimationSpeed = 2.0f; spline.NodeToStartOn = 7; spline.NodeToEndOn = 9;
    spline.OnInitialActivate();
    if (g_baseActivations != 1 || g_splineUpdates != 1) { std::printf("calls fail\n"); ++fails; }
    for (int i = 0; i < 3; ++i)
        if (keys[i].Pos.X != 100.0f + i || keys[i].Pos.Y != 200.0f + 2 * i || keys[i].Pos.Z != 300.0f + 3 * i) { std::printf("key %d fail\n", i); ++fails; }
    if (spline.PauseFor != 0.0f || spline.AnimationSpeed != 0.0f) { std::printf("timers fail\n"); ++fails; }
    if (spline.NodeToStartOn != 0 || spline.NodeToEndOn != 2) { std::printf("nodes fail %ld %ld\n", spline.NodeToStartOn, spline.NodeToEndOn); ++fails; }
    // an empty key list: no positions touched, end node -1
    spline.KeyCameras_last = keys; spline.NodeToEndOn = 5;
    spline.OnInitialActivate();
    if (spline.NodeToEndOn != -1 || keys[0].Pos.X != 100.0f) { std::printf("empty fail\n"); ++fails; }
    if (fails) { std::printf("0075e270_TEST FAIL\n"); return 1; }
    std::printf("0075e270_TEST PASS\n");
    return 0;
}