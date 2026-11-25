// Behaviour test for 0x0088FD60 CGameScriptInterface::IsEntityInLimbo (NF431): links the row object. No class body is defined here:
// the classes come from the row's shared headers. The script thing is a vftable whose slot 11 (+0x2c, GetPThing) is a __fastcall
// stand-in returning a thing image; the flag byte at the retail offset +0x91 carries Dead (bit 0) and InLimbo (bit 5).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISINLIMBO_DIRECT
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include <stdio.h>

static unsigned char g_thing[0x100];
static void* g_ret = 0;
static int g_calls = 0;
extern "C" void* __fastcall nf431_get_pthing(const void*) { ++g_calls; return g_ret; }   // CScriptThing slot 11
static void* g_vtbl[79];
static void* g_script[3];
static long g_gsi[0x50 / 4];

static int check(void* ret, unsigned char flags, bool want)
{
    g_ret = ret; g_thing[0x91] = flags; g_calls = 0;
    const CGameScriptInterface* gsi = (const CGameScriptInterface*)g_gsi;
    bool got = gsi->CGameScriptInterface::IsEntityInLimbo(*(const CScriptThing*)g_script);
    return got == want && g_calls == 1;
}

int main(void)
{
    for (int i = 0; i < 79; ++i) g_vtbl[i] = 0;
    g_vtbl[11] = (void*)&nf431_get_pthing;
    g_script[0] = g_vtbl;
    if (!check(0, 0x20, false) || !check(g_thing, 0x21, false) || !check(g_thing, 0x20, true) || !check(g_thing, 0x00, false)
        || !check(g_thing, 0xdf, false)) {
        printf("FAIL_0088fd60\n");
        return 1;
    }
    printf("PASS_0088fd60 CGameScriptInterface_0088fd60_TEST PASS\n");
    return 0;
}
