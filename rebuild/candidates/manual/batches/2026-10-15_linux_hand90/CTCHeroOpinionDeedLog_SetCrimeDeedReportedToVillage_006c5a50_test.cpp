#include <cstdio>
// Behaviour of CTCHeroOpinionDeedLog::SetCrimeDeedReportedToVillage (0x006C5A50): flags the logged deed, ignores unknown handles.
struct COpinionDeed { long DeedHandle; bool IsCrimeReportedToVillage; bool IsCrime; };
struct SOpinionDeedNode { SOpinionDeedNode* Next; long Key; COpinionDeed* Value; };
struct COpinionDeedIterator {
    SOpinionDeedNode* m_pNode;
    COpinionDeedIterator(SOpinionDeedNode* node) : m_pNode(node) {}
    bool operator!=(const COpinionDeedIterator& other) const { return m_pNode != other.m_pNode; }
    SOpinionDeedNode* operator->() const { return m_pNode; }
};
struct COpinionDeedMap {
    SOpinionDeedNode* m_pHeader;
    COpinionDeedIterator find(const long& key)
    {
        for (SOpinionDeedNode* n = m_pHeader->Next; n != m_pHeader; n = n->Next)
            if (n->Key == key) return COpinionDeedIterator(n);
        return COpinionDeedIterator(m_pHeader);
    }
    COpinionDeedIterator end() { return COpinionDeedIterator(m_pHeader); }
};
struct CTCHeroOpinionDeedLog_Retail {
    COpinionDeedMap OpinionDeedMap;
    void SetCrimeDeedReportedToVillage(long handle)
    {
        COpinionDeedIterator it = OpinionDeedMap.find(handle);
        if (it != OpinionDeedMap.end())
            it->Value->IsCrimeReportedToVillage = true;
    }
};
int main()
{
    COpinionDeed d1; d1.DeedHandle = 3; d1.IsCrimeReportedToVillage = false; d1.IsCrime = true;
    SOpinionDeedNode head, n1;
    head.Next = &n1; n1.Next = &head; n1.Key = 3; n1.Value = &d1; head.Key = -1; head.Value = 0;
    CTCHeroOpinionDeedLog_Retail log; log.OpinionDeedMap.m_pHeader = &head;
    log.SetCrimeDeedReportedToVillage(7);   // unknown: must not touch the header's null value
    if (d1.IsCrimeReportedToVillage) { std::printf("006c5a50_TEST FAIL\n"); return 1; }
    log.SetCrimeDeedReportedToVillage(3);
    if (!d1.IsCrimeReportedToVillage || !d1.IsCrime) { std::printf("006c5a50_TEST FAIL\n"); return 1; }
    std::printf("006c5a50_TEST PASS\n");
    return 0;
}
