// Behaviour test for 0x006896B0 CInputProcessInventoryClothing::GetPInventory (NF478 respell): links the row object and calls the protected const member through a
// derived accessor on raw storage; the result must be the PInventory word at +0x34, and the object must be left unchanged.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessInventoryClothing.h"
#include <cstdio>
#include <cstring>
struct Probe : CInputProcessInventoryClothing {
    Probe(); // never constructed: the test works on raw storage
    NInventory::CTCInventoryBase* Ask() const { return CInputProcessInventoryClothing::GetPInventory(); }
};
static unsigned char g_process[0x80], g_copy[0x80], g_inventory[0x10];
int main()
{
    for (int i = 0; i < 0x80; ++i) g_process[i] = (unsigned char)(0xA0 ^ i);
    void* inv = g_inventory; std::memcpy(g_process + 0x34, &inv, sizeof(inv)); std::memcpy(g_copy, g_process, sizeof(g_process));
    const Probe* p = (const Probe*)g_process;
    bool ok = (void*)p->Ask() == inv && !std::memcmp(g_copy, g_process, sizeof(g_process));
    if (ok) { std::printf("AUTO_TINY_006896b0_TEST PASS\n"); return 0; }
    std::printf("NF478_006896b0_TEST FAIL\n");
    return 1;
}
