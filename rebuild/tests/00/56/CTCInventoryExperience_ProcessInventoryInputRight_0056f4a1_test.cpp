// Behaviour test for 0x0056F4A1 CTCInventoryExperience::ProcessInventoryInputRight: the method and the same-TU reset come from the source
// object; the reset must restore every current spend before the base handler runs.
#include <cstdio>
#include <cstring>
static char g_seq[64] = "";
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
    virtual void OnSelectedCategoryChanged() {}            // slot 28 (+0x70)
    virtual void OnHighlightedItemChanged() {}             // slot 29 (+0x74)
    virtual void _v30() {}
    virtual void _v31() {}
    virtual void _v32() {}
    virtual void _v33() {}
    virtual void _v34() {}
    virtual void _v35() {}
    virtual void _v36() {}
    virtual void _v37() {}
    virtual void ProcessInventoryInputUp() {}              // slot 38 (+0x98)
    virtual void ProcessInventoryInputDown() {}            // slot 39 (+0x9c)
    virtual void ProcessInventoryInputLeft() { std::strcat(g_seq, "tLeft;"); } // slot 40 (+0xa0)
    virtual void ProcessInventoryInputRight() { std::strcat(g_seq, "Right;"); } // slot 41 (+0xa4)
    virtual void _v42() {}
    virtual void _v43() {}
    virtual void _v44() {}
    virtual void _v45() {}
    virtual void _v46() {}
    virtual void _v47() {}
    virtual void _v48() {}
    virtual void _v49() {}
    virtual void _v50() {}
    virtual void _v51() {}
    virtual void _v52() {}
    virtual void _v53() {}
    virtual void _v54() {}
    virtual void _v55() {}
    virtual void _v56() {}
    virtual void _v57() {}
    virtual void _v58() {}
    virtual void _v59() {}
    virtual void _v60() {}
    virtual void ProcessButtonYReleased() { std::strcat(g_seq, "eased;"); } // slot 61 (+0xf4)
};

} // namespace NInventory

struct ExperienceSpendInfo {
    long currentSpend;      // +0x0
    long initialSpend;      // +0x4
    long levelUpSpend;      // +0x8
};

struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start;
    ExperienceSpendInfo* _M_finish;
    ExperienceSpendInfo* _M_end_of_storage;
};

class CTCInventoryExperience : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryExperience@@, vtable 0x0124F11C
public:
    unsigned char _pad_004[0x1E0 - 4];
    SSpendInfoVector ExperienceSpendValuesPool;     // +0x1E0
    SSpendInfoVector ExperienceSpendValuesStats;    // +0x1EC
    SSpendInfoVector ExperienceSpendValuesAbility;  // +0x1F8
    long ExperienceSpendMagnitude;                  // +0x204
    void ResetExperienceSpendValues();              // 0x0056F03D (Ego_r.pdb QAEXXZ, 83 bytes in both builds)
    virtual void ProcessInventoryInputLeft();         // slot 40 (0056f497)
    virtual void ProcessInventoryInputRight();        // slot 41 (0056f4a1)
    virtual void ProcessButtonYReleased();            // slot 61 (0056f4b0)
};
void CTCInventoryExperience::ProcessInventoryInputLeft() { }
void CTCInventoryExperience::ProcessButtonYReleased() { }
static ExperienceSpendInfo g_pool[4], g_stats[10], g_abil[20];
int main()
{
    CTCInventoryExperience e; int i;
    for (i = 0; i < 20; ++i) { g_abil[i].currentSpend = 7; g_abil[i].initialSpend = 2; if (i < 10) { g_stats[i].currentSpend = 5; g_stats[i].initialSpend = 1; } if (i < 4) { g_pool[i].currentSpend = 9; g_pool[i].initialSpend = 3; } }
    e.ExperienceSpendValuesPool._M_start = g_pool; e.ExperienceSpendValuesStats._M_start = g_stats; e.ExperienceSpendValuesAbility._M_start = g_abil;
    e.ExperienceSpendMagnitude = 11;
    e.ProcessInventoryInputRight();
    int ok = std::strcmp(g_seq, "Right;") == 0 && e.ExperienceSpendMagnitude == 0 && g_pool[3].currentSpend == 3 && g_stats[9].currentSpend == 1
          && g_abil[0].currentSpend == 7 && g_abil[1].currentSpend == 2 && g_abil[19].currentSpend == 2;
    if (!ok) { std::printf("0056f4a1_TEST FAIL\n"); return 1; }
    std::printf("0056f4a1_TEST PASS\n");
    return 0;
}
