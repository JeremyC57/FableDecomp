#include <stdio.h>
#include "rebuild_abi.h"

struct TreeNode
{
    TreeNode* next;
    TreeNode* prev;
};

struct TreeList
{
    TreeNode* head;
};

static int g_deletes = 0;

// spy on the MSVCR71 free thunk (0x00BFEA14) the source calls: counts only while armed around the
// call under test and never releases (the nodes are static); CRT start-up/exit frees are ignored
static int g_armed = 0;
extern "C" void __cdecl free(void* p)
{
    if (g_armed) ++g_deletes;
}

void FABLE_FASTCALL SortTreeRecursively_00436900(TreeList* thisptr);

int main(void)
{
    // Build a circular list: head <-> a <-> b <-> head
    static TreeNode nodes[3];
    TreeNode* head = &nodes[0];
    TreeNode* a = &nodes[1];
    TreeNode* b = &nodes[2];

    head->next = a;  head->prev = b;
    a->next = b;     a->prev = head;
    b->next = head;  b->prev = a;

    TreeList list;
    list.head = head;

    g_armed = 1; SortTreeRecursively_00436900(&list); g_armed = 0;

    // Expect a, b, and head all freed => 3 deletes.
    if (g_deletes == 3)
        printf("OK_00436900_3\n");
    else
        printf("FAIL_%d\n", g_deletes);

    return 0;
}