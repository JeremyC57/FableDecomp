// behaviour test for 0x00936180: null and dead things are dead, live ones are not
#include <cstdio>
struct Thing { unsigned char flags; };
struct Ptr { Thing* p; Thing* GetPItem() const { return p; } };
static bool Dead(const Ptr& t) { if (t.GetPItem() != 0) { if (!(t.GetPItem()->flags & 1)) return false; } return true; }
int main()
{
    Thing live = { 2 }, dead = { 1 }; Ptr a = { &live }, b = { &dead }, n = { 0 };
    if (Dead(a) || !Dead(b) || !Dead(n)) { std::printf("00936180_TEST FAIL\n"); return 1; }
    std::printf("00936180_TEST PASS\n");
    return 0;
}