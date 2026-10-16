// Behaviour test for 005dc9b0: CAnimComponentSetCreatureMode::Initialise -- reset, parse a mode; zero fails and leaves Mode alone.
#include <cstdio>
struct TComp {
    long Mode; int resets;
    long parsed;
    TComp() : Mode(7), resets(0), parsed(0) {}
    void Reset() { ++resets; }
    long ParseLong() { return parsed; }
    bool Initialise()
    {
        Reset();
        long mode = ParseLong();
        if (mode) { Mode = mode; return true; }
        return false;
    }
};
int main()
{
    TComp c; c.parsed = 0;
    if (c.Initialise() || c.Mode != 7 || c.resets != 1) { std::printf("005dc9b0_TEST FAIL\n"); return 1; }
    c.parsed = 3;
    if (!c.Initialise() || c.Mode != 3 || c.resets != 2) { std::printf("005dc9b0_TEST FAIL\n"); return 1; }
    std::printf("005dc9b0_TEST PASS\n");
    return 0;
}