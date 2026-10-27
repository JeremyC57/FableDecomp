// Behaviour test for 0x0080FF20: GetInterface<CTCAISummon>: bit 0xfa set -> the map entry (or end's) component and true
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SEntry { long Type; void* Component; };
struct SThing { unsigned long Bits[9]; SEntry* First; SEntry* Last;
    bool GetSummon(void** out)
    {
        const long k = 0xfa;
        if (Bits[k >> 5] & (1UL << (k & 31))) {
            SEntry* it = First; while (it != Last && it->Type < k) ++it;
            if (it == Last || k < it->Type) it = Last;
            *out = it->Component; return true;
        }
        return false;
    } };
int main()
{
    int summon = 5, endc = 9;
    SEntry e[3]; e[0].Type = 0x31; e[0].Component = 0; e[1].Type = 0xfa; e[1].Component = &summon; e[2].Type = 0; e[2].Component = &endc;
    SThing t; for (int i = 0; i < 9; ++i) t.Bits[i] = 0; t.First = e; t.Last = e + 2;
    void* out = 0;
    CHECK(!t.GetSummon(&out) && out == 0);
    t.Bits[7] = 0x4000000; CHECK(t.GetSummon(&out) && out == &summon);
    if (!g_fail) printf("0080ff20_TEST PASS\n");
    return g_fail;
}