// Behaviour test for 0x00802390 (links the REAL row object; NF123 q4690): GetMaxTargetingDistance returns the float at +0x48.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_MAXDISTANCE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CTCTargetingPlayer.h"
#include <stdio.h>
int main()
{
    float object[0x40];
    for (int i = 0; i < 0x40; ++i) object[i] = 0.0f;
    object[0x48 / 4] = 37.5f;
    float d = ((const CTCTargetingPlayer*)object)->GetMaxTargetingDistance();
    if (d != 37.5f) { printf("FAIL_00802390\n"); return 1; }
    printf("OK_00802390");
    return 0;
}
