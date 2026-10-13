#include <cstdio>

struct V {
    virtual float Step() { return 0.25f; }
};
struct Action : V { unsigned char pad[0x3C]; float RunningTimeSeconds;
    void Update() { RunningTimeSeconds += Step(); } };
int main()
{
    Action a; a.RunningTimeSeconds = 1.0f; a.Update();
    if (a.RunningTimeSeconds != 1.25f) { std::printf("00692480_TEST FAIL\n"); return 1; }
    std::printf("00692480_TEST PASS\n");
    return 0;
}