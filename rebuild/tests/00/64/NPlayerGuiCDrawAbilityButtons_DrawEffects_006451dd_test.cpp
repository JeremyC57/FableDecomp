// Behaviour test for NPlayerGui::CDrawAbilityButtons::DrawEffects (0x006451DD): a model of the row -- an ability with an icon
// effect fills an empty emitter slot with an emitter at position + offset; a filled slot or a zero effect leaves it alone.
#include <stdio.h>
struct Emitter { long Type; float X, Y; };
static Emitter g_pool[4]; static int g_made = 0;
static Emitter* Create2DEmitter(long type, float x, float y) { Emitter* e = &g_pool[g_made++]; e->Type = type; e->X = x; e->Y = y; return e; }
struct Ability { long IconEffectIndex; float OffX, OffY; };
struct Buttons { Emitter* Effects[3];
    void DrawEffects(long index, float x, float y, const Ability& a)
    {
        if (a.IconEffectIndex > 0)
            if (Effects[index] == 0)
                Effects[index] = Create2DEmitter(a.IconEffectIndex, x + a.OffX, y + a.OffY);
    }
};
int main()
{
    Buttons b = { { 0, 0, 0 } };
    Ability a = { 7, 1.0f, 2.0f }, none = { 0, 0, 0 };
    b.DrawEffects(1, 10.0f, 20.0f, a);
    int ok = b.Effects[1] && b.Effects[1]->Type == 7 && b.Effects[1]->X == 11.0f && b.Effects[1]->Y == 22.0f && g_made == 1;
    b.DrawEffects(1, 0.0f, 0.0f, a);
    ok = ok && g_made == 1;
    b.DrawEffects(2, 0.0f, 0.0f, none);
    ok = ok && b.Effects[2] == 0 && g_made == 1;
    printf(ok ? "006451dd_TEST PASS\n" : "006451dd_TEST FAIL\n");
    return ok ? 0 : 1;
}