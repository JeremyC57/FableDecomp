#include <cstdio>
struct CMatrix3x4 { float m[4][3]; };
struct C3DDummyObject { CMatrix3x4 Transform; long BoneIndex; C3DDummyObject(const CMatrix3x4& t, long b) : Transform(t), BoneIndex(b) { } };
int main()
{
    CMatrix3x4 t; for (int r = 0; r < 4; ++r) for (int c = 0; c < 3; ++c) t.m[r][c] = (float)(r * 10 + c);
    C3DDummyObject d(t, 7);
    if (sizeof(C3DDummyObject) != 0x34 || d.BoneIndex != 7) { std::printf("00a2e490_TEST FAIL\n"); return 1; }
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 3; ++c) if (d.Transform.m[r][c] != (float)(r * 10 + c)) { std::printf("00a2e490_TEST FAIL\n"); return 1; }
    std::printf("00a2e490_TEST PASS\n"); return 0;
}
