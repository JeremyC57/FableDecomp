// Behaviour test for CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh (0x00B39080): a copy of the logic over a
// minimal model -- no parent / stats not loaded (loading started) / no dummy all fail; otherwise the dummy transform is
// post-multiplied by the parent's object transform and the rotation rows are divided by the parent's scale.
#include <cstdio>
#include <cmath>

struct Matrix {
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void SetMultiply(const Matrix& a, const Matrix& b)
    {
        E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
        E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
        E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
        E21 = a.E21 * b.E11 + a.E22 * b.E21 + a.E23 * b.E31;
        E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
        E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
        E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
        E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
        E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
        E41 = a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31 + b.E41;
        E42 = a.E41 * b.E12 + a.E42 * b.E22 + a.E43 * b.E32 + b.E42;
        E43 = a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33 + b.E43;
    }
    void PreScale(float s) { E11 *= s; E12 *= s; E13 *= s; E21 *= s; E22 *= s; E23 *= s; E31 *= s; E32 *= s; E33 *= s; }
};
static Matrix Make(float d, float tx)
{
    Matrix m = { d, 0, 0, 0, d, 0, 0, 0, d, tx, 0, 0 };
    return m;
}

struct Parent {
    bool StatsLoaded;
    bool LoadingStarted;
    bool HasDummy;
    float Scale;
    Matrix Dummy;
    Matrix Object;
};

static bool GetParentTransformAnimatedMesh(Parent* parent, Matrix& transform)
{
    if (!parent)
        return false;
    if (parent->StatsLoaded) {
        if (parent->HasDummy) {
            transform = parent->Dummy;
            Matrix object_transform = parent->Object;
            Matrix dummy_transform(transform);
            transform.SetMultiply(dummy_transform, object_transform);
            transform.PreScale(1.0f / parent->Scale);
            return true;
        }
        return false;
    }
    parent->LoadingStarted = true;
    return false;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    Matrix out = Make(1, 0);
    if (GetParentTransformAnimatedMesh(0, out)) { std::printf("00b39080_TEST FAIL\n"); return 1; }
    Parent p;
    p.StatsLoaded = false; p.LoadingStarted = false; p.HasDummy = true; p.Scale = 2.0f;
    p.Dummy = Make(1.0f, 3.0f); p.Object = Make(2.0f, 5.0f);
    if (GetParentTransformAnimatedMesh(&p, out) || !p.LoadingStarted) { std::printf("00b39080_TEST FAIL\n"); return 1; }
    p.StatsLoaded = true; p.HasDummy = false;
    if (GetParentTransformAnimatedMesh(&p, out)) { std::printf("00b39080_TEST FAIL\n"); return 1; }
    p.HasDummy = true;
    if (!GetParentTransformAnimatedMesh(&p, out)) { std::printf("00b39080_TEST FAIL\n"); return 1; }
    // dummy (I, t=3) * object (2I, t=5): rotation 2I scaled back by 1/2 -> I; translation 3*2+5 = 11 (unscaled)
    if (!Near(out.E11, 1.0f) || !Near(out.E22, 1.0f) || !Near(out.E33, 1.0f) || !Near(out.E12, 0.0f) || !Near(out.E41, 11.0f)) {
        std::printf("00b39080_TEST FAIL\n");
        return 1;
    }
    std::printf("00b39080_TEST PASS\n");
    return 0;
}