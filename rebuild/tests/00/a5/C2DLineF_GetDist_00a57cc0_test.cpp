#include <cstdio>
// Behaviour copy of C2DLineF::GetDist(const C2DBoxF*, ...) (0x00A57CC0): 0 on intersection, else the nearest edge distance.
struct Box { float MinX, MinY, MaxX, MaxY; };
struct Line {
    float X1, Y1, X2, Y2;
    Line(float a, float b, float c, float d) : X1(a), Y1(b), X2(c), Y2(d) {}
    // stub edge distance: distance between segment midpoints along x+y, params = edge X1/Y1, flag = edge goes right
    float EdgeDist(const Line& e, float* a, float* b, bool* f) const
    {
        float mx = (X1 + X2) * 0.5f - (e.X1 + e.X2) * 0.5f, my = (Y1 + Y2) * 0.5f - (e.Y1 + e.Y2) * 0.5f;
        *a = e.X1; *b = e.Y1; *f = e.X2 > e.X1;
        return (mx < 0 ? -mx : mx) + (my < 0 ? -my : my);
    }
    bool Hits(const Box* bx) const { return X1 >= bx->MinX && X1 <= bx->MaxX && Y1 >= bx->MinY && Y1 <= bx->MaxY; }
    float GetDist(const Box* box, float* pT1, float* pT2, bool* pOnLeft) const
    {
        bool flag = false, firstFlag = false;
        if (Hits(box)) { if (pT1) *pT1 = 0; if (pT2) *pT2 = 0; return 0.0f; }
        float t1, t2, a, b;
        float best = EdgeDist(Line(box->MinX, box->MinY, box->MaxX, box->MinY), &t1, &t2, &firstFlag);
        bool onLeft;
        float d = EdgeDist(Line(box->MaxX, box->MinY, box->MaxX, box->MaxY), &a, &b, &flag);
        if (d < best) { onLeft = flag; best = d; } else onLeft = firstFlag;
        if (t1 > a) t1 = a;
        if (t2 > b) t2 = b;
        d = EdgeDist(Line(box->MaxX, box->MaxY, box->MinX, box->MaxY), &a, &b, &flag);
        if (d < best) { onLeft = flag; best = d; }
        if (t1 > a) t1 = a;
        if (t2 > b) t2 = b;
        d = EdgeDist(Line(box->MinX, box->MaxY, box->MinX, box->MinY), &a, &b, &flag);
        if (d < best) { onLeft = flag; best = d; }
        if (t1 > a) t1 = a;
        if (t2 > b) t2 = b;
        if (pT1) *pT1 = t1;
        if (pT2) *pT2 = t2;
        if (pOnLeft) *pOnLeft = onLeft;
        return best;
    }
};
int main() {
    Box box = {0.0f, 0.0f, 2.0f, 2.0f};
    float t1 = -1, t2 = -1; bool side = true;
    Line inside(1.0f, 1.0f, 5.0f, 5.0f);
    bool ok = inside.GetDist(&box, &t1, &t2, &side) == 0.0f && t1 == 0.0f && t2 == 0.0f;
    Line above(0.0f, 10.0f, 2.0f, 10.0f);
    float d = above.GetDist(&box, &t1, &t2, &side);
    ok = ok && d == 8.0f && side == false && t1 == 0.0f && t2 == 0.0f;
    Line below(1.0f, -5.0f, 1.0f, -6.0f);
    d = below.GetDist(&box, 0, 0, &side);
    ok = ok && d == 5.5f && side == true;
    if (!ok) { std::printf("00a57cc0_TEST FAIL\n"); return 1; }
    std::printf("00a57cc0_TEST PASS\n"); return 0;
}