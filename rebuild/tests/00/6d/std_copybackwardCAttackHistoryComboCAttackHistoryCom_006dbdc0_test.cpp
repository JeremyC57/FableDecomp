// Behaviour test for 0x006DBDC0 __copy_backward over CAttackHistoryCombo: [first, last) is assigned backwards onto
// the range ending at result (safe for an overlapping shift right) and the start of the written range is returned.
#include <cstdio>

static int g_vectorAssigns = 0;
struct ComboVector { int tag; ComboVector& operator=(const ComboVector& o) { tag = o.tag; ++g_vectorAssigns; return *this; } };
struct CAttackHistoryCombo { ComboVector AttackCombo; float Multiplier; };

static CAttackHistoryCombo* CopyBackward(CAttackHistoryCombo* first, CAttackHistoryCombo* last, CAttackHistoryCombo* result)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

int main()
{
    CAttackHistoryCombo a[5];
    for (int i = 0; i < 5; ++i) { a[i].AttackCombo.tag = i; a[i].Multiplier = 1.5f * i; }
    CAttackHistoryCombo* r = CopyBackward(a, a + 3, a + 5);       // shift [0,3) to [2,5)
    if (r != a + 2 || g_vectorAssigns != 3) { std::printf("006dbdc0_TEST FAIL\n"); return 1; }
    if (a[2].AttackCombo.tag != 0 || a[3].AttackCombo.tag != 1 || a[4].AttackCombo.tag != 2 || a[4].Multiplier != 3.0f) { std::printf("006dbdc0_TEST FAIL\n"); return 1; }
    r = CopyBackward(a, a, a + 4);
    if (r != a + 4 || g_vectorAssigns != 3) { std::printf("006dbdc0_TEST FAIL\n"); return 1; }
    std::printf("006dbdc0_TEST PASS\n");
    return 0;
}