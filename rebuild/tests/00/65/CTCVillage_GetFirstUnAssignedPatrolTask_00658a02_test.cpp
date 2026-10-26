#include <cstdio>
class CVillageTaskPatrol { public:
    bool valid, unassigned;
    virtual ~CVillageTaskPatrol() {}
    virtual bool IsValid() { return valid; }
    virtual bool IsUnassigned() { return unassigned; }
};
struct SPatrolTaskNode { SPatrolTaskNode* next; SPatrolTaskNode* prev; CVillageTaskPatrol* Object; };
class CTCVillage { public:
    SPatrolTaskNode* PatrolTasks;
    CVillageTaskPatrol* GetFirstUnAssignedPatrolTask()
    {
        SPatrolTaskNode* head = PatrolTasks;
        for (SPatrolTaskNode* n = head->next; n != head; n = n->next) {
            CVillageTaskPatrol* task = n->Object;
            if (task->IsValid() && task->IsUnassigned()) return task;
        }
        return 0;
    }
};
int main()
{
    CVillageTaskPatrol a, b, c;
    a.valid = false; a.unassigned = true; b.valid = true; b.unassigned = false; c.valid = true; c.unassigned = true;
    SPatrolTaskNode head, na, nb, nc;
    head.next = &na; na.next = &nb; nb.next = &nc; nc.next = &head;
    na.Object = &a; nb.Object = &b; nc.Object = &c;
    CTCVillage v; v.PatrolTasks = &head;
    if (v.GetFirstUnAssignedPatrolTask() != &c) { std::printf("00658a02_TEST FAIL\n"); return 1; }
    c.unassigned = false;
    if (v.GetFirstUnAssignedPatrolTask() != 0) { std::printf("00658a02_TEST FAIL\n"); return 1; }
    head.next = &head;
    if (v.GetFirstUnAssignedPatrolTask() != 0) { std::printf("00658a02_TEST FAIL\n"); return 1; }
    std::printf("00658a02_TEST PASS\n");
    return 0;
}