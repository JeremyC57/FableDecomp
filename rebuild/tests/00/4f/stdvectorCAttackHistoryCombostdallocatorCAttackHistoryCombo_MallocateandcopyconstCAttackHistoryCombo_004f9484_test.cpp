// Behaviour test for 0x004F9484 std::vector<CAttackHistoryCombo>::_M_allocate_and_copy(n, first, last): a block of n
// combos is allocated and [first, last) copy-constructed into its front (each combo's own pair vector duplicated,
// the multiplier copied); n == 0 with an empty range returns null.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_vector_copies = 0;

struct PairVectorModel {
    long* data; unsigned int count;
    PairVectorModel() : data(0), count(0) { }
    PairVectorModel(const PairVectorModel& o) : data(0), count(o.count)
    {
        ++g_vector_copies;
        if (count) { data = (long*)std::malloc(count * sizeof(long)); for (unsigned int i = 0; i < count; ++i) data[i] = o.data[i]; }
    }
};

struct ComboModel {
    PairVectorModel AttackCombo;
    float Multiplier;
    ComboModel() : Multiplier(0.0f) { }
    ComboModel(const ComboModel& o) : AttackCombo(o.AttackCombo), Multiplier(o.Multiplier) { }
};

static ComboModel* allocate_and_copy_model(unsigned int n, const ComboModel* first, const ComboModel* last)
{
    ComboModel* result = n != 0 ? (ComboModel*)std::malloc(n * sizeof(ComboModel)) : 0;
    ComboModel* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) ComboModel(*first);
    return result;
}

int main()
{
    ComboModel src[2];
    src[0].Multiplier = 1.5f; src[1].Multiplier = 2.5f;
    src[1].AttackCombo.count = 2; src[1].AttackCombo.data = (long*)std::malloc(2 * sizeof(long));
    src[1].AttackCombo.data[0] = 11; src[1].AttackCombo.data[1] = 22;
    ComboModel* r = allocate_and_copy_model(4, src, src + 2);
    if (r == 0 || g_vector_copies != 2) { std::printf("004f9484_TEST FAIL\n"); return 1; }
    if (r[0].Multiplier != 1.5f || r[1].Multiplier != 2.5f || r[1].AttackCombo.count != 2 || r[1].AttackCombo.data == src[1].AttackCombo.data || r[1].AttackCombo.data[1] != 22) { std::printf("004f9484_TEST FAIL\n"); return 1; }
    if (allocate_and_copy_model(0, src, src) != 0 || g_vector_copies != 2) { std::printf("004f9484_TEST FAIL\n"); return 1; }
    std::printf("004f9484_TEST PASS\n");
    return 0;
}