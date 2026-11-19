// Behaviour test for CCodaListener::Commit (0x00C40590), a portable model. The update bits masked with 0x38000 are OR-ed into the
// global commit mask. When bit 0x20000 is set, front and top are normalised; if their dot product lies outside [-0.05, 0.05] the top
// vector is forced perpendicular, top = OrthogonalRight(right, front), else right = OrthogonalRight(front, top), where OrthogonalRight(a, b) = (b.y*a.z - a.y*b.z, b.z*a.x - a.z*b.x, b.x*a.y - a.x*b.y).
// The update bits are always cleared.
#include <stdio.h>
#include <math.h>
struct V { float x, y, z; };
static void Normalize(V& v) { float m = (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z); if (m > 0.0f) { v.x /= m; v.y /= m; v.z /= m; } }
static void OrthogonalRight(V& o, const V& a, const V& b)
{
    o.x = b.y * a.z - a.y * b.z; o.y = b.z * a.x - a.z * b.x; o.z = b.x * a.y - a.x * b.y;
}
struct Listener {
    V front, top, right; unsigned int update;
    void Commit(unsigned int& commit)
    {
        commit |= update & 0x38000;
        if (update & 0x20000) {
            Normalize(front); Normalize(top);
            float dot = front.x * top.x + front.y * top.y + front.z * top.z;
            if (dot > 0.05f || dot < -0.05f) OrthogonalRight(top, right, front);
            else OrthogonalRight(right, front, top);
        }
        update = 0;
    }
};
static bool Near(float a, float b) { return fabs(a - b) < 1e-5; }
int main()
{
    int bad = 0;
    unsigned int commit = 0x1;
    Listener l = { { 0, 0, 2 }, { 0, 3, 0 }, { 9, 9, 9 }, 0x20000 | 0x8000 | 0x40 };
    l.Commit(commit);                                            // perpendicular: right rebuilt from front (0,0,1) and top (0,1,0)
    if (commit != (0x1 | 0x28000) || l.update != 0) ++bad;
    if (!Near(l.front.z, 1.0f) || !Near(l.top.y, 1.0f)) ++bad;
    if (!Near(l.right.x, 1.0f) || !Near(l.right.y, 0.0f) || !Near(l.right.z, 0.0f)) ++bad;    // x = top.y*front.z - front.y*top.z = 1
    Listener k = { { 0, 0, 1 }, { 0, 1, 1 }, { 1, 0, 0 }, 0x20000 };
    k.Commit(commit);                                            // dot = 0.707: top forced from right (1,0,0) and front (0,0,1)
    if (!Near(k.top.x, 0.0f) || !Near(k.top.y, 1.0f) || !Near(k.top.z, 0.0f) || !Near(k.right.x, 1.0f)) ++bad;
    Listener n = { { 0, 0, 5 }, { 0, 5, 5 }, { 7, 7, 7 }, 0x10000 };
    unsigned int c2 = 0;
    n.Commit(c2);                                                // no orientation bit: nothing normalised
    if (c2 != 0x10000 || n.front.z != 5.0f || n.right.x != 7.0f || n.update != 0) ++bad;
    printf(bad ? "00c40590_TEST FAIL\n" : "00c40590_TEST PASS\n");
    return bad ? 1 : 0;
}
