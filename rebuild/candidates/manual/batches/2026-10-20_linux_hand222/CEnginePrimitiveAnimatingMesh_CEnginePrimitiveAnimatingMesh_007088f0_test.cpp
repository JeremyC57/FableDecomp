// Behaviour test for 0x007088F0 CEnginePrimitiveAnimatingMesh::CEnginePrimitiveAnimatingMesh (model copy).
#include <cstdio>

struct CAnimRes { long RefCount; };
struct CBlend { CAnimRes* Animation; CBlend(CAnimRes* a) : Animation(a) {} CBlend(const CBlend& o) : Animation(o.Animation) { if (Animation) ++Animation->RefCount; } };
struct CAnimInterp { CBlend StartState, EndState; CAnimInterp(CAnimRes* a, CAnimRes* b) : StartState(a), EndState(b) {} };
struct CPos { float v[6]; };
struct COrient { float v[12]; };
struct CRGB { unsigned char B, G, R, A; };
struct CTheme { unsigned char a, b, c, d; };
struct CMesh {
    long Type, InterpolationSet; unsigned char t0, t1, t2;
    unsigned long MeshIndex; float Size, FadeStart; unsigned char Alpha; unsigned char Lit : 1;
    CPos WorldPos; COrient Orientation; CAnimInterp Anim; void* MeshEffect; CTheme Theme; CRGB Tint; bool Bullet;
    CMesh(unsigned long mesh, const CAnimInterp& anim, const CPos& p, const COrient& o, float size, bool lit, unsigned char alpha,
          CRGB tint, float fade, long interp, const CTheme& theme, bool bullet)
        : Type(9), InterpolationSet(interp), t0(0), t1(0), t2(0), MeshIndex(mesh), Size(size), FadeStart(fade), Alpha(alpha), Lit(lit),
          WorldPos(p), Orientation(o), Anim(anim), MeshEffect(0), Theme(theme), Bullet(bullet)
    { Tint.R = tint.R; Tint.G = tint.G; Tint.B = tint.B; Tint.A = tint.A; }
};

int main()
{
    CAnimRes a; a.RefCount = 1; CAnimRes b; b.RefCount = 1;
    CAnimInterp anim(&a, &b);
    CPos p; for (int i = 0; i < 6; ++i) p.v[i] = (float)i;
    COrient o; for (int i = 0; i < 12; ++i) o.v[i] = (float)(i * 2);
    CRGB tint = { 1, 2, 3, 4 }; CTheme th = { 5, 6, 7, 8 };
    CMesh m(42, anim, p, o, 1.5f, true, 0x7f, tint, 9.0f, 2, th, true);
    if (m.Type != 9 || m.InterpolationSet != 2 || m.MeshIndex != 42 || m.Size != 1.5f || m.FadeStart != 9.0f || m.Alpha != 0x7f ||
        m.Lit != 1 || m.WorldPos.v[5] != 5.0f || m.Orientation.v[11] != 22.0f || a.RefCount != 2 || b.RefCount != 2 ||
        m.MeshEffect != 0 || m.Theme.c != 7 || m.Tint.R != 3 || !m.Bullet)
    { std::printf("007088f0_TEST FAIL\n"); return 1; }
    CMesh n(1, anim, p, o, 0.0f, false, 0, tint, 0.0f, 0, th, false);
    if (n.Lit != 0 || n.Bullet) { std::printf("007088f0_TEST FAIL\n"); return 1; }
    std::printf("007088f0_TEST PASS\n");
    return 0;
}
