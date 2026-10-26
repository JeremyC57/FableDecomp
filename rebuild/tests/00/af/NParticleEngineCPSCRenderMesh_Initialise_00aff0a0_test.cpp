// Behaviour test for 0x00AFF0A0 CPSCRenderMesh::Initialise: a mesh primitive (type 0x1c, mesh bank, identity transform,
// blend mode/op) and a trail primitive (type 0x1a, trail bank, zero flags, trail blend mode/op) are created once each.
#include <cstdio>

class CMatrix3x4 { public: float m[12]; void SetIdentity() { for (int i = 0; i < 12; ++i) m[i] = 0.0f; m[0] = m[4] = m[8] = 1.0f; } };
struct SInfo { long Type; long BankIndex; unsigned short Flags; unsigned long BlendMode, BlendOp; CMatrix3x4 Transform; };
static SInfo g_last[4]; static int g_creates;
class CIEngine { public: long CreateChildPrimitive(void*, const SInfo* info, long) { g_last[g_creates] = *info; return 200 + g_creates++; } };
static SInfo MakeMesh(long bank, const CMatrix3x4& t, unsigned long mode, unsigned long op) { SInfo i; i.Type = 0x1c; i.BankIndex = bank; i.Flags = 0; i.BlendMode = mode; i.BlendOp = op; i.Transform = t; return i; }
struct CPSCRenderMesh {
    bool Initialised; long BankIndex, TrailBankIndex;
    unsigned long BlendMode : 3, TrailBlendMode : 3, BlendOp : 3, TrailBlendOp : 3;
    long ChildPrimIndex, ChildTrailPrimIndex;
    bool Initialise(CIEngine* engine, void* parent)
    {
        if (engine) {
            if (BankIndex && ChildPrimIndex == -1) {
                CMatrix3x4 identity; identity.SetIdentity();
                unsigned long blendMode = BlendMode; unsigned long blendOp = BlendOp;
                SInfo info = MakeMesh(BankIndex, identity, blendMode, blendOp);
                ChildPrimIndex = engine->CreateChildPrimitive(parent, &info, 0);
            }
            if (TrailBankIndex && ChildTrailPrimIndex == -1) {
                SInfo info; info.Type = 0x1a; info.BankIndex = TrailBankIndex; info.Flags = 0; info.BlendMode = TrailBlendMode; info.BlendOp = TrailBlendOp;
                ChildTrailPrimIndex = engine->CreateChildPrimitive(parent, &info, 0);
            }
            Initialised = true;
        }
        return Initialised;
    }
};
int main()
{
    CPSCRenderMesh s; s.Initialised = false; s.BankIndex = 7; s.TrailBankIndex = 0; s.BlendMode = 5; s.TrailBlendMode = 6; s.BlendOp = 2; s.TrailBlendOp = 1;
    s.ChildPrimIndex = -1; s.ChildTrailPrimIndex = -1;
    CIEngine engine;
    if (!s.Initialise(&engine, 0) || g_creates != 1 || s.ChildPrimIndex != 200 || s.ChildTrailPrimIndex != -1) { std::printf("00aff0a0_TEST FAIL\n"); return 1; }
    const SInfo& m = g_last[0];
    if (m.Type != 0x1c || m.BankIndex != 7 || m.BlendMode != 5 || m.BlendOp != 2 || m.Transform.m[0] != 1.0f || m.Transform.m[4] != 1.0f || m.Transform.m[8] != 1.0f || m.Transform.m[1] != 0.0f || m.Transform.m[11] != 0.0f) { std::printf("00aff0a0_TEST FAIL\n"); return 1; }
    s.TrailBankIndex = 9; s.Initialise(&engine, 0);
    if (g_creates != 2 || g_last[1].Type != 0x1a || g_last[1].BankIndex != 9 || g_last[1].Flags != 0 || g_last[1].BlendMode != 6 || g_last[1].BlendOp != 1 || s.ChildTrailPrimIndex != 201) { std::printf("00aff0a0_TEST FAIL\n"); return 1; }
    std::printf("00aff0a0_TEST PASS\n");
    return 0;
}