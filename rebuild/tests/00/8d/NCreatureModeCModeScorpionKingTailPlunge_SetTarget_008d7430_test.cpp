#include <cstdio>
// CModeScorpionKingTailPlunge::SetTarget model: PTarget = target, TargetPos = its position.
class C3DVector { public: float x, y, z; };
class CThing { public: C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer(CThing* item) : m_pItem(item) { }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { CThing* item = o.m_pItem; m_pItem = item; return *this; }
    CThing* operator->() const { return m_pItem; }
};
struct CModeScorpionKingTailPlunge {
    CIntelligentPointer PTarget; C3DVector TargetPos;
    CModeScorpionKingTailPlunge() : PTarget(0) { }
    void SetTarget(CThing* target)
    {
        PTarget = CIntelligentPointer(target);
        TargetPos = PTarget->GetPosition();
    }
};
int main()
{
    CThing target; target.pos.x = 1.0f; target.pos.y = 2.0f; target.pos.z = 3.0f;
    CModeScorpionKingTailPlunge m; m.SetTarget(&target);
    if (!(m.PTarget.m_pItem == &target && m.TargetPos.x == 1.0f && m.TargetPos.y == 2.0f && m.TargetPos.z == 3.0f)) { std::printf("008d7430_TEST FAIL\n"); return 1; }
    std::printf("008d7430_TEST PASS\n");
    return 0;
}