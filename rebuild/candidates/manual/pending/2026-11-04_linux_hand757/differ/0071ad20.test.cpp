// Behaviour test for 0x0071AD20 CTCEmoteIcon::DrawEmoteTypeIcon: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct Icon { bool valid; int placed; int released; float z; };
static void Draw(Icon& i, float camDistSq, float drawDist, bool focal, float focalZ)
{
    if (camDistSq < drawDist * drawDist) {
        if (focal) { i.z = focalZ + 0.5f; if (i.valid) i.released++; i.placed++; i.valid = true; }
    } else if (i.valid) { i.released++; i.valid = false; }
}
int main() {
    Icon i = { false, 0, 0, 0.0f };
    Draw(i, 4.0f, 3.0f, true, 1.0f); int ok = i.placed == 1 && i.valid && i.z == 1.5f && i.released == 0;
    Draw(i, 4.0f, 3.0f, true, 2.0f); ok &= i.placed == 2 && i.released == 1;
    Draw(i, 16.0f, 3.0f, true, 2.0f); ok &= !i.valid && i.released == 2;
    std::printf(ok ? "0071ad20_TEST PASS\n" : "0071ad20_TEST FAIL\n");
    return ok ? 0 : 1;
}
