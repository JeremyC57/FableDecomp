// Behaviour test for 0x00752E00 (parked row): the row's logic over a flat model.
#include <cstdio>

struct Look { bool Turning; float Target, Turn, HeadRange, Body; long Flags; long State; float Vel;
    void SetBodyMoving() {
        if (Turning) {
            float target = Target;
            if (!((Flags & 1) && !(Flags & 6)) && !(Flags & 0x80)) target = target - (Turn < 0 ? -1.0f : 1.0f) * HeadRange * 0.25f;
            Body = target; return;
        }
        long s = State | 8; if (State != s) State = s; Vel = 0.0f;
    } };
int main() {
    Look l = { true, 0.5f, 0.1f, 0.4f, 0, 0, 0, 1 }; l.SetBodyMoving();
    bool ok = l.Body == 0.4f;
    l.Flags = 1; l.SetBodyMoving(); ok = ok && l.Body == 0.5f;
    l.Turning = false; l.SetBodyMoving(); ok = ok && l.State == 8 && l.Vel == 0.0f;
    std::printf(ok ? "00752e00_TEST PASS\n" : "00752e00_TEST FAIL\n");
    return ok ? 0 : 1;
}
