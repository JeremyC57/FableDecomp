// Behaviour test for 0x006C3DF0 CTCFadeOutAndIn::AddThingToFade(CThing&).
#include <cstdio>
#include <vector>
#include <algorithm>
class CThing { public: int id; };
static int g_live = 0;
struct IPtr {                                   // CIntelligentPointer<CThing>: equal when observing the same thing
    CThing* item;
    IPtr(CThing* p) : item(p) { ++g_live; }
    IPtr(const IPtr& o) : item(o.item) { ++g_live; }
    ~IPtr() { --g_live; }
    bool operator==(const IPtr& o) const { return item == o.item; }
};
class CTCFadeOutAndIn { public:
    std::vector<IPtr> FadingThings;
    void AddThingToFade(CThing& thing) {
        if (std::find(FadingThings.begin(), FadingThings.end(), IPtr(&thing)) == FadingThings.end())
            FadingThings.push_back(IPtr(&thing));
    }
};
int main() {
    int ok = 1;
    {
        CThing a = { 1 }, b = { 2 }, c = { 3 };
        CTCFadeOutAndIn tc;
        tc.AddThingToFade(a);
        tc.AddThingToFade(b);
        tc.AddThingToFade(a);                   // already fading: no second entry
        tc.AddThingToFade(c);
        tc.AddThingToFade(b);
        ok &= tc.FadingThings.size() == 3;
        ok &= tc.FadingThings.size() == 3 && tc.FadingThings[0].item == &a && tc.FadingThings[1].item == &b && tc.FadingThings[2].item == &c;
        ok &= g_live == 3;                      // every temporary died
    }
    ok &= g_live == 0;
    if (!ok) { std::printf("006c3df0_TEST FAIL\n"); return 1; }
    std::printf("006c3df0_TEST PASS\n");
    return 0;
}