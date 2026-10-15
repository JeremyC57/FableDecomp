#include <cstdio>

struct CThing { int id; };
struct CTCBase {
    virtual ~CTCBase() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CListNode { CListNode* next; CListNode* prev; };
static CListNode g_sentinel;
struct CRetailList_CAnimInit {            // one sentinel-node pointer, the sentinel linked to itself
    CListNode* head;
    CRetailList_CAnimInit() : head(&g_sentinel) { g_sentinel.next = g_sentinel.prev = &g_sentinel; }
    bool empty() const { return head->next == head; }
};
struct CTCAnimatingObject : CTCBase {
    bool                  Animating;
    long                  CurrentAnimationPriority;
    CRetailList_CAnimInit QueuedAnims;
    CTCAnimatingObject(CThing* owner) : CTCBase(owner), Animating(false), CurrentAnimationPriority(-1), QueuedAnims() { }
};

int main()
{
    CThing owner = { 1 };
    CTCAnimatingObject a(&owner);
    if (a.ParentThing != &owner || a.Animating || a.CurrentAnimationPriority != -1 || !a.QueuedAnims.empty())
        { std::printf("004de839_TEST FAIL\n"); return 1; }
    std::printf("004de839_TEST PASS\n");
    return 0;
}
