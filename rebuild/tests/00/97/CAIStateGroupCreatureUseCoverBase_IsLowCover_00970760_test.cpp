#include <cstdio>

struct Ph { unsigned char pad[0x5c]; float Height; };
struct Th { unsigned char pad[0x70]; Ph* PhysicsTC; };
struct G { bool IsLowCover(Th* t) { if (t->PhysicsTC->Height < 1.2f) return true; return false; } };
int main()
{
    Ph p; Th t; t.PhysicsTC = &p; G g; p.Height = 0.8f; if (!g.IsLowCover(&t)) { std::printf("00970760_TEST FAIL\n"); return 1; }
    p.Height = 1.2f; if (g.IsLowCover(&t)) { std::printf("00970760_TEST FAIL\n"); return 1; }
    std::printf("00970760_TEST PASS\n");
    return 0;
}