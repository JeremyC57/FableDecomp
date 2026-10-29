#include <cstdio>
#include <new>
// Behaviour model of NUISystem::CRedefinerList::Initialise @ 0x005566A0 (copy of the source's logic, stand-in callees).
static int g_log[32], g_n = 0, g_destroyed = 0, g_defDestroyed = 0;
class C2DVector { public: float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {} };
static void DestroyObj(void*) { ++g_destroyed; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template<class T> class CCountedPointer { public:
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* o) : m_pObject(o)
    { if (o) { m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyObj; m_pCount->m_pObject = o; } else m_pCount = 0; }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); operator delete(m_pCount); }
        }
    }
    T* operator->() const { return m_pObject; }
};
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
struct CDefVirtual { virtual ~CDefVirtual() {} virtual void Destroy() { ++g_defDestroyed; } };
struct CRedefinerListDef : CDefVirtual, CRefCounted {
    long UndefinedWarningDef;
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer {
    CRedefinerListDef* m_p;
    CDefPointer(CRedefinerListDef* p) : m_p(p) { if (m_p) ++m_p->m_count; }
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }   // copies keep the count exact with or without NRVO
    ~CDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
    CRedefinerListDef* operator->() const { return m_p; }
};
struct CComponent {
    int id; CComponent* parent; long createdFrom;
    CComponent(int i) : id(i), parent(0), createdFrom(0) {}
    virtual ~CComponent() {}
    virtual C2DVector GetPosition() const { return C2DVector((float)id, 0.0f); }
    virtual void Initialise() { g_log[g_n++] = 100 + id; }
    virtual void SetParent(CComponent* p) { parent = p; g_log[g_n++] = 200 + id; }
    virtual CComponent* GetParent() { return parent; }
    virtual void AddChild(const CCountedPointer<CComponent>& c) { g_log[g_n++] = 300 + c->id; }
};
static CComponent g_created(7);
namespace NUISystem { class CManager { public:
    static CManager* GetInstance() { static CManager m; return &m; }
    CComponent* CreateComponent(long def) { g_created.createdFrom = def; g_log[g_n++] = 400; return &g_created; }
}; }
static CRedefinerListDef g_def;
struct CRedefinerList : CComponent {
    CComponent* PUndefinedWarning;
    static CRedefinerList* s_pActiveList;
    CRedefinerList() : CComponent(0), PUndefinedWarning(0) {}
    void ListInitialise() { g_log[g_n++] = 1; }
    CDefPointer GetDef() { CDefPointer p(&g_def); return p; }
    void HideComponent(CComponent* c, float s, float e, C2DVector pos, unsigned char a)
    { g_log[g_n++] = 500 + c->id; if (s != 0.0f || e != 0.0f || a != 0 || pos.X != (float)c->id) g_log[g_n++] = -1; }
    void Initialise()
    {
        ListInitialise();
        s_pActiveList = this;
        CDefPointer def = GetDef();
        if (def->UndefinedWarningDef) {
            PUndefinedWarning = NUISystem::CManager::GetInstance()->CreateComponent(def->UndefinedWarningDef);
            CCountedPointer<CComponent> warning(PUndefinedWarning);
            GetParent()->AddChild(warning);
            warning->SetParent(GetParent());
            warning->Initialise();
            HideComponent(PUndefinedWarning, 0.0f, 0.0f, PUndefinedWarning->GetPosition(), 0);
        }
    }
};
CRedefinerList* CRedefinerList::s_pActiveList = 0;
int main()
{
    CComponent owner(9);
    CRedefinerList l; l.parent = &owner;
    g_def.m_count = 1; g_def.UndefinedWarningDef = 0x55;
    l.Initialise();
    int want[] = { 1, 400, 307, 207, 107, 507 };
    int ok = CRedefinerList::s_pActiveList == &l && l.PUndefinedWarning == &g_created && g_created.createdFrom == 0x55
          && g_created.parent == &owner && g_n == 6 && g_destroyed == 1 && g_def.m_count == 1 && g_defDestroyed == 0;
    for (int i = 0; ok && i < 6; ++i) if (g_log[i] != want[i]) ok = 0;
    g_n = 0; g_def.UndefinedWarningDef = 0; g_def.m_count = 0;      // temporary is the last reference
    CRedefinerList l2; l2.parent = &owner; l2.Initialise();
    ok = ok && g_n == 1 && l2.PUndefinedWarning == 0 && g_defDestroyed == 1 && CRedefinerList::s_pActiveList == &l2;
    if (!ok) { std::printf("005566a0_TEST FAIL\n"); return 1; }
    std::printf("005566a0_TEST PASS\n");
    return 0;
}