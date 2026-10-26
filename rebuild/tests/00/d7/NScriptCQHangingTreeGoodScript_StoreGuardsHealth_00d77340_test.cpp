// Behaviour test for 0x00d77340 CQ_HangingTreeGoodScript::StoreGuardsHealth
#include <cstdio>
class CScriptThing { public: float m_health; };
struct CGameScriptInterface_Calls { virtual float GetHealth(const CScriptThing& thing) const { return thing.m_health; } };
template <class T> struct CRetailVector {
    T* m_pBegin; T* m_pEnd; T* m_pCapacity;
    unsigned long size() const { return m_pEnd - m_pBegin; }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
    void push_back(const T& value) { *m_pEnd++ = value; }
};
struct CQ_HangingTreeGoodScript {
    int m_terminateAfter; int m_checks;
    CGameScriptInterface_Calls* GSI;
    CRetailVector<CScriptThing> CurrentGuardVector; CRetailVector<float> GuardsHealthVector;
    bool IsActiveThreadTerminating() { return ++m_checks > m_terminateAfter; }
    void StoreGuardsHealth()
    {
        for (unsigned long i = 0; i < CurrentGuardVector.size(); ++i) {
            if (IsActiveThreadTerminating()) return;
            float health = GSI->GetHealth(CurrentGuardVector[i]);
            GuardsHealthVector.push_back(health);
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    CScriptThing guards[3] = { { 10.0f }, { 20.0f }, { 30.0f } }; float health[3] = { 0, 0, 0 };
    CGameScriptInterface_Calls gsi;
    CQ_HangingTreeGoodScript s; s.GSI = &gsi; s.m_terminateAfter = 100; s.m_checks = 0;
    s.CurrentGuardVector.m_pBegin = guards; s.CurrentGuardVector.m_pEnd = s.CurrentGuardVector.m_pCapacity = guards + 3;
    s.GuardsHealthVector.m_pBegin = s.GuardsHealthVector.m_pEnd = health; s.GuardsHealthVector.m_pCapacity = health + 3;
    s.StoreGuardsHealth();
    bool ok = s.GuardsHealthVector.size() == 3 && health[1] == 20.0f && s.m_checks == 4;
    s.GuardsHealthVector.m_pEnd = health; s.m_checks = 0; s.m_terminateAfter = 1;   // terminates on the second guard
    s.StoreGuardsHealth();
    ok = ok && s.GuardsHealthVector.size() == 1 && s.m_checks == 2;
    if (!ok) { std::printf("00d77340_TEST FAIL\n"); return 1; }
    std::printf("00d77340_TEST PASS\n");
    return 0;
}