// behaviour test for 0x00405db0 CActionInputControl::IsSameButton: same controller type, then the button of that type
// (3 mouse, 2 keyboard, 1 xbox) must match; other types compare only the controller type
#include <cstdio>
struct Ctl { long GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton; float X, Y; };
static bool IsSameButton(const Ctl& a, const Ctl& b)
{
    if (a.ControllerType != b.ControllerType) return false;
    switch (a.ControllerType) {
    case 3: if (a.MouseButton != b.MouseButton) return false; break;
    case 2: if (a.KeyboardKey != b.KeyboardKey) return false; break;
    case 1: if (a.XboxButton != b.XboxButton) return false; break;
    }
    return true;
}
static Ctl Make(long type, long key, long xbox, long mouse)
{
    Ctl c; c.GameAction = 9; c.ControllerType = type; c.KeyboardKey = key; c.XboxButton = xbox; c.MouseButton = mouse; c.X = c.Y = 0;
    return c;
}
int main()
{
    int fails = 0;
    if (IsSameButton(Make(1, 0, 5, 0), Make(2, 0, 5, 0))) fails |= 1;
    if (!IsSameButton(Make(3, 1, 2, 7), Make(3, 9, 9, 7))) fails |= 2;
    if (IsSameButton(Make(3, 1, 2, 7), Make(3, 1, 2, 8))) fails |= 4;
    if (!IsSameButton(Make(2, 4, 0, 0), Make(2, 4, 1, 1))) fails |= 8;
    if (IsSameButton(Make(2, 4, 0, 0), Make(2, 5, 0, 0))) fails |= 16;
    if (!IsSameButton(Make(1, 0, 6, 0), Make(1, 3, 6, 3))) fails |= 32;
    if (IsSameButton(Make(1, 0, 6, 0), Make(1, 0, 7, 0))) fails |= 64;
    if (!IsSameButton(Make(0, 1, 2, 3), Make(0, 4, 5, 6))) fails |= 128;
    if (fails) { std::printf("00405db0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00405db0_TEST PASS\n");
    return 0;
}