#include <cstdio>
// Behaviour test for 0x0056e5ec: base handler runs, then (InventorySelectionState == 1 only) the ExperienceSpendAdd flags are cleared.
struct ExperienceSpendEvent { bool enable; bool initiate; unsigned char _pad[2]; long time; };
static int g_baseCalls = 0;
struct CTCInventoryBase { void ProcessButtonAReleased() { ++g_baseCalls; } };
struct CTCInventoryExperience : CTCInventoryBase {
    long InventorySelectionState;
    ExperienceSpendEvent ExperienceSpendAdd;
    ExperienceSpendEvent ExperienceSpendRemove;
    void ProcessButtonAReleased();
};
void CTCInventoryExperience::ProcessButtonAReleased()
{
    CTCInventoryBase::ProcessButtonAReleased();
    if (InventorySelectionState == 1) {
        ExperienceSpendAdd.enable = false;
        ExperienceSpendAdd.initiate = false;
    }
}
int main()
{
    CTCInventoryExperience e;
    e.InventorySelectionState = 1;
    e.ExperienceSpendAdd.enable = true; e.ExperienceSpendAdd.initiate = true; e.ExperienceSpendAdd.time = 7;
    e.ExperienceSpendRemove.enable = true; e.ExperienceSpendRemove.initiate = true;
    e.ProcessButtonAReleased();
    if (g_baseCalls != 1 || e.ExperienceSpendAdd.enable || e.ExperienceSpendAdd.initiate || e.ExperienceSpendAdd.time != 7 || !e.ExperienceSpendRemove.enable || !e.ExperienceSpendRemove.initiate) { std::printf("0056e5ec_TEST FAIL\n"); return 1; }
    e.InventorySelectionState = 0;
    e.ExperienceSpendAdd.enable = true; e.ExperienceSpendAdd.initiate = true;
    e.ProcessButtonAReleased();
    if (g_baseCalls != 2 || !e.ExperienceSpendAdd.enable || !e.ExperienceSpendAdd.initiate) { std::printf("0056e5ec_TEST FAIL\n"); return 1; }
    std::printf("0056e5ec_TEST PASS\n");
    return 0;
}