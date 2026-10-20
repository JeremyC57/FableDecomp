#include <cstdio>
struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()                // inlined into ~CDefPointer
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CCombatTypeDef : CDefBase {
    unsigned char _pad08[0x6c - 0x08];
    float FrontFarLimit;                   // +0x6c
    float FrontMiddleLimit;                // +0x70
    float FrontNearLimit;                  // +0x74
    float SideLimit;                       // +0x78
    float RearLimit;                       // +0x7c
    float PreferredCombatDistance;         // +0x80
};
template<class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
    T* operator->() const { return m_p; }
    operator T*() const { return m_p; }
};
struct CThingCreatureBase {
    CDefPointer<CCombatTypeDef> GetPCombatTypeDef() const;     // 0x00664D40
    const CCombatTypeDef* GetCombatTypeDef() const { return GetPCombatTypeDef(); }
    float GetPrefferedCombatDistance();
};

float CThingCreatureBase::GetPrefferedCombatDistance()
{
    if (GetCombatTypeDef()->PreferredCombatDistance < 1e-4f) {
        float nearLimit = GetCombatTypeDef()->FrontNearLimit;
        float middleLimit = GetCombatTypeDef()->FrontMiddleLimit;
        return (GetCombatTypeDef()->FrontFarLimit - middleLimit) * 0.2f + middleLimit;
    }
    return GetCombatTypeDef()->PreferredCombatDistance;
}
// ---- test doubles ----
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() {}
struct CTestDef : CCombatTypeDef { int destroyed; virtual void Destroy() { ++destroyed; } };
static CTestDef* g_def; static int g_calls;
static CDefPointer<CCombatTypeDef> g_holder;      // copied memberwise into each returned temporary (no local, no NRVO question)
CDefPointer<CCombatTypeDef> CThingCreatureBase::GetPCombatTypeDef() const
{
    ++g_calls; ++g_def->m_count; g_holder.m_p = g_def; return g_holder;
}
static bool near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
int main()
{
    CTestDef def; def.m_count = 1; def.destroyed = 0; g_def = &def;
    def.FrontFarLimit = 10.0f; def.FrontMiddleLimit = 5.0f; def.FrontNearLimit = 2.0f; def.PreferredCombatDistance = 7.5f;
    CThingCreatureBase c;
    bool ok = c.GetPrefferedCombatDistance() == 7.5f && g_calls == 2 && def.m_count == 1;
    def.PreferredCombatDistance = 0.0f; g_calls = 0;
    ok = ok && near(c.GetPrefferedCombatDistance(), 6.0f) && g_calls == 4 && def.m_count == 1 && def.destroyed == 0;
    def.m_count = 0;                                  // the temporary's release now reaches zero
    c.GetPrefferedCombatDistance();
    ok = ok && def.destroyed == 4;
    g_holder.m_p = 0;
    if (!ok) { std::printf("00667c00_TEST FAIL\n"); return 1; }
    std::printf("00667c00_TEST PASS\n");
    return 0;
}