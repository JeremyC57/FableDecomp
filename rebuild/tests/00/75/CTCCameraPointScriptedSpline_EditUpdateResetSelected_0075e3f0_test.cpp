// behaviour test for 0x0075E3F0 CTCCameraPointScriptedSpline::EditUpdateResetSelected
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CCamera {
    C3DVector WorldPosition; C3DVector Up; C3DVector Forward; float HorizontalFOV;
    C3DVector GetViewVector() const { return Forward; }
    float GetHorizontalFOV() const { return HorizontalFOV; }
};
struct CKeyCamera { float DeltaIndex; C3DVector Pos; C3DVector LookDirection; float FOV; };
static int g_updates = 0;
struct SS {
    CKeyCamera* first; CKeyCamera* last; CKeyCamera* sel;
    void UpdateSplines() { ++g_updates; }
    void EditUpdateResetSelected(CCamera* c)
    {
        CKeyCamera* key = sel;
        if (key != last) { key->Pos = c->WorldPosition; key->LookDirection = c->GetViewVector(); key->FOV = c->GetHorizontalFOV(); }
        UpdateSplines();
    }
};
int main()
{
    CKeyCamera k[2] = {};
    CCamera c; c.WorldPosition.X = 1; c.WorldPosition.Y = 2; c.WorldPosition.Z = 3;
    c.Forward.X = 4; c.Forward.Y = 5; c.Forward.Z = 6; c.Up.X = c.Up.Y = c.Up.Z = 0; c.HorizontalFOV = 70;
    SS s; s.first = k; s.last = k + 2; s.sel = k + 2;
    s.EditUpdateResetSelected(&c);
    bool ok = g_updates == 1 && k[1].FOV == 0;
    s.sel = k + 1;
    s.EditUpdateResetSelected(&c);
    ok = ok && g_updates == 2 && k[1].Pos.Y == 2 && k[1].LookDirection.Z == 6 && k[1].FOV == 70 && k[0].FOV == 0;
    if (!ok) { std::printf("0075e3f0_TEST FAIL\n"); return 1; }
    std::printf("0075e3f0_TEST PASS\n");
    return 0;
}