// Behaviour test for 0x006928B0 CCreatureActionBase::CanAnimationTransitionOut (links the REAL row object, NF259). Raw storage whose PAnimEntry Data
// word (+0x74) points at a dummy entry; the test stubs the out-of-line CAnimationEntry::GetFlags (0x005DC210) to return chosen flags.
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
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CANIMATIONENTRY_GETFLAGS
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CAnimationEntry.h"
#include <stdio.h>
static long g_flags;
long CAnimationEntry::GetFlags() const { return g_flags; }
int main()
{
    static const long flags[4] = { 0x01, 0x1F, 0x10, 0x00 };
    static const bool expected[4] = { 0, 0, 1, 1 };
    unsigned long entry[4] = { 0 };
    unsigned long storage[0xa8 / 4] = { 0 };
    storage[0x74 / 4] = (unsigned long)entry;
    const CCreatureActionBase* action = (const CCreatureActionBase*)storage;
    class CPeek : public CCreatureActionBase { public: bool Call() const { return CCreatureActionBase::CanAnimationTransitionOut(); } };
    for (int i = 0; i < 4; ++i)
    {
        g_flags = flags[i];
        if (((const CPeek*)action)->Call() != expected[i]) { printf("006928b0_TEST FAIL\n"); return 1; }
    }
    printf("006928b0_TEST PASS\n");
    return 0;
}
