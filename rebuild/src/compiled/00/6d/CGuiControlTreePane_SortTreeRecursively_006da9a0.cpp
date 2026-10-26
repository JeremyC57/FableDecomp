#include "rebuild_abi.h"
// Leaf: SortTreeRecursively

// Target: 0x00436900  CGuiControlTreePane::SortTreeRecursively
// __fastcall (modeled as __fastcall this-only), void return.
//
// `this` [+0x0] holds a pointer to a circular list sentinel/head node.
// Each node: [+0x0] = next, [+0x4] = prev. The routine walks head->next
// deleting every node until it loops back to the head, resets the head to
// the empty (self-referential) state, then frees the head node itself.
// Callee 0x7c8114 is operator delete.

class TreeNode
{ public:
    TreeNode* next;   // +0x0
    TreeNode* prev;   // +0x4
};

struct TreeList
{
    TreeNode* head;   // +0x0
};

void __cdecl free(void*);

void FABLE_FASTCALL SortTreeRecursively_00436900(TreeList* thisptr)
{
    TreeNode* head = thisptr->head;   // mov eax,[edi]
    TreeNode* cur = head->next;       // mov esi,[eax]
    while (cur != thisptr->head)      // cmp esi,eax / je ; loop: cmp esi,[edi] / jne
    {
        TreeNode* n = cur;            // mov eax,esi
        cur = cur->next;              // mov esi,[esi]
        free(n);    // push eax / call
    }

    thisptr->head->next = thisptr->head;   // mov eax,[edi] / mov [eax],eax
    thisptr->head->prev = thisptr->head;   // mov eax,[edi] / mov [eax+4],eax

    TreeNode* h = thisptr->head;      // mov edi,[edi]
    if (h != 0)                       // test edi,edi / je
    {
        free(h);    // push edi / call
    }
}