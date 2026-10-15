// Behaviour test for 0075d8b0 (CTCCameraPointScriptedSpline::IsCombatSplineValidForAnimAndThing): true only for a combat spline whose valid-animation set holds the index.
#include <cstdio>

struct CThing { long m_id; };

struct SAnimNode { long m_key; };

struct CAnimSetIterator {
    SAnimNode* m_node;
    CAnimSetIterator() { }
};

struct CAnimSet {
    SAnimNode*    m_head;                           // the end sentinel
    SAnimNode*    m_nodes;
    unsigned long m_size;
    CAnimSetIterator find(const long& key)
    {
        CAnimSetIterator it;
        it.m_node = m_head;
        for (unsigned long i = 0; i < m_size; ++i)
            if (m_nodes[i].m_key == key) { it.m_node = &m_nodes[i]; break; }
        return it;
    }
};

struct CTCCameraPointScriptedSpline {
    bool     m_combat;
    CAnimSet ValidAnimList;
    bool IsCombatSpline() { return m_combat; }
    bool IsCombatSplineValidForAnimAndThing(long anim, CThing*)
    {
        if (IsCombatSpline()) {
            CAnimSetIterator it = ValidAnimList.find(anim);
            if (it.m_node != ValidAnimList.m_head)
                return true;
        }
        return false;
    }
};

int main()
{
    SAnimNode head = { 0 };
    SAnimNode nodes[2] = { { 3 }, { 7 } };
    CTCCameraPointScriptedSpline s;
    s.m_combat = false; s.ValidAnimList.m_head = &head; s.ValidAnimList.m_nodes = nodes; s.ValidAnimList.m_size = 2;
    bool ok = !s.IsCombatSplineValidForAnimAndThing(7, 0);
    s.m_combat = true;
    ok = ok && s.IsCombatSplineValidForAnimAndThing(7, 0) && s.IsCombatSplineValidForAnimAndThing(3, 0) && !s.IsCombatSplineValidForAnimAndThing(5, 0);
    if (!ok) { std::printf("0075d8b0_TEST FAIL\n"); return 1; }
    std::printf("0075d8b0_TEST PASS\n");
    return 0;
}