#include <cstdio>
// Behaviour model of CTCInventoryBase::GetRightStickPosition (0x005BC5C6): with a right-stick event queued for the
// main player the out vector is the event's analogue vector, else (0, 0).
struct V2 { float x, y; };
static void Get(bool queued, float ex, float ey, V2& out)
{
    if (queued) { out.x = ex; out.y = ey; }
    else { out.x = 0.0f; out.y = 0.0f; }
}
int main()
{
    V2 v = { 9.0f, 9.0f };
    Get(true, 0.5f, -0.25f, v);
    if (v.x != 0.5f || v.y != -0.25f) { std::printf("005bc5c6_TEST FAIL\n"); return 1; }
    Get(false, 0.5f, -0.25f, v);
    if (v.x != 0.0f || v.y != 0.0f) { std::printf("005bc5c6_TEST FAIL\n"); return 1; }
    std::printf("005bc5c6_TEST PASS\n");
    return 0;
}
