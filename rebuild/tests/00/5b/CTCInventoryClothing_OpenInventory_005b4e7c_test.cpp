#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
struct Clothing;
namespace NUISystem { class CManager { public:
    Clothing* opened;
    virtual void OnInventoryOpened(Clothing* inventory) { opened = inventory; std::strcat(g_seq, "F"); }
    static CManager* __fastcall GetInstance() { static CManager m; return &m; }
}; }
struct Base { void OpenInventory() { std::strcat(g_seq, "B"); } };
struct Clothing : Base {
    bool UseMannequin; bool flagAtSetup;
    void InitialiseMannequin() { std::strcat(g_seq, "I"); }
    void SetUpSelectedClothes() { flagAtSetup = UseMannequin; std::strcat(g_seq, "S"); }
    void OpenInventory()
    {
        InitialiseMannequin();
        UseMannequin = true;
        SetUpSelectedClothes();
        Base::OpenInventory();
        NUISystem::CManager::GetInstance()->OnInventoryOpened(this);
    }
};
int main()
{
    Clothing c; c.UseMannequin = false; c.flagAtSetup = false;
    NUISystem::CManager::GetInstance()->opened = 0;
    c.OpenInventory();
    if (std::strcmp(g_seq, "ISBF") || !c.flagAtSetup || NUISystem::CManager::GetInstance()->opened != &c) { std::printf("005b4e7c_TEST FAIL\n"); return 1; }
    std::printf("005b4e7c_TEST PASS\n");
    return 0;
}