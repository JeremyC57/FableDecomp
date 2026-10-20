// Behaviour test for 0x00A772D0 CNavNavigableLeafNode::CNavNavigableLeafNode(file ctor): base fields from the arguments
// (leaf, not blocked), region = the 1-based list entry (0 = none), preferability read or 0x40, neighbour indices read.
#include <cstdio>
#include <cstring>

struct C2DVector { float X, Y; };
struct CAFile {
    const unsigned char* p;
    void Read(void* d, unsigned long n, long) { std::memcpy(d, p, n); p += n; }
};
struct CNavRegion { long id; };
struct SNode { SNode* next; SNode* prev; CNavRegion r; };
struct CIt { SNode* n; CNavRegion& operator*() const { return n->r; } };
static void advance(CIt& it, long n) { if (n > 0) while (n--) it.n = it.n->next; else while (n++) it.n = it.n->prev; }
struct CNavNavigableLeafNode {
    C2DVector Position; unsigned char QuadTreeLevel, NavLayer; bool LeafNode, Blocked;
    CNavRegion* PRegion; int Neighbours[8]; unsigned int Count; unsigned char PreferabilityCost;
    CNavNavigableLeafNode(const C2DVector& pos, unsigned char level, unsigned char layer, CAFile& file, SNode* regions, bool readPreferability)
        : Position(pos), QuadTreeLevel(level), NavLayer(layer), LeafNode(true), Blocked(false), PRegion(0)
    {
        int regionIndex; file.Read(&regionIndex, 4, 0);
        if (regionIndex > 0) { CIt it = { regions->next }; advance(it, regionIndex - 1); PRegion = &*it; }
        if (readPreferability) file.Read(&PreferabilityCost, 1, 0); else PreferabilityCost = 0x40;
        unsigned int n; file.Read(&n, 4, 0); Count = n;
        for (unsigned int i = 0; i < n; ++i) { int index; file.Read(&index, 4, 0); Neighbours[i] = index; }
    }
};
static void Put(unsigned char*& w, int v) { std::memcpy(w, &v, 4); w += 4; }
int main()
{
    SNode head, a, b, c; head.next = &a; a.next = &b; b.next = &c; c.next = &head; head.prev = &c; c.prev = &b; b.prev = &a; a.prev = &head;
    a.r.id = 1; b.r.id = 2; c.r.id = 3;
    unsigned char buf[64]; unsigned char* w = buf;
    Put(w, 3); *w++ = 0x22; Put(w, 2); Put(w, 17); Put(w, 42);
    CAFile f = { buf }; C2DVector pos = { 1.5f, -2.0f };
    CNavNavigableLeafNode n(pos, 4, 1, f, &head, true);
    if (n.Position.X != 1.5f || n.QuadTreeLevel != 4 || n.NavLayer != 1 || !n.LeafNode || n.Blocked) { std::printf("00a772d0_TEST FAIL\n"); return 1; }
    if (n.PRegion != &c.r || n.PreferabilityCost != 0x22 || n.Count != 2 || n.Neighbours[0] != 17 || n.Neighbours[1] != 42) { std::printf("00a772d0_TEST FAIL\n"); return 1; }
    w = buf; Put(w, 0); Put(w, 0); CAFile g = { buf };
    CNavNavigableLeafNode m(pos, 0, 0, g, &head, false);
    if (m.PRegion != 0 || m.PreferabilityCost != 0x40 || m.Count != 0) { std::printf("00a772d0_TEST FAIL\n"); return 1; }
    std::printf("00a772d0_TEST PASS\n");
    return 0;
}