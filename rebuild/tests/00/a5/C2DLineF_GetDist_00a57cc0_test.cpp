#include <cstdio>
// Leaf: ?GetDist@C2DLineF@@QBEMABVC2DBoxF@@PAM1PA_N@Z
// Flags:
#define FABLE_RETAIL_NAV_WIDTH_PASS30
#define FABLE_RETAIL_NAV_BOX_DISTANCE_PASS30
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#include "engine/C2DBoxF.h"
#include "engine/C2DLineF.h"

float C2DLineF::GetDist(const C2DBoxF& box, float* pT1, float* pT2, bool* pOnLeft) const
{
    C2DLineF line;
    bool flag = false;
    bool firstFlag = false;
    if (ContainedBy(box)) {
        if (pT1)
            *pT1 = 0;
        if (pT2)
            *pT2 = 0;
        return 0.0f;
    }
    float t1, t2, a, b;
    line.Initialise(box.TLX, box.TLY, box.BRX, box.TLY);
    float best = GetDist(line, &t1, &t2, &firstFlag);
    bool onLeft;
    line.Initialise(box.BRX, box.TLY, box.BRX, box.BRY);
    float d = GetDist(line, &a, &b, &flag);
    if (d < best) {
        onLeft = flag;
        best = d;
    } else
        onLeft = firstFlag;
    if (t1 > a)
        t1 = a;
    if (t2 > b)
        t2 = b;
    line.Initialise(box.BRX, box.BRY, box.TLX, box.BRY);
    d = GetDist(line, &a, &b, &flag);
    if (d < best) {
        onLeft = flag;
        best = d;
    }
    if (t1 > a)
        t1 = a;
    if (t2 > b)
        t2 = b;
    line.Initialise(box.TLX, box.BRY, box.TLX, box.TLY);
    d = GetDist(line, &a, &b, &flag);
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
bool C2DLineF::ContainedBy(const C2DBoxF& b)const {
 return Point1.X>=b.TLX && Point1.X<=b.BRX && Point1.Y>=b.TLY && Point1.Y<=b.BRY &&
        Point2.X>=b.TLX && Point2.X<=b.BRX && Point2.Y>=b.TLY && Point2.Y<=b.BRY;
}
float C2DLineF::GetDist(const C2DLineF& edge,float* a,float* b,bool* onLeft)const {
 float x=(Point1.X+Point2.X-edge.Point1.X-edge.Point2.X)*.5f;
 float y=(Point1.Y+Point2.Y-edge.Point1.Y-edge.Point2.Y)*.5f;
 *a=edge.Point1.X;*b=edge.Point1.Y;*onLeft=edge.Point2.X>edge.Point1.X;
 return (x<0?-x:x)+(y<0?-y:y);
}
int main() {
    C2DBoxF box(0,0,2,2);
    float t1 = -1, t2 = -1; bool side = true;
    C2DLineF inside;inside.Initialise(1.0f, 1.0f, 1.5f, 1.5f);
    bool ok = inside.GetDist(box, &t1, &t2, &side) == 0.0f && t1 == 0.0f && t2 == 0.0f;
    C2DLineF above;above.Initialise(0.0f, 10.0f, 2.0f, 10.0f);
    float d = above.GetDist(box, &t1, &t2, &side);
    ok = ok && d == 8.0f && side == false && t1 == 0.0f && t2 == 0.0f;
    C2DLineF below;below.Initialise(1.0f, -5.0f, 1.0f, -6.0f);
    d = below.GetDist(box, 0, 0, &side);
    ok = ok && d == 5.5f && side == true;
    if (!ok) { std::printf("00a57cc0_TEST FAIL\n"); return 1; }
    std::printf("00a57cc0_TEST PASS\n"); return 0;
}