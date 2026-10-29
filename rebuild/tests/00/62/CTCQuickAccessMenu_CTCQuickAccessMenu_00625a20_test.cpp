// Behaviour test for CTCQuickAccessMenu::~CTCQuickAccessMenu @ 0x00625a20: leave the TC list, delete every component,
// empty the vector, then the UI manager resets in retail order (observers, action params, previous-selection list, bastard child).
#include <stdio.h>
#include <stdlib.h>
static char g_log[256];
static void logc(char c) { size_t n = 0; while (g_log[n]) ++n; g_log[n] = c; g_log[n + 1] = 0; }
static int g_deleted = 0;
class CComponent { public:
    virtual ~CComponent() { g_deleted++; logc('d'); }
};
class CManager { public:
    void ClearObservers() { logc('O'); }
    void ClearActionParams() { logc('A'); }
    void ClearBastardChild() { logc('B'); }
    static CManager* GetInstance() { static CManager m; return &m; }
};
struct ListNode { ListNode* next; ListNode* prev; void* data; };
static ListNode g_head;   // GPreviousSelected header node
static void list_push(void* v) { ListNode* n = (ListNode*)malloc(sizeof(ListNode)); n->data = v; n->next = &g_head; n->prev = g_head.prev; g_head.prev->next = n; g_head.prev = n; }
static int list_size() { int c = 0; for (ListNode* n = g_head.next; n != &g_head; n = n->next) ++c; return c; }
static void list_clear()
{
    ListNode* cur = g_head.next;
    while (cur != &g_head) { ListNode* tmp = cur; cur = cur->next; free(tmp); }
    g_head.next = &g_head; g_head.prev = &g_head; logc('L');
}
class CTCBase { public:
    virtual ~CTCBase() { logc('T'); }
    void RemoveFromGlobalTCs() { logc('R'); }
};
class CTCQuickAccessMenu : public CTCBase { public:
    bool Active;
    CComponent** Start; CComponent** Finish; CComponent** End;
    bool Quitting;
    double LastRenderTime;
    bool FirstRenderFrame;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    virtual ~CTCQuickAccessMenu()
    {
        RemoveFromGlobalTCs();
        for (unsigned long i = 0; i < size(); ++i)
            delete Start[i];
        Finish = Start;   // clear()
        CManager::GetInstance()->ClearObservers();
        CManager::GetInstance()->ClearActionParams();
        list_clear();
        CManager::GetInstance()->ClearBastardChild();
        logc('V'); free(Start);   // vector dtor
    }
};
int main()
{
    g_head.next = g_head.prev = &g_head;
    list_push(0); list_push(&g_head);
    CTCQuickAccessMenu* m = new CTCQuickAccessMenu;
    m->Start = (CComponent**)malloc(4 * sizeof(CComponent*));
    m->Start[0] = new CComponent; m->Start[1] = 0; m->Start[2] = new CComponent;
    m->Finish = m->Start + 3; m->End = m->Start + 4;
    CTCBase* b = m;
    delete b;
    if (g_deleted != 2 || list_size() != 0 || g_head.next != &g_head) { printf("00625a20_TEST FAIL\n"); return 1; }
    const char* want = "RddOALBVT";
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) { printf("00625a20_TEST FAIL\n"); return 1; }
    printf("00625a20_TEST PASS\n");
    return 0;
}