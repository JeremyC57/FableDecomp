// Behaviour test for NShape::CTrack::GetMinDistanceToTrack 0x00760E70 (copy of the model's logic over a plain point list).
#include <cstdio>
#include <cmath>
#include <vector>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static V3 Sub(const V3& a, const V3& b) { return V3(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static float Mag(const V3& v) { return (float)std::sqrt(v.Z * v.Z + v.Y * v.Y + v.X * v.X); }
static void AccurateNormalise(V3& v) { float m = Mag(v); if (m > 0.0f) { v.X /= m; v.Y /= m; v.Z /= m; } }
static bool GetMinDistanceToTrack(const std::vector<V3>& pts, const V3& pos, float& distance)
{
    if (pts.size() > 1) {
        V3 flat(pos.X, pos.Y, 0.0f);
        float best = 1e10f;
        size_t i = 0;
        V3 prev = pts[i];
        for (++i; i != pts.size(); ++i) {
            float tlx = prev.X, tly = prev.Y, brx = prev.X, bry = prev.Y;
            V3 cur = pts[i];
            if (cur.X < prev.X) tlx = cur.X;
            if (cur.Y < prev.Y) tly = cur.Y;
            if (cur.X > prev.X) brx = cur.X;
            if (cur.Y > prev.Y) bry = cur.Y;
            float d;
            if (flat.X >= tlx && flat.X < brx && flat.Y >= tly && flat.Y < bry) {
                V3 dir(cur.X - prev.X, cur.Y - prev.Y, 0.0f);
                AccurateNormalise(dir);
                V3 perp(dir.Y, -dir.X, 0.0f);
                V3 o = Sub(pos, prev);
                float dot = perp.Y * o.Y; dot += perp.X * o.X; dot += perp.Z * o.Z;
                d = (float)std::fabs(dot);
            } else {
                float d1 = Mag(Sub(prev, flat));
                float d2 = Mag(Sub(cur, flat));
                d = d1 < d2 ? d1 : d2;
            }
            if (d < best) best = d;
            prev = cur;
        }
        distance = best;
        return true;
    }
    return false;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    std::vector<V3> pts;
    float dist = -1.0f;
    pts.push_back(V3(0, 0, 0));
    if (GetMinDistanceToTrack(pts, V3(1, 1, 0), dist) || dist != -1.0f) { std::printf("00760e70_TEST FAIL\n"); return 1; }
    pts.push_back(V3(10, 10, 0));          // diagonal segment
    // inside the xy box: perpendicular distance to the line y = x from (6,4) is sqrt(2)
    if (!GetMinDistanceToTrack(pts, V3(6, 4, 5), dist) || !Near(dist, 1.41421356f)) { std::printf("00760e70_TEST FAIL\n"); return 1; }
    // outside the box: nearer end point (0,0,0) from (-3,-4) -> 5
    if (!GetMinDistanceToTrack(pts, V3(-3, -4, 7), dist) || !Near(dist, 5.0f)) { std::printf("00760e70_TEST FAIL\n"); return 1; }
    // second segment along +X at y=10; point (15,10.5) is outside its zero-height box -> end-point distance 5.0249
    pts.push_back(V3(20, 10, 2));
    if (!GetMinDistanceToTrack(pts, V3(15, 10.5f, 0), dist)) { std::printf("00760e70_TEST FAIL\n"); return 1; }
    float e1 = std::sqrt(25.0f + 0.25f), e2 = std::sqrt(25.0f + 0.25f + 4.0f);
    if (!Near(dist, e1 < e2 ? e1 : e2)) { std::printf("00760e70_TEST FAIL\n"); return 1; }
    std::printf("00760e70_TEST PASS\n");
    return 0;
}