// 0x0073DA00 test (NF244, q12681): links the row's CModeStandBase::IsAllowingPlayerSpecialMovementMoves (engine/CModeStandBase.h) and
// calls it non-virtually on zeroed storage (the body reads nothing): true.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE
#include <stdio.h>
#include <string.h>
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
static unsigned long storage[0x40];
int main()
{
    memset(storage, 0, sizeof(storage));
    const CModeStandBase* mode = (const CModeStandBase*)storage;
    if (!mode->CModeStandBase::IsAllowingPlayerSpecialMovementMoves()) { printf("AUTO_TINY_0073da00_TEST FAIL\n"); return 1; }
    printf("AUTO_TINY_0073da00_TEST PASS\n");
    return 0;
}
