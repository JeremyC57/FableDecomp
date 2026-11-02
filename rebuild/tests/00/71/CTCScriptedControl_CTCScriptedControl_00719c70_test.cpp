// Behaviour test for 0x00719C70 ~CTCScriptedControl: AttitudeTags, then PActionList (each counted action released), then
// PScriptHandleImp, then ~CTCBase.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
static int g_released = 0;
struct Node { Node* next; Node* prev; int* count; };
class CList {
public:
    CList(const char* t, bool counted) : Tag(t), Counted(counted) { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    void push(int* c) { Node* n = (Node*)std::malloc(sizeof(Node)); n->count = c; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    ~CList()
    {
        Node* cur = head->next;
        while (cur != head) {
            Node* tmp = cur; cur = cur->next;
            if (Counted && tmp->count && --*tmp->count == 0) ++g_released;
            std::free(tmp);
        }
        std::free(head);
        Log(Tag);
    }
    Node* head; const char* Tag; bool Counted;
};
class CTCBase { public: virtual ~CTCBase() { Log("B"); } };
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer() { Log("H"); } };
class CTCScriptedControl : public CTCBase {
public:
    CTCScriptedControl() : PActionList("A", true), AttitudeTags("T", false) { }
    CBaseIntelligentPointer PScriptHandleImp;
    CList                   PActionList;
    bool                    Locked;
    int                     ScriptAIPriority;
    CList                   AttitudeTags;
};
int main()
{
    int a = 1, b = 2;
    g_log[0] = 0;
    CTCBase* control = new CTCScriptedControl;
    ((CTCScriptedControl*)control)->PActionList.push(&a);
    ((CTCScriptedControl*)control)->PActionList.push(&b);
    ((CTCScriptedControl*)control)->AttitudeTags.push(0);
    delete control;
    if (std::strcmp(g_log, "TAHB") != 0 || g_released != 1 || a != 0 || b != 1) { std::printf("00719c70_TEST FAIL\n"); return 1; }
    std::printf("00719c70_TEST PASS\n");
    return 0;
}