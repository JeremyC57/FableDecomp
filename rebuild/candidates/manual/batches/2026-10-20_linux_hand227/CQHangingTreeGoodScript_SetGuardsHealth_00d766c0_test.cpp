// Behaviour test for CQ_HangingTreeGoodScript::SetGuardsHealth (0x00D766C0): with saved health values, every living
// guard whose saved value is non-zero gets ModifyThingHealth(saved - (long)current); dead guards and zero entries are
// skipped; the saved list is cleared at the end; an empty list does nothing; a terminate request stops early.
#include <cstdio>

struct CBaseClass { virtual ~CBaseClass() {} };
struct CScriptThing : CBaseClass { bool alive; float hp; virtual bool IsAlive() const { return alive; } };
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last) { T* d = first; for (T* s = last; s != _M_finish; ++s) *d++ = *s; _M_finish = d; return first; }
    void clear() { erase(begin(), end()); }
};
struct CGameScriptInterface {
    int mods; const CScriptThing* who[8]; float delta[8];
    virtual float GetHealth(const CScriptThing& t) { return t.hp; }
    virtual void ModifyThingHealth(const CScriptThing& t, float d, bool b) { who[mods] = &t; delta[mods] = b ? -999.0f : d; ++mods; }
};
struct CScriptBase { int termAfter; bool IsActiveThreadTerminating() { return termAfter-- == 0; } };
struct CQ_HangingTreeGoodScript : CScriptBase {
    CGameScriptInterface* m_pInterface;
    vector<CScriptThing> CurrentGuardVector;
    vector<float> GuardsHealthVector;
    void SetGuardsHealth();
};
void CQ_HangingTreeGoodScript::SetGuardsHealth()
{
    vector<float>& health = GuardsHealthVector;
    if (health.size() == 0)
        return;
    for (unsigned int i = 0; i < CurrentGuardVector.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        if (CurrentGuardVector[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            long current = m_pInterface->GetHealth(CurrentGuardVector[i]);
            if (health[i] != 0.0f)
                m_pInterface->ModifyThingHealth(CurrentGuardVector[i], health[i] - current, false);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    health.clear();
}
int main()
{
    int bad = 0;
    CScriptThing guards[3];
    guards[0].alive = true;  guards[0].hp = 40.7f;
    guards[1].alive = false; guards[1].hp = 10.0f;
    guards[2].alive = true;  guards[2].hp = 5.2f;
    float saved[3] = { 100.0f, 50.0f, 0.0f };
    CGameScriptInterface gi; gi.mods = 0;
    CQ_HangingTreeGoodScript s; s.m_pInterface = &gi; s.termAfter = 100;
    s.CurrentGuardVector._M_start = guards; s.CurrentGuardVector._M_finish = guards + 3; s.CurrentGuardVector._M_end_of_storage = guards + 3;
    s.GuardsHealthVector._M_start = saved; s.GuardsHealthVector._M_finish = saved + 3; s.GuardsHealthVector._M_end_of_storage = saved + 3;
    s.SetGuardsHealth();
    float want = 60.0f;   // 100 - (long)40.7
    if (gi.mods != 1 || gi.who[0] != &guards[0] || gi.delta[0] != want || s.GuardsHealthVector.size() != 0) bad = 1;
    // empty list: no work
    gi.mods = 0; s.termAfter = 100; s.SetGuardsHealth();
    if (gi.mods != 0 || s.termAfter != 100) bad = 1;
    // terminate at the first test: list kept
    s.GuardsHealthVector._M_finish = saved + 3; s.termAfter = 0; gi.mods = 0;
    s.SetGuardsHealth();
    if (gi.mods != 0 || s.GuardsHealthVector.size() != 3) bad = 1;
    if (bad) { std::printf("00d766c0_TEST FAIL\n"); return 1; }
    std::printf("00d766c0_TEST PASS\n");
    return 0;
}
