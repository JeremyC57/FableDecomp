#include <cstdio>
static int g_capture = 0, g_anim = 0;
struct CUserProfileManager {
    static CUserProfileManager& Get() { static CUserProfileManager m; return m; }
    void CaptureBackBufferForUIBackgroundIfNonExistant() { ++g_capture; }
};
struct CCategory { unsigned char _body[0x2C]; int selected; void SetAsSelected(bool s) { selected = s ? 1 : -1; } };
struct CCategoryVector { CCategory* first; CCategory* last; CCategory* end_of_storage; CCategory& operator[](unsigned long n) { return first[n]; } };
struct Inv {
    CCategoryVector Categories; bool InventoryOpen; long SelectedCategoryIndex; bool CategoryMoving; float CategoryMovement;
    void UpdateInventoryAnim() { ++g_anim; }
    void GlobalUpdate(float)
    {
        if (InventoryOpen) {
            CUserProfileManager::Get().CaptureBackBufferForUIBackgroundIfNonExistant();
            if (CategoryMoving && CategoryMovement == 1.0f) {
                CategoryMoving = false;
                Categories[SelectedCategoryIndex].SetAsSelected(true);
            }
            UpdateInventoryAnim();
        }
    }
};
int main()
{
    CCategory cats[2]; cats[0].selected = cats[1].selected = 0;
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 2; inv.Categories.end_of_storage = cats + 2;
    inv.InventoryOpen = false; inv.SelectedCategoryIndex = 1; inv.CategoryMoving = true; inv.CategoryMovement = 1.0f;
    inv.GlobalUpdate(0.1f);
    if (g_capture || g_anim || !inv.CategoryMoving) { std::printf("005bdd39_TEST FAIL\n"); return 1; }
    inv.InventoryOpen = true; inv.CategoryMovement = 0.5f; inv.GlobalUpdate(0.1f);
    if (g_capture != 1 || g_anim != 1 || !inv.CategoryMoving || cats[1].selected) { std::printf("005bdd39_TEST FAIL\n"); return 1; }
    inv.CategoryMovement = 1.0f; inv.GlobalUpdate(0.1f);
    if (g_anim != 2 || inv.CategoryMoving || cats[1].selected != 1 || cats[0].selected) { std::printf("005bdd39_TEST FAIL\n"); return 1; }
    std::printf("005bdd39_TEST PASS\n");
    return 0;
}
