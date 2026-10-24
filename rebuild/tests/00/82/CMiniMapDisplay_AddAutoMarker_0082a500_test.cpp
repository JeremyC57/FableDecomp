// Behaviour test: CMiniMapDisplay::AddAutoMarker -- a positive graphic index appends an automatic marker for the thing; zero or less adds nothing
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

struct CMiniMapDisplay {
    CMarkerList Markers;
    void AddAutoMarker(const CThing& thing, long graphicIndex)
    {
        if (graphicIndex > 0)
            Markers.push_back(CMarkerInfo(thing, graphicIndex, true));
    }
};

int main()
{
    NodeBase sentinel;
    sentinel.next = sentinel.prev = &sentinel;
    CMiniMapDisplay d;
    d.Markers.node = &sentinel;
    CThing door = { 7 }, chest = { 8 };
    d.AddAutoMarker(door, 0);
    d.AddAutoMarker(door, -3);
    if (sentinel.next != &sentinel || g_live != 0)
        { std::printf("0082a500_TEST FAIL\n"); return 1; }
    d.AddAutoMarker(door, 4);
    d.AddAutoMarker(chest, 9);
    Node* first = (Node*)sentinel.next;
    Node* second = (Node*)first->next;
    if (second->next != &sentinel || first->data.PThing.GetPItem() != &door || first->data.GraphicIndex != 4 || !first->data.AutoMarker
        || second->data.PThing.GetPItem() != &chest || second->data.GraphicIndex != 9 || !second->data.AutoMarker || g_live != 2)
        { std::printf("0082a500_TEST FAIL\n"); return 1; }
    std::printf("0082a500_TEST PASS\n");
    return 0;
}