// Behaviour test for 0x006A5D50 ~CThingPhysical: the thing leaves the current thing manager's physical list, then
// POriginalCreator, PLastCreator and the health-over-time list (every node freed) are destroyed, then ~CThing.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
static int g_freed = 0;
struct Node { Node* next; Node* prev; long frames; float health; };
class CHealthList {
public:
    CHealthList() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    void push(long f) { Node* n = (Node*)std::malloc(sizeof(Node)); n->frames = f; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    ~CHealthList()
    {
        Node* cur = head->next;
        while (cur != head) { Node* tmp = cur; cur = cur->next; std::free(tmp); ++g_freed; }
        head->next = head; head->prev = head;
        std::free(head); ++g_freed;
        Log("L");
    }
    Node* head;
};
class CThingPhysical;
class CThingManager { public: CThingManager() : Removed(0) { } void RemoveFromPhysicalList(CThingPhysical& thing) { Removed = &thing; Log("R"); } CThingPhysical* Removed; };
class CBaseIntelligentPointer { public: CBaseIntelligentPointer(const char* t) : Tag(t) { } virtual ~CBaseIntelligentPointer() { Log(Tag); } const char* Tag; };
class CThing {
public:
    virtual ~CThing() { Log("T"); }
    static CThingManager* PCurrentThingManager;
protected:
    CThingManager& GetThingManager() { return *PCurrentThingManager; }
};
CThingManager* CThing::PCurrentThingManager = 0;
class CThingPhysical : public CThing {
public:
    CThingPhysical() : PLastCreator("a"), POriginalCreator("o") { }
    virtual ~CThingPhysical() { GetThingManager().RemoveFromPhysicalList(*this); }
    CHealthList             HealthOverTimeEffects;
    float                   Scale;
    CBaseIntelligentPointer PLastCreator;
    CBaseIntelligentPointer POriginalCreator;
};
int main()
{
    CThingManager manager;
    CThing::PCurrentThingManager = &manager;
    g_log[0] = 0;
    CThingPhysical* thing = new CThingPhysical;
    thing->HealthOverTimeEffects.push(3);
    thing->HealthOverTimeEffects.push(4);
    CThingPhysical* expected = thing;
    delete thing;
    if (std::strcmp(g_log, "RoaLT") != 0 || manager.Removed != expected || g_freed != 3) { std::printf("006a5d50_TEST FAIL\n"); return 1; }
    std::printf("006a5d50_TEST PASS\n");
    return 0;
}