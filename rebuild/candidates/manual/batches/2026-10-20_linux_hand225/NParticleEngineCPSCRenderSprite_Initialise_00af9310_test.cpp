// Behaviour test for 0x00AF9310 CPSCRenderSprite::Initialise: a sprite primitive (type 0x1b, sprite bank, flags,
// blend mode/op) and a trail primitive (type 0x1a, trail bank, flags, trail blend mode/op) are created once each when
// their bank is set; the component is marked initialised; a null engine leaves it untouched.
#include <cstdio>

struct SInfo { long Type, Reserved; bool A, B, C; long BankIndex; unsigned short Flags; unsigned long BlendMode, BlendOp; };
static SInfo g_last[4]; static int g_creates;
struct CIEngine { long CreateChildPrimitive(void* parent, const SInfo* info, long) { g_last[g_creates] = *info; return 100 + g_creates++; } };
struct CPSCRenderSprite {
    bool Initialised; long SpriteBankIndex, TrailBankIndex;
    unsigned long BlendMode : 3, TrailBlendMode : 3, BlendOp : 3, TrailBlendOp : 3, Flags : 10;
    long ChildPrimIndex, ChildTrailPrimIndex;
    bool Initialise(CIEngine* engine, void* parent)
    {
        if (engine) {
            unsigned long trailBlendOp = TrailBlendOp;
            unsigned long trailBlendMode = TrailBlendMode;
            SInfo info;
            if (SpriteBankIndex && ChildPrimIndex == -1) {
                info.Type = 0x1b; info.Reserved = 0; info.A = info.B = info.C = false;
                info.BankIndex = SpriteBankIndex; info.Flags = (unsigned short)Flags; info.BlendMode = BlendMode; info.BlendOp = BlendOp;
                ChildPrimIndex = engine->CreateChildPrimitive(parent, &info, 0);
            }
            if (TrailBankIndex && ChildTrailPrimIndex == -1) {
                info.Type = 0x1a; info.Reserved = 0; info.A = info.B = info.C = false;
                info.BankIndex = TrailBankIndex; info.Flags = (unsigned short)Flags; info.BlendMode = trailBlendMode; info.BlendOp = trailBlendOp;
                ChildTrailPrimIndex = engine->CreateChildPrimitive(parent, &info, 0);
            }
            Initialised = true;
        }
        return Initialised;
    }
};
int main()
{
    CPSCRenderSprite s; s.Initialised = false; s.SpriteBankIndex = 5; s.TrailBankIndex = 6;
    s.BlendMode = 1; s.TrailBlendMode = 2; s.BlendOp = 3; s.TrailBlendOp = 4; s.Flags = 0x155; s.ChildPrimIndex = -1; s.ChildTrailPrimIndex = -1;
    CIEngine engine;
    if (s.Initialise(0, 0) || g_creates) { std::printf("00af9310_TEST FAIL\n"); return 1; }
    if (!s.Initialise(&engine, 0) || g_creates != 2 || s.ChildPrimIndex != 100 || s.ChildTrailPrimIndex != 101) { std::printf("00af9310_TEST FAIL\n"); return 1; }
    if (g_last[0].Type != 0x1b || g_last[0].BankIndex != 5 || g_last[0].Flags != 0x155 || g_last[0].BlendMode != 1 || g_last[0].BlendOp != 3) { std::printf("00af9310_TEST FAIL\n"); return 1; }
    if (g_last[1].Type != 0x1a || g_last[1].BankIndex != 6 || g_last[1].Flags != 0x155 || g_last[1].BlendMode != 2 || g_last[1].BlendOp != 4) { std::printf("00af9310_TEST FAIL\n"); return 1; }
    s.Initialise(&engine, 0);
    if (g_creates != 2) { std::printf("00af9310_TEST FAIL\n"); return 1; }
    std::printf("00af9310_TEST PASS\n");
    return 0;
}
