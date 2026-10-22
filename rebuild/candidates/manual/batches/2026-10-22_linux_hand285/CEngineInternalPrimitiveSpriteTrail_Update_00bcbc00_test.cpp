#include <cstdio>
#include <vector>
#include <list>
// Model of the sprite-trail ring update: extend mode shifts stored points by the offset (when not negligible), grows the
// ring to min(size + new, cap), wraps the cursor and writes new points at the cursor; replace mode resizes and copies from 0.
struct E { float X, Y, Z, W; };
struct Ring { std::vector<E> Pts; long NextPos; unsigned long MaxLength; };
static void Update(Ring& r, const std::list<E>& in, bool extend, float ox, float oy, float oz)
{
    if (extend) {
        float eps = 0.0001f;
        if (ox * ox + oy * oy + oz * oz > eps * eps)
            for (unsigned long i = 0; i < r.Pts.size(); ++i) { r.Pts[i].X += ox; r.Pts[i].Y += oy; r.Pts[i].Z += oz; }
        long newSize = (long)(in.size() + r.Pts.size());
        if ((unsigned long)newSize > r.MaxLength) newSize = (long)r.MaxLength;
        E blank = { 0, 0, 0, 0 };
        r.Pts.resize(newSize, blank);
        r.NextPos = (long)((unsigned long)r.NextPos % r.MaxLength);
        for (std::list<E>::const_iterator it = in.begin(); it != in.end(); ++it) {
            if (r.NextPos >= newSize) r.NextPos = 0;
            r.Pts[r.NextPos] = *it;
            ++r.NextPos;
        }
    } else {
        E blank = { 0, 0, 0, 0 };
        r.Pts.resize(in.size(), blank);
        r.NextPos = 0;
        for (std::list<E>::const_iterator it = in.begin(); it != in.end(); ++it) { r.Pts[r.NextPos] = *it; ++r.NextPos; }
    }
}
int main()
{
    int bad = 0;
    Ring r; r.NextPos = 0; r.MaxLength = 3;
    std::list<E> in;
    E a = { 1, 0, 0, 1 }, b = { 2, 0, 0, 1 };
    in.push_back(a); in.push_back(b);
    Update(r, in, false, 0, 0, 0);
    if (r.Pts.size() != 2 || r.NextPos != 2 || r.Pts[1].X != 2.0f) bad = 1;
    std::list<E> more; E c = { 3, 0, 0, 1 }, d = { 4, 0, 0, 1 };
    more.push_back(c); more.push_back(d);
    Update(r, more, true, 0, 1, 0);                 // shift by (0,1,0), grow to cap 3, write 3 at slot 2, 4 wraps to slot 0
    if (r.Pts.size() != 3 || r.Pts[2].X != 3.0f || r.Pts[0].X != 4.0f || r.NextPos != 1) bad = 1;
    if (r.Pts[1].Y != 1.0f || r.Pts[1].X != 2.0f) bad = 1;
    Update(r, more, true, 0.00001f, 0, 0);          // negligible offset: no shift
    if (r.Pts[0].X != 4.0f || r.Pts[0].Y != 0.0f || r.Pts[1].X != 3.0f || r.NextPos != 3) bad = 1;
    if (bad) { std::printf("00bcbc00_TEST FAIL\n"); return 1; }
    std::printf("00bcbc00_TEST PASS\n");
    return 0;
}
