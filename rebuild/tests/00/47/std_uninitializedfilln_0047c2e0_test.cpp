// Behaviour test for 0x0047C2E0 std::__uninitialized_fill_n<CQuestDesc*> (model copy over stand-in members).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_stringCopies = 0, g_vectorCopies = 0;
struct CCharStringModel { const char* p; CCharStringModel(const char* s) : p(s) { } CCharStringModel(const CCharStringModel& o) : p(o.p) { ++g_stringCopies; } };
struct VectorModel { int n; VectorModel(int k) : n(k) { } VectorModel(const VectorModel& o) : n(o.n) { ++g_vectorCopies; } };
struct CQuestDesc {
    unsigned long    ScriptID;
    CCharStringModel TextDescription;
    long             QuestStatus;
    VectorModel      QuestSubObjectives;
    CQuestDesc(unsigned long id, const char* t, long s, int n) : ScriptID(id), TextDescription(t), QuestStatus(s), QuestSubObjectives(n) { }
};

static CQuestDesc* uninitialized_fill_n_model(CQuestDesc* first, unsigned int n, const CQuestDesc& x)
{
    CQuestDesc* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CQuestDesc(x);
    return cur;
}

int main()
{
    CQuestDesc x(42, "quest", 3, 5);
    CQuestDesc* raw = (CQuestDesc*)std::malloc(4 * sizeof(CQuestDesc));
    CQuestDesc* end = uninitialized_fill_n_model(raw, 3, x);
    int ok = end == raw + 3 && raw[2].ScriptID == 42 && raw[1].QuestStatus == 3 && raw[0].QuestSubObjectives.n == 5
          && g_stringCopies == 3 && g_vectorCopies == 3;
    if (uninitialized_fill_n_model(raw, 0, x) != raw) ok = 0;
    std::free(raw);
    if (!ok) { std::printf("0047c2e0_TEST FAIL\n"); return 1; }
    std::printf("0047c2e0_TEST PASS\n");
    return 0;
}