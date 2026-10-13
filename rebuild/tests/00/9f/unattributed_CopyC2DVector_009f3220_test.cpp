#include <cstdio>
struct V { float X, Y; }; static V __fastcall Copy(const V& s, long) { return s; }
int main()
{
    V s; s.X = 1.5f; s.Y = -2.0f; V r = Copy(s, 7); if (r.X != 1.5f || r.Y != -2.0f) { std::printf("009f3220_TEST FAIL\n"); return 1; }

    std::printf("009f3220_TEST PASS\n");
    return 0;
}