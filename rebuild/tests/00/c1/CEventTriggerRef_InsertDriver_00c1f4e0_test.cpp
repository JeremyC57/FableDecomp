// Behaviour test for 00c1f4e0: CItemAction::Handle -- forward (a, b) to the item's target; on a negative result reset the item type (-1: SetFamily2, other: SetVariant2) and ask again
#include <cstdio>

struct CItemTarget {
    long m_results[2]; int m_calls; long m_a, m_b;
    virtual long Handle(long a, long b) { m_a = a; m_b = b; return m_results[m_calls++ > 0 ? 1 : 0]; }
};
struct CItem {
    CItemTarget* m_target;
    virtual CItemTarget* GetTarget() { return m_target; }
};

struct CItemAction {
    void* m_pDispatcher;
    CItem* m_pItem;
    int m_variant, m_family;
    void SetVariant2() { ++m_variant; }
    void SetFamily2() { ++m_family; }
    long Handle(long a, long b)
    {
        long result = m_pItem->GetTarget()->Handle(a, b);
        if (result < 0) {
            if (result == -1)
                SetFamily2();
            else
                SetVariant2();
            result = m_pItem->GetTarget()->Handle(a, b);
        }
        return result;
    }
};

static bool Run(long r0, long r1, long want, int calls, int variant, int family)
{
    CItemTarget t; t.m_results[0] = r0; t.m_results[1] = r1; t.m_calls = 0;
    CItem item; item.m_target = &t;
    CItemAction a; a.m_pDispatcher = 0; a.m_pItem = &item; a.m_variant = 0; a.m_family = 0;
    long got = a.Handle(11, 22);
    return got == want && t.m_calls == calls && a.m_variant == variant && a.m_family == family && t.m_a == 11 && t.m_b == 22;
}

int main()
{
    if (!Run(5, 9, 5, 1, 0, 0) || !Run(0, 9, 0, 1, 0, 0)) { std::printf("00c1f4e0_TEST FAIL\n"); return 1; }
    if (!Run(-1, 4, 4, 2, 0, 1)) { std::printf("00c1f4e0_TEST FAIL\n"); return 1; }
    if (!Run(-2, -7, -7, 2, 1, 0)) { std::printf("00c1f4e0_TEST FAIL\n"); return 1; }
    std::printf("00c1f4e0_TEST PASS\n");
    return 0;
}