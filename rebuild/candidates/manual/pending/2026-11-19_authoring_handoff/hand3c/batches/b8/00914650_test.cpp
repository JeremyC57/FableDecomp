// Behaviour test for CAIStateGroup_BirdStartled::IsRunnable (0x00914650), a portable model of the row. Only a bird on the A* navigator
// can be startled. It searches the rough box of half-size ceil(2.5) = 3 cells round its cell for a creature that is neither a dead
// creature (type 13) nor of the bird's own def. The final distance test compares the bird with itself, as the original does, so it
// always passes: finding such a creature is enough.
#include <stdio.h>
#include <math.h>
const unsigned long NAVIGATOR_TYPE_A_STAR = 3;
struct Thing { float X, Y; unsigned char Type; unsigned long DefGlobalIndex; };
struct Bird { Thing self; unsigned long NavigatorType; const Thing* others; int count; };
static bool IsRunnable(const Bird& b)
{
    if (b.NavigatorType != NAVIGATOR_TYPE_A_STAR) return false;
    long r = (long)ceil(5.0 * 0.5);
    long cx = (long)b.self.X, cy = (long)b.self.Y;
    const Thing* found = 0;
    for (int i = 0; i < b.count; ++i)
    {
        const Thing& t = b.others[i];
        long tx = (long)t.X, ty = (long)t.Y;
        if (tx < cx - r || tx > cx + r || ty < cy - r || ty > cy + r) continue;
        if (t.Type == 13 || t.DefGlobalIndex == b.self.DefGlobalIndex) continue;
        found = &t; break;
    }
    if (!found) return false;
    float dx = b.self.X - b.self.X, dy = b.self.Y - b.self.Y;             // the bird against itself
    return dx * dx + dy * dy < 5.0f * 5.0f;
}
int main()
{
    int bad = 0;
    Thing near_villager = { 12.0f, 10.0f, 4, 77 }, far_villager = { 40.0f, 10.0f, 4, 77 };
    Thing dead = { 11.0f, 10.0f, 13, 77 }, other_bird = { 10.0f, 11.0f, 4, 5 };
    Thing a[] = { near_villager }, b[] = { far_villager, dead, other_bird };
    Bird bird = { { 10.0f, 10.0f, 4, 5 }, 3, a, 1 };
    if (!IsRunnable(bird)) ++bad;                                            // scared by a nearby living creature
    bird.NavigatorType = 1;
    if (IsRunnable(bird)) ++bad;                                             // not the A* navigator
    bird.NavigatorType = 3; bird.others = b; bird.count = 3;
    if (IsRunnable(bird)) ++bad;                                             // only far, dead or same-def things
    printf(bad ? "00914650_TEST FAIL\n" : "00914650_TEST PASS\n");
    return bad ? 1 : 0;
}
