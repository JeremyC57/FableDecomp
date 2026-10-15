// Behaviour test for 0x009DC500: a quad becomes triangles (v0,v1,v2) and (v0,v2,v3); opaque AddTri with mode 1, AddAlphaTri otherwise.
#include <cstdio>
struct V { int id; };
struct Tex { int id; };
struct R { int n; int tri[4][3]; int alpha[4]; int mode[4]; Tex* tex[4];
  void rec(const V& a, const V& b, const V& c, int m, Tex* t, int al) { tri[n][0] = a.id; tri[n][1] = b.id; tri[n][2] = c.id; mode[n] = m; tex[n] = t; alpha[n] = al; ++n; }
  void AddTri(const V& a, const V& b, const V& c, int m, Tex* t) { rec(a, b, c, m, t, 0); }
  void AddAlphaTri(const V& a, const V& b, const V& c, int m, Tex* t) { rec(a, b, c, m, t, 1); }
  void AddQuad(const V& v0, const V& v1, const V& v2, const V& v3, unsigned int m, Tex* t) {
    if (m == 1) { AddTri(v0, v1, v2, 1, t); AddTri(v0, v2, v3, 1, t); } else { AddAlphaTri(v0, v1, v2, m, t); AddAlphaTri(v0, v2, v3, m, t); } } };
int main()
{
    V a = {0}, b = {1}, c = {2}, d = {3}; Tex t = {9};
    R r; r.n = 0; r.AddQuad(a, b, c, d, 1, &t); r.AddQuad(a, b, c, d, 4, 0);
    bool ok = r.n == 4 && r.alpha[0] == 0 && r.alpha[1] == 0 && r.tri[1][1] == 2 && r.tri[1][2] == 3 && r.mode[0] == 1 && r.tex[1] == &t
           && r.alpha[2] == 1 && r.mode[3] == 4 && r.tri[2][0] == 0 && r.tri[2][2] == 2 && r.tex[3] == 0;
    if (!ok) { std::printf("009dc500_TEST FAIL\n"); return 1; }
    std::printf("009dc500_TEST PASS\n");
    return 0;
}
