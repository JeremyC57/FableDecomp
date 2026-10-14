#include <cstdio>

struct CBaseObject { int x; };
static int g_resolved = 0; static bool g_isResolved = true, g_finishedTalking = false;
struct CScriptThing {
    virtual void v000() { }
    virtual void v001() { }
    virtual void v002() { }
    virtual void v003() { }
    virtual void v004() { }
    virtual void v005() { }
    virtual void v006() { }
    virtual void v007() { }
    virtual void v008() { }
    virtual void v009() { }
    virtual void v010() { }
    virtual void v011() { }
    virtual void v012() { }
    virtual void v013() { }
    virtual void v014() { }
    virtual void v015() { }
    virtual void v016() { }
    virtual void v017() { }
    virtual void v018() { }
    virtual void v019() { }
    virtual void v020() { }
    virtual void v021() { }
    virtual void v022() { }
    virtual void v023() { }
    virtual void v024() { }
    virtual void v025() { }
    virtual void v026() { }
    virtual void v027() { }
    virtual void v028() { }
    virtual void v029() { }
    virtual void v030() { }
    virtual void v031() { }
    virtual void v032() { }
    virtual void v033() { }
    virtual void v034() { }
    virtual void v035() { }
    virtual void v036() { }
    virtual void v037() { }
    virtual void v038() { }
    virtual void v039() { }
    virtual void v040() { }
    virtual void v041() { }
    virtual void v042() { }
    virtual void v043() { }
    virtual void v044() { }
    virtual void v045() { }
    virtual void v046() { }
    virtual void v047() { }
    virtual void v048() { }
    virtual void v049() { }
    virtual void v050() { }
    virtual void v051() { }
    virtual void v052() { }
    virtual void v053() { }
    virtual void v054() { }
    virtual void v055() { }
    virtual void v056() { }
    virtual void v057() { }
    virtual void v058() { }
    virtual void v059() { }
    virtual void v060() { }
    virtual void v061() { }
    virtual void v062() { }
    virtual void v063() { }
    virtual void v064() { }
    virtual void v065() { }
    virtual void v066() { }
    virtual void v067() { }
    virtual void v068() { }
    virtual void v069() { }
    virtual void v070() { }
    virtual void Resolve() { ++g_resolved; }
    virtual void v072() { }
    virtual void v073() { }
    virtual void v074() { }
    virtual void v075() { }
    virtual unsigned char HasFinishedTalking() { return g_finishedTalking; }
    virtual unsigned char IsResolved() { return g_isResolved; }
    void* PImp_Data; void* PImp_Info; };
struct CIntelligentPointer { virtual void v00() { } CBaseObject* PItem; CBaseObject* GetPItem() { return PItem; } };
struct CActionTalkedTo { virtual void v00() { } void* m_pThing; bool m_active; bool m_finished; unsigned char _pad[2]; CIntelligentPointer m_target; CScriptThing m_talker;
    void Update()
    {
        if (!m_talker.IsResolved()) m_talker.Resolve();
        if (m_talker.HasFinishedTalking() || m_target.GetPItem() == 0) m_finished = true;
    } };
int main()
{
    CBaseObject o; CActionTalkedTo a; a.m_finished = false; a.m_target.PItem = &o;
    a.Update(); if (a.m_finished || g_resolved != 0) { std::printf("00713240_TEST FAIL\n"); return 1; }
    g_isResolved = false; a.Update(); if (a.m_finished || g_resolved != 1) { std::printf("00713240_TEST FAIL\n"); return 1; }
    g_finishedTalking = true; a.Update(); if (!a.m_finished) { std::printf("00713240_TEST FAIL\n"); return 1; }
    a.m_finished = false; g_finishedTalking = false; a.m_target.PItem = 0; a.Update(); if (!a.m_finished) { std::printf("00713240_TEST FAIL\n"); return 1; }
    std::printf("00713240_TEST PASS\n"); return 0;
}