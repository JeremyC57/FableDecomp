#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CBase {
    C3DVector LookAt; C3DVector Position; float FOV;
    void SetPosition(const C3DVector& p) { Position = p; }
    void SetObservedPos(const C3DVector& p) { LookAt = p; }
    void SetFOV(float f) { FOV = f; }
};
struct CScriptModuleTransitionBetweenPoints {
    CBase* POwner;
    C3DVector LookAtPos; C3DVector StartPos; float FOV;
    void OnInitLeaf()
    {
        POwner->SetPosition(StartPos);
        POwner->SetObservedPos(LookAtPos);
        POwner->SetFOV(FOV);
    }
};
int main()
{
    CBase b; b.FOV = 0.0f;
    CScriptModuleTransitionBetweenPoints m; m.POwner = &b;
    m.StartPos.X = 1.0f; m.StartPos.Y = 2.0f; m.StartPos.Z = 3.0f;
    m.LookAtPos.X = 4.0f; m.LookAtPos.Y = 5.0f; m.LookAtPos.Z = 6.0f; m.FOV = 0.25f;
    m.OnInitLeaf();
    if (b.Position.Z != 3.0f || b.LookAt.X != 4.0f || b.LookAt.Z != 6.0f || b.FOV != 0.25f) { std::printf("00852040_TEST FAIL\n"); return 1; }
    std::printf("00852040_TEST PASS\n");
    return 0;
}