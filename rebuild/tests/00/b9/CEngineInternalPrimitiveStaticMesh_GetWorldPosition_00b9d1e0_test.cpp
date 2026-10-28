// Behaviour test for the GetWorldPosition override at 0xb9d1e0: the translation row of the object transform, always true.
#include <cstdio>
struct V { float X, Y, Z; };
struct M { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    V GetTranslation() const { V v; v.X = E41; v.Y = E42; v.Z = E43; return v; } };
struct Prim { M ObjectTransform; bool GetWorldPosition(V& position) { position = ObjectTransform.GetTranslation(); return true; } };
int main()
{
    Prim p = { { 1, 0, 0, 0, 1, 0, 0, 0, 1, 10.0f, -2.5f, 7.0f } }; V v = { 0, 0, 0 };
    if (!p.GetWorldPosition(v) || v.X != 10.0f || v.Y != -2.5f || v.Z != 7.0f) { std::printf("00b9d1e0_TEST FAIL\n"); return 1; }
    std::printf("00b9d1e0_TEST PASS\n");
    return 0;
}