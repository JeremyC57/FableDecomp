// Behaviour test for 0x00CBD4C0 CScriptBase::Main (NF432): links the row object and calls the member directly (qualified, no vftable) on
// a filled CScriptBase image in static storage; the body is empty, so the image must be unchanged. A scratch accessor struct derived from
// CScriptBase names the protected member (no body of CScriptBase, nothing instantiated).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
#include <stdio.h>
#include <string.h>
struct NF432Access : CScriptBase {
    static void call(CScriptBase* p);
};
static long g_obj[sizeof(CScriptBase) / 4 + 1]; static long g_ctx[4];
void NF432Access::call(CScriptBase* p) { static_cast<NF432Access*>(p)->CScriptBase::Main(); }
int main(void)
{
    long before[sizeof(g_obj) / 4];
    memset(g_obj, 0x5A, sizeof(g_obj)); memcpy(before, g_obj, sizeof(g_obj));
    NF432Access::call((CScriptBase*)g_obj);
    if (memcmp(before, g_obj, sizeof(g_obj)) == 0) { printf("PASS_00cbd4c0\n"); return 0; }
    printf("FAIL_00cbd4c0\n");
    return 1;
}
