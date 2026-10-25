#include <cstdio>
static int g_calls = 0; static bool g_between = false;
struct CQuestManager { bool IsBetweenQuestStartAndFinishScreens() { return g_between; } };
static CQuestManager g_qm; static CQuestManager* g_pQuestManager = &g_qm;
struct CCategory { unsigned char _body[0x2C]; int stripped; void RemoveAllNonQuestPersistentItems() { ++stripped; ++g_calls; } };
struct CCategoryVector {
    CCategory* first; CCategory* last; CCategory* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory& operator[](unsigned long n) { return first[n]; }
};
struct Inv {
    CCategoryVector Categories;
    void OnInitialActivate()
    {
        if (!g_pQuestManager->IsBetweenQuestStartAndFinishScreens()) {
            for (unsigned long i = 0; i < Categories.size(); ++i)
                Categories[i].RemoveAllNonQuestPersistentItems();
        }
    }
};
int main()
{
    CCategory cats[3]; for (int k = 0; k < 3; ++k) cats[k].stripped = 0;
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    g_between = true; inv.OnInitialActivate();
    if (g_calls != 0) { std::printf("005bd1eb_TEST FAIL\n"); return 1; }
    g_between = false; inv.OnInitialActivate();
    if (g_calls != 3 || cats[0].stripped != 1 || cats[2].stripped != 1) { std::printf("005bd1eb_TEST FAIL\n"); return 1; }
    std::printf("005bd1eb_TEST PASS\n");
    return 0;
}