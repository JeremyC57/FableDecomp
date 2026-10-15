#include <cstdio>
// CModeJackOfBladesScream::SetSwordObject model: PSword = sword, InitialSwordPos = its position.
struct C3DVector { float x, y, z; };
struct CThing { C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer(CThing* item) : m_pItem(item) { }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { CThing* item = o.m_pItem; m_pItem = item; return *this; }
    CThing* operator->() const { return m_pItem; }
};
struct CModeJackOfBladesScream {
    CIntelligentPointer PSword; C3DVector InitialSwordPos;
    CModeJackOfBladesScream() : PSword(0) { }
    void SetSwordObject(CThing* sword)
    {
        PSword = CIntelligentPointer(sword);
        InitialSwordPos = PSword->GetPosition();
    }
};
int main()
{
    CThing sword; sword.pos.x = 1.0f; sword.pos.y = 2.0f; sword.pos.z = 3.0f;
    CModeJackOfBladesScream m; m.SetSwordObject(&sword);
    if (!(m.PSword.m_pItem == &sword && m.InitialSwordPos.x == 1.0f && m.InitialSwordPos.y == 2.0f && m.InitialSwordPos.z == 3.0f)) { std::printf("008d7370_TEST FAIL\n"); return 1; }
    std::printf("008d7370_TEST PASS\n");
    return 0;
}
