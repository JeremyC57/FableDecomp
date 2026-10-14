#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; C3DVector() : X(0), Y(0), Z(0) { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
static const void* g_copied;
struct CCreatureAction_BlockBase { virtual void v00() {} unsigned char _body[0x124 - 4];
    CCreatureAction_BlockBase() { std::memset(_body, 0, sizeof(_body)); }
    CCreatureAction_BlockBase(const CCreatureAction_BlockBase& o) { g_copied = &o; std::memcpy(_body, o._body, sizeof(_body)); } };
#pragma pack(push, 1)
struct CCreatureAction_BlockRespond : CCreatureAction_BlockBase { unsigned char UseHitPos; C3DVector HitPos; unsigned char FaceTargetAtStart; virtual void v00() {}
    CCreatureAction_BlockRespond() : UseHitPos(0), HitPos(), FaceTargetAtStart(0) { }
    CCreatureAction_BlockRespond(const CCreatureAction_BlockRespond& other) : CCreatureAction_BlockBase(other), UseHitPos(other.UseHitPos), HitPos(other.HitPos), FaceTargetAtStart(other.FaceTargetAtStart) { } };
#pragma pack(pop)
int main()
{
    CCreatureAction_BlockRespond a; a.UseHitPos = 1; a.HitPos.X = 1.5f; a.HitPos.Y = -2.0f; a.HitPos.Z = 8.0f; a.FaceTargetAtStart = 1; a._body[3] = 7;
    CCreatureAction_BlockRespond b(a);
    if (g_copied != &a || b.UseHitPos != 1 || b.HitPos.X != 1.5f || b.HitPos.Y != -2.0f || b.HitPos.Z != 8.0f || b.FaceTargetAtStart != 1 || b._body[3] != 7 || (char*)&b.HitPos - (char*)&b != 0x125 || (char*)&b.FaceTargetAtStart - (char*)&b != 0x131) { std::printf("00855e80_TEST FAIL\n"); return 1; }
    std::printf("00855e80_TEST PASS\n");
    return 0;
}