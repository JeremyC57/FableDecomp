// Behaviour test for 0x00641AE0 CTCLight::DrawFlare: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <vector>
struct Flare { long bank; long id; unsigned char alpha; };
struct Light { bool overridden; long halo; std::vector<int> sprites; Flare last; int added; };
static void DrawFlare(Light& l, long index, float fade, unsigned long long uid)
{
    if ((unsigned long)index >= l.sprites.size()) { l.sprites.resize(index + 1, 0); l.sprites[index] = ++l.added; }
    long halo;
    if (!l.overridden && (halo = l.halo) > 0) { l.last.bank = halo; l.last.id = (int)(unsigned int)uid; l.last.alpha = (unsigned char)fade; }
}
int main() {
    Light l; l.overridden = false; l.halo = 9; l.added = 0; l.last.bank = 0;
    DrawFlare(l, 2, 200.0f, 0x100000005ULL);
    int ok = l.sprites.size() == 3 && l.sprites[2] == 1 && l.last.bank == 9 && l.last.id == 5 && l.last.alpha == 200;
    l.overridden = true; l.last.bank = 0; DrawFlare(l, 1, 10.0f, 1); ok &= l.last.bank == 0 && l.added == 1;
    std::printf(ok ? "00641ae0_TEST PASS\n" : "00641ae0_TEST FAIL\n");
    return ok ? 0 : 1;
}
