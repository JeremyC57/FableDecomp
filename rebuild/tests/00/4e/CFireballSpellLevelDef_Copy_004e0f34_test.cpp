// Test for 0x004E0F34 CFireballSpellLevelDef::Copy (NF319, 2026-11-23: the row now defines the real member, so the test calls it on the
// retail class model instead of the old CFireballSpellLevelDef_Methods alias). Copy runs the CDefClassBase assignment on the base
// subobject (stubbed here to record its operands) and copies SpellLevel (+0x28). Called qualified on raw storage: no vtable is needed.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CFIREBALLSPELLLEVELDEF
#define FABLE_RETAIL_CFIREBALLSPELLLEVELDEF_RTTI_DECLS
#include "engine/CFireballSpellLevelDef.h"
#include "engine/CDefClassBase.h"
#include <cstdio>

static void* g_self = 0;
static const void* g_src = 0;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other)
{
    g_self = this;
    g_src = &other;
    return *this;
}

static unsigned long g_dst[0x40];
static unsigned long g_srcbuf[0x40];

int main()
{
    CFireballSpellLevelDef* dst = (CFireballSpellLevelDef*)g_dst;
    const CFireballSpellLevelDef* src = (const CFireballSpellLevelDef*)g_srcbuf;
    g_dst[0x28 / 4] = 0x11111111;
    g_srcbuf[0x28 / 4] = 0xDEADBEEF;
    dst->CFireballSpellLevelDef::Copy((const CDefClassBase*)src);
    if (g_dst[0x28 / 4] == 0xDEADBEEF && g_self == (void*)g_dst && g_src == (const void*)g_srcbuf)
        printf("004e0f34_TEST PASS\n");
    else
        printf("FAIL %lx\n", g_dst[0x28 / 4]);
    return 0;
}
