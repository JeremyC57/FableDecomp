// Behaviour test for CSteeringGroup::CSteeringGroup @ 0x00C29F50 (unplaced, empty, polymorphic).
#include <cstdio>

struct CSteeringAgent;

class CSteeringGroup {
public:
    CSteeringGroup() : m_index(-1), m_pFirst(0) { }
    virtual ~CSteeringGroup() { }
    virtual long* GetIndexSlot() { return &m_index; }
    long            m_index;
    CSteeringAgent* m_pFirst;
};

int main()
{
    CSteeringGroup group;
    CSteeringGroup* heap = new CSteeringGroup;
    bool ok = group.m_index == -1 && group.m_pFirst == 0 && *group.GetIndexSlot() == -1;
    ok = ok && heap->m_index == -1 && heap->m_pFirst == 0 && heap->GetIndexSlot() == &heap->m_index;
    delete heap;
    if (!ok) { std::printf("00c29f50_TEST FAIL\n"); return 1; }
    std::printf("00c29f50_TEST PASS\n");
    return 0;
}