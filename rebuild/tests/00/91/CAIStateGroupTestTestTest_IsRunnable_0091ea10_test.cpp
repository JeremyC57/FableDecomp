// Behaviour test for 0x0091EA10 CAIStateGroup_TestTestTest::IsRunnable (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_TESTTESTTEST_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_TestTestTest.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_TestTestTest)];
int main(void)
{
    CAIStateGroup_TestTestTest* p = (CAIStateGroup_TestTestTest*)g_obj;
    bool r = p->CAIStateGroup_TestTestTest::IsRunnable();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_0091ea10_TEST PASS\n"); return 0; }
    printf("FAIL_0091ea10\n");
    return 1;
}
