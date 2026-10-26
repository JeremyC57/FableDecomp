#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour model of NUISystem::CSpellContainerList::~CSpellContainerList @ 0x00544C10 (copy of the source logic, stand-ins).
static int g_log[16], g_n = 0, g_actionsDestroyed = 0, g_baseDtor = 0;
template<class T> struct CActionList {
    struct SNode { SNode* next; SNode* prev; T value; };
    SNode* head;
    CActionList() : head(0) { SNode* node = (SNode*)std::malloc(sizeof(SNode)); node->next = node; node->prev = node; head = node; }
    ~CActionList() { std::free(head); }
};
struct CAction {
    long ActionType; CActionList<void*> UIActionParams; CActionList<long> ThingActionParams; bool Valid;
    CAction() : ActionType(0), Valid(true) {}
};
static void DestroyAction(void* object) { ++g_actionsDestroyed; delete (CAction*)object; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; SCountBlock(void* o) : m_references(1), m_pfnDestroy(DestroyAction), m_pObject(o) {} };
template<class T> class CCountedPointer { public:
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* object) : m_pObject(object) { if (object) m_pCount = new SCountBlock(m_pObject); else m_pCount = 0; }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); operator delete(m_pCount); }
        }
    }
    T* operator->() const { return m_pObject; }
};
struct CObserver { virtual ~CObserver() {} int state; };
struct CComponentRoot { virtual ~CComponentRoot() {} };
struct CChangingStateComponent : CComponentRoot, CObserver { virtual ~CChangingStateComponent() { ++g_baseDtor; g_log[g_n++] = 9; } };
static CObserver* g_removed = 0; static long g_executedType = -1; static bool g_executedValid = false;
struct CFrontEndManager {
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
    void RemoveObserver(CObserver* o) { g_removed = o; g_log[g_n++] = 1; }
    void ExecuteAction(const CCountedPointer<CAction>& a) { g_executedType = a->ActionType; g_executedValid = a->Valid; g_log[g_n++] = 2; }
};
template<class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; ~CRetailVector() { if (_M_start) { std::free(_M_start); g_log[g_n++] = 3; } } };
struct CSpellContainerList : CChangingStateComponent {
    CRetailVector<void*> SpellContainers;
    virtual ~CSpellContainerList()
    {
        CFrontEndManager::GetInstance()->RemoveObserver(this);
        CCountedPointer<CAction> action(new CAction());
        action->ActionType = 0xd3;
        CFrontEndManager::GetInstance()->ExecuteAction(action);
    }
};
int main()
{
    CSpellContainerList* l = new CSpellContainerList;
    l->SpellContainers._M_start = (void**)std::malloc(16); l->SpellContainers._M_finish = l->SpellContainers._M_start;
    CObserver* asObserver = l;
    delete l;
    int want[] = { 1, 2, 3, 9 };
    int ok = g_removed == asObserver && g_executedType == 0xd3 && g_executedValid && g_actionsDestroyed == 1 && g_baseDtor == 1 && g_n == 4;
    for (int i = 0; ok && i < 4; ++i) if (g_log[i] != want[i]) ok = 0;
    if (!ok) { std::printf("00544c10_TEST FAIL\n"); return 1; }
    std::printf("00544c10_TEST PASS\n");
    return 0;
}