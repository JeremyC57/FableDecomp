// Behaviour test for CRegionConnectionNode::GetNextNodeOnWayToRegion (0x00828360): breadth-first search over a
// region graph held in a sorted (key, node) array, with a malloc'd circular list as the queue.
#include <cstdio>
#include <cstdlib>

struct ListNode { ListNode* next; ListNode* prev; const struct Node* data; };

struct Node {
    long RegionID;
    long* ConnStart;
    long* ConnFinish;
    long* ConnEnd;
    mutable long MostRecentSearchID;
    mutable long FirstNodeInSearch;
    unsigned int size() const { return (unsigned int)(ConnFinish - ConnStart); }
};

struct Entry { long first; Node* object; void* block; };

struct Graph {
    Entry* first;
    Entry* last;
    Entry* LowerBound(const long& key) const
    {
        Entry* f = first;
        int len = (int)(last - first);
        while (len > 0) {
            int half = len / 2;
            Entry* middle = f + half;
            if (middle->first < key) { f = middle + 1; len = len - half - 1; }
            else len = half;
        }
        return f;
    }
    const Node* GetRegionNode(long region) const
    {
        Entry* it = LowerBound(region);
        if (it == last || region < it->first) return 0;
        return it->object;
    }
};

static long g_SearchID = 0;

static void ListPushBack(ListNode* head, const Node* n)
{
    ListNode* t = (ListNode*)std::malloc(sizeof(ListNode));
    t->data = n;
    ListNode* p = head->prev;
    t->next = head; t->prev = p; p->next = t; head->prev = t;
}
static void ListFree(ListNode* head)
{
    ListNode* cur = head->next;
    while (cur != head) { ListNode* t = cur; cur = cur->next; std::free(t); }
    std::free(head);
}

static long GetNext(const Node* self, long toRegion, const Graph& graph)
{
    if (self->RegionID == toRegion) return -1;
    ++g_SearchID;
    ListNode* head = (ListNode*)std::malloc(sizeof(ListNode));
    head->next = head; head->prev = head;
    self->MostRecentSearchID = g_SearchID;
    self->FirstNodeInSearch = 0;
    for (unsigned int i = 0; i < self->size(); ++i) {
        const Node* node = graph.GetRegionNode(self->ConnStart[i]);
        if (node != 0) {
            if (node->RegionID == toRegion) { ListFree(head); return toRegion; }
            node->MostRecentSearchID = g_SearchID;
            node->FirstNodeInSearch = node->RegionID;
            ListPushBack(head, node);
        }
    }
    while (head->next != head) {
        const Node* cur = head->next->data;
        for (unsigned int i = 0; i < cur->size(); ++i) {
            const Node* node = graph.GetRegionNode(cur->ConnStart[i]);
            if (node != 0 && node->MostRecentSearchID != g_SearchID) {
                if (node->RegionID == toRegion) { long r = cur->FirstNodeInSearch; ListFree(head); return r; }
                node->MostRecentSearchID = g_SearchID;
                node->FirstNodeInSearch = cur->FirstNodeInSearch;
                ListPushBack(head, node);
            }
        }
        ListNode* f = head->next;
        f->prev->next = f->next; f->next->prev = f->prev;
        std::free(f);
    }
    ListFree(head);
    return -1;
}

// regions: 10 - 20 - 30 - 40 (chain), 20 - 50, 60 isolated, 10 also lists 99 (not in the graph)
static long c10[] = { 20, 99 };
static long c20[] = { 10, 30, 50 };
static long c30[] = { 20, 40 };
static long c40[] = { 30 };
static long c50[] = { 20 };
static long c60[] = { 0 };

int main()
{
    Node n10 = { 10, c10, c10 + 2, c10 + 2, 0, 0 };
    Node n20 = { 20, c20, c20 + 3, c20 + 3, 0, 0 };
    Node n30 = { 30, c30, c30 + 2, c30 + 2, 0, 0 };
    Node n40 = { 40, c40, c40 + 1, c40 + 1, 0, 0 };
    Node n50 = { 50, c50, c50 + 1, c50 + 1, 0, 0 };
    Node n60 = { 60, c60, c60, c60, 0, 0 };
    Entry entries[] = { { 10, &n10, 0 }, { 20, &n20, 0 }, { 30, &n30, 0 }, { 40, &n40, 0 }, { 50, &n50, 0 },
                        { 60, &n60, 0 } };
    Graph g = { entries, entries + 6 };
    int bad = 0;
    if (GetNext(&n10, 10, g) != -1) bad |= 1;          // already there
    if (GetNext(&n10, 20, g) != 20) bad |= 2;          // direct neighbour
    if (GetNext(&n10, 40, g) != 20) bad |= 4;          // two hops through 20
    if (GetNext(&n40, 50, g) != 30) bad |= 8;          // 40 -> 30 -> 20 -> 50
    if (GetNext(&n10, 60, g) != -1) bad |= 16;         // unreachable
    if (GetNext(&n20, 40, g) != 30) bad |= 32;
    if (g_SearchID != 5) bad |= 64;                    // the target==self call does not stamp
    if (g.GetRegionNode(99) != 0 || g.GetRegionNode(5) != 0 || g.GetRegionNode(30) != &n30) bad |= 128;
    if (bad) { std::printf("00828360_TEST FAIL\n"); return 1; }
    std::printf("00828360_TEST PASS\n");
    return 0;
}