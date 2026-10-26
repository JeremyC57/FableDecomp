#include <cstdio>
class CThing { public: int id; };
struct CCategory { CThing* thing; unsigned char _body[0x2C - sizeof(CThing*)]; CThing* GetAssociatedThing() { return thing; } };
struct Inv {
    CCategory* first; long SelectedCategoryIndex;
    long GetSelectedCategoryIndex() { return SelectedCategoryIndex; }
    CCategory* GetCategoryWithIndex(long index) { return first + index; }
    CThing* GetSelectedCategoryThing() { return GetCategoryWithIndex(GetSelectedCategoryIndex())->GetAssociatedThing(); }
};
int main()
{
    CThing a, b; CCategory cats[2]; cats[0].thing = &a; cats[1].thing = &b;
    Inv inv; inv.first = cats; inv.SelectedCategoryIndex = 1;
    if (inv.GetSelectedCategoryThing() != &b) { std::printf("0061a040_TEST FAIL\n"); return 1; }
    inv.SelectedCategoryIndex = 0;
    if (inv.GetSelectedCategoryThing() != &a) { std::printf("0061a040_TEST FAIL\n"); return 1; }
    std::printf("0061a040_TEST PASS\n");
    return 0;
}