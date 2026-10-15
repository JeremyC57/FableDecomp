#pragma optimize("s", on)
// CTCVillage::GetFirstUnAssignedPatrolTask() -- the first patrol task in the village's PatrolTasks list that answers yes to both of its slot-3 and slot-4 queries (valid and not yet assigned), else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658A02, length 51 bytes. Size TU (the loop enters with `jmp` to its
// bottom test instead of a duplicated entry test): `optimize("s",on)`.
// Callees: two virtuals of CVillageTaskPatrol: slot 3 (+0x0C) and slot 4 (+0x10), both bool (descriptive names).
// Shape: a plain iterator loop over the retail one-pointer std::list (sentinel in edi, node in esi); the element is a
// CCountedPointer<CVillageTaskPatrol> whose object pointer sits at node+8 (ebx, kept for the result); `&&` of the two
// virtual calls; the null result shares the epilogue (`xor eax,eax`), the found arm jumps back into it.
// Layout: CTCVillage.h is the PDB layout (PatrolTasks +0xAC); with 4-byte retail lists it sits at +0x54
// (RemoveThingFromAllTasks 0x00659EA9 walks the six task lists +0x4C..+0x60), so a local model.
struct CVillageTaskPatrol {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool IsValid();                 // slot 3 (+0x0C)
    virtual bool IsUnassigned();            // slot 4 (+0x10)
};

struct SPatrolTaskNode {                    // retail std::list node of CCountedPointer<CVillageTaskPatrol>
    SPatrolTaskNode*    next;               // +0x00
    SPatrolTaskNode*    prev;               // +0x04
    CVillageTaskPatrol* Object;             // +0x08 CCountedPointer::Object
};

struct CTCVillage {
    void*            __vftable;             // +0x00
    unsigned char    _pad_0x04[0x50];
    SPatrolTaskNode* PatrolTasks;           // +0x54 retail std::list (sentinel pointer only)
    CVillageTaskPatrol* GetFirstUnAssignedPatrolTask();
};

CVillageTaskPatrol* CTCVillage::GetFirstUnAssignedPatrolTask()
{
    SPatrolTaskNode* head = PatrolTasks;
    for (SPatrolTaskNode* n = head->next; n != head; n = n->next) {
        CVillageTaskPatrol* task = n->Object;
        if (task->IsValid() && task->IsUnassigned())
            return task;
    }
    return 0;
}
