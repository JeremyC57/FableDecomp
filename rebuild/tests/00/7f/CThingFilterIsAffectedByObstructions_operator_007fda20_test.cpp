// Behaviour test for 0x007FDA20: IsAffectedByObstructions: false only for a mode-managed thing in HitByEpicSpell (0x5f)
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SMode { unsigned long Type; unsigned long GetType() const { return Type; } };
struct SThing { unsigned long Bits[8]; SMode* Mode;
    bool HasTC(long k) const { return (Bits[k >> 5] & (1UL << (k & 31))) != 0; } };

static bool IsAffectedByObstructions(const SThing& t)
{
    if (t.HasTC(0x31) && t.Mode->GetType() == 0x5f) return false;
    return true;
}
int main()
{
    SMode m; m.Type = 0x5f; SThing t; for (int i = 0; i < 8; ++i) t.Bits[i] = 0; t.Mode = &m;
    CHECK(IsAffectedByObstructions(t));
    t.Bits[1] = 0x20000; CHECK(!IsAffectedByObstructions(t));
    m.Type = 0x42; CHECK(IsAffectedByObstructions(t));
    if (!g_fail) printf("007fda20_TEST PASS\n");
    return g_fail;
}