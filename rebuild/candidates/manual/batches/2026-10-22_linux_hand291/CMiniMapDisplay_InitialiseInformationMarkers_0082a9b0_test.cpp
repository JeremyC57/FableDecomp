// Behaviour test for CMiniMapDisplay::InitialiseInformationMarkers (0x0082A9B0): which graphic each information thing gets.
#include <cstdio>

struct T { int region; bool info; bool follower; int def; };
static int Graphic(const T& t, int currentRegion, const int* guards)
{
    if (t.region != currentRegion || !t.info) return -1;           // no marker
    if (t.follower && (t.def == guards[0] || t.def == guards[1] || t.def == guards[2] || t.def == guards[3]))
        return 81;                                                  // [8].Secondary
    return 21;                                                      // [2].Secondary
}

int main()
{
    int guards[4] = { 10, 11, 12, 13 };
    T a = { 1, true, true, 12 }, b = { 1, true, false, 12 }, c = { 1, true, true, 99 }, d = { 2, true, true, 10 }, e = { 1, false, true, 10 };
    int ok = Graphic(a, 1, guards) == 81 && Graphic(b, 1, guards) == 21 && Graphic(c, 1, guards) == 21
          && Graphic(d, 1, guards) == -1 && Graphic(e, 1, guards) == -1;
    std::printf(ok ? "0082a9b0_TEST PASS\n" : "0082a9b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
