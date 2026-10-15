// Behaviour test for CThing::vfunc_29 @ 0x004C77B0: Drawable && Activated && !Dead over all 8 combinations.
#include <cstdio>
struct CThing {
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char Drawable : 1;
    bool vfunc_29() { return Drawable && Activated && !Dead; }
};
int main()
{
    for (int m = 0; m < 8; ++m) {
        CThing t; t.CreatedByOwnerThingCreation = 1; t.IncludeInThingScans = 1;
        t.Drawable = m & 1; t.Activated = (m >> 1) & 1; t.Dead = (m >> 2) & 1;
        bool want = (m == 3);
        if (t.vfunc_29() != want) { std::printf("004c77b0_TEST FAIL\n"); return 1; }
    }
    std::printf("004c77b0_TEST PASS\n");
    return 0;
}
