// Behaviour test for 0x00A19AE0 CNavigatorBase::CalculateNavigableWidthOfLine (links the REAL row object; NF318 moved it off the
// CNavigatorBase_Methods alias): the member reads nothing through this, so it is called qualified (direct call) on raw storage.
#define FABLE_RETAIL_CNAVIGATORBASE
#include "engine/CNavigatorBase.h"
#include "engine/C3DVector.h"
#include <cstdio>

int main()
{
    unsigned long storage[0x40] = { 0 };
    CNavigatorBase* nav = (CNavigatorBase*)storage;
    C3DVector a(1.0f, 2.0f, 3.0f), pos(4.0f, 5.0f, 6.0f);
    float d = 9.0f, e = 8.0f;
    nav->CNavigatorBase::CalculateNavigableWidthOfLine(a, pos, d, e);
    if (d == 0.0f && e == 0.0f) {
        printf("00a19ae0_TEST PASS\n");
        return 0;
    }
    printf("FAIL d=%f e=%f\n", d, e);
    return 1;
}
