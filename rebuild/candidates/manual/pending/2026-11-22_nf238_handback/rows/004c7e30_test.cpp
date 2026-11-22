// Test for 0x004C7E30 CThing::IsLocal on the shared engine/CThing.h (NF238): links the row object. The thing and the thing manager
// are raw buffers at the offsets the retail body reads (type byte at this+0x10; record table pointer at manager+4; 20-byte records,
// the bool at +9); the private static CThing::PCurrentThingManager (0x013B8A1C) is defined here, as earlier header tests do.
// Cases: a local type (true), a non-local type (false), and the record stride (neighbouring bytes do not leak in).
#include <stdio.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGMAPMANAGER_PLACE_PASS27
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"

static char g_records[256];
static char* g_manager[2] = { 0, g_records };
CThingManager* CThing::PCurrentThingManager = reinterpret_cast<CThingManager*>(g_manager);

int main()
{
    static unsigned char thingBuf[0x200];
    const CThing& thing = *reinterpret_cast<const CThing*>(thingBuf);
    thingBuf[0x10] = 2; g_records[2*20 + 9] = 1; g_records[3*20 + 9] = 0;
    if (!thing.IsLocal()) { printf("004c7e30_TEST FAIL 1\n"); return 1; }
    thingBuf[0x10] = 3;
    if (thing.IsLocal()) { printf("004c7e30_TEST FAIL 2\n"); return 1; }
    g_records[3*20 + 8] = 1; g_records[3*20 + 10] = 1; g_records[2*20 + 9] = 0;
    if (thing.IsLocal()) { printf("004c7e30_TEST FAIL 3\n"); return 1; }
    g_records[3*20 + 9] = 1;
    if (!thing.IsLocal()) { printf("004c7e30_TEST FAIL 4\n"); return 1; }
    printf("TN_4c7e30_OK\n");
    return 0;
}
