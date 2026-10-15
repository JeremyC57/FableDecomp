// Behaviour test for 0x00920990: reset the enemy pointer, store the rise position, run the base activation.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CThing { C3DVector pos; const C3DVector* GetPosition() const { return &pos; } };
struct CIP { CThing* p; };
struct CGroup { CThing* creature; CIP PEnemy; C3DVector RisePos; int baseActivations;
    void BaseOnActivate() { ++baseActivations; }
    void OnActivate()
    {
        CIP none = { 0 };
        PEnemy = none;
        RisePos = *creature->GetPosition();
        BaseOnActivate();
    }
};
int main()
{
    CThing c = { { 1.0f, 2.0f, 3.0f } }, enemy = { { 0, 0, 0 } };
    CGroup g; g.creature = &c; g.PEnemy.p = &enemy; g.RisePos.X = g.RisePos.Y = g.RisePos.Z = 9.0f; g.baseActivations = 0;
    g.OnActivate();
    if (g.PEnemy.p != 0 || g.RisePos.X != 1.0f || g.RisePos.Y != 2.0f || g.RisePos.Z != 3.0f || g.baseActivations != 1) {
        std::printf("00920990_TEST FAIL\n"); return 1;
    }
    std::printf("00920990_TEST PASS\n");
    return 0;
}
