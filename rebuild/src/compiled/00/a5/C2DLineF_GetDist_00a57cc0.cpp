// C2DLineF::GetDist(const C2DBoxF*, float*, float*, bool*) const -- distance from the segment to a box: 0 when they intersect, else the nearest of the four edge distances (corner order min/min, max/min, max/max, min/max), with the smallest edge parameters and the nearest edge's side flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A57CC0, length 553 bytes (`ret 10h`). Callees: C2DLineF::IntersectsWith(const C2DBoxF*)
//   @ 0x00A577B0, C2DLineF::GetDist(const C2DLineF&, float*, float*, bool*) @ 0x00A57430; 0.0f @ 0x0122DEDC.
// Shape: each edge is a C2DLineF temporary sharing one frame slot; the second edge's parameters land in the dead box argument
//   slot; the side flag is chosen with an explicit else (`onLeft = firstFlag`) so VC sinks the first flag into the else arm.
// Layout: C2DLineF {X1, Y1, X2, Y2} (landed IntersectsWith 0x00A57AD0); C2DBoxF {MinX, MinY, MaxX, MaxY} floats.
// Leaf: GetDist
class C2DBoxF { public: float MinX, MinY, MaxX, MaxY; };
class C2DLineF { public:
    float X1, Y1, X2, Y2;
    C2DLineF(float x1, float y1, float x2, float y2) : X1(x1), Y1(y1), X2(x2), Y2(y2) {}
    bool IntersectsWith(const C2DBoxF* box) const;                                       // 0x00A577B0
    float GetDist(const C2DLineF& line, float* t1, float* t2, bool* onLeft) const;       // 0x00A57430
    float GetDist(const C2DBoxF* box, float* t1, float* t2, bool* onLeft) const;
};

float C2DLineF::GetDist(const C2DBoxF* box, float* pT1, float* pT2, bool* pOnLeft) const
{
    bool flag = false;
    bool firstFlag = false;
    if (IntersectsWith(box)) {
        if (pT1)
            *pT1 = 0;
        if (pT2)
            *pT2 = 0;
        return 0.0f;
    }
    float t1, t2, a, b;
    float best = GetDist(C2DLineF(box->MinX, box->MinY, box->MaxX, box->MinY), &t1, &t2, &firstFlag);
    bool onLeft;
    float d = GetDist(C2DLineF(box->MaxX, box->MinY, box->MaxX, box->MaxY), &a, &b, &flag);
    if (d < best) {
        onLeft = flag;
        best = d;
    } else
        onLeft = firstFlag;
    if (t1 > a)
        t1 = a;
    if (t2 > b)
        t2 = b;
    d = GetDist(C2DLineF(box->MaxX, box->MaxY, box->MinX, box->MaxY), &a, &b, &flag);
    if (d < best) {
        onLeft = flag;
        best = d;
    }
    if (t1 > a)
        t1 = a;
    if (t2 > b)
        t2 = b;
    d = GetDist(C2DLineF(box->MinX, box->MaxY, box->MinX, box->MinY), &a, &b, &flag);
    if (d < best) {
        onLeft = flag;
        best = d;
    }
    if (t1 > a)
        t1 = a;
    if (t2 > b)
        t2 = b;
    if (pT1)
        *pT1 = t1;
    if (pT2)
        *pT2 = t2;
    if (pOnLeft)
        *pOnLeft = onLeft;
    return best;
}