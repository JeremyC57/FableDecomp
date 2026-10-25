// Behaviour test for _STL::__find_if<C3DVector*, CIsOutsideRadius> (0x008C2F70): the first point farther than the radius,
// across the unrolled body and every switch tail, or `last` when all are inside.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
struct Outside { float Radius; V Centre; bool operator()(const V& p) const {
    float dx = p.X - Centre.X, dy = p.Y - Centre.Y, dz = p.Z - Centre.Z; return std::sqrt(dx * dx + dy * dy + dz * dz) > Radius; } };
static V* FindIf(V* first, V* last, Outside pred)
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
    Outside pred = { 2.0f, { 0, 0, 0 } };
    bool ok = true;
    for (int n = 0; n <= 11 && ok; ++n) {
        V pts[11];
        for (int i = 0; i < 11; ++i) { pts[i].X = 1; pts[i].Y = 0; pts[i].Z = 1; }
        ok = FindIf(pts, pts + n, pred) == pts + n;             // all inside: last
        for (int k = 0; k < n && ok; ++k) {
            pts[k].Z = 5;                                         // first outside at k
            ok = FindIf(pts, pts + n, pred) == pts + k;
            pts[k].Z = 1;
        }
    }
    V edge = { 2, 0, 0 };
    ok = ok && FindIf(&edge, &edge + 1, pred) == &edge + 1;     // exactly on the radius is not outside
    if (!ok) { std::printf("008c2f70_TEST FAIL\n"); return 1; }
    std::printf("008c2f70_TEST PASS\n");
    return 0;
}