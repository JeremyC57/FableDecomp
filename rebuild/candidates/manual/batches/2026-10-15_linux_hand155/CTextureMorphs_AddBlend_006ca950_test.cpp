// Behaviour test for 0x006CA950: AddBlend finds or creates the texture's morph (with a white base layer), clears on a base-type blend, appends the blend.
#include <cstdio>
struct Blend { unsigned int tex; int type; unsigned int diffuse, spec; };
struct Morph { int tex; int id; Blend blends[8]; int n; };
struct Morphs { Morph m[4]; int n;
  Morph* AddMorph(unsigned int tex) { Morph& r = m[n++]; r.tex = (int)tex; r.id = 0; r.n = 0; return &r; }
  void AddBlend(unsigned int tex, const Blend& b) {
    Morph* morph = 0; for (int i = 0; i < n; ++i) if (m[i].tex == (int)tex) { morph = &m[i]; break; }
    if (!morph) { morph = AddMorph(tex); Blend base = {tex, 3, 0xffffffffu, 0}; morph->blends[morph->n++] = base; }
    if (b.type == 3) morph->n = 0;
    morph->blends[morph->n++] = b; } };
int main()
{
    Morphs ms; ms.n = 0;
    Blend b1 = {5, 1, 1, 2}, b2 = {5, 2, 3, 4}, b3 = {5, 3, 5, 6}, b4 = {7, 1, 7, 8};
    ms.AddBlend(5, b1);
    bool ok = ms.n == 1 && ms.m[0].n == 2 && ms.m[0].blends[0].type == 3 && ms.m[0].blends[0].diffuse == 0xffffffffu && ms.m[0].blends[0].spec == 0 && ms.m[0].blends[1].diffuse == 1;
    ms.AddBlend(5, b2); ok = ok && ms.n == 1 && ms.m[0].n == 3;
    ms.AddBlend(5, b3); ok = ok && ms.m[0].n == 1 && ms.m[0].blends[0].diffuse == 5;
    ms.AddBlend(7, b4); ok = ok && ms.n == 2 && ms.m[1].n == 2;
    if (!ok) { std::printf("006ca950_TEST FAIL\n"); return 1; }
    std::printf("006ca950_TEST PASS\n");
    return 0;
}
