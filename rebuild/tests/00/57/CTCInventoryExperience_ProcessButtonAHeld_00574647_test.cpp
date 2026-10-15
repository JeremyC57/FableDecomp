#include <cstdio>
// Behaviour test for 0x00574647: base call; state 1: enabled -> AddExperienceSpend; first press -> initiate + time = offset * scale.
struct ExperienceSpendEvent { bool enable; bool initiate; unsigned char _pad[2]; long time; };
static int g_base = 0, g_step = 0, g_destroyed = 0;
struct CInventoryDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long refCount;
    long ExperienceSpendTimeOffset;
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};
struct CInventoryDefPointer {
    CInventoryDef* m_p;
    CInventoryDefPointer(CInventoryDef* p) : m_p(p) {}
    CInventoryDefPointer(const CInventoryDefPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CInventoryDefPointer() { m_p->Release(); }
    CInventoryDef* operator->() const { return m_p; }
};
static long g_ExperienceSpendTimeScale = 3;
struct CTCInventoryBase { void ProcessButtonAHeld() { ++g_base; } };
struct CTCInventoryExperience : CTCInventoryBase {
    long InventorySelectionState;
    CInventoryDefPointer PDef;
    ExperienceSpendEvent ExperienceSpendAdd;
    ExperienceSpendEvent ExperienceSpendRemove;
    CTCInventoryExperience(CInventoryDef* d) : PDef(d) {}
    void AddExperienceSpend() { ++g_step; }
    void ProcessButtonAHeld();
};
void CTCInventoryExperience::ProcessButtonAHeld()
{
    CTCInventoryBase::ProcessButtonAHeld();
    if (InventorySelectionState == 1) {
        if (ExperienceSpendAdd.enable) {
            AddExperienceSpend();
        } else if (!ExperienceSpendAdd.initiate) {
            ExperienceSpendAdd.initiate = true;
            long scale = g_ExperienceSpendTimeScale;
            CInventoryDefPointer def(PDef);
            ExperienceSpendAdd.time = def->ExperienceSpendTimeOffset * scale;
        }
    }
}
int main()
{
    CInventoryDef def; def.refCount = 1; def.ExperienceSpendTimeOffset = 14;
    CTCInventoryExperience e(&def);
    e.ExperienceSpendAdd.enable = false; e.ExperienceSpendAdd.initiate = false; e.ExperienceSpendAdd.time = 0;
    e.ExperienceSpendRemove.enable = true; e.ExperienceSpendRemove.initiate = false; e.ExperienceSpendRemove.time = 0;
    e.InventorySelectionState = 0; e.ProcessButtonAHeld();
    if (g_base != 1 || e.ExperienceSpendAdd.initiate || g_step != 0) { std::printf("00574647_TEST FAIL\n"); return 1; }
    e.InventorySelectionState = 1; e.ProcessButtonAHeld();
    if (g_base != 2 || !e.ExperienceSpendAdd.initiate || e.ExperienceSpendAdd.time != 42 || def.refCount != 1 || g_destroyed != 0 || g_step != 0 || e.ExperienceSpendRemove.time != 0) { std::printf("00574647_TEST FAIL\n"); return 1; }
    e.ExperienceSpendAdd.time = 5; e.ProcessButtonAHeld();                     // already initiated: nothing
    if (e.ExperienceSpendAdd.time != 5 || g_step != 0) { std::printf("00574647_TEST FAIL\n"); return 1; }
    e.ExperienceSpendAdd.enable = true; e.ProcessButtonAHeld();
    if (g_step != 1 || g_base != 4) { std::printf("00574647_TEST FAIL\n"); return 1; }
    std::printf("00574647_TEST PASS\n");
    return 0;
}