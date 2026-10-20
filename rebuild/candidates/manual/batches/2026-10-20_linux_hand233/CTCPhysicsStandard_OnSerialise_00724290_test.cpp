// Behaviour test for CTCPhysicsStandard::OnSerialise (0x00724290): base first, then Forward XYZ, Up XYZ with default 0,
// values written back into RHSet.
#include <cstdio>
#include <cstring>

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector() : X(0), Y(0), Z(0) {}
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

static const char* g_names[8];
static int g_count = 0;
static int g_baseCalls = 0;

struct CPersistContext {
    bool Loading;
    void Transfer(const char* name, float& value, const float& defaultValue)
    {
        if (defaultValue != 0.0f) return;
        g_names[g_count++] = name;
        if (Loading) value = (float)g_count;      // 1..6 in call order
        else value += 100.0f;                     // saving must not change the stored values (copy is written back)
    }
};

struct CTCPhysicsBase {
    virtual ~CTCPhysicsBase() {}
    virtual void OnSerialise(CPersistContext&, bool) { ++g_baseCalls; }
};

struct CTCPhysicsStandard : CTCPhysicsBase {
    CRightHandedSet RHSet;
    void OnSerialise(CPersistContext& context, bool loading)
    {
        CTCPhysicsBase::OnSerialise(context, loading);
        C3DVector up = RHSet.Up;
        C3DVector forward = RHSet.Forward;
        context.Transfer("RHSetForwardX", forward.X, 0.0f);
        context.Transfer("RHSetForwardY", forward.Y, 0.0f);
        context.Transfer("RHSetForwardZ", forward.Z, 0.0f);
        context.Transfer("RHSetUpX", up.X, 0.0f);
        context.Transfer("RHSetUpY", up.Y, 0.0f);
        context.Transfer("RHSetUpZ", up.Z, 0.0f);
        RHSet.Forward = forward;
        RHSet.Up = up;
    }
};

int main()
{
    CTCPhysicsStandard p;
    CPersistContext ctx;
    ctx.Loading = true;
    p.OnSerialise(ctx, true);
    static const char* expect[6] = { "RHSetForwardX", "RHSetForwardY", "RHSetForwardZ", "RHSetUpX", "RHSetUpY", "RHSetUpZ" };
    bool ok = g_baseCalls == 1 && g_count == 6;
    for (int i = 0; ok && i < 6; ++i) ok = std::strcmp(g_names[i], expect[i]) == 0;
    ok = ok && p.RHSet.Forward.X == 1.0f && p.RHSet.Forward.Y == 2.0f && p.RHSet.Forward.Z == 3.0f
            && p.RHSet.Up.X == 4.0f && p.RHSet.Up.Y == 5.0f && p.RHSet.Up.Z == 6.0f;
    if (!ok) { std::printf("00724290_TEST FAIL\n"); return 1; }
    std::printf("00724290_TEST PASS\n");
    return 0;
}
