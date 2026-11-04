// Behaviour test for Coin::Coin (0x008EF200), g++ model of the mass terms: IMass = density / (r^2 * 2pi), AIMass = IMass * 2.5 / r^2.
#include <cstdio>
#include <cmath>
struct Coin { bool Owned; const void* Thing; float Radius, IMass, AIMass; long Sound; };
static Coin Make(float radius, float density, const void* thing, bool owned)
{
    Coin c;
    c.Owned = owned; c.Thing = thing; c.Radius = radius;
    c.IMass = density / (radius * radius * 6.283185307);
    c.AIMass = c.IMass * 2.5 / (radius * radius);
    c.Sound = -1;
    return c;
}
int main()
{
    int t = 0;
    Coin c = Make(2.0f, 3.0f, &t, true);
    int ok = c.Owned && c.Thing == &t && c.Radius == 2.0f && std::fabs(c.IMass - 3.0 / (4.0 * 6.283185307)) < 1e-6
          && std::fabs(c.AIMass - c.IMass * 2.5 / 4.0) < 1e-6 && c.Sound == -1;
    std::printf(ok ? "008ef200_TEST PASS\n" : "008ef200_TEST FAIL\n");
    return ok ? 0 : 1;
}
