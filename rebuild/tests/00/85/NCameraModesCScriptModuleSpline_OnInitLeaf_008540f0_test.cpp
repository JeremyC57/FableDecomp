#include <cstdio>
struct CTCCameraPointScriptedSpline {
    long numKeys; long start; long end;
    long NumKeyCameras() const { return numKeys; }
    void InitialiseSpline(long s, long e) { start = s; end = e; }
};
struct CThing {
    CTCCameraPointScriptedSpline* spline;
    bool GetTC(CTCCameraPointScriptedSpline** out, long type)
    {
        if (type == 0x3f && spline) { *out = spline; return true; }
        return false;
    }
};
struct CScriptModuleSpline {
    CThing* camPoint; float Index; long NodeToStartOn; long NodeToEndOn; int updates;
    CThing* GetPCameraPoint() const { return camPoint; }
    void Update() { ++updates; Index = 99.0f; }
    void OnInitLeaf()
    {
        CTCCameraPointScriptedSpline* spline;
        if (GetPCameraPoint()->GetTC(&spline, 0x3f)) {
            spline->InitialiseSpline(NodeToStartOn, NodeToEndOn);
            Index = (float)NodeToStartOn / (spline->NumKeyCameras() - 1);
        }
        float index = Index;
        Update();
        Index = index;
    }
};
int main()
{
    CTCCameraPointScriptedSpline sp; sp.numKeys = 5; sp.start = sp.end = -1;
    CThing t; t.spline = &sp;
    CScriptModuleSpline m; m.camPoint = &t; m.Index = 0.0f; m.NodeToStartOn = 2; m.NodeToEndOn = 4; m.updates = 0;
    m.OnInitLeaf();
    if (m.Index != 0.5f || sp.start != 2 || sp.end != 4 || m.updates != 1) { std::printf("008540f0_TEST FAIL\n"); return 1; }
    t.spline = 0; m.Index = 0.25f;
    m.OnInitLeaf();
    if (m.Index != 0.25f || m.updates != 2) { std::printf("008540f0_TEST FAIL\n"); return 1; }
    std::printf("008540f0_TEST PASS\n");
    return 0;
}