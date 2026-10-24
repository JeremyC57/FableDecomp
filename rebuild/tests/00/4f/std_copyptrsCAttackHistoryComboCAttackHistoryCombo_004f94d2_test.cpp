// Behaviour test for 0x004F94D2 std::__copy_ptrs<CAttackHistoryCombo*>(first, last, result, __false_type): each combo of
// [first, last) is assigned in order onto result (vector and multiplier), the advanced result is returned, an empty
// or reversed range assigns nothing.
#include <cstdio>

static int g_assigns = 0;

struct PairVectorModel {
    long value;
    PairVectorModel& operator=(const PairVectorModel& o) { value = o.value; ++g_assigns; return *this; }
};

struct ComboModel {
    PairVectorModel AttackCombo;
    float Multiplier;
    ComboModel& operator=(const ComboModel& o) { AttackCombo = o.AttackCombo; Multiplier = o.Multiplier; return *this; }
};

static ComboModel* copy_ptrs_model(ComboModel* first, ComboModel* last, ComboModel* result)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    ComboModel a[3], b[4];
    for (int i = 0; i < 3; ++i) { a[i].AttackCombo.value = 10 + i; a[i].Multiplier = 0.5f * (float)(i + 1); }
    for (int i = 0; i < 4; ++i) { b[i].AttackCombo.value = -1; b[i].Multiplier = -1.0f; }
    if (copy_ptrs_model(a, a + 3, b) != b + 3 || g_assigns != 3) { std::printf("004f94d2_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (b[i].AttackCombo.value != 10 + i || b[i].Multiplier != 0.5f * (float)(i + 1)) { std::printf("004f94d2_TEST FAIL\n"); return 1; }
    if (b[3].AttackCombo.value != -1) { std::printf("004f94d2_TEST FAIL\n"); return 1; }
    if (copy_ptrs_model(a + 2, a + 2, b) != b || copy_ptrs_model(a + 2, a, b) != b || g_assigns != 3) { std::printf("004f94d2_TEST FAIL\n"); return 1; }
    std::printf("004f94d2_TEST PASS\n");
    return 0;
}