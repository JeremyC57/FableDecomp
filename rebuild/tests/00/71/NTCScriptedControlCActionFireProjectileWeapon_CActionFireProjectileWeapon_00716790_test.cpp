// Behaviour test for CActionFireProjectileWeapon::CActionFireProjectileWeapon @ 0x00716790: zeroed action, target, empty sentinel list, index -1.
#include <cstdio>
#include <cstdlib>
class CThing { public: int id; };
class CScriptThing { public: CThing* PImp_Data; };
static CScriptThing GetScriptThing(CThing* t) { CScriptThing s; s.PImp_Data = t; return s; }
struct CActionList {
    struct node { node* next; node* prev; void* object; void* info; };
    node* head;
    CActionList() : head(0) { node* s = (node*)std::malloc(sizeof(node)); s->next = s; s->prev = s; head = s; }
    bool empty() const { return head->next == head; }
};
struct CActionFireProjectileWeapon {
    void* PTCScript; bool Activated, Terminated; CScriptThing PTarget; CActionList ActionList; long CurrentActionIndex;
    CActionFireProjectileWeapon(CThing* t) : PTCScript(0), Activated(false), Terminated(false), PTarget(GetScriptThing(t)), ActionList(), CurrentActionIndex(-1) {}
};
int main()
{
    CThing th; th.id = 3; CActionFireProjectileWeapon a(&th);
    if (a.PTCScript || a.Activated || a.Terminated || a.PTarget.PImp_Data != &th || !a.ActionList.empty() || a.ActionList.head->prev != a.ActionList.head
        || a.CurrentActionIndex != -1) { std::printf("00716790_TEST FAIL\n"); return 1; }
    std::free(a.ActionList.head);
    std::printf("00716790_TEST PASS\n");
    return 0;
}