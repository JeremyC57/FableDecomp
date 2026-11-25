// Behaviour test for 0x0064D2A9 NPlayerGui::CDrawSpiritsHealth::CDrawSpiritsHealth (NF432): links the row object. CDrawBase's constructor and its two virtuals are
// defined here; the object is built in static storage (no destructor runs). Checks: both references reach CDrawBase's constructor by
// address, the vptr is the class's own (not CDrawBase's), and its slot 0 / slot 1 call ClearPrimitivesIfNecessary / ClearPrimitives.
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#include "engine/NPlayerGui_CDrawSpiritsHealth.h"
#include <stdio.h>
#include <new>
static const void* g_gui = 0; static const void* g_ill = 0; static int g_slot = 0; static const void* g_baseVptr = 0;
NPlayerGui::CDrawBase::CDrawBase(CPlayerGui& playerGui, std::vector<CDrawBase*>& illustrators)
{
    g_gui = &playerGui; g_ill = &illustrators; g_baseVptr = *(const void* const*)this;
}
void NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary() { g_slot = 1; }
void NPlayerGui::CDrawBase::ClearPrimitives() { g_slot = 2; }
static __declspec(noinline) NPlayerGui::CDrawBase* AsBase(void* p) { return (NPlayerGui::CDrawBase*)(NPlayerGui::CDrawSpiritsHealth*)p; }
static double g_obj[(sizeof(NPlayerGui::CDrawSpiritsHealth) + 7) / 8];
static char g_guiStore[16]; static double g_illStore[4];
int main(void)
{
    CPlayerGui& gui = *(CPlayerGui*)g_guiStore;
    std::vector<NPlayerGui::CDrawBase*>& ill = *(std::vector<NPlayerGui::CDrawBase*>*)g_illStore;
    NPlayerGui::CDrawSpiritsHealth* p = new (g_obj) NPlayerGui::CDrawSpiritsHealth(gui, ill);
    int ok = ((void*)p == (void*)g_obj) && g_gui == (const void*)g_guiStore && g_ill == (const void*)g_illStore;
    ok = ok && *(const void* const*)g_obj != g_baseVptr;
    NPlayerGui::CDrawBase* b = AsBase(g_obj);
    b->ClearPrimitivesIfNecessary(); ok = ok && g_slot == 1;
    b->ClearPrimitives(); ok = ok && g_slot == 2;
    if (ok) { printf("0064d2a9_TEST PASS\n"); return 0; }
    printf("0064d2a9_TEST FAIL\n");
    return 1;
}
