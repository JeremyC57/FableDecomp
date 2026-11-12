// Leaf: ?InvalidateAndClearAllSecuritySweepTasks@CTCVillage@@AAEXXZ
// Name (agentNF32, 2026-11-12; was the free __fastcall ?CTCVillage_InvalidateAndClearAllSecuritySweepTasks@@YIXPAVCTCVillage@@@Z): Ego_r.pdb
//   0x0069D86E and FableWin.pdb 0x021619D9 ?InvalidateAndClearAllSecuritySweepTasks@CTCVillage@@AAEXXZ (private member, no arguments);
//   engine/CTCVillage.h declares it private (FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE); EndSecuritySweep 0x006590BF tail-jumps here.
//   The layout stays the pre-existing local model of this row (the list clear 0x0065FEFC keeps its stand-in spelling).
#pragma optimize("s",on)
// InvalidateAndClearAllSecuritySweepTasks 0x006590a0  thiscall (this in ecx)
// this+0x5c is an intrusive doubly-linked list (sentinel node). Each list node:
//   +0 next, +8 payload(task). task+0x30 = invalidate flag byte.
// After marking all tasks invalid, tail-calls the list clear routine.

struct Task { char pad[0x30]; unsigned char flag; };

struct ListNode {
    ListNode* next;   // +0
    ListNode* prev;   // +4
    Task*     task;   // +8
};

struct List {
    ListNode* head;   // +0  -> sentinel
};

class CTCVillage { public:
    char pad[0x5c];
    List list;        // +0x5c
private:
    void InvalidateAndClearAllSecuritySweepTasks();
};

// The tail-called clear routine (0x65fefc). Thiscall on the List* (ecx = this+0x5c).
extern "C" void __fastcall List_Clear(List* self);

void CTCVillage::InvalidateAndClearAllSecuritySweepTasks()
{
    List* lst = &list;                       // add ecx,0x5c
    ListNode* sentinel = lst->head;          // mov edx,[ecx]
    ListNode* n = sentinel->next;            // mov eax,[edx]
    while (n != sentinel) {                   // cmp/je
        n->task->flag = 1;                    // esi=[eax+8]; byte[esi+0x30]=1
        n = n->next;                          // mov eax,[eax]
    }
    List_Clear(lst);                          // jmp 0x65fefc (tail call, ecx preserved)
}