// Behaviour test for CTCVillage::IsHeroMurderer (0x0065A1FD): any recorded crime handle whose deed is a murder (type 9) with
// the flag set makes the hero a murderer; no hero or no deed log means false.
#include <cstdio>
struct Deed { long Type; bool Flag; };
static Deed g_deeds[4] = { { 3, true }, { 9, false }, { 9, true }, { 1, true } };
static Deed* GetDeed(long h) { return (h >= 0 && h < 4) ? &g_deeds[h] : 0; }
static bool IsHeroMurderer(bool hero, bool log, const long* crimes, int n)
{
    if (hero && log) {
        for (int i = 0; i < n; ++i) {
            Deed* d = GetDeed(crimes[i]);
            if (d && d->Type == 9 && d->Flag) return true;
        }
    }
    return false;
}
int main()
{
    long a[2] = { 0, 1 }, b[3] = { 7, 1, 2 };
    if (IsHeroMurderer(true, true, a, 2) || !IsHeroMurderer(true, true, b, 3) || IsHeroMurderer(false, true, b, 3) ||
        IsHeroMurderer(true, false, b, 3)) { std::printf("0065a1fd_TEST FAIL\n"); return 1; }
    std::printf("0065a1fd_TEST PASS\n");
    return 0;
}