#include <cstdio>
struct CThingCreatureDef { float MaxHealth; };
template <class T> struct CDefPointer {
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};
struct CThingCreatureBase {
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;
    unsigned char Drawable : 1;
    unsigned char _flags92_2 : 6;
    float MaxHealth;
    float Health;
    CDefPointer<CThingCreatureDef> PDef;
    bool m_baseOk;
    bool ConstructFromLevelScript(const char* script) { (void)script; return m_baseOk; }
};
struct CThingPlayerCreature : CThingCreatureBase {
    bool vfunc_4(const char* script);
};
bool CThingPlayerCreature::vfunc_4(const char* script)
{
    if (CThingCreatureBase::ConstructFromLevelScript(script))
    {
        MaxHealth = PDef->MaxHealth;
        if (Health == 0.0f)
            Health = MaxHealth;
        Drawable = true;
        return true;
    }
    return false;
}
int main()
{
    int bad = 0;
    CThingCreatureDef d; d.MaxHealth = 120.0f;
    CThingPlayerCreature c; c.PDef.m_pDef = &d; c.MaxHealth = 1.0f; c.Health = 0.0f;
    c.DeletedOnOwnerLevelOutOfScope = 1; c.Drawable = 0; c._flags92_2 = 0; c.m_baseOk = false;
    if (c.vfunc_4("x") || c.MaxHealth != 1.0f || c.Drawable) bad = 1;
    c.m_baseOk = true;
    if (!c.vfunc_4("x") || c.MaxHealth != 120.0f || c.Health != 120.0f || !c.Drawable || !c.DeletedOnOwnerLevelOutOfScope) bad = 2;
    c.Health = 30.0f; d.MaxHealth = 150.0f;
    if (!c.vfunc_4("x") || c.MaxHealth != 150.0f || c.Health != 30.0f) bad = 3;
    if (bad) { std::printf("006a9960_TEST FAIL\n"); return 1; }
    std::printf("006a9960_TEST PASS\n");
    return 0;
}
