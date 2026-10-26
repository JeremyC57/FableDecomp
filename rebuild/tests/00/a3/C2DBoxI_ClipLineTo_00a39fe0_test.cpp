#include <cstdio>
// Behaviour copy of C2DBoxI::ClipLineTo (0x00A39FE0) with a real segment intersection for IntersectsWith.
class C2DVector { public: float x; float y; };
class C2DLineF { public:
    float x1, y1, x2, y2;
    C2DLineF(float ax, float ay, float bx, float by) : x1(ax), y1(ay), x2(bx), y2(by) {}
    bool IntersectsWith(const C2DLineF& o, C2DVector* p, float tol) {
        float dx = x2 - x1, dy = y2 - y1, ex = o.x2 - o.x1, ey = o.y2 - o.y1;
        float den = dx * ey - dy * ex; if (den > -tol && den < tol) return false;
        float t = ((o.x1 - x1) * ey - (o.y1 - y1) * ex) / den, u = ((o.x1 - x1) * dy - (o.y1 - y1) * dx) / den;
        if (t < 0 || t > 1 || u < 0 || u > 1) return false;
        p->x = x1 + t * dx; p->y = y1 + t * dy; return true;
    }
};
class C2DBoxI { public:
    long minX, minY, maxX, maxY;
    void ClipLineTo(C2DLineF& line) const {
        C2DVector hit; bool first = true; float eps = 0.0001f;
        C2DLineF top((float)minX, (float)minY, (float)maxX, (float)minY);
        if (line.IntersectsWith(top, &hit, eps)) { if (first) { line.x1 = hit.x; line.y1 = hit.y; } else { line.x2 = hit.x; line.y2 = hit.y; } first = false; }
        C2DLineF right((float)maxX, (float)minY, (float)maxX, (float)maxY);
        if (line.IntersectsWith(right, &hit, eps)) { if (first) { line.x1 = hit.x; line.y1 = hit.y; } else { line.x2 = hit.x; line.y2 = hit.y; } first = false; }
        C2DLineF bottom((float)minX, (float)maxY, (float)maxX, (float)maxY);
        if (line.IntersectsWith(bottom, &hit, eps)) { if (first) { line.x1 = hit.x; line.y1 = hit.y; } else { line.x2 = hit.x; line.y2 = hit.y; } first = false; }
        C2DLineF left((float)minX, (float)minY, (float)minX, (float)maxY);
        if (line.IntersectsWith(left, &hit, eps)) { if (first) { line.x1 = hit.x; line.y1 = hit.y; } else { line.x2 = hit.x; line.y2 = hit.y; } }
    }
};
int main() {
    C2DBoxI b = { 0, 0, 10, 10 };
    C2DLineF l(5.0f, -5.0f, 5.0f, 15.0f);          // crosses top (start) then bottom (end)
    b.ClipLineTo(l);
    C2DLineF m(5.0f, -5.0f, 5.0f, 5.0f);           // crosses top only: start replaced
    b.ClipLineTo(m);
    C2DLineF n(20.0f, 20.0f, 30.0f, 30.0f);        // misses: unchanged
    b.ClipLineTo(n);
    if (l.x1 != 5.0f || l.y1 != 0.0f || l.x2 != 5.0f || l.y2 != 10.0f || m.x1 != 5.0f || m.y1 != 0.0f || m.x2 != 5.0f || m.y2 != 5.0f
        || n.x1 != 20.0f || n.x2 != 30.0f) { std::printf("00a39fe0_TEST FAIL\n"); return 1; }
    std::printf("00a39fe0_TEST PASS\n"); return 0;
}