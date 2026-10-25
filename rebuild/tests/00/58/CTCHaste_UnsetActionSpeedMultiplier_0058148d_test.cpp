// Behaviour test for CTCHaste::UnsetActionSpeedMultiplier @ 0x0058148D: links the reconstructed object; the map erase
// and UpdateActionSpeedMultiplier are stubs over a small array model. The source's entry leaves the multiplier map
// and the combined multiplier is recomputed from the remaining entries.
#include <cstdio>
struct SEntry { void* key; float value; };
static SEntry g_entries[4]; static int g_count = 0;
struct CMultiplierMap {
    void* m_head;
    unsigned long erase(void* const& key);
};
unsigned long CMultiplierMap::erase(void* const& key)
{
    unsigned long n = 0; int w = 0;
    for (int r = 0; r < g_count; ++r) {
        if (g_entries[r].key == key) { ++n; continue; }
        g_entries[w++] = g_entries[r];
    }
    g_count = w;
    return n;
}
class CTCHaste {
public:
    void* __vftable; void* ParentThing; unsigned char _pad_0x08[4];
    float Multiplier; bool ToTerminate; long PauseFrames; CMultiplierMap Multipliers;
    void UnsetActionSpeedMultiplier(void* source);
private:
    void UpdateActionSpeedMultiplier();
};
void CTCHaste::UpdateActionSpeedMultiplier()
{
    Multiplier = 1.0f;
    for (int i = 0; i < g_count; ++i)
        Multiplier *= g_entries[i].value;
}
int main()
{
    int a, b;
    CTCHaste h = { };
    g_count = 2;
    g_entries[0].key = &a; g_entries[0].value = 2.0f;
    g_entries[1].key = &b; g_entries[1].value = 0.5f;
    h.Multiplier = 1.0f;
    h.UnsetActionSpeedMultiplier(&b);
    bool ok = g_count == 1 && h.Multiplier == 2.0f;
    h.UnsetActionSpeedMultiplier(&b);           // absent: unchanged map, recomputed anyway
    if (g_count != 1 || h.Multiplier != 2.0f) ok = false;
    h.UnsetActionSpeedMultiplier(&a);
    if (g_count != 0 || h.Multiplier != 1.0f) ok = false;
    if (!ok) { std::printf("0058148d_TEST FAIL\n"); return 1; }
    std::printf("0058148d_TEST PASS\n");
    return 0;
}
