#include <cstdio>
// Behaviour test for COpinionReactionManager::GetToleranceToBeingHit @ 0x00701940
class CThing { public: long m_attitude; };
struct SToleranceNode { long m_color; SToleranceNode* m_pParent; SToleranceNode* m_pLeft; SToleranceNode* m_pRight; long first; float second; };
struct CToleranceIterator {
    SToleranceNode* m_pNode;
    CToleranceIterator() {}
    CToleranceIterator(SToleranceNode* node) : m_pNode(node) {}
    SToleranceNode* operator->() const { return m_pNode; }
    bool operator!=(const CToleranceIterator& other) const { return m_pNode != other.m_pNode; }
};
struct CToleranceMap {
    SToleranceNode* m_pHeader;
    SToleranceNode* m_pEntries; long m_count;
    CToleranceIterator find(const long& key) const
    {
        for (long i = 0; i < m_count; ++i) if (m_pEntries[i].first == key) return CToleranceIterator(&m_pEntries[i]);
        return CToleranceIterator(m_pHeader);
    }
    CToleranceIterator end() const { return CToleranceIterator(m_pHeader); }
};
struct COpinionReactionManagerDef { CToleranceMap ToleranceToBeingHit; };
class COpinionReactionManager {
public:
    const COpinionReactionManagerDef* PDef;
    long GetThingAttitudeToPlayer(const CThing* thing) const { return thing->m_attitude; }
    float GetToleranceToBeingHit(const CThing* thing) const
    {
        long attitude = GetThingAttitudeToPlayer(thing);
        const CToleranceMap& tolerance = PDef->ToleranceToBeingHit;
        CToleranceIterator it = tolerance.find(attitude);
        if (it != tolerance.end())
            return it->second;
        return 0.0f;
    }
};
int main()
{
    SToleranceNode header; SToleranceNode entries[2];
    entries[0].first = 3; entries[0].second = 0.25f; entries[1].first = 5; entries[1].second = 4.5f;
    COpinionReactionManagerDef def; def.ToleranceToBeingHit.m_pHeader = &header; def.ToleranceToBeingHit.m_pEntries = entries; def.ToleranceToBeingHit.m_count = 2;
    COpinionReactionManager m; m.PDef = &def;
    CThing a; a.m_attitude = 5; CThing b; b.m_attitude = 3; CThing c; c.m_attitude = 9;
    if (m.GetToleranceToBeingHit(&a) != 4.5f || m.GetToleranceToBeingHit(&b) != 0.25f || m.GetToleranceToBeingHit(&c) != 0.0f) { std::printf("00701940_TEST FAIL\n"); return 1; }
    std::printf("00701940_TEST PASS\n");
    return 0;
}