// Behaviour test: list<CMiniMapDisplay::CMarkerInfo>::insert -- a new node holding a copy of the marker is linked before the position
#include <cstdio>
#include <cstdlib>
#include <new>

struct CThing { int id; };
static int g_live = 0;
struct CIntelligentPointer {                            // CIntelligentPointer<const CThing>: ctor/SetPItem/GetPItem/dtor counted
    const CThing* item;
    CIntelligentPointer(const CThing* p) : item(p) { ++g_live; }
    CIntelligentPointer(const CIntelligentPointer& o) : item(o.item) { ++g_live; }
    ~CIntelligentPointer() { --g_live; }
    const CThing* GetPItem() const { return item; }
};
struct CMarkerInfo {                                    // CMiniMapDisplay::CMarkerInfo
    CIntelligentPointer PThing;
    long GraphicIndex;
    bool AutoMarker;
    CMarkerInfo(const CThing& thing, long graphicIndex, bool autoMarker) : PThing(&thing), GraphicIndex(graphicIndex), AutoMarker(autoMarker) { }
};
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CMarkerInfo data; };
struct CMarkerList {                                    // STLport list<CMarkerInfo> (one sentinel pointer)
    NodeBase* node;
    NodeBase* insert(NodeBase* position, const CMarkerInfo& x)   // list::insert 0x0082C920
    {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        new (&tmp->data) CMarkerInfo(x);
        NodeBase* n = position;
        NodeBase* prev = n->prev;
        tmp->next = n;
        tmp->prev = prev;
        prev->next = tmp;
        n->prev = tmp;
        return tmp;
    }
    void push_back(const CMarkerInfo& x) { insert(node, x); }
};

int main()
{
    NodeBase sentinel;
    sentinel.next = sentinel.prev = &sentinel;
    CMarkerList l;
    l.node = &sentinel;
    CThing a = { 1 }, b = { 2 };
    {
        CMarkerInfo m1(a, 10, true);
        CMarkerInfo m2(b, 20, false);
        NodeBase* n1 = l.insert(&sentinel, m1);          // end(): append
        NodeBase* n2 = l.insert(n1, m2);                 // before the first: new front
        if (sentinel.next != n2 || n2->next != n1 || n1->next != &sentinel || sentinel.prev != n1 || n1->prev != n2 || n2->prev != &sentinel)
            { std::printf("0082c920_TEST FAIL\n"); return 1; }
        const CMarkerInfo& d1 = ((Node*)n1)->data;
        const CMarkerInfo& d2 = ((Node*)n2)->data;
        if (d1.PThing.GetPItem() != &a || d1.GraphicIndex != 10 || !d1.AutoMarker || d2.PThing.GetPItem() != &b || d2.GraphicIndex != 20 || d2.AutoMarker)
            { std::printf("0082c920_TEST FAIL\n"); return 1; }
    }
    if (g_live != 2)                                     // the two node copies are still alive, the temporaries are gone
        { std::printf("0082c920_TEST FAIL\n"); return 1; }
    std::printf("0082c920_TEST PASS\n");
    return 0;
}