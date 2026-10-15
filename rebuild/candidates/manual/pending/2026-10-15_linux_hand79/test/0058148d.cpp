// Behaviour test for CTCHaste::RemoveMultiplier @ 0x0058148D: the source's entry leaves the multiplier map
// and the combined multiplier is recomputed from the remaining entries.
#include <cstdio>

struct CMultiplierMap {
    void* keys[4]; float values[4]; int count;
    unsigned long erase(void* const& key)
    {
        unsigned long n = 0; int w = 0;
        for (int r = 0; r < count; ++r) {
            if (keys[r] == key) { ++n; continue; }
            keys[w] = keys[r]; values[w] = values[r]; ++w;
        }
        count = w;
        return n;
    }
};

struct CTCHaste {
    float          Multiplier;
    CMultiplierMap Multipliers;
    void RecalculateMultiplier()
    {
        Multiplier = 1.0f;
        for (int i = 0; i < Multipliers.count; ++i)
            Multiplier *= Multipliers.values[i];
    }
    void RemoveMultiplier(void* source)
    {
        Multipliers.erase(source);
        RecalculateMultiplier();
    }
};

int main()
{
    int a, b;
    CTCHaste h;
    h.Multipliers.count = 2;
    h.Multipliers.keys[0] = &a; h.Multipliers.values[0] = 2.0f;
    h.Multipliers.keys[1] = &b; h.Multipliers.values[1] = 0.5f;
    h.Multiplier = 1.0f;
    h.RemoveMultiplier(&b);
    bool ok = h.Multipliers.count == 1 && h.Multiplier == 2.0f;
    h.RemoveMultiplier(&b);                     // absent: unchanged map, recomputed anyway
    if (h.Multipliers.count != 1 || h.Multiplier != 2.0f) ok = false;
    h.RemoveMultiplier(&a);
    if (h.Multipliers.count != 0 || h.Multiplier != 1.0f) ok = false;
    if (!ok) { std::printf("0058148d_TEST FAIL\n"); return 1; }
    std::printf("0058148d_TEST PASS\n");
    return 0;
}
