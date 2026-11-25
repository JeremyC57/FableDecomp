// Behaviour test for 0x00644627 NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary (NF432): links the row object and calls the member
// (qualified, no vftable) on a CDrawBase image in static storage whose PrimitiveHandles (+0x1c begin, +0x20 end) span two 8-byte
// handles. CEnginePrimitiveHandle::Clear (declaration-only in the header, retail 0x0041BE70) and the for_each instance (retail COMDAT
// 0x0043F360; an explicit specialization names the row's implicit symbol) are defined here; Clear records each handle it is called on.
// Drawing (+0x18) set: no handle is cleared; clear: both are, in order.
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DECL_ONLY
#include "stlport/fable_algorithm.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include <stdio.h>
#include <string.h>
static const void* g_cleared[4]; static int g_count = 0;
void CEnginePrimitiveHandle::Clear() { if (g_count < 4) g_cleared[g_count] = this; ++g_count; }
namespace std {
template<> mem_fun_ref_t<void, CEnginePrimitiveHandle> FABLE_FASTCALL for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last,
    mem_fun_ref_t<void, CEnginePrimitiveHandle> f)
{
    for ( ; first != last; ++first) f(*first);
    return f;
}
}
static long g_obj[sizeof(NPlayerGui::CDrawBase) / 4];
static long g_handles[4];            // two 8-byte handles
static void Setup(bool drawing)
{
    memset(g_obj, 0, sizeof(g_obj)); g_count = 0;
    ((unsigned char*)g_obj)[0x18] = drawing ? 1 : 0;
    g_obj[0x1c / 4] = (long)&g_handles[0]; g_obj[0x20 / 4] = (long)&g_handles[4]; g_obj[0x24 / 4] = (long)&g_handles[4];
}
int main(void)
{
    NPlayerGui::CDrawBase* p = (NPlayerGui::CDrawBase*)g_obj;
    Setup(true);
    p->NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary();
    int ok = g_count == 0;
    Setup(false);
    p->NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary();
    ok = ok && g_count == 2 && g_cleared[0] == (const void*)&g_handles[0] && g_cleared[1] == (const void*)&g_handles[2];
    if (ok) { printf("UNINITWINDOW_OK\n"); return 0; }
    printf("FAIL_00644627\n");
    return 1;
}
