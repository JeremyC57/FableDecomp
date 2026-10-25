// Behaviour test for 0x0056ECCB CTCInventoryExperience::ProcessInventoryInputDown: the method comes from the source object.
#include <cstdio>
#include <stddef.h>
static int g_baseUp = 0, g_baseDown = 0, g_refresh = 0, g_v29 = 0; static long g_pos = -9, g_max = -9;
static struct SLongVector* g_items = 0;
struct SLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};

namespace NInventory {

class CTCInventoryBase {
public:
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual void v04() {}
    virtual void v05() {}
    virtual void v06() {}
    virtual void v07() {}
    virtual void v08() {}
    virtual void v09() {}
    virtual void v10() {}
    virtual void v11() {}
    virtual void v12() {}
    virtual void v13() {}
    virtual void v14() {}
    virtual void v15() {}
    virtual void v16() {}
    virtual void v17() {}
    virtual void v18() {}
    virtual void v19() {}
    virtual void v20() {}
    virtual void v21() {}
    virtual void v22() {}
    virtual void v23() {}
    virtual void v24() {}
    virtual void v25() {}
    virtual void v26() {}
    virtual void v27() {}
    virtual void v28() {}
    virtual void vfunc_29() { ++g_v29; }                    // slot 29 (+0x74)
    virtual void v30() {}
    virtual void v31() {}
    virtual void v32() {}
    virtual void v33() {}
    virtual void v34() {}
    virtual void v35() {}
    virtual void v36() {}
    virtual void v37() {}
    virtual void ProcessInventoryInputUp() { ++g_baseUp; }           // slot 38 (0x005BDD8C)
    virtual void ProcessInventoryInputDown() { ++g_baseDown; }         // slot 39 (0x005BDDB8)
    static void __stdcall UpdateIndicatorSlider(SLongVector* items, long pos, long max);    // 0x005BC58C (folded empty body)
    unsigned char _pad_004[0x18];
    long State;                                 // +0x01C (InventorySelectionState)
};

} // namespace NInventory

class CTCInventoryExperience : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryExperience@@, vtable 0x0124F11C
public:
    unsigned char _pad_020[0x19C];
    long statIndicatorPos;                      // +0x1BC
    long statIndicatorMax;                      // +0x1C0
    unsigned char _pad_1c4[0x44];
    SLongVector MenuDisplayListExperienceItem;  // +0x208
    void IncrementStatIndicator();              // 0x0056E5CA
    virtual void ProcessInventoryInputUp();     // slot 38 (0x0056EC8C)
    virtual void ProcessInventoryInputDown();   // slot 39 (0x0056ECCB)
};
void CTCInventoryExperience::ProcessInventoryInputUp() { }
void CTCInventoryExperience::IncrementStatIndicator() { statIndicatorPos += (1); }
void __stdcall NInventory::CTCInventoryBase::UpdateIndicatorSlider(SLongVector* items, long pos, long max) { ++g_refresh; g_items = items; g_pos = pos; g_max = max; }
int main()
{
    CTCInventoryExperience e; e.statIndicatorPos = 2; e.statIndicatorMax = 5; int ok = 1;
    e.State = 0; e.ProcessInventoryInputDown();
    ok &= g_baseDown == 1 && g_refresh == 0 && g_v29 == 0;
    e.State = 1; e.ProcessInventoryInputDown();
    ok &= g_baseDown == 1 && g_refresh == 1 && g_v29 == 1 && g_items == &e.MenuDisplayListExperienceItem && g_pos == 2 + (1) && g_max == 5;
    e.State = 2; e.ProcessInventoryInputDown();
    ok &= g_baseDown == 1 && g_refresh == 1 && g_v29 == 1 && g_baseUp == 0;
    ok &= offsetof(CTCInventoryExperience, statIndicatorPos) == 0x1bc && offsetof(CTCInventoryExperience, MenuDisplayListExperienceItem) == 0x208;
    if (!ok) { std::printf("0056eccb_TEST FAIL\n"); return 1; }
    std::printf("0056eccb_TEST PASS\n");
    return 0;
}
