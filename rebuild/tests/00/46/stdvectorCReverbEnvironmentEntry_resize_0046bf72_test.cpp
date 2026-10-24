// Behaviour test for 0x0046BF72 std::vector<CReverbEnvironmentEntry>::resize(n): forwards n and a zero-filled value
#include <cstdio>
struct CReverbEnvironmentEntry { float Params[21]; };
struct EntryVector {
    unsigned int lastN; int allZero; int calls;
    void resize(unsigned int n, const CReverbEnvironmentEntry& x)
    {
        ++calls; lastN = n; allZero = 1;
        for (int i = 0; i < 21; ++i) if (x.Params[i] != 0.0f) allZero = 0;
    }
    void resize(unsigned int n) { resize(n, CReverbEnvironmentEntry()); }
};
int main()
{
    EntryVector v; v.lastN = 0; v.allZero = 0; v.calls = 0;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !v.allZero) { std::printf("0046bf72_TEST FAIL\n"); return 1; }
    v.resize(0);
    if (v.calls != 2 || v.lastN != 0 || !v.allZero) { std::printf("0046bf72_TEST FAIL\n"); return 1; }
    std::printf("0046bf72_TEST PASS\n");
    return 0;
}