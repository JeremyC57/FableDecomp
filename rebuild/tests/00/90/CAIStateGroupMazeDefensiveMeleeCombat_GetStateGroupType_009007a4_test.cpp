// Behaviour test for 0x009007A4 CAIStateGroup_MazeDefensiveMeleeCombat::GetStateGroupType (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_MAZEDEFENSIVEMELEECOMBAT_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_MAZEBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_MazeDefensiveMeleeCombat.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_MazeDefensiveMeleeCombat)];
int main(void)
{
    const CAIStateGroup_MazeDefensiveMeleeCombat* p = (const CAIStateGroup_MazeDefensiveMeleeCombat*)g_obj;
    EAIStateGroupType r = p->CAIStateGroup_MazeDefensiveMeleeCombat::GetStateGroupType();
    int ok = (r == (EAIStateGroupType)1);
    if (ok) { printf("PASS_009007a4\n"); return 0; }
    printf("FAIL_009007a4\n");
    return 1;
}
