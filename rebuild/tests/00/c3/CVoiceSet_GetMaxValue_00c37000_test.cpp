// Behaviour test for 00c37000: CVoiceSet::GetMaxValue -- a stack visitor keeps the largest item value, starting from 0
#include <cstdio>
struct SItem { float m_value; float GetValue() const { return m_value; } };
struct IValueVisitor {
    virtual void Visit(void* item) = 0;
    virtual ~IValueVisitor() { }
};
struct CMaxValueVisitor : IValueVisitor {
    float m_max;
    CMaxValueVisitor() : m_max(0) { }
    virtual void Visit(void* item)
    {
        if (((SItem*)item)->GetValue() > m_max)
            m_max = ((SItem*)item)->GetValue();
    }
};
struct IItemCollection {
    SItem* m_items; int m_count;
    void ForEach(IValueVisitor* visitor) { for (int i = 0; i < m_count; ++i) visitor->Visit(&m_items[i]); }
};
struct CVoiceSet {
    IItemCollection m_items;
    float GetMaxValue()
    {
        CMaxValueVisitor visitor;
        m_items.ForEach(&visitor);
        return visitor.m_max;
    }
};
int main()
{
    SItem items[4] = { {0.25f}, {0.75f}, {0.5f}, {-2.0f} };
    CVoiceSet set; set.m_items.m_items = items; set.m_items.m_count = 4;
    float m = set.GetMaxValue();
    float d = m - 0.75f; if (d < 0) d = -d;
    int ok = d < 1e-6f;
    set.m_items.m_count = 0;
    ok = ok && set.GetMaxValue() == 0.0f;
    set.m_items.m_items = items + 3; set.m_items.m_count = 1;
    ok = ok && set.GetMaxValue() == 0.0f;   // negative values never beat the initial 0
    std::printf(ok ? "00c37000_TEST PASS\n" : "00c37000_TEST FAIL\n");
    return ok ? 0 : 1;
}