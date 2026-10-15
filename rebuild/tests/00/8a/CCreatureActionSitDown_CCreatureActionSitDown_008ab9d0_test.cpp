// Behaviour test for 0x008AB9D0 CCreatureAction_SitDown(owner, animation, mode): base arguments and the counted mode pointer (add-ref by the member,
// release of the by-value parameter).
#include <cstdio>
struct CThing { int id; };
struct CAnimationEntry { int id; };
struct CModeSitBase { int id; };
static int g_destroyed = 0;
static void DestroyMode(CModeSitBase*) { ++g_destroyed; }
struct SCountBlock { int m_references; void (*m_pfnDestroy)(CModeSitBase*); CModeSitBase* m_pObject; };
struct CCountedPointer {
    CModeSitBase* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(CModeSitBase* o, SCountBlock* c) : m_pObject(o), m_pCount(c) { }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; }
        }
        m_pObject = 0; m_pCount = 0;
    }
};
struct CSoundPair { int FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CCreatureActionBase {
    CThing* m_owner; float m_a, m_b; CAnimationEntry* m_anim; CSoundPair m_sound; int m_c, m_d;
    CCreatureActionBase(CThing* owner, float a, float b, CAnimationEntry* anim, const CSoundPair& sound, int c, int d)
        : m_owner(owner), m_a(a), m_b(b), m_anim(anim), m_sound(sound), m_c(c), m_d(d) { }
};
struct CCreatureAction_SitDown : CCreatureActionBase {
    CCountedPointer PMode;
    CCreatureAction_SitDown(CThing* owner, CAnimationEntry* animation, CCountedPointer mode)
        : CCreatureActionBase(owner, -1.0f, 0.0f, animation, CSoundPair(), 0, 1), PMode(mode) { }
};
int main()
{
    CThing owner; CAnimationEntry anim; CModeSitBase mode;
    SCountBlock* block = new SCountBlock; block->m_references = 0; block->m_pfnDestroy = DestroyMode; block->m_pObject = &mode;
    {
        CCountedPointer caller(&mode, block); block->m_references = 1;
        CCreatureAction_SitDown* a = new CCreatureAction_SitDown(&owner, &anim, caller);
        if (a->m_owner != &owner || a->m_a != -1.0f || a->m_b != 0.0f || a->m_anim != &anim || a->m_c != 0 || a->m_d != 1
            || a->PMode.m_pObject != &mode || a->PMode.m_pCount != block || block->m_references != 2 || g_destroyed != 0) { std::printf("008ab9d0_TEST FAIL\n"); return 1; }
        delete a;   // member release
        if (block->m_references != 1 || g_destroyed != 0) { std::printf("008ab9d0_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1) { std::printf("008ab9d0_TEST FAIL\n"); return 1; }
    std::printf("008ab9d0_TEST PASS\n");
    return 0;
}