// Behaviour test for 0x00CDEBB0 CEntityScriptBase::Init (NF434): links the row object and calls the member directly (qualified, no vftable)
// on a filled CEntityScriptBase image in static storage; the body is empty, so the image must be unchanged. A scratch accessor struct
// derived from CEntityScriptBase names the member (no body of CEntityScriptBase, nothing instantiated).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CEntityScriptBase.h"
#include <stdio.h>
#include <string.h>
struct NF434Access : CEntityScriptBase {
    static void call(CEntityScriptBase* p);
};
static long g_obj[sizeof(CEntityScriptBase) / 4 + 1]; static long g_ctx[4];
void NF434Access::call(CEntityScriptBase* p) { p->CEntityScriptBase::Init(); }
int main(void)
{
    long before[sizeof(g_obj) / 4];
    memset(g_obj, 0x5A, sizeof(g_obj)); memcpy(before, g_obj, sizeof(g_obj));
    NF434Access::call((CEntityScriptBase*)g_obj);
    if (memcmp(before, g_obj, sizeof(g_obj)) == 0) { printf("PASS_00cdebb0\n"); return 0; }
    printf("FAIL_00cdebb0\n");
    return 1;
}
