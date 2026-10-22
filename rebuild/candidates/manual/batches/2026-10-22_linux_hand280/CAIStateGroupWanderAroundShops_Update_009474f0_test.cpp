// Behaviour test for CAIStateGroup_WanderAroundShops::Update (0x009474F0): with a live shop the creature registers as a shopper, then for each browse walks to the pretty thing, looks at it, keeps looking until the random end frame and picks the next pretty thing; a dead shop only terminates; a missing next pretty thing stops early.
#include <cstdio>
struct CBrowse {
    bool shopAlive, registered, terminated;
    int prettyLeft;            // how many pretty things the finder still returns
    unsigned long count;
    int walks, looks, picks;
    long frame, endFrame;
    void Update()
    {
        if (shopAlive) {
            registered = true;
            for (unsigned long i = 0; i < count; ++i) {
                ++walks;
                ++looks;
                frame = 0;
                for (;;) {
                    ++looks;
                    ++frame;
                    if (frame < endFrame)
                        continue;
                    break;
                }
                ++picks;
                if (--prettyLeft < 0)
                    break;
            }
        }
        terminated = true;
    }
};
int main()
{
    CBrowse b = { true, false, false, 5, 3, 0, 0, 0, 0, 2 };
    b.Update();
    bool ok = b.registered && b.walks == 3 && b.picks == 3 && b.looks == 9 && b.terminated;
    CBrowse d = { false, false, false, 5, 3, 0, 0, 0, 0, 2 };
    d.Update();
    ok = ok && !d.registered && d.walks == 0 && d.terminated;
    CBrowse e = { true, false, false, 0, 3, 0, 0, 0, 0, 1 };
    e.Update();
    ok = ok && e.walks == 1 && e.picks == 1 && e.terminated;
    if (!ok) { std::printf("009474f0_TEST FAIL\n"); return 1; }
    std::printf("009474f0_TEST PASS\n");
    return 0;
}
