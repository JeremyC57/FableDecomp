#include <cstdio>
#include <cmath>
// Model of CTurningAnimCompare::operator()(a, b) @ 0x0087C660 with its retail quirk: both rotations are read from a.
class CAnimationEntry { public:
    float Rotation;
    mutable int Calls;
    void GetRotationInAnimation(float* rotation) const { ++Calls; *rotation = Rotation; }
};
class CTurningAnimCompare {
public:
    float RotationAngle;
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const;
};
bool CTurningAnimCompare::operator()(const CAnimationEntry* a, const CAnimationEntry* b) const
{
    float rotation_a;
    float rotation_b;
    a->GetRotationInAnimation(&rotation_a);
    a->GetRotationInAnimation(&rotation_b);
    float delta_a = (float)fabs(RotationAngle - rotation_a);
    float delta_b = (float)fabs(RotationAngle - rotation_b);
    if (delta_a < delta_b)
        return true;
    return false;
}
int main()
{
    CAnimationEntry a = { 0.1f, 0 };
    CAnimationEntry b = { 0.9f, 0 };
    CTurningAnimCompare comp;
    comp.RotationAngle = 0.0f;
    bool ok = true;
    if (comp(&a, &b)) ok = false;               // a is closer, but the quirk compares a with itself
    if (comp(&b, &a)) ok = false;
    if (a.Calls != 2 || b.Calls != 2) ok = false;   // each call reads only its left entry, twice
    comp.RotationAngle = 0.5f;
    if (comp(&a, &a)) ok = false;
    if (!ok) { std::printf("0087c660_TEST FAIL\n"); return 1; }
    std::printf("0087c660_TEST PASS\n");
    return 0;
}