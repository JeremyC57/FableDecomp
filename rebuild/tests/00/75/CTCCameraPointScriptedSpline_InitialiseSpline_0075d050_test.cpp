// Behaviour test for 0075d050 (CTCCameraPointScriptedSpline::InitialiseSpline): timers reset, start/end nodes stored, an end of -1 becomes the last key camera index.
#include <cstdio>

struct CKeyCamera { unsigned char _body[0x38]; };

struct CTCCameraPointScriptedSpline {
    CKeyCamera* KeyCameras_first;
    CKeyCamera* KeyCameras_last;
    CKeyCamera* KeyCameras_end;
    float       PauseFor;
    float       TimeToPlay;
    float       EditPreviewIndex;
    long        NodeToStartOn;
    long        NodeToEndOn;
    float       AnimationSpeed;
    long NumKeyCameras() const { return (long)(KeyCameras_last - KeyCameras_first); }
    void InitialiseSpline(long startNode, long endNode)
    {
        AnimationSpeed = 0.0f;
        PauseFor = 0.0f;
        NodeToStartOn = startNode;
        NodeToEndOn = endNode;
        if (endNode == -1)
            NodeToEndOn = NumKeyCameras() - 1;
    }
};

int main()
{
    static CKeyCamera cams[5];
    CTCCameraPointScriptedSpline s;
    s.KeyCameras_first = cams; s.KeyCameras_last = cams + 5; s.KeyCameras_end = cams + 5;
    s.PauseFor = 2.0f; s.TimeToPlay = 3.0f; s.AnimationSpeed = 1.0f;
    s.InitialiseSpline(1, 3);
    if (s.PauseFor != 0.0f || s.AnimationSpeed != 0.0f || s.TimeToPlay != 3.0f || s.NodeToStartOn != 1 || s.NodeToEndOn != 3) { std::printf("0075d050_TEST FAIL\n"); return 1; }
    s.InitialiseSpline(0, -1);
    if (s.NodeToStartOn != 0 || s.NodeToEndOn != 4) { std::printf("0075d050_TEST FAIL\n"); return 1; }
    std::printf("0075d050_TEST PASS\n");
    return 0;
}