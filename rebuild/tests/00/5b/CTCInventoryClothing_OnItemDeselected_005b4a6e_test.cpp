#include <cstdio>
class CThing { public: int id; };
struct Clothing {
    CThing* PMannequin; bool UseMannequin; int setups; CThing* dressed;
    void SetUpSelectedClothes() { ++setups; }
    void SetThingAsWearingSelectedClothes(CThing* thing) { dressed = thing; }
    void OnItemDeselected(long, long)
    {
        SetUpSelectedClothes();
        if (UseMannequin && PMannequin)
            SetThingAsWearingSelectedClothes(PMannequin);
    }
};
int main()
{
    CThing m; Clothing c; c.PMannequin = &m; c.UseMannequin = false; c.setups = 0; c.dressed = 0;
    c.OnItemDeselected(1, 2);
    if (c.setups != 1 || c.dressed) { std::printf("005b4a6e_TEST FAIL\n"); return 1; }
    c.UseMannequin = true; c.PMannequin = 0; c.OnItemDeselected(1, 2);
    if (c.setups != 2 || c.dressed) { std::printf("005b4a6e_TEST FAIL\n"); return 1; }
    c.PMannequin = &m; c.OnItemDeselected(1, 2);
    if (c.setups != 3 || c.dressed != &m) { std::printf("005b4a6e_TEST FAIL\n"); return 1; }
    std::printf("005b4a6e_TEST PASS\n");
    return 0;
}