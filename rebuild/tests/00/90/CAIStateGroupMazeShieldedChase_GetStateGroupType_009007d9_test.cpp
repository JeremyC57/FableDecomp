// Behaviour test for 0x009007D9 CAIStateGroup_MazeShieldedChase::GetStateGroupType (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_MAZESHIELDEDCHASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_MazeShieldedChase.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_MazeShieldedChase)];
int main(void)
{
    const CAIStateGroup_MazeShieldedChase* p = (const CAIStateGroup_MazeShieldedChase*)g_obj;
    EAIStateGroupType r = p->CAIStateGroup_MazeShieldedChase::GetStateGroupType();
    int ok = (r == (EAIStateGroupType)1);
    if (ok) { printf("PASS_009007d9\n"); return 0; }
    printf("FAIL_009007d9\n");
    return 1;
}
