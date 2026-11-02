// Behaviour test for 0x0073AEA0 ~CTCCreatureModeManager: RemoveAllModes (deactivate the front mode, clear the list, re-add the
// default mode), then AvailableIdleModes freed, the list cleared again and its sentinel freed, then ~CTCBase.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CModeBase { char C; bool Active; void OnDeactivate() { char b[3] = { 'D', C, 0 }; Log(b); Active = false; } };
struct Node { Node* next; Node* prev; CModeBase* mode; };
class CModeList {
public:
    CModeList() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    ~CModeList() { clear(); std::free(head); Log("L"); }
    bool empty() const { return head->next == head; }
    CModeBase* front() const { return head->next->mode; }
    void push_back(CModeBase* m) { Node* n = (Node*)std::malloc(sizeof(Node)); n->mode = m; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    void clear() { Node* cur = head->next; while (cur != head) { Node* t = cur; cur = cur->next; char b[3] = { 'X', t->mode->C, 0 }; Log(b); std::free(t); } head->next = head->prev = head; }
    Node* head;
};
class CTCBase { public: virtual ~CTCBase() { Log("B"); } };
static CModeBase g_default = { '0', false };
class CTCCreatureModeManager : public CTCBase {
public:
    CTCCreatureModeManager() : IdleModes(0) { }
    virtual ~CTCCreatureModeManager() { RemoveAllModes(); if (IdleModes) { std::free(IdleModes); Log("V"); } }
    void RemoveAllModes()
    {
        if (!PModes.empty())
            PModes.front()->OnDeactivate();
        PModes.clear();
        AddMode(0);
    }
    bool AddMode(unsigned long) { PModes.push_back(&g_default); Log("A"); return true; }
    CModeList PModes;
    long TimeLeftForCurrentMode;
    long TimeForNextIdleMode;
    int* IdleModes;
};
int main()
{
    CModeBase a = { 'a', true }, b = { 'b', false };
    g_log[0] = 0;
    CTCCreatureModeManager* m = new CTCCreatureModeManager;
    m->PModes.push_back(&a);
    m->PModes.push_back(&b);
    m->IdleModes = (int*)std::malloc(8);
    delete (CTCBase*)m;
    bool ok = std::strcmp(g_log, "DaXaXbAVX0LB") == 0 && !a.Active;
    std::printf(ok ? "0073aea0_TEST PASS\n" : "0073aea0_TEST FAIL\n");
    return ok ? 0 : 1;
}