#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CBase {
    C3DVector Position; float FOV;
    void SetPosition(const C3DVector& p) { Position = p; }
    float GetFOV() const { return FOV; }
};
struct CScriptModuleFOVZoom {
    CBase* POwner;
    long WorldFrames; float FOVIncrement; float FOVFraction; C3DVector StartPos;
    void OnInitLeaf()
    {
        FOVIncrement = (FOVFraction - 1.0f) * POwner->GetFOV() / WorldFrames;
        POwner->SetPosition(StartPos);
    }
};
int main()
{
    CBase b; b.FOV = 2.0f; b.Position.X = b.Position.Y = b.Position.Z = 0.0f;
    CScriptModuleFOVZoom m; m.POwner = &b; m.WorldFrames = 4; m.FOVFraction = 0.5f;
    m.StartPos.X = 1.0f; m.StartPos.Y = 2.0f; m.StartPos.Z = 3.0f;
    m.OnInitLeaf();
    if (m.FOVIncrement != -0.25f || b.Position.Y != 2.0f || b.Position.Z != 3.0f) { std::printf("008520c0_TEST FAIL\n"); return 1; }
    std::printf("008520c0_TEST PASS\n");
    return 0;
}