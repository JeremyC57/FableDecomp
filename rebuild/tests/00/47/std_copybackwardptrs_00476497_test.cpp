// Behaviour test for 0x00476497 std::__copy_backward_ptrs<CQuickAccessItemButtonGuiDef*> (model: overlapping right shift).
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

static ButtonDef* CopyBackwardPtrs(ButtonDef* first, ButtonDef* last, ButtonDef* result)
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

int main()
{
    ButtonDef a[5];
    for (int i = 0; i < 5; ++i) { a[i].Name = i; a[i].ButtonGraphic = 10 * i; }
    ButtonDef* r = CopyBackwardPtrs(a, a + 3, a + 4);   // shift a[0..2] right by one (overlapping)
    int ok = r == a + 1 && a[1].Name == 0 && a[2].Name == 1 && a[3].ButtonGraphic == 20 && a[4].Name == 4
          && g_baseAssigns == 3;
    if (!ok) { std::printf("00476497_TEST FAIL\n"); return 1; }
    std::printf("00476497_TEST PASS\n");
    return 0;
}