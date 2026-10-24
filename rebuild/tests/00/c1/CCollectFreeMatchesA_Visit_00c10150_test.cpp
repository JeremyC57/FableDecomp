// behaviour test for 0x00C10150 CCollectFreeMatchesA::Visit: collect items that match the key and carry no claim
#include <cstdio>

struct CClaimInfo {
    long m_claim;
    virtual long GetClaim() { return m_claim; }
};

struct CVisitedItem {
    long m_key;
    CClaimInfo m_info;
    virtual CClaimInfo* GetClaimInfo() { return &m_info; }
    long MatchesKey(long key) { return m_key == key; }        // stand-in for 0x00C1EC70
};

struct CItemList {
    CVisitedItem* m_items[8];
    int m_count;
    CItemList() : m_count(0) { }
    virtual void Add(CVisitedItem* item) { m_items[m_count++] = item; }
};

struct CItemVisitor {
    virtual void Visit(CVisitedItem* item) = 0;
    virtual ~CItemVisitor() { }
};

struct CCollectFreeMatchesA : public CItemVisitor {
    CItemList* m_pList;
    long m_key;
    virtual void Visit(CVisitedItem* item);
};

void CCollectFreeMatchesA::Visit(CVisitedItem* item)
{
    if (item->MatchesKey(m_key)) {
        if (item->GetClaimInfo()->GetClaim() == 0)
            m_pList->Add(item);
    }
}

int main()
{
    CVisitedItem items[4];
    long keys[4] = { 5, 5, 6, 5 }, claims[4] = { 0, 1, 0, 0 };
    for (int i = 0; i < 4; ++i) { items[i].m_key = keys[i]; items[i].m_info.m_claim = claims[i]; }
    CItemList list;
    CCollectFreeMatchesA visitor; visitor.m_pList = &list; visitor.m_key = 5;
    CItemVisitor* v = &visitor;
    for (int i = 0; i < 4; ++i) v->Visit(&items[i]);
    if (list.m_count != 2 || list.m_items[0] != &items[0] || list.m_items[1] != &items[3]) { std::printf("00c10150_TEST FAIL\n"); return 1; }
    std::printf("00c10150_TEST PASS\n");
    return 0;
}