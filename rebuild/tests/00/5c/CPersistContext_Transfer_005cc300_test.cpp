// Test for 0x005CC300 CPersistContext::Transfer<CDefIndex>(name, value): it must forward the same name and value with a zeroed
// default CDefIndex to the three-argument Transfer<CDefIndex> (0x005CC1E8, stubbed here to record its arguments).
// NF188 (2026-11-20, q4510): rewritten on the shared engine/CPersistContext.h + engine/CDefIndex.h (was a standalone host copy of the
//   stand-in Transfer_long_ / TransferImpl); links the row object.
#include <stdio.h>
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"
#include "engine/CDefIndex.h"


static const char* g_name;
static CDefIndex* g_value;
static long g_default = -1;
static int g_calls;

template<> void CPersistContext::Transfer<CDefIndex>(const char* name, CDefIndex& value, const CDefIndex& defaultValue)
{
    ++g_calls; g_name = name; g_value = &value; g_default = defaultValue;
}

int main()
{
    static unsigned char storage[64];
    CPersistContext* ctx = reinterpret_cast<CPersistContext*>(storage);
    CDefIndex v(7);
    const char* nm = "ActiveMeleeWeaponDefIndex2";
    ctx->Transfer(nm, v);   // the row (0x005CC300): the primary template is declaration-only here
    if (g_calls == 1 && g_name == nm && g_value == &v && g_default == 0 && (long)v == 7) {
        printf("TRANSFER_LONG_OK\n");
        return 0;
    }
    printf("TRANSFER_LONG_FAIL calls=%d def=%ld\n", g_calls, g_default);
    return 1;
}
