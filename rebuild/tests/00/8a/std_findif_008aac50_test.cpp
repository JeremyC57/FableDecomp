// Behaviour test for std::__find_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> > (0x008AAC50).
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
};
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    float GetSquaredMagnitude() const { return X * X + Y * Y; }
};
class CVectorFilter_FurtherFromPointThan { public:
    float Radius; C3DVector CentrePos;
    bool operator()(const C3DVector& pos) const
    {
        C2DVector delta(pos - CentrePos);
        return delta.GetSquaredMagnitude() > Radius * Radius;
    }
};
struct NotFurther {                                     // unary_negate
    CVectorFilter_FurtherFromPointThan Pred;
    bool operator()(const C3DVector& x) const { return !Pred(x); }
};
static C3DVector* FindIf(C3DVector* first, C3DVector* last, NotFurther pred)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
    }
    switch (last - first) {
    case 3: if (pred(*first)) return first; ++first;
    case 2: if (pred(*first)) return first; ++first;
    case 1: if (pred(*first)) return first; ++first;
    default: return last;
    }
}

int main()
{
    C3DVector pts[9];
    for (int i = 0; i < 9; ++i) pts[i] = C3DVector(10.0f + i, 0.0f, 100.0f);
    NotFurther p; p.Pred.Radius = 2.0f; p.Pred.CentrePos = C3DVector(0.0f, 0.0f, -50.0f);
    bool ok = FindIf(pts, pts + 9, p) == pts + 9;                       // none within radius (Z ignored)
    pts[6] = C3DVector(1.0f, 1.0f, 999.0f);
    ok = ok && FindIf(pts, pts + 9, p) == pts + 6;                      // in the 4-way loop
    pts[8] = C3DVector(0.0f, -2.0f, 0.0f);                              // exactly on the radius counts as within
    ok = ok && FindIf(pts + 7, pts + 9, p) == pts + 8;                  // remainder path
    ok = ok && FindIf(pts, pts, p) == pts;
    if (!ok) { std::printf("008aac50_TEST FAIL\n"); return 1; }
    std::printf("008aac50_TEST PASS\n");
    return 0;
}