// Behaviour test for 0x00550A70 CEnginePrimitive2DAnimatingMesh::CEnginePrimitive2DAnimatingMesh (model copy).
#include <cstdio>

struct CAnimRes { long RefCount; };
struct CBlend { CAnimRes* Animation; CBlend(CAnimRes* a) : Animation(a) {} CBlend(const CBlend& o) : Animation(o.Animation) { if (Animation) ++Animation->RefCount; } };
struct CAnimInterp { CBlend StartState, EndState; CAnimInterp(const CBlend& s) : StartState(s), EndState(s) {} };
struct CBox { float a, b, c, d; };
struct CSphere { float x, y, z, r; };
struct COrient { float v[12]; };
struct CRGB { unsigned char B, G, R, A; };
struct CMesh2D {
    long Type, InterpolationSet; unsigned char t0, t1, t2;
    CBox ClipBox; unsigned long MeshIndex; unsigned char Alpha; COrient Orientation; CAnimInterp Anim; CSphere BoundingSphere; CRGB Tint;
    CMesh2D(unsigned long mesh, const CBlend& anim, const CBox& box, const CSphere& s, const COrient& o, long interp, unsigned char alpha, CRGB tint)
        : Type(0x25), InterpolationSet(interp), t0(0), t1(0), t2(0), ClipBox(box), MeshIndex(mesh), Alpha(alpha), Orientation(o),
          Anim(anim), BoundingSphere(s) { Tint.R = tint.R; Tint.G = tint.G; Tint.B = tint.B; Tint.A = tint.A; }
};

int main()
{
    CAnimRes res; res.RefCount = 1;
    CBlend blend(&res);
    CBox box = { 1, 2, 3, 4 }; CSphere s = { 5, 6, 7, 8 }; COrient o; for (int i = 0; i < 12; ++i) o.v[i] = (float)i;
    CRGB tint = { 10, 20, 30, 40 };
    CMesh2D m(77, blend, box, s, o, 3, 0x80, tint);
    if (m.Type != 0x25 || m.InterpolationSet != 3 || m.t1 != 0 || m.ClipBox.d != 4 || m.MeshIndex != 77 || m.Alpha != 0x80 ||
        m.Orientation.v[11] != 11.0f || m.Anim.StartState.Animation != &res || m.Anim.EndState.Animation != &res ||
        res.RefCount != 3 || m.BoundingSphere.r != 8 || m.Tint.B != 10 || m.Tint.A != 40)
    { std::printf("00550a70_TEST FAIL\n"); return 1; }
    std::printf("00550a70_TEST PASS\n");
    return 0;
}