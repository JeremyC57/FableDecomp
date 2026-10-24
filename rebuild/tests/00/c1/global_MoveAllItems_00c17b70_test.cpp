// Behaviour test for 00c17b70: MoveAllItems -- reserves room in dest for both counts, then moves items one by one (remove from source, add to dest) until the source reports no first item
#include <cstdio>

class PKCContainer {
public:
    void* m_items[16];
    long m_count;
    long m_reserved;
    void* m_lastOwner;
    PKCContainer() : m_count(0), m_reserved(0), m_lastOwner(0) { }
    virtual void Reserve(long count) { m_reserved = count; }
    virtual void Add(void* owner, void* item) { m_lastOwner = owner; m_items[m_count++] = item; }
    virtual void Remove(void* owner, void* item)
    {
        m_lastOwner = owner;
        for (long i = 0; i < m_count; ++i)
            if (m_items[i] == item) { for (long j = i; j + 1 < m_count; ++j) m_items[j] = m_items[j + 1]; --m_count; return; }
    }
    virtual long GetCount() { return m_count; }
    virtual void* GetFirst() { return m_count ? m_items[0] : 0; }
};

void MoveAllItems(void* owner, PKCContainer* source, PKCContainer* dest)
{
    dest->Reserve(source->GetCount() + dest->GetCount());
    for (void* item = source->GetFirst(); item; item = source->GetFirst()) {
        source->Remove(owner, item);
        dest->Add(owner, item);
    }
}

int main()
{
    int a, b, c, d, owner;
    PKCContainer src, dst;
    src.Add(0, &a); src.Add(0, &b); src.Add(0, &c);
    dst.Add(0, &d);
    MoveAllItems(&owner, &src, &dst);
    if (dst.m_reserved != 4 || src.m_count != 0 || dst.m_count != 4) { std::printf("00c17b70_TEST FAIL\n"); return 1; }
    if (dst.m_items[0] != &d || dst.m_items[1] != &a || dst.m_items[2] != &b || dst.m_items[3] != &c) { std::printf("00c17b70_TEST FAIL\n"); return 1; }
    if (src.m_lastOwner != &owner || dst.m_lastOwner != &owner) { std::printf("00c17b70_TEST FAIL\n"); return 1; }
    PKCContainer empty;
    MoveAllItems(&owner, &empty, &dst);
    if (dst.m_reserved != 4 || dst.m_count != 4) { std::printf("00c17b70_TEST FAIL\n"); return 1; }
    std::printf("00c17b70_TEST PASS\n");
    return 0;
}