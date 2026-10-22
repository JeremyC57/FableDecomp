// Behaviour test for CNavigatorAStar::InitialiseFromPreCalculatedRoute (0x00A1E370): the search state is dropped (the
// in-progress counter only when states exist, the state tree reset, old way points freed, the open list emptied), the
// way-point list is rebuilt from the route in order with the valid flag set, the steering agent gets the first and
// (with more than one point) the second way point, and the goal / asked-for fields and flags are set.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct V2 { float X, Y; };
struct V3 { float X, Y, Z; };
struct RoutePoint { V2 first; unsigned char second; };
struct WayPoint { V2 Position; unsigned char Type; bool Valid; };
struct WNode { WNode* next; WNode* prev; WayPoint data; };
struct TNode { long color; TNode* parent; TNode* left; TNode* right; };
static long g_InProgress = 5;
static int g_Erased, g_Removed, g_Widths;
static const WayPoint* g_Goal1; static const WayPoint* g_Goal2;
static int g_NodeTag = 77;
struct Nav {
    bool OnRoute, Interrupted;
    TNode* header; unsigned long count;
    void** heapStart; void** heapFinish;
    void* GoalNode; V2 GoalPosition; void* NodeAskedToGetTo; V3 PosAskedToGetTo;
    WNode* Route; V2 LastWayPointVisited, StringLeadPosition;
    bool Initialised, GotRoute, SlowedDown, Culled, PartWay; unsigned char cw, acw;
    unsigned long RouteSize() const { unsigned long n = 0; for (WNode* p = Route->next; p != Route; p = p->next) ++n; return n; }
    void Init(const V3& from, const RoutePoint* route, unsigned long n, const V3& to)
    {
        (void)from;
        if (count != 0) --g_InProgress;
        ++g_Removed;
        if (count != 0) { ++g_Erased; header->left = header; header->parent = 0; header->right = header; count = 0; }
        WNode* cur = Route->next;
        while (cur != Route) { WNode* t = cur; cur = cur->next; std::free(t); }
        Route->next = Route; Route->prev = Route;
        heapFinish = heapStart;
        GotRoute = true; Culled = true; PartWay = false;
        for (unsigned long i = 0; i < n; ++i) {
            WNode* t = (WNode*)std::malloc(sizeof(WNode));
            t->data.Position = route[i].first; t->data.Type = route[i].second; t->data.Valid = true;
            WNode* p = Route->prev; t->next = Route; t->prev = p; p->next = t; Route->prev = t;
        }
        ++g_Widths;
        g_Goal1 = &Route->next->data;
        g_Goal2 = RouteSize() > 1 ? &Route->next->next->data : &Route->next->data;
        StringLeadPosition = route[0].first; SlowedDown = true; LastWayPointVisited = route[0].first;
        GoalNode = &g_NodeTag;
        GoalPosition.X = to.X; GoalPosition.Y = to.Y;
        NodeAskedToGetTo = GoalNode; PosAskedToGetTo = to;
        Initialised = true; OnRoute = true; Interrupted = false; cw = 0; acw = 0;
    }
};
int main()
{
    int bad = 0;
    WNode sentinel; sentinel.next = sentinel.prev = &sentinel;
    TNode hdr; hdr.parent = &hdr; hdr.left = hdr.right = 0;
    void* heap[4];
    Nav n; std::memset(&n, 0, sizeof(n));
    n.Route = &sentinel; n.header = &hdr; n.count = 3; n.heapStart = heap; n.heapFinish = heap + 3;
    n.cw = 4; n.acw = 2; n.Interrupted = true;
    WNode* old = (WNode*)std::malloc(sizeof(WNode));                        // a stale way point
    old->next = &sentinel; old->prev = &sentinel; sentinel.next = sentinel.prev = old;
    RoutePoint r[3] = { { { 1.0f, 2.0f }, 1 }, { { 3.0f, 4.0f }, 2 }, { { 5.0f, 6.0f }, 3 } };
    V3 from = { 0.0f, 0.0f, 0.0f }, to = { 7.0f, 8.0f, 9.0f };
    n.Init(from, r, 3, to);
    if (g_InProgress != 4 || g_Erased != 1 || n.count != 0 || hdr.parent != 0 || hdr.left != &hdr || n.heapFinish != heap) bad |= 1;
    if (n.RouteSize() != 3 || n.Route->next->data.Type != 1 || n.Route->prev->data.Position.Y != 6.0f || !n.Route->next->next->data.Valid) bad |= 2;
    if (g_Goal1 != &n.Route->next->data || g_Goal2 != &n.Route->next->next->data) bad |= 4;
    if (n.StringLeadPosition.X != 1.0f || n.LastWayPointVisited.Y != 2.0f || n.GoalPosition.X != 7.0f || n.PosAskedToGetTo.Z != 9.0f) bad |= 8;
    if (!n.GotRoute || !n.Culled || n.PartWay || !n.SlowedDown || !n.Initialised || !n.OnRoute || n.Interrupted || n.cw || n.acw) bad |= 16;
    if (n.NodeAskedToGetTo != &g_NodeTag || n.GoalNode != &g_NodeTag) bad |= 32;
    n.Init(from, r, 1, to);                                                  // no states: counter kept; one point twice
    if (g_InProgress != 4 || g_Erased != 1 || n.RouteSize() != 1 || g_Goal2 != g_Goal1) bad |= 64;
    WNode* cur = sentinel.next; while (cur != &sentinel) { WNode* t = cur; cur = cur->next; std::free(t); }
    if (bad) { std::printf("00a1e370_TEST FAIL %d\n", bad); return 1; }
    std::printf("00a1e370_TEST PASS\n");
    return 0;
}