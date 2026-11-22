// Behaviour test for 0x00692670 CCreatureActionBase::IsPreventingLookTurning (links the REAL row object, NF259). The row makes a virtual call
// through slot 49 (+0xC4, IsPreventingNormalMovement); the test gives raw storage a vptr to a table whose slot 49 is a probe member of a
// test-only class (no model of CCreatureActionBase), and checks both answers.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include <stdio.h>
class CProbe {
public:
    bool Yes() const { return true; }
    bool No() const { return false; }
};
typedef bool (CProbe::*TProbe)() const;
static void* ProbeAddress(TProbe probe)
{
    union { TProbe member; void* address; } u;
    u.member = probe;
    return u.address;
}
int main()
{
    void* table[0x50];
    for (int i = 0; i < 0x50; ++i) table[i] = 0;
    unsigned long storage[0xa8 / 4] = { 0 };
    storage[0] = (unsigned long)table;
    const CCreatureActionBase* action = (const CCreatureActionBase*)storage;
    table[0xC4 / 4] = ProbeAddress(&CProbe::Yes);
    bool a = action->CCreatureActionBase::IsPreventingLookTurning();
    table[0xC4 / 4] = ProbeAddress(&CProbe::No);
    bool b = action->CCreatureActionBase::IsPreventingLookTurning();
    if (a && !b) { printf("00692670_TEST PASS\n"); return 0; }
    printf("00692670_TEST FAIL\n");
    return 1;
}
