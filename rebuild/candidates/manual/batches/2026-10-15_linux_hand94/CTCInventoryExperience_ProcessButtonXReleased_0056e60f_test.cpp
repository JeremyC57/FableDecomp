#include <cstdio>
// Behaviour test for 0x0056e60f: base handler runs, then (InventorySelectionState == 1 only) the ExperienceSpendRemove flags are cleared.
struct ExperienceSpendEvent { bool enable; bool initiate; unsigned char _pad[2]; long time; };
static int g_baseCalls = 0;
struct CTCInventoryBase { void ProcessButtonXReleased() { ++g_baseCalls; } };
struct CTCInventoryExperience : CTCInventoryBase {
    long InventorySelectionState;
    ExperienceSpendEvent ExperienceSpendAdd;
    ExperienceSpendEvent ExperienceSpendRemove;
    void ProcessButtonXReleased();
};
void CTCInventoryExperience::ProcessButtonXReleased()
{
    CTCInventoryBase::ProcessButtonXReleased();
    if (InventorySelectionState == 1) {
        ExperienceSpendRemove.enable = false;
        ExperienceSpendRemove.initiate = false;
    }
}
int main()
{
    CTCInventoryExperience e;
    e.InventorySelectionState = 1;
    e.ExperienceSpendRemove.enable = true; e.ExperienceSpendRemove.initiate = true; e.ExperienceSpendRemove.time = 7;
    e.ExperienceSpendAdd.enable = true; e.ExperienceSpendAdd.initiate = true;
    e.ProcessButtonXReleased();
    if (g_baseCalls != 1 || e.ExperienceSpendRemove.enable || e.ExperienceSpendRemove.initiate || e.ExperienceSpendRemove.time != 7 || !e.ExperienceSpendAdd.enable || !e.ExperienceSpendAdd.initiate) { std::printf("0056e60f_TEST FAIL\n"); return 1; }
    e.InventorySelectionState = 0;
    e.ExperienceSpendRemove.enable = true; e.ExperienceSpendRemove.initiate = true;
    e.ProcessButtonXReleased();
    if (g_baseCalls != 2 || !e.ExperienceSpendRemove.enable || !e.ExperienceSpendRemove.initiate) { std::printf("0056e60f_TEST FAIL\n"); return 1; }
    std::printf("0056e60f_TEST PASS\n");
    return 0;
}
