#include <cstdio>
// CModeJackOfBladesFloat::OnDeactivate model: kill each live, not-yet-dead emitter.
class CThing { public: bool dead; int kills; void Kill(bool) { ++kills; dead = true; } };
struct CIP { CThing* item; CThing* GetPItem() const { return item; } };
struct CModeJackOfBladesFloat {
    CIP PEmitter;
    CIP PEmitter2;
    void OnDeactivate()
    {
        if (PEmitter.GetPItem() && !PEmitter.GetPItem()->dead)
            PEmitter.GetPItem()->Kill(true);
        if (PEmitter2.GetPItem() && !PEmitter2.GetPItem()->dead)
            PEmitter2.GetPItem()->Kill(true);
    }
};
int main()
{
    CThing a = { false, 0 }, b = { true, 0 };
    CModeJackOfBladesFloat m; m.PEmitter.item = &a; m.PEmitter2.item = &b;
    m.OnDeactivate();
    CModeJackOfBladesFloat n; n.PEmitter.item = 0; n.PEmitter2.item = 0;
    n.OnDeactivate();
    if (!(a.kills == 1 && a.dead && b.kills == 0)) { std::printf("008d7730_TEST FAIL\n"); return 1; }
    std::printf("008d7730_TEST PASS\n");
    return 0;
}