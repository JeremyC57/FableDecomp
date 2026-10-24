#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CBase {
    C3DVector LookAt; C3DVector Position;
    void SetPosition(const C3DVector& p) { Position = p; }
    void SetObservedPos(const C3DVector& p) { LookAt = p; }
};
struct CScriptModuleFullTransition {
    CBase* POwner; C3DVector LookAtPos; C3DVector StartPos;
    void OnInitLeaf()
    {
        POwner->SetPosition(StartPos);
        POwner->SetObservedPos(LookAtPos);
    }
};
int main()
{
    CBase b;
    CScriptModuleFullTransition m; m.POwner = &b;
    m.StartPos.X = 1.0f; m.StartPos.Y = 2.0f; m.StartPos.Z = 3.0f;
    m.LookAtPos.X = -1.0f; m.LookAtPos.Y = -2.0f; m.LookAtPos.Z = -3.0f;
    m.OnInitLeaf();
    if (b.Position.X != 1.0f || b.Position.Z != 3.0f || b.LookAt.Y != -2.0f || b.LookAt.Z != -3.0f) { std::printf("008521e0_TEST FAIL\n"); return 1; }
    std::printf("008521e0_TEST PASS\n");
    return 0;
}