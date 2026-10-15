#include <cstdio>
struct V { float x, y, z; V& operator-=(const V& o) { x -= o.x; y -= o.y; z -= o.z; return *this; } };
struct Arr { V buf[4]; V* first; V* last;
    unsigned long size() const { return (unsigned long)(last - first); }
    V& operator[](unsigned long i) { return *(first + i); }
    void Assign(const Arr& o) { first = buf; last = buf + o.size(); for (unsigned long i = 0; i < o.size(); ++i) buf[i] = o.first[i]; } };
struct Mesh { Arr pts; V cog;
    void Get(Arr& out) const { out.Assign(pts); for (unsigned long i = 0; i < out.size(); ++i) out[i] -= cog; } };
int main()
{
    Mesh m; m.pts.first = m.pts.buf; m.pts.last = m.pts.buf + 2;
    m.pts.buf[0].x = 1.0f; m.pts.buf[0].y = 1.0f; m.pts.buf[0].z = 1.0f; m.pts.buf[1].x = 3.0f; m.pts.buf[1].y = 5.0f; m.pts.buf[1].z = 7.0f;
    m.cog.x = 1.0f; m.cog.y = 2.0f; m.cog.z = 3.0f;
    Arr out; m.Get(out);
    int ok = out.size() == 2 && out[0].x == 0.0f && out[0].y == -1.0f && out[1].z == 4.0f && m.pts.buf[1].z == 7.0f;
    if (!ok) { std::printf("00a6cb20_TEST FAIL\n"); return 1; }
    std::printf("00a6cb20_TEST PASS\n");
    return 0;
}