#include <cstdio>
class C3DVector { public:
    float x, y, z;
    C3DVector(float a, float b, float c) : x(a), y(b), z(c) { }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};
class CRightHandedSet { public:
    C3DVector f, u; int calls;
    CRightHandedSet() : f(0, 0, 0), u(0, 0, 0), calls(0) { }
    void SetOrientationVectors(const C3DVector& forward, const C3DVector& up) { f = forward; u = up; ++calls; }
};
static void SetOrientationVectorsFromCopies(CRightHandedSet* set, const C3DVector* forward, const C3DVector* up)
{
    set->SetOrientationVectors(C3DVector(*forward), C3DVector(*up));
}
int main()
{
    CRightHandedSet s; C3DVector a(1, 2, 3), b(4, 5, 6);
    SetOrientationVectorsFromCopies(&s, &a, &b);
    if (s.calls != 1 || s.f.x != 1 || s.f.z != 3 || s.u.x != 4 || s.u.y != 5 || s.u.z != 6) { std::printf("00c33920_TEST FAIL\n"); return 1; }
    std::printf("00c33920_TEST PASS\n");
    return 0;
}