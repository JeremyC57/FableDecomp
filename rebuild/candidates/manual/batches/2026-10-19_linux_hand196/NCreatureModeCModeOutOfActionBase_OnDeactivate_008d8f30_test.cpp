// behaviour copy of NCreatureMode::CModeOutOfActionBase::OnDeactivate
#include <cstdio>
static int g_resume = -1, g_wake = 0, g_kill = 0, g_base = 0; static unsigned long g_setFlags = 0xffff;
struct CTCTargetedFlags { unsigned long m_flags; void SetFlags(unsigned long f) { g_setFlags = f; m_flags = f; }
    void ClearFlag(unsigned long flag) { if (m_flags & flag) SetFlags(m_flags ^ flag); } };
struct CPhysicsObject { void Wake() { ++g_wake; } };
struct CThing {
    unsigned char m_state; bool hasTC9; CTCTargetedFlags tc9; CPhysicsObject* PPhysics; unsigned char m_flags91;
    void SetSuspendAnimation(bool s, long) { g_resume = s; }
    void Kill(bool) { ++g_kill; }
    bool HasTC(long type) const { return type == 9 && hasTC9; }
    template<class T> T* GetTC(long) { return &tc9; }
};
struct CIP { CThing* PItem; CThing* GetPItem() const { return PItem; } };
static bool IS_THING_ALIVE(const CIP& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; } return false; }
struct CModeOutOfActionBase {
    CThing* ParentThing; CIP PEmitter;
    void BaseOnDeactivate() { ++g_base; }
    void OnDeactivate()
    {
        if (ParentThing->m_state == 1)
            ParentThing->SetSuspendAnimation(false, 0);
        if (ParentThing->HasTC(9))
            ParentThing->GetTC<CTCTargetedFlags>(9)->ClearFlag(4);
        ParentThing->PPhysics->Wake();
        if (IS_THING_ALIVE(PEmitter))
            PEmitter.GetPItem()->Kill(true);
        BaseOnDeactivate();
    }
};
int main()
{
    CPhysicsObject phys; CThing c; c.m_state = 1; c.hasTC9 = true; c.tc9.m_flags = 6; c.PPhysics = &phys; c.m_flags91 = 0;
    CThing e; e.m_flags91 = 0;
    CModeOutOfActionBase m; m.ParentThing = &c; m.PEmitter.PItem = &e;
    m.OnDeactivate();
    if (g_resume != 0 || g_setFlags != 2 || c.tc9.m_flags != 2 || g_wake != 1 || g_kill != 1 || g_base != 1) { std::printf("008d8f30_TEST FAIL\n"); return 1; }
    g_setFlags = 0xffff; e.m_flags91 = 1; m.OnDeactivate();
    if (g_setFlags != 0xffff || g_kill != 1 || g_wake != 2) { std::printf("008d8f30_TEST FAIL\n"); return 1; }
    std::printf("008d8f30_TEST PASS\n");
    return 0;
}
