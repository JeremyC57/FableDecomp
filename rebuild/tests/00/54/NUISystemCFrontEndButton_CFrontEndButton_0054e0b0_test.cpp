// Behaviour test for NUISystem::CFrontEndButton::CFrontEndButton @ 0x0054E0B0
#include <cstdio>
#include <cstdlib>
static long g_owner = 0; static int g_initialized = 0;
struct CClickable { long m_owner; CClickable(long owner) : m_owner(owner) { g_owner = owner; } virtual ~CClickable() {} };
template <class T> struct SRetailAllocProxy { T* m_data; SRetailAllocProxy(T* data) : m_data(data) {} };
template <class T> struct CRetailVector { T* m_start; T* m_finish; SRetailAllocProxy<T> m_endOfStorage; CRetailVector() : m_start(0), m_finish(0), m_endOfStorage(0) {} };
template <class T> struct CRetailList {
    struct SNode { SNode* m_next; SNode* m_prev; T m_data; };
    SNode* m_node;
    CRetailList() : m_node(0) { SNode* s = (SNode*)std::malloc(sizeof(SNode)); s->m_next = s; s->m_prev = s; m_node = s; }
};
struct CCountedPointerAction { void* m_pObject; void* m_pCount; };
struct CFrontEndButton : CClickable {
    float m_fLastInputTime;
    CRetailVector<CCountedPointerAction> Actions, ActionsOnDelete;
    CRetailList<void*> UIActionParams;
    bool m_sawEmptyList;
    void Initialize() { ++g_initialized; m_sawEmptyList = UIActionParams.m_node && UIActionParams.m_node->m_next == UIActionParams.m_node; }
    CFrontEndButton(long owner) : CClickable(owner) { Initialize(); }
};
int main()
{
    CFrontEndButton b(77);
    if (g_owner != 77 || b.m_owner != 77 || g_initialized != 1 || !b.m_sawEmptyList || b.Actions.m_start || b.Actions.m_endOfStorage.m_data ||
        b.ActionsOnDelete.m_finish || b.UIActionParams.m_node->m_prev != b.UIActionParams.m_node) {
        std::printf("0054e0b0_TEST FAIL\n"); return 1;
    }
    std::free(b.UIActionParams.m_node);
    std::printf("0054e0b0_TEST PASS\n");
    return 0;
}