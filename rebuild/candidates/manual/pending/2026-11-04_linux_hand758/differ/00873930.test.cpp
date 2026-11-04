// Behaviour test for NCameraModes::CLookAtFront::CLookAtFrontInit::Alloc (0x00873930), g++ model: camera at position + facing * string length, raised by the height offset, looking at the focal point.
#include <cstdio>
struct V3 { float X, Y, Z; };
struct Mode { V3 pos, focus; bool flag; };
static Mode Alloc(const V3& thingPos, const V3& facing, const V3& focal, float stringLength, float heightOffset)
{
    V3 offset = { facing.X * stringLength, facing.Y * stringLength, facing.Z * stringLength };
    offset.X += thingPos.X; offset.Y += thingPos.Y; offset.Z += thingPos.Z;
    offset.Z += heightOffset;
    Mode m = { offset, focal, true };
    return m;
}
int main()
{
    V3 p = { 10, 20, 1 }, f = { 0, 1, 0 }, foc = { 10, 20, 2.5f };
    Mode m = Alloc(p, f, foc, 4.0f, 1.5f);
    int ok = m.pos.X == 10 && m.pos.Y == 24 && m.pos.Z == 2.5f && m.focus.Z == 2.5f && m.flag;
    std::printf(ok ? "00873930_TEST PASS\n" : "00873930_TEST FAIL\n");
    return ok ? 0 : 1;
}
