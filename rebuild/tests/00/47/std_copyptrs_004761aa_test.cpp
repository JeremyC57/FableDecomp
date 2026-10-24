// Behaviour test for 0x004761AA std::__copy_ptrs<CQuickAccessItemButtonGuiDef*> (model: base assignment counted, members copied).
#include <cstdio>

static int g_baseAssigns = 0;
struct BaseModel {
    int Name;
    BaseModel& operator=(const BaseModel& o) { Name = o.Name; ++g_baseAssigns; return *this; }
};
struct ButtonDef : BaseModel {
    long ButtonGraphic;
    float PositionX, PositionY, OffsetX, OffsetY;
};

static ButtonDef* CopyPtrs(ButtonDef* first, ButtonDef* last, ButtonDef* result)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    ButtonDef a[4];
    for (int i = 0; i < 4; ++i) { a[i].Name = i; a[i].ButtonGraphic = 10 * i; a[i].PositionX = a[i].OffsetY = (float)i; }
    ButtonDef* end = CopyPtrs(a + 1, a + 4, a);   // erase(a[0]) shift
    int ok = end == a + 3 && a[0].Name == 1 && a[2].ButtonGraphic == 30 && a[1].OffsetY == 2.0f && g_baseAssigns == 3;
    if (CopyPtrs(a, a, a + 2) != a + 2) ok = 0;
    if (!ok) { std::printf("004761aa_TEST FAIL\n"); return 1; }
    std::printf("004761aa_TEST PASS\n");
    return 0;
}