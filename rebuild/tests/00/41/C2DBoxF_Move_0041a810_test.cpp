// test for 0041a810 C2DBoxF::Move
#include <stdio.h>
class C2DVector {
public:
    float X, Y;
};
class C2DBoxF {
public:
    void Move(const C2DVector& pos);
    float TLX, TLY;                                      // +0x00
    float BRX, BRY;                                      // +0x08
};
void C2DBoxF::Move(const C2DVector& pos)
{
    BRX += pos.X - TLX;
    BRY += pos.Y - TLY;
    TLX = pos.X;
    TLY = pos.Y;
}

int main()
{
    C2DBoxF b; b.TLX = 1.0f; b.TLY = 2.0f; b.BRX = 5.0f; b.BRY = 8.0f;
    C2DVector p = { 10.0f, -4.0f };
    b.Move(p);
    int ok = b.TLX == 10.0f && b.TLY == -4.0f && b.BRX == 14.0f && b.BRY == 2.0f;
    printf(ok ? "0041a810_TEST PASS\n" : "0041a810_TEST FAIL\n");
    return ok ? 0 : 1;
}