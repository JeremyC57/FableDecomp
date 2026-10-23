#include <cstdio>
// Behaviour model of CSwordOfStrength::Main's crowd pick: first live crowd member that is not the current puller, only when the crowd has more than one.
struct Thing { int id; bool alive; };
static int Pick(const Thing* crowd, long count, int current)
{
    Thing thing = { 0, false };
    if (count > 1) {
        for (long i = 0; i < count; ++i) {
            thing = crowd[i];
            if (thing.alive && thing.id != current)
                break;
        }
    }
    return thing.alive ? thing.id : -1;
}
int main()
{
    Thing a[3] = { { 1, true }, { 2, false }, { 3, true } };
    Thing b[1] = { { 7, true } };
    Thing c[2] = { { 4, true }, { 5, false } };
    if (Pick(a, 3, 1) != 3 || Pick(b, 1, 0) != -1 || Pick(c, 2, 4) != -1 || Pick(a, 3, 9) != 1)
    { std::printf("00ed54d0_TEST FAIL\n"); return 1; }
    std::printf("00ed54d0_TEST PASS\n");
    return 0;
}
