#include <cstdio>
#include <new>
// Behaviour model of NUISystem::CSlider::Initialise @ 0x0054A5D0 (copy of the source's logic with stand-in callees).
static int g_destroyed = 0, g_log[32], g_n = 0;
static void Destroy(void*) { ++g_destroyed; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template<class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object) { m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = Destroy; m_pCount->m_pObject = object; }
        else m_pCount = 0;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
    }
};
struct CComponent {
    int id; CComponent* parent; CComponent* added; int inits;
    CComponent(int i) : id(i), parent(0), added(0), inits(0) {}
    virtual ~CComponent() {}
    virtual void Initialise() { ++inits; g_log[g_n++] = 100 + id; }
    virtual void SetParent(CComponent* p) { parent = p; g_log[g_n++] = 200 + id; }
    virtual CComponent* GetParent() { return parent; }
    virtual void AddChild(const CCountedPointer<CComponent>& child)
    {
        added = child.m_pObject; g_log[g_n++] = 300 + child.m_pObject->id;
        if (child.m_pCount->m_references != 1) g_log[g_n++] = -1;
    }
};
static int g_baseInit = 0;
struct CSlider : CComponent {
    CComponent* PLeftArrow; CComponent* PRightArrow;
    CSlider() : CComponent(0), PLeftArrow(0), PRightArrow(0) {}
    void SpriteInitialise() { ++g_baseInit; g_log[g_n++] = 1; }
    void Initialise()
    {
        SpriteInitialise();
        if (PRightArrow) {
            CCountedPointer<CComponent> child(PRightArrow);
            GetParent()->AddChild(child);
            PRightArrow->SetParent(GetParent());
            PRightArrow->Initialise();
        }
        if (PLeftArrow) {
            CCountedPointer<CComponent> child(PLeftArrow);
            GetParent()->AddChild(child);
            PLeftArrow->SetParent(GetParent());
            PLeftArrow->Initialise();
        }
    }
};
int main()
{
    CComponent owner(9), left(1), right(2);
    CSlider s; s.parent = &owner; s.PLeftArrow = &left; s.PRightArrow = &right;
    s.Initialise();
    int want[] = { 1, 302, 202, 102, 301, 201, 101 };
    int ok = g_n == 7 && g_destroyed == 2 && left.parent == &owner && right.parent == &owner && owner.added == &left;
    for (int i = 0; ok && i < 7; ++i) if (g_log[i] != want[i]) ok = 0;
    CSlider bare; bare.parent = &owner; g_n = 0; bare.Initialise();
    ok = ok && g_n == 1 && g_destroyed == 2;
    if (!ok) { std::printf("0054a5d0_TEST FAIL\n"); return 1; }
    std::printf("0054a5d0_TEST PASS\n");
    return 0;
}