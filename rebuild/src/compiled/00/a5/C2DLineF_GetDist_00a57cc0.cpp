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