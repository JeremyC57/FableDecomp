// Behaviour test for 0x0061A100 CTCInventoryQuests::GetContinuedCategoryRepeatDelay: the method comes from the source object.
#include <cstdio>
namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase() {}
    virtual void _v01() {}
    virtual void _v02() {}
    virtual void _v03() {}
    virtual void _v04() {}
    virtual void _v05() {}
    virtual void _v06() {}
    virtual void _v07() {}
    virtual void _v08() {}
    virtual void _v09() {}
    virtual void _v10() {}
    virtual void _v11() {}
    virtual void _v12() {}
    virtual void _v13() {}
    virtual void _v14() {}
    virtual void _v15() {}
    virtual void _v16() {}
    virtual void _v17() {}
    virtual void _v18() {}
    virtual void _v19() {}
    virtual void _v20() {}
    virtual void _v21() {}
    virtual void _v22() {}
    virtual void _v23() {}
    virtual void _v24() {}
    virtual void _v25() {}
    virtual void _v26() {}
    virtual void _v27() {}
    virtual void _v28() {}
    virtual void _v29() {}
    virtual void _v30() {}
    virtual void _v31() {}
    virtual void _v32() {}
    virtual void _v33() {}
    virtual void _v34() {}
    virtual void _v35() {}
    virtual void _v36() {}
    virtual void _v37() {}
    virtual void _v38() {}
    virtual void _v39() {}
    virtual void _v40() {}
    virtual void _v41() {}
    virtual float GetInitialCategoryRepeatDelay() const { return BaseValue42; } // slot 42 (0x005BD404)
    virtual float GetContinuedCategoryRepeatDelay() const { return BaseValue43; } // slot 43 (0x005BD427)
    virtual float GetInitialItemRepeatDelay() const { return BaseValue44; } // slot 44 (0x005BD44A)
    virtual float GetContinuedItemRepeatDelay() const { return BaseValue45; } // slot 45 (0x005BD46D)
    float BaseValue42, BaseValue43, BaseValue44, BaseValue45; unsigned char _pad_0x14[0x130 - 0x14];
};

} // namespace NInventory

class CTCInventoryQuests : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryQuests@@, vtable 0x012585B4
public:
    unsigned char _pad_0x130[0x157 - 0x130];
    bool ActiveTestQuestMode;                                    // +0x157 (Ego_r.pdb +0x177)
    virtual float GetInitialCategoryRepeatDelay() const;         // slot 42 (0x0061A0E0)
    virtual float GetContinuedCategoryRepeatDelay() const;       // slot 43 (0x0061A100)
};
float CTCInventoryQuests::GetInitialCategoryRepeatDelay() const { return -1.0f; }
int main()
{
    CTCInventoryQuests q; q.BaseValue42 = 2.25f; q.BaseValue43 = 3.5f; q.ActiveTestQuestMode = false;
    if (q.GetContinuedCategoryRepeatDelay() != 3.5f) { std::printf("0061a100_TEST FAIL\n"); return 1; }
    q.ActiveTestQuestMode = true;
    if (q.GetContinuedCategoryRepeatDelay() != 0.035f) { std::printf("0061a100_TEST FAIL\n"); return 1; }
    std::printf("0061a100_TEST PASS\n");
    return 0;
}
