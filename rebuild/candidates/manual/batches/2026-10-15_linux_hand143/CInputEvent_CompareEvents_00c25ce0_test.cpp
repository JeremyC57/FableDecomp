// Behaviour test for 0x00c25ce0: the event-key comparators (equality and strict ordering) over a host copy of the key.
#include <cstdio>
struct Block { unsigned int m_0; unsigned int m_id; };
struct Key {
    Block* m_pBlock; void* m_pRef; unsigned int m_index; unsigned int m_kind;
    Block* GetBlock() const { return m_pBlock; }
};
static bool Equal(const Key& a, const Key& b)
{
    if (a.m_kind != b.m_kind) return false;
    if (a.m_index != b.m_index) return false;
    if (a.m_index < 0x8000 && a.GetBlock()->m_id != b.GetBlock()->m_id) return false;
    if (a.m_index == 0) { if (a.m_pRef && b.m_pRef) return a.m_pRef == b.m_pRef; return a.m_pBlock == b.m_pBlock; }
    return true;
}
static bool Less(const Key& a, const Key& b)
{
    if (a.m_kind != b.m_kind) return a.m_kind < b.m_kind;
    if (a.m_index != b.m_index) return a.m_index < b.m_index;
    if (a.m_index < 0x8000 && a.GetBlock()->m_id != b.GetBlock()->m_id) return a.GetBlock()->m_id < b.GetBlock()->m_id;
    if (a.m_index == 0) { if (a.m_pRef && b.m_pRef) return a.m_pRef < b.m_pRef; return a.m_pBlock < b.m_pBlock; }
    return false;
}
int main()
{
    Block blocks[2]; blocks[0].m_id = 5; blocks[1].m_id = 7;
    int refs[2];
    Key k1 = { &blocks[0], &refs[0], 0, 1 };
    Key k2 = { &blocks[0], &refs[0], 0, 1 };
    Key k3 = { &blocks[1], &refs[0], 0, 1 };
    Key k4 = { &blocks[0], &refs[0], 0, 2 };
    Key k5 = { &blocks[0], &refs[0], 0x9000, 1 };
    Key k6 = { &blocks[1], &refs[1], 0x9000, 1 };
    int ok = 1;
    if (!Equal(k1, k2) || Less(k1, k2) || Less(k2, k1)) ok = 0;
    if (Equal(k1, k3) || !Less(k1, k3) || Less(k3, k1)) ok = 0;   // block ids 5 < 7
    if (Equal(k1, k4) || !Less(k1, k4)) ok = 0;                   // kind 1 < 2
    if (!Equal(k5, k6) || Less(k5, k6) || Less(k6, k5)) ok = 0;   // high index: equal regardless of pointers
    if (Equal(k1, k5) || !Less(k1, k5)) ok = 0;
    if (!ok) { std::printf("00c25ce0_TEST FAIL\n"); return 1; }
    std::printf("00c25ce0_TEST PASS\n");
    return 0;
}
