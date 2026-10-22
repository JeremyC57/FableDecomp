// C2DLineF::GetDist(const C2DLineF&, float*, float*, bool*) const -- distance between two segments: 0 when they cross, else the smallest endpoint-to-segment distance, with the end distance (zeroed when the other line's ends lie on opposite sides), the projected-point distance and which side the nearest point lies on
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A57430, length 764 bytes (`ret 10h`) + a 4-entry jump table. Callees:
//   C2DLineF::Intersects2D(const C2DLineF&) @ 0x00A57030, C2DLineF::GetPointOnLine(const C2DVector&, C2DVector&, float, C2DVector*, bool*) @ 0x00A571A0
//   (PDB ?GetPointOnLine@C2DLineF@@QBEMABVC2DVector@@AAV2@MPAV2@PA_N@Z; catalog label "GetDistanceToPoint"); 0.0f @ 0x0122DEDC.
// Shape: the case-2 vector is NOT a named local -- VC CSEs `other.Point1 - c2` from the squared distance into the switch (fst to the frame);
//   the true arm takes `float d = (float)sqrt(best); *onLeft = true; return d;` and the shared tail a named `dist` so fsqrt schedules before the pops.
// Layout: C2DLineF {C2DVector Point1 (+0), Point2 (+8)} -- same 16 bytes as the landed {X1,Y1,X2,Y2} view.
// Leaf: GetDist
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float operator*(const C2DVector& o) const { return X * o.X + Y * o.Y; }
    float GetLengthSquared() const { return X * X + Y * Y; }
};
struct C2DLineF {
    C2DVector Point1;   // +0x00
    C2DVector Point2;   // +0x08
    bool Intersects2D(const C2DLineF& other) const;      // 0x00A57030
    float GetPointOnLine(const C2DVector& p, C2DVector& closest, float radius, C2DVector* projected, bool* before) const; // 0x00A571A0
    float GetDist(const C2DLineF& other, float* endDist, float* projDist, bool* onLeft) const;
};

float C2DLineF::GetDist(const C2DLineF& other, float* endDist, float* projDist, bool* onLeft) const
{
    if (Intersects2D(other)) {
        if (endDist)
            *endDist = 0.0f;
        if (projDist)
            *projDist = 0.0f;
        return 0.0f;
    }
    C2DVector c0, c1, c2, c3, p2, p3;
    bool before2, before3;
    other.GetPointOnLine(Point1, c0, 0.0f, 0, 0);
    other.GetPointOnLine(Point2, c1, 0.0f, 0, 0);
    float d2 = GetPointOnLine(other.Point1, c2, 0.0f, &p2, &before2);
    float d3 = GetPointOnLine(other.Point2, c3, 0.0f, &p3, &before3);
    if (endDist) {
        if (d2 < d3)
            *endDist = d2;
        else
            *endDist = d3;
        if (*endDist != 0.0f && before2 != before3)
            *endDist = 0.0f;
    }
    if (projDist) {
        float a = (other.Point1 - p2).GetLengthSquared();
        float b = (other.Point2 - p3).GetLengthSquared();
        if (a < b)
            *projDist = (float)sqrt(a);
        else
            *projDist = (float)sqrt(b);
    }
    int nearest = 0;
    float s0 = (Point1 - c0).GetLengthSquared();
    float s1 = (Point2 - c1).GetLengthSquared();
    float s2 = (other.Point1 - c2).GetLengthSquared();
    float s3 = (other.Point2 - c3).GetLengthSquared();
    float best = s0;
    if (s1 < best) { best = s1; nearest = 1; }
    if (s2 < best) { best = s2; nearest = 2; }
    if (s3 < best) { best = s3; nearest = 3; }
    if (onLeft) {
        C2DVector dir = Point2 - Point1;
        C2DVector perp(-dir.Y, dir.X);
        C2DVector v;
        switch (nearest) {
        case 0: v = c0 - Point1; break;
        case 1: v = c1 - Point2; break;
        case 2: v = other.Point1 - c2; break;
        case 3: v = other.Point2 - c3; break;
        }
        if (v * perp > 0.0f) {
            float d = (float)sqrt(best);
            *onLeft = true;
            return d;
        }
        *onLeft = false;
    }
    float dist = (float)sqrt(best);
    return dist;
}
