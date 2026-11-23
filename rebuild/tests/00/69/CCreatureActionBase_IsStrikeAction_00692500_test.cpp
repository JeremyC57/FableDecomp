// Behaviour test for 0x00692500 CCreatureActionBase::IsStrikeAction (links the REAL row object; NF318 moved it off the flattened free stand-in).
// The member reads nothing through this: call it qualified (a direct call, no vftable) on raw storage whose vptr word is set.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
#include <cstdio>

int main()
{
    unsigned long storage[0x40] = { 0 };
    storage[0] = 0x12345678;
    const CCreatureActionBase* action = (const CCreatureActionBase*)storage;
    bool r = action->CCreatureActionBase::IsStrikeAction();
    if (r == false) {
        std::printf("CCreatureActionBase_00692500_TEST PASS\n");
        return 0;
    }
    std::printf("CCreatureActionBase_00692500_TEST FAIL\n");
    return 1;
}
