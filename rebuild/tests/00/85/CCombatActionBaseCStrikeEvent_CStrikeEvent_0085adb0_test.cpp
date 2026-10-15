// Behaviour test for 0x0085ADB0: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model of the strike-event constructor's member placement.
struct Str { char s[16]; Str(const char* t) { std::strcpy(s, t); } Str(const Str& o) { std::strcpy(s, o.s); } };
struct Thing { int id; };
struct IP { const Thing* item; IP(const Thing* t) : item(0) { SetPItem(t); } void SetPItem(const Thing* t) { item = t; } };
struct Vec { void* a; void* b; void* c; Vec() : a(0), b(0), c(0) {} };
struct Strike {
    bool Legacy, PrimaryLegacy, Weapon; Str HelperPointName; int StrikeEventIndex, StartFrame, EndFrame; IP PWeapon; Vec StruckThingList;
    Strike(Thing* w, int idx, const Str& helper, int start, bool isWeapon, bool legacy, bool primary)
        : Legacy(legacy), PrimaryLegacy(primary), Weapon(isWeapon), HelperPointName(helper), StrikeEventIndex(idx), StartFrame(start), EndFrame(0), PWeapon(w) {}
};
static void Run()
{
    Thing sword; sword.id = 1;
    Strike s(&sword, 3, Str("hand_r"), 12, true, false, true);
    CHECK(!s.Legacy && s.PrimaryLegacy && s.Weapon && std::strcmp(s.HelperPointName.s, "hand_r") == 0);
    CHECK(s.StrikeEventIndex == 3 && s.StartFrame == 12 && s.EndFrame == 0 && s.PWeapon.item == &sword && s.StruckThingList.a == 0 && s.StruckThingList.c == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0085adb0_TEST FAIL\n"); return 1; }
    std::printf("0085adb0_TEST PASS\n");
    return 0;
}