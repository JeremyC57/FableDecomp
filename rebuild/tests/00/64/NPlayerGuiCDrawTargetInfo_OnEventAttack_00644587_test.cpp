// Behaviour test for 0x00644587 NPlayerGui::CDrawTargetInfo::OnEventAttack: open the attack fade over the def duration, no fade-out, to opaque black (standalone copy of the logic)
#include <cstdio>
struct Colour { unsigned char B, G, R, A; };
struct Fade { bool Active, Opening, Closing; float In, Out; Colour To;
    void Open(float in, float out, Colour c) { Active = true; Opening = true; Closing = false; In = in; Out = out; To = c; } };
struct Info { Fade Attack, Block; void OnEventAttack(float duration) { Colour c; c.R = 0; c.G = 0; c.B = 0; c.A = 255; Attack.Open(duration, 0.0f, c); } };

int main()
{
    Info i; i.Attack.Active = false; i.Block.Active = false; i.Attack.Closing = true;
    i.OnEventAttack(0.25f);
    if (!i.Attack.Active || !i.Attack.Opening || i.Attack.Closing || i.Attack.In != 0.25f || i.Attack.Out != 0.0f) { std::printf("00644587_TEST FAIL\n"); return 1; }
    if (i.Attack.To.A != 255 || i.Attack.To.R || i.Attack.To.G || i.Attack.To.B || i.Block.Active) { std::printf("00644587_TEST FAIL\n"); return 1; }
    std::printf("00644587_TEST PASS\n");
    return 0;
}