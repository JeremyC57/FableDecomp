// Behaviour test for NUISystem::CSprite::GetWidth (0x0041C6C0): the sprite's width is the component Size.X converted with the C
// float-to-integer truncation (__ftol2): whole values pass through, fractions are cut toward zero.
#include <cstdio>
struct C2DVector { float X, Y; };
struct Sprite {
    C2DVector Size;
    unsigned long GetWidth() { return (unsigned long)Size.X; }
};
int main()
{
    Sprite s;
    s.Size.X = 640.0f; s.Size.Y = 480.0f;
    if (s.GetWidth() != 640u) { std::printf("0041c6c0_TEST FAIL 1\n"); return 1; }
    s.Size.X = 12.75f; s.Size.Y = 7.5f;
    if (s.GetWidth() != 12u) { std::printf("0041c6c0_TEST FAIL 2\n"); return 1; }
    s.Size.X = 0.25f; s.Size.Y = 0.9f;
    if (s.GetWidth() != 0u) { std::printf("0041c6c0_TEST FAIL 3\n"); return 1; }
    std::printf("0041c6c0_TEST PASS\n");
    return 0;
}