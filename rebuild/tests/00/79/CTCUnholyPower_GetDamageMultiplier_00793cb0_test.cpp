// Behaviour test for 0x00793CB0 CTCUnholyPower::GetDamageMultiplier() const (NF446): links the row object and calls it by its own symbol.
// No class body is defined here: the classes come from the row's shared headers (its prelude, verbatim). The component is a raw buffer
// with PDef (+0x0C, retail `mov eax,[ecx+0Ch]`) and SpellLevel (+0x50, `mov ecx,[ecx+50h]`); the def's DamageTakenMultiplier begins at +0xE8.
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CThingCreatureBase.h"
#include "engine/CTCUnholyPower.h"
#include "engine/CSpecialAbilitiesUnholyPowerDef.h"

#include <stdio.h>
#include <string.h>
static unsigned char g_tc[0x100];
static unsigned char g_def[0x100];
static float g_mult[4] = { 0.5f, 1.25f, 7.5f, 3.0f };
static int check(long level, float want)
{
    memset(g_tc, 0, sizeof g_tc); memset(g_def, 0, sizeof g_def);
    *(void**)(g_def + 0xE8) = g_mult;
    *(void**)(g_tc + 0x0C) = g_def;
    *(long*)(g_tc + 0x50) = level;
    return ((const CTCUnholyPower*)g_tc)->CTCUnholyPower::GetDamageMultiplier() == want;
}
int main(void)
{
    if (!check(0, 0.5f) || !check(2, 7.5f) || !check(3, 3.0f)) { printf("00793cb0_TEST FAIL\n"); return 1; }
    printf("00793cb0_TEST PASS\n"); return 0;
}
