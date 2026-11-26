// Behaviour test for 0x00689C20 CInputProcessInventoryMagicScreen::ProcessInput (NF477 respell): links the row object and calls the member non-virtually.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessInventoryMagicScreen.h"
#include <cstdio>
static int g_calls; static void* g_this; static void* g_event; static void* g_input;
void CInputProcessInventoryBase::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
{ ++g_calls; g_this = this; g_event = (void*)&event; g_input = (void*)&processed_input; }
static unsigned int g_guard = 0x5A5A5A5A;
static unsigned int g_storage[0x80];
int main()
{
    CInputProcessInventoryMagicScreen* p = (CInputProcessInventoryMagicScreen*)g_storage;
    bool ok = false;
    CInputEvent* e = (CInputEvent*)0x1234; CProcessedInput* q = (CProcessedInput*)0x5678;
    p->CInputProcessInventoryMagicScreen::ProcessInput(*e, *q);
    ok = (g_calls == 1 && g_this == (void*)p && g_event == (void*)e && g_input == (void*)q);
    if (ok) { std::printf("PASS_00689c20\n"); return 0; }
    std::printf("NF477_00689c20_TEST FAIL\n");
    return 1;
}
