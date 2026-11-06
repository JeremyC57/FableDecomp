// Behaviour test for CTCFishing::FrameUpdate (0x007B0CD0), the hook and reel stages: the hook stage moves on to reeling once the
// presses reach zero and back to waiting (button alpha 0x80) when the hook window runs out; while reeling the fish pulls the line
// out by Speed / fps per frame unless the player presses, the strain falls by the rod's reduction while resting (clamped at 0), and
// the fish gets away once the distance reaches the rod's maximum length. A state model.
#include <cstdio>
struct Fishing {
    long Stage, Frame, FrameStageFinishes, HookPresses; unsigned char Alpha;
    bool Pulling, Pressed; float Distance, Strain; bool GotAway;
    void Hook()
    {
        if (Frame >= FrameStageFinishes || HookPresses == 0) {
            if (HookPresses == 0) Stage = 4;
            else { Stage = 2; Alpha = 0x80; }
        }
    }
    void Reel(float speed, long fps, float reduction, float maxLength)
    {
        if (!Pressed) {
            if (Pulling) { Alpha = 0x80; Distance += speed / fps; }
            else {
                Alpha = 0xff;
                if (Strain > 0.0f) { Strain -= reduction; if (Strain < 0.0f) Strain = 0.0f; }
            }
        }
        if (Distance >= maxLength) GotAway = true;
    }
};
int main()
{
    Fishing f = { 3, 10, 20, 0, 0xff, false, false, 5.0f, 0.25f, false };
    f.Hook();
    bool ok = f.Stage == 4;
    Fishing g = { 3, 21, 20, 2, 0xff, false, false, 5.0f, 0.25f, false };
    g.Hook();
    ok = ok && g.Stage == 2 && g.Alpha == 0x80;
    Fishing h = { 3, 10, 20, 2, 0xff, false, false, 5.0f, 0.25f, false };
    h.Hook();
    ok = ok && h.Stage == 3;
    f.Reel(2.0f, 4, 0.5f, 10.0f);
    ok = ok && f.Strain == 0.0f && f.Alpha == 0xff && f.Distance == 5.0f;
    f.Pulling = true;
    for (int i = 0; i < 9; ++i) f.Reel(2.0f, 4, 0.5f, 10.0f);
    ok = ok && f.Distance == 9.5f && !f.GotAway && f.Alpha == 0x80;
    f.Reel(2.0f, 4, 0.5f, 10.0f);
    ok = ok && f.GotAway;
    std::printf(ok ? "007b0cd0_TEST PASS\n" : "007b0cd0_TEST FAIL\n");
    return ok ? 0 : 1;
}