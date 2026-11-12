// Behaviour test for CPhysicsMeshObject::GetNavigationLevelAt (0x00A11D10): a model of the row -- no navigation volumes gives 0
// without touching the matrix; otherwise the world position goes through the inverse of the position matrix (cofactors over the
// determinant, translation through the result) and the mesh answers with the level whose volume contains the mesh-space point.
#include <stdio.h>
#include <math.h>
struct V { float X, Y, Z; };
struct M {
    float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
    void SetInverse(const M& m)
    {
        float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
        float inv = 1.0f / det;
        E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
        E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
        E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
        E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
        E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
        E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
        E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
        E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
        E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
        E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
        E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
        E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
    }
    void TransformPoint(const V& s, V& d) const
    {
        d.X = E11 * s.X + E21 * s.Y + E31 * s.Z + E41;
        d.Y = E12 * s.X + E22 * s.Y + E32 * s.Z + E42;
        d.Z = E13 * s.X + E23 * s.Y + E33 * s.Z + E43;
    }
};
struct Volume { long level; V mn, mx; };                    // a navigation level and its (box) containment volume
struct Mesh {
    const Volume* first; const Volume* last;                // NavigationContainmentVolumes begin / end
    mutable int asked; mutable V seen;
    long GetNavigationLevelAt(const V& p) const             // the callee 0x00A73D10: first volume containing the point, else 0
    {
        ++asked; seen = p;
        for (const Volume* v = first; v != last; ++v)
            if (p.X >= v->mn.X && p.X <= v->mx.X && p.Y >= v->mn.Y && p.Y <= v->mx.Y && p.Z >= v->mn.Z && p.Z <= v->mx.Z)
                return v->level;
        return 0;
    }
};
struct Object {
    const Mesh* mesh; M PositionMatrix;
    long GetNavigationLevelAt(const V& pos) const
    {
        if (!(mesh->first == mesh->last)) {
            M inverse;
            inverse.SetInverse(PositionMatrix);
            V local_pos;
            inverse.TransformPoint(pos, local_pos);
            return mesh->GetNavigationLevelAt(local_pos);
        }
        return 0;
    }
};
static int Near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    int ok = 1;
    static const Volume vols[2] = { { 3, { 0, 0, 0 }, { 1, 1, 1 } }, { 7, { 4, 4, 4 }, { 6, 6, 6 } } };
    // 90 degrees about Z, scaled by 2, translated to (10, 20, 30): mesh (x,y,z) -> world (10 - 2y, 20 + 2x, 30 + 2z).
    M pm = { 0,2,0, -2,0,0, 0,0,2, 10,20,30 };
    Mesh none = { vols, vols, 0, { 0, 0, 0 } };
    Object a = { &none, pm };
    V w1 = { 9, 21, 31 };                                   // mesh (0.5, 0.5, 0.5)
    ok = ok && a.GetNavigationLevelAt(w1) == 0 && none.asked == 0;          // empty: 0, callee never asked
    Mesh two = { vols, vols + 2, 0, { 0, 0, 0 } };
    Object b = { &two, pm };
    ok = ok && b.GetNavigationLevelAt(w1) == 3 && two.asked == 1;
    ok = ok && Near(two.seen.X, 0.5f) && Near(two.seen.Y, 0.5f) && Near(two.seen.Z, 0.5f);
    V w2 = { 0, 30, 40 };                                   // mesh (5, 5, 5)
    ok = ok && b.GetNavigationLevelAt(w2) == 7;
    ok = ok && Near(two.seen.X, 5.0f) && Near(two.seen.Y, 5.0f) && Near(two.seen.Z, 5.0f);
    V w3 = { 0.5f, 0.5f, 0.5f };                            // world coordinates that are NOT inside any volume once inverted
    ok = ok && b.GetNavigationLevelAt(w3) == 0 && two.asked == 3;
    // The inverse really is the inverse: forward then back is the identity on a general (sheared) matrix.
    M g = { 1,2,0.5f, -1,3,2, 0.25f,-2,4, 5,-6,7 }, gi;
    gi.SetInverse(g);
    V p = { 1.5f, -2.5f, 3.25f }, f, r;
    g.TransformPoint(p, f); gi.TransformPoint(f, r);
    ok = ok && Near(r.X, p.X) && Near(r.Y, p.Y) && Near(r.Z, p.Z);
    if (!ok) { printf("00a11d10_TEST FAIL\n"); return 1; }
    printf("00a11d10_TEST PASS\n");
    return 0;
}