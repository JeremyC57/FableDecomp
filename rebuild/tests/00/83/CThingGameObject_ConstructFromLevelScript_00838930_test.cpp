// Behaviour test for 0x00838930 CThingGameObject::ConstructFromLevelScript (NF171 2026-11-20, on the shared headers): links the row; the
// base CThingPhysical::ConstructFromLevelScript is stubbed. The override forwards the script pointer and the object unchanged and returns
// the base's result. Keeps the catalog PassPattern PASS_00838930.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"

static int g_calls = 0;
static const void* g_this = 0;
static const char* g_script = 0;
static bool g_result = false;
bool CThingPhysical::ConstructFromLevelScript(const char* script) { ++g_calls; g_this = this; g_script = script; return g_result; }

int main(void)
{
    static unsigned char storage[0x400];
    memset(storage, 0, sizeof(storage));
    CThingGameObject* object = reinterpret_cast<CThingGameObject*>(storage);
    const char* text = "script";
    g_result = true;
    bool r1 = object->CThingGameObject::ConstructFromLevelScript(text);
    if (!r1 || g_calls != 1 || g_this != (const void*)static_cast<CThingPhysical*>(object) || g_script != text) { printf("FAIL1\n"); return 1; }
    g_result = false;
    bool r2 = object->CThingGameObject::ConstructFromLevelScript(0);
    if (r2 || g_calls != 2 || g_script != 0) { printf("FAIL2\n"); return 1; }
    printf("PASS_00838930\n");
    return 0;
}
