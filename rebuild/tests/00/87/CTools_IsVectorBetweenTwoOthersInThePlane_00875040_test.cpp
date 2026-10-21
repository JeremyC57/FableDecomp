// Behaviour test for 00875040: src/00875040.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTools::IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTools::IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v) -- with all three directions normalised: true when v is at least as close in angle to a as b is, i.e. a.b >= v.b and a.v >= v.b
// Leaf: IsVectorBetweenTwoOthersInThePlane
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875040, length 220 bytes (`ret 0Ch`, `this` unused; 224 with padding).
// Callees: C3DVector::Normalise @ 0x00A14440 (x3, on frame copies).
// Shape: each direction is bound to a const reference to an inline Normalised() copy (a named copy normalised in
// place and returned by value); the reference-bound temporaries get retail's frame slots (b at +0, v at +0xc, a at +0x18),
// named copies swap a and v. The v.b dot product is a named float kept on the FPU across both compares (`>=`);
// each dot product sums x, y, z in source (VC emits z first).
struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
    void Normalise();                                       // 0x00A14440
    C3DVector Normalised() const { C3DVector r(*this); r.Normalise(); return r; }
};

struct CTools {
    bool IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v);
};

bool CTools::IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v)
{
    const C3DVector& na = a.Normalised();
    const C3DVector& nb = b.Normalised();
    const C3DVector& nv = v.Normalised();
    float vb = nb.x * nv.x + nb.y * nv.y + nb.z * nv.z;
    if (na.x * nb.x + na.y * nb.y + na.z * nb.z >= vb && na.x * nv.x + na.y * nv.y + na.z * nv.z >= vb)
        return true;
    return false;
}

// ---- stubs and driver ----
#include <cmath>
static int g_normalises = 0;
void C3DVector::Normalise()
{
    ++g_normalises;
    float len = (float)sqrt(x * x + y * y + z * z);
    if (len > 0) { x /= len; y /= len; z /= len; }
}
int main()
{
    CTools t;
    float ax[3] = { 1, 0, 0 }, bx[3] = { 0, 1, 0 }, v1[3] = { 2, 2, 0 }, v2[3] = { -1, 3, 0 }, v3[3] = { 5, 0.1f, 0 };
    const C3DVector& a = *(const C3DVector*)ax;
    const C3DVector& b = *(const C3DVector*)bx;
    float ac[3] = { 3, 0, 0 };
    // v halfway between a and b: a.b = 0 >= v.b = 0.707? no -> false
    CHECK(!t.IsVectorBetweenTwoOthersInThePlane(a, b, *(const C3DVector*)v1));
    CHECK(g_normalises == 3);
    // v nearly along a: v.b ~ 0.02, a.b = 0 -> false (a.b must reach v.b)
    CHECK(!t.IsVectorBetweenTwoOthersInThePlane(a, b, *(const C3DVector*)v3));
    // b = v2 (mostly +y, a little -x), v = (1,1): v.b = (1*-1 + 1*3)/(sqrt2*sqrt10) = 0.447; a.b = -1/sqrt10 = -0.316 -> false
    CHECK(!t.IsVectorBetweenTwoOthersInThePlane(a, *(const C3DVector*)v2, *(const C3DVector*)v1));
    // b opposite-ish to a: b = (-1, 0.2, 0): a.b = -0.98; v = (0, 1, 0): v.b = 0.196 -> false; v = (-1,-1,0): v.b = 0.56 -> false
    // a = b direction: a.b = 1 >= v.b, a.v = v.b -> true (inputs unscaled: normalisation must happen)
    CHECK(t.IsVectorBetweenTwoOthersInThePlane(*(const C3DVector*)ac, *(const C3DVector*)ax, *(const C3DVector*)v1));
    // a=(1,0,0), b=(-1,0,0), v=(0,1,0): a.b=-1, v.b=0 -> false; v=(-1,0.0,0)=b: v.b=1 -> false
    // a=(0,1,0), b=(1,0,0) scaled, v=(1,1,0): a.b=0, v.b=0.707 -> false; swap a and v roles: a=(1,1,0), b=(0,1,0), v=(1,0,0):
    // a.b=0.707 >= v.b=0 and a.v=0.707 >= 0 -> true
    float a2[3] = { 4, 4, 0 }, b2[3] = { 0, 7, 0 }, vv[3] = { 9, 0, 0 };
    CHECK(t.IsVectorBetweenTwoOthersInThePlane(*(const C3DVector*)a2, *(const C3DVector*)b2, *(const C3DVector*)vv));
    // the caller's vectors are untouched (normalised copies)
    CHECK(a2[0] == 4 && b2[1] == 7 && vv[0] == 9);
    // a.v below v.b: a=(1,0,0), b=(1,1,0), v=(0,1,0) -> v.b=0.707, a.b=0.707 >= ok, a.v=0 < 0.707 -> false
    float a3[3] = { 1, 0, 0 }, b3[3] = { 1, 1, 0 }, v3b[3] = { 0, 1, 0 };
    CHECK(!t.IsVectorBetweenTwoOthersInThePlane(*(const C3DVector*)a3, *(const C3DVector*)b3, *(const C3DVector*)v3b));
    if (fails == 0) printf("00875040_TEST PASS\n");
    return fails;
}
