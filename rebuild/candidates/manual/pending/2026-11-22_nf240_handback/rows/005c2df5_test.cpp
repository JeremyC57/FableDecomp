// Test for 0x005c2df5 CTCInventoryWeapons::IsRangedWeaponDrawn on the shared headers (NF240): links the row object. The component is a
// raw buffer (CTCBase::ParentThing at +4); CTCCarrying::IsThingCarryingRangedWeapon (retail out of line) is stubbed here and records
// the thing it was given. Cases: the parent thing is forwarded by reference and a true result is returned; a false result is
// returned; a different parent thing is forwarded.
#include <stdio.h>
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27
#define FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26
#include "engine/ETCInterfaceType.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"

static const CThing* g_seen = 0;
static bool g_result = false;
bool FABLE_FASTCALL CTCCarrying::IsThingCarryingRangedWeapon(const CThing& thing) { g_seen = &thing; return g_result; }

int main()
{
    static char thingA[0x100], thingB[0x100];
    static void* buf[0x80];
    const CTCInventoryWeapons* inv = reinterpret_cast<const CTCInventoryWeapons*>(buf);
    buf[1] = thingA; g_result = true;
    if (!inv->IsRangedWeaponDrawn() || g_seen != reinterpret_cast<CThing*>(thingA)) { printf("005c2df5_TEST FAIL 1\n"); return 1; }
    g_result = false; g_seen = 0;
    if (inv->IsRangedWeaponDrawn() || g_seen != reinterpret_cast<CThing*>(thingA)) { printf("005c2df5_TEST FAIL 2\n"); return 1; }
    buf[1] = thingB; g_result = true;
    if (!inv->IsRangedWeaponDrawn() || g_seen != reinterpret_cast<CThing*>(thingB)) { printf("005c2df5_TEST FAIL 3\n"); return 1; }
    printf("PASS_005c2df5\n");
    return 0;
}
