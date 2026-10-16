#include <cstdio>
#include <cstring>
#include <cmath>
// Behaviour model of CThing::ComputeDistanceToMorphDummy: distance to the "EYE_SET_L" dummy, 1.0 when it is missing.
struct V { float X, Y, Z; };
struct App {
    bool has; V dummy;
    bool GetDummyObject(const char* name, long, unsigned long, V* pos, void*, float) { if (!has || std::strcmp(name, "EYE_SET_L")) return false; *pos = dummy; return true; }
};
struct Thing {
    App* PTCGraphicAppearance; V position;
    float ComputeDistanceToMorphDummy()
    {
        V d; char set[24];
        bool missing;
        { App* a = PTCGraphicAppearance; missing = a->GetDummyObject("EYE_SET_L", 0, 0, &d, set, 1.0f) ? false : true; }
        if (missing) return 1.0f;
        float dx = d.X - position.X, dy = d.Y - position.Y, dz = d.Z - position.Z;
        return (float)std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};
int main()
{
    App a; a.has = true; a.dummy.X = 3.0f; a.dummy.Y = 4.0f; a.dummy.Z = 12.0f;
    Thing t; t.PTCGraphicAppearance = &a; t.position.X = 0; t.position.Y = 0; t.position.Z = 0;
    if (std::fabs(t.ComputeDistanceToMorphDummy() - 13.0f) > 1e-4f) { std::printf("00661aa0_TEST FAIL\n"); return 1; }
    a.has = false;
    if (t.ComputeDistanceToMorphDummy() != 1.0f) { std::printf("00661aa0_TEST FAIL\n"); return 1; }
    std::printf("00661aa0_TEST PASS\n");
    return 0;
}