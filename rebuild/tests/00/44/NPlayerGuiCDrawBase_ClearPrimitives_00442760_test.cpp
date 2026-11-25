// Behaviour test for 0x00442760 NPlayerGui::CDrawBase::ClearPrimitives (NF432): links the row object and calls the member (qualified,
// no vftable) on a filled object image; the body is empty, so the image must be unchanged afterwards.
#include "engine/NPlayerGui_CDrawBase.h"
#include <stdio.h>
#include <string.h>
static unsigned char g_obj[sizeof(NPlayerGui::CDrawBase)];
int main(void)
{
    unsigned char before[sizeof(g_obj)];
    memset(g_obj, 0x5A, sizeof(g_obj)); memcpy(before, g_obj, sizeof(g_obj));
    NPlayerGui::CDrawBase* p = (NPlayerGui::CDrawBase*)g_obj;
    p->NPlayerGui::CDrawBase::ClearPrimitives();
    if (memcmp(before, g_obj, sizeof(g_obj)) == 0) { printf("PASS_00442760\n"); return 0; }
    printf("FAIL_00442760\n");
    return 1;
}
