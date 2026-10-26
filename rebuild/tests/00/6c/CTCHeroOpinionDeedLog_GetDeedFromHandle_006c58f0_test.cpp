#include <cstdio>
// Behaviour of CTCHeroOpinionDeedLog::GetDeedFromHandle (0x006C58F0): the mapped deed for a logged handle, null otherwise.
class COpinionDeed { public: long DeedHandle; };
struct SOpinionDeedNode { SOpinionDeedNode* Next; long Key; COpinionDeed* Value; };
struct COpinionDeedIterator {
    SOpinionDeedNode* m_pNode;
    COpinionDeedIterator(SOpinionDeedNode* node) : m_pNode(node) {}
    bool operator==(const COpinionDeedIterator& other) const { return m_pNode == other.m_pNode; }
    SOpinionDeedNode* operator->() const { return m_pNode; }
};
struct COpinionDeedMap {
    SOpinionDeedNode* m_pHeader;   // sentinel; entries chained from m_pHeader->Next
    COpinionDeedIterator find(const long& key) const
    {
        for (SOpinionDeedNode* n = m_pHeader->Next; n != m_pHeader; n = n->Next)
            if (n->Key == key) return COpinionDeedIterator(n);
        return COpinionDeedIterator(m_pHeader);
    }
    COpinionDeedIterator end() const { return COpinionDeedIterator(m_pHeader); }
};
struct CTCHeroOpinionDeedLog_Retail {
    COpinionDeedMap OpinionDeedMap;
    COpinionDeed* GetDeedFromHandle(long handle) const
    {
        COpinionDeedIterator it = OpinionDeedMap.find(handle);
        if (it == OpinionDeedMap.end())
            return 0;
        return it->Value;
    }
};
int main()
{
    COpinionDeed d1, d2; d1.DeedHandle = 3; d2.DeedHandle = 9;
    SOpinionDeedNode head, n1, n2;
    head.Next = &n1; n1.Next = &n2; n2.Next = &head;
    n1.Key = 3; n1.Value = &d1; n2.Key = 9; n2.Value = &d2; head.Key = -1; head.Value = 0;
    CTCHeroOpinionDeedLog_Retail log; log.OpinionDeedMap.m_pHeader = &head;
    if (log.GetDeedFromHandle(3) != &d1 || log.GetDeedFromHandle(9) != &d2 || log.GetDeedFromHandle(5) != 0) {
        std::printf("006c58f0_TEST FAIL\n"); return 1;
    }
    std::printf("006c58f0_TEST PASS\n");
    return 0;
}